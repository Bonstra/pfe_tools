#include <stdio.h>

#include "hw.h"

void hif_dump_regs(volatile struct hif_regs *hif)
{
	printf("[0x00] VERSION: 0x%.8x\n", hif->version);
	printf("[0x04] TX_CTRL: 0x%.8x\n", hif->tx_ctrl);
	printf("[0x08] TX_CUR_BD_ADDR: 0x%.8x\n", hif->tx_curr_bd_addr);
	printf("[0x0c] TX_ALLOC: 0x%.8x\n", hif->tx_alloc);
	printf("[0x10] TX_BDP_ADDR: 0x%.8x\n", hif->tx_bdp_addr);
	printf("[0x14] TX_STATUS: 0x%.8x\n", hif->tx_status);
	printf("[0x20] RX_CTRL: 0x%.8x\n", hif->rx_ctrl);
	printf("[0x24] RX_BDP_ADDR: 0x%.8x\n", hif->rx_bdp_addr);
	printf("[0x30] RX_STATUS: 0x%.8x\n", hif->rx_status);
	printf("[0x34] INT_SRC: 0x%.8x\n", hif->int_src);
	printf("[0x38] INT_ENABLE: 0x%.8x\n", hif->int_enable);
	printf("[0x3c] POLL_CTRL: 0x%.8x\n", hif->poll_ctrl);
	printf("[0x40] RX_CURR_BD_ADDR: 0x%.8x\n", hif->rx_curr_bd_addr);
	printf("[0x44] RX_ALLOC: 0x%.8x\n", hif->rx_alloc);
	printf("[0x48] TX_DMA_STATUS: 0x%.8x\n", hif->tx_dma_status);
	printf("[0x4c] RX_DMA_STATUS: 0x%.8x\n", hif->rx_dma_status);
	printf("[0x50] INT_COAL: 0x%.8x\n", hif->int_coal);
}

void hif_dump(void)
{
	printf("==== HIF (0x%.8x) ====\n", CBUS_BASE + HIF_OFFSET);
	hif_dump_regs(hif);
}

void hif_nocpy_dump_regs(volatile struct hif_nocpy_regs *hif)
{
	printf("[0x00] VERSION: 0x%.8x\n", hif->version);
	printf("[0x04] TX_CTRL: 0x%.8x\n", hif->tx_ctrl);
	printf("[0x08] TX_CUR_BD_ADDR: 0x%.8x\n", hif->tx_curr_bd_addr);
	printf("[0x0c] TX_ALLOC: 0x%.8x\n", hif->tx_alloc);
	printf("[0x10] TX_BDP_ADDR: 0x%.8x\n", hif->tx_bdp_addr);
	printf("[0x14] TX_STATUS: 0x%.8x\n", hif->tx_status);
	printf("[0x20] RX_CTRL: 0x%.8x\n", hif->rx_ctrl);
	printf("[0x24] RX_BDP_ADDR: 0x%.8x\n", hif->rx_bdp_addr);
	printf("[0x30] RX_STATUS: 0x%.8x\n", hif->rx_status);
	printf("[0x34] INT_SRC: 0x%.8x\n", hif->int_src);
	printf("[0x38] INT_ENABLE: 0x%.8x\n", hif->int_enable);
	printf("[0x3c] POLL_CTRL: 0x%.8x\n", hif->poll_ctrl);
	printf("[0x40] RX_CURR_BD_ADDR: 0x%.8x\n", hif->rx_curr_bd_addr);
	printf("[0x44] RX_ALLOC: 0x%.8x\n", hif->rx_alloc);
	printf("[0x48] TX_DMA_STATUS: 0x%.8x\n", hif->tx_dma_status);
	printf("[0x4c] RX_DMA_STATUS: 0x%.8x\n", hif->rx_dma_status);
	printf("[0x50] RX_INQ0_PKTPTR: 0x%.8x\n", hif->rx_inq0_pktptr);
	printf("[0x54] RX_INQ1_PKTPTR: 0x%.8x\n", hif->rx_inq1_pktptr);
	printf("[0x60] TX_PORT_NO: 0x%.8x\n", hif->tx_port_no);
	printf("[0x64] LMEM_ALLOC_ADDR: 0x%.8x\n", hif->lmem_alloc_addr);
	printf("[0x68] CLASS_ADDR: 0x%.8x\n", hif->class_addr);
	printf("[0x70] TMU_PORT0_ADDR: 0x%.8x\n", hif->tmu_port0_addr);
	printf("[0x74] TMU_PORT1_ADDR: 0x%.8x\n", hif->tmu_port1_addr);
	printf("[0x7c] TMU_PORT2_ADDR: 0x%.8x\n", hif->tmu_port2_addr);
	printf("[0x80] TMU_PORT3_ADDR: 0x%.8x\n", hif->tmu_port3_addr);
	printf("[0x84] TMU_PORT4_ADDR: 0x%.8x\n", hif->tmu_port4_addr);
	printf("[0x90] INT_COAL: 0x%.8x\n", hif->int_coal);
}

