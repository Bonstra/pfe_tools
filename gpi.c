#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdio.h>

#include "hw.h"

void gpi_reset(volatile struct gpi_regs *gpi)
{
	gpi->ctrl |= 0x02;
}

void gpi_enable(volatile struct gpi_regs *gpi)
{
	gpi->ctrl = 0x01;
}

void gpi_disable(volatile struct gpi_regs *gpi)
{
	gpi->ctrl = 0x00;
}

bool gpi_is_enabled(volatile struct gpi_regs *gpi)
{
	return !!(gpi->ctrl & 1);
}

void gpi_init(volatile struct gpi_regs *gpi)
{
	const uint32_t LMEM_RETRIES = 0x40;
	if (!gpi)
		return;

	gpi_enable(gpi);

	gpi->rx_config = 0;

	gpi_reset(gpi);

	gpi->ddr_alloc_addr = CBUS_BASE_PE + BMU2_OFFSET +
		offsetof(struct bmu_regs, alloc_ctrl);
	gpi->ddr_free_addr = CBUS_BASE_PE + BMU2_OFFSET +
		offsetof(struct bmu_regs, free_ctrl);
	gpi->lmem_alloc_addr = CBUS_BASE_PE + BMU1_OFFSET +
		offsetof(struct bmu_regs, alloc_ctrl);
	gpi->lmem_free_addr = CBUS_BASE_PE + BMU1_OFFSET +
		offsetof(struct bmu_regs, free_ctrl);

	gpi->buf_size = (DDR_POOL_BUFLEN_BYTES << 16) | LMEM_POOL_BUFLEN_BYTES;
	gpi->hdr_size = (DDR_POOL_HDR_SIZE << 16) | LMEM_POOL_HDR_SIZE;
	gpi->ddr_data_offset = DDR_POOL_HDR_SIZE;
	gpi->lmem_data_offset = LMEM_POOL_HDR_SIZE;

	/* Forward incoming packets to HIF_NOCPY IN FIFO */
	gpi->class_addr = CBUS_BASE_PE + HIF_NOCPY_OFFSET +
		offsetof(struct hif_nocpy_regs, rx_inq0_pktptr);

	gpi->tmlf_tx = 0xbc;
	gpi->dtx_aseq = 0x40;
	gpi->rx_config = (LMEM_RETRIES << 16) | 0x3; /* Use LMEM + DDR */
}

void gpi_dump_regs(volatile struct gpi_regs *gpi)
{
	uint32_t ctrl = gpi->ctrl;
	printf("[0x00] VERSION: 0x%.8x\n", gpi->version);
	printf("[0x04] CTRL: 0x%.8x\n", ctrl);
	if ((ctrl & 0x1) == 0) {
		printf("GPI is disabled. Dumping further registers is unsafe.\n");
		return;
	}
	printf("[0x08] RX_CONFIG: 0x%.8x\n", gpi->rx_config);
	printf("[0x0c] HDR_SIZE: 0x%.8x\n", gpi->hdr_size);
	printf("[0x10] BUF_SIZE: 0x%.8x\n", gpi->buf_size);
	printf("[0x14] LMEM_ALLOC_ADDR: 0x%.8x\n", gpi->lmem_alloc_addr);
	printf("[0x18] LMEM_FREE_ADDR: 0x%.8x\n", gpi->lmem_free_addr);
	printf("[0x1c] DDR_ALLOC_ADDR: 0x%.8x\n", gpi->ddr_alloc_addr);
	printf("[0x20] DDR_FREE_ADDR: 0x%.8x\n", gpi->ddr_free_addr);
	printf("[0x24] CLASS_ADDR: 0x%.8x\n", gpi->class_addr);
	printf("[0x28] DRX_FIFO: 0x%.8x\n", gpi->drx_fifo);
	printf("[0x2c] TRX_FIFO: 0x%.8x\n", gpi->trx_fifo);
	printf("[0x30] INQ_PKTPTR: 0x%.8x\n", gpi->inq_pktptr);
	printf("[0x34] DDR_DATA_OFFSET: 0x%.8x\n", gpi->ddr_data_offset);
	printf("[0x38] LMEM_DATA_OFFSET: 0x%.8x\n", gpi->lmem_data_offset);
	printf("[0x4c] TMLF_TX: 0x%.8x\n", gpi->tmlf_tx);
	printf("[0x50] DTX_ASEQ: 0x%.8x\n", gpi->dtx_aseq);
	printf("[0x54] FIFO_STATUS: 0x%.8x\n", gpi->fifo_status);
	printf("[0x58] FIFO_DEBUG: 0x%.8x\n", gpi->fifo_debug);
	printf("[0x5c] TX_PAUSE_TIME: 0x%.8x\n", gpi->tx_pause_time);
	printf("[0x60] LMEM_SEC_BUF_DATA_OFFSET: 0x%.8x\n", gpi->lmem_sec_buf_data_offset);
	printf("[0x64] DDR_SEC_BUF_DATA_OFFSET: 0x%.8x\n", gpi->ddr_sec_buf_data_offset);
}

int egpi_dump(unsigned int egpino)
{
	uintptr_t base;
	volatile struct gpi_regs *regs;
	switch(egpino) {
	case 1:
		base = CBUS_BASE + EGPI1_OFFSET;
		regs = egpi1;
		break;
	case 2:
		base = CBUS_BASE + EGPI2_OFFSET;
		regs = egpi2;
		break;
	case 3:
		base = CBUS_BASE + EGPI3_OFFSET;
		regs = egpi3;
		break;
	default:
		fprintf(stderr, "Invalid EGPI number: %u\n", egpino);
		return -EINVAL;
	}
	printf("==== EGPI%u (0x%.8"PRIxPTR") ====\n", egpino, base);
	gpi_dump_regs(regs);
	return 0;
}

int hgpi_dump(void)
{
	printf("==== HGPI (0x%.8"PRIxPTR") ====\n", CBUS_BASE + HGPI_OFFSET);
	gpi_dump_regs(hgpi);
	return 0;
}

