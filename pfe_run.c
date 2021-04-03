#define _GNU_SOURCE
#include <inttypes.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

#include "hw.h"
#include "mem.h"
#include "regs.h"

struct hif_info {
	unsigned int tx_bd;
	unsigned int rx_bd;
};

static struct hif_info hinfo;

static void hexdump_bytes(const void *base, intptr_t phys_base, off_t offset, size_t len)
{
	intptr_t p_off = phys_base + offset;
	uint8_t *bytes = (uint8_t*)base + offset;
	size_t i;
	for (i = 0; i < len; i++, p_off++) {
		uint8_t byte = bytes[i];
		if (i % 16 == 0) {
			printf("0x%.8"PRIxPTR":", p_off);
		}
		printf(" %.2x", byte);
		if (i % 16 == 15) {
			printf("\n");
		}
	}
	if (i % 16 > 0) {
		printf("\n");
	}
}

static void reset_pfe_sys(void)
{
	const struct timespec delay = {
		.tv_sec = 0,
		.tv_nsec = 50000000
	};
	clkreset[CLKRESET_AXI_RESET_1_OFFSET >> 2] |=
		CLKRESET_PFE_SYS_RST;
	nanosleep(&delay, NULL);
	clkreset[CLKRESET_AXI_RESET_1_OFFSET >> 2] &=
		~CLKRESET_PFE_SYS_RST;
}

static void enable_pfe_sys_clk(void)
{
	clkreset[CLKRESET_AXI_CLK_CNTRL_1_OFFSET >> 2] |=
		CLKRESET_PFE_SYS_CLK_ENABLE;
}

static void disable_pfe_core_clk(void)
{
	clkreset[CLKRESET_PFE_CLK_CNTRL_OFFSET >> 2] &=
		~CLKRESET_PFE_CORE_CLK_ENABLE;
}

static void init_pfe(void)
{
	printf("Enabling clocks...\n");
	enable_pfe_sys_clk();
	disable_pfe_core_clk();
	reset_pfe_sys();

	printf("Initializing BMUs...\n");
	bmu1_init();
	bmu2_init();

	printf("Initializing EGPI1...\n");
	gpi_init(egpi1);
	printf("Initializing EGPI2...\n");
	gpi_init(egpi2);

	printf("Initializing HIF_NOCPY...\n");
	hif_nocpy_init();

	printf("Initializing EMAC1...\n");
	emac_init(emac1);
	printf("Initializing EMAC2...\n");
	emac_init(emac2);
}

static void clear_rx_overrun(void)
{
	/* Disable EGPI2 so we do not receive new packets while resetting
	 * the BMUs */
	gpi_disable(egpi2);
	gpi_disable(egpi1);
	/* Reset all BMUs */
	bmu_reset(bmu2);
	bmu_reset(bmu1);
	/* Re-arm HIF_NOPY Rx */
	hif_nocpy->rx_ctrl |= 0x4;
	/* Re-enable EGPI2 */
	gpi_enable(egpi1);
	gpi_enable(egpi2);
}

static void wait_hif_rx_packet(void)
{
	while(!(hif_nocpy->int_src & HIF_INTSRC_RXPKT));
}

static void ack_hif_rx_interrupt(void)
{
	hif_nocpy->int_src = HIF_INTSRC_RXPKT;
}

static void free_buf(uint32_t addr)
{
	if (is_lmem_addr(addr)) {
		bmu_free(bmu1, addr);
	} else if (is_ddr_addr(addr)) {
		bmu_free(bmu2, addr);
	}
}

static void handle_rx_bd(const struct hif_desc *bd_orig)
{
	volatile struct hif_desc *bd = &hif_rx_descs[hinfo.rx_bd];
	uint32_t first_addr = bd_orig->data;
	uint32_t next_addr = 0;
	uint16_t pktlen = 0;
	uint8_t pktdata[1504];
	struct class_rx_hdr hdr;

	if (!is_lmem_addr(first_addr) && !is_ddr_addr(first_addr)) {
		printf("Invalid buffer address: 0x%.8" PRIx32 "\n",
				first_addr);
		goto finish;
	}
	if (!is_lmem_addr(first_addr)) {
		printf("Buffer address: 0x%.8" PRIx32 " not in LMEM\n",
				first_addr);
		goto release;
	}
	copy_from_lmem(&hdr, first_addr, sizeof(hdr));
	hexdump_bytes(&hdr, first_addr, 0, sizeof(hdr));

	next_addr = be32_to_cpu(hdr.next_ptr);
	pktlen = be16_to_cpu(hdr.length);
	if (!is_ddr_addr(next_addr)) {
		printf("Next buffer address: 0x%.8" PRIx32 " not in DDR\n",
				next_addr);
		goto release;
	}
	if (pktlen > 1504) {
		printf("Packet too long: %" PRIu16 "\n", pktlen);
		goto release;
	}
	copy_from_lmem(pktdata, first_addr + 0x10,
			(pktlen > 0x70) ? 0x70 : pktlen);
	if (pktlen > 0x70) {
		copy_from_ddr(&pktdata[0x70], next_addr + 0x100,
				pktlen - 0x70);
	}
	printf("Rx BD index: %u Rx: %" PRIu32 " bytes\n", hinfo.rx_bd,
			pktlen);
	//hexdump_bytes(pktdata, 0, 0, pktlen);

release:
	free_buf(next_addr);
	free_buf(first_addr);
finish:
	bd->data = 0;
	hif_rx_ack_bd(bd);
}

void rx_loop(void)
{
	const unsigned int NUM_BDS = HIF_RX_POOL_SIZE / sizeof(struct hif_desc);
	while(true) {
		struct hif_desc cur_bd;
		unsigned int rxdone;
		wait_hif_rx_packet();
		printf("INT_RXPKT\n");
		ack_hif_rx_interrupt();

		cur_bd = hif_rx_descs[hinfo.rx_bd];
		rxdone = 0;
		while(!(cur_bd.ctrl & HIF_DESC_CTRL_DESC_EN)) {
			handle_rx_bd(&cur_bd);

			rxdone++;
			hinfo.rx_bd++;
			if (hinfo.rx_bd >= NUM_BDS)
				hinfo.rx_bd = 0;
			cur_bd = hif_rx_descs[hinfo.rx_bd];
		}
		/* HIF_NOCPY Rx overrun, reset */
		if (rxdone == NUM_BDS) {
			printf("Rx overrun!\n");
			/* TODO: Wait for Tx queue to be empty */
			clear_rx_overrun();
		}
	}
}

int main(int argc, char *argv[])
{
	if (map_create())
		return EXIT_FAILURE;

	init_pfe();
	rx_loop();

	map_destroy();
	return EXIT_SUCCESS;
}