void hif_nocpy_dump(void)
{
	printf("==== HIF_NOCPY (0x%.8x) ====\n", CBUS_BASE + HIF_NOCPY_OFFSET);
	hif_nocpy_dump_regs(hif_nocpy);
}

static void init_hif_nocpy_rx_bds(void)
{
	const size_t NUM_BDS = HIF_RX_POOL_SIZE / sizeof(struct hif_desc);
	size_t i;
	volatile struct hif_desc *desc =
		(volatile struct hif_desc *)&ddr[HIF_RX_POOL_OFFSET];

	for (i = 0; i < NUM_BDS; i++, desc++) {
		uint32_t next = PFE_DDR_BASE + HIF_RX_POOL_OFFSET;
		uint32_t ctrl = HIF_DESC_CTRL_PKT_INT_EN | \
				HIF_DESC_CTRL_LIFM | HIF_DESC_CTRL_DIR | \
				HIF_DESC_CTRL_DESC_EN;
		desc->status = 0;
		desc->data = 0;
		if (i != (NUM_BDS - 1))
			next += (i + 1) * sizeof(struct hif_desc);
		desc->next = next;
		desc->ctrl = ctrl;
	}
}

static void init_hif_nocpy_tx_bds(void)
{
	const size_t NUM_BDS = HIF_TX_POOL_SIZE / sizeof(struct hif_desc);
	size_t i;
	volatile struct hif_desc *desc =
		(volatile struct hif_desc *)&ddr[HIF_TX_POOL_OFFSET];

	for (i = 0; i < NUM_BDS; i++, desc++) {
		uint32_t next = PFE_DDR_BASE + HIF_TX_POOL_OFFSET;
		desc->status = 0;
		desc->data = 0;
		if (i != (NUM_BDS - 1))
			next += (i + 1) * sizeof(struct hif_desc);
		desc->next = next;
		desc->ctrl = 0;
	}
}

void hif_nocpy_init(void)
{
	if (!ddr || !hif_nocpy)
		return;

	hif_nocpy->rx_ctrl = 0;
	hif_nocpy->tx_ctrl = 0;

	printf("Initializing Rx BDs...\n");
	init_hif_nocpy_rx_bds();
	printf("Initializing Tx BDs...\n");
	init_hif_nocpy_tx_bds();

	hif_nocpy->rx_bdp_addr = PFE_DDR_BASE + HIF_RX_POOL_OFFSET;
	hif_nocpy->tx_bdp_addr = PFE_DDR_BASE + HIF_TX_POOL_OFFSET;

	/* EGPI2 -> HIF_NOCPY */
	egpi2->class_addr = CBUS_BASE_PE + HIF_NOCPY_OFFSET + 0x50;
	/* HIF_NOCPY -> EGPI2 */
	hif_nocpy->tmu_port0_addr = CBUS_BASE_PE + EGPI2_OFFSET + 0x30;
	hif_nocpy->tmu_port1_addr = CBUS_BASE_PE + EGPI2_OFFSET + 0x30;

	hif_nocpy->poll_ctrl = 0x04000400;
	hif_nocpy->rx_ctrl = 0x5; /* DMA_EN | BDP_CH_START_WSTB */
	hif_nocpy->tx_ctrl = 0x5; /* DMA_EN | BDP_CH_START_WSTB */
	hif_nocpy->int_enable |= 0x15; /* INT | RXPKT_INT | TXPKT_INT */
}
