#ifndef REGS_H
#define REGS_H

#include <stdint.h>

#define PFE_DDR_BASE 0x03400000
#define PFE_DDR_SIZE 0x00c00000

#define HIF_RX_POOL_OFFSET 0xa80000
#define HIF_RX_POOL_SIZE 0x400
#define HIF_TX_POOL_OFFSET 0xa80400
#define HIF_TX_POOL_SIZE 0x400

#define LMEM_POOL_OFFSET 0x0
#define LMEM_POOL_SIZE 0x8000
#define LMEM_POOL_BUFLEN_LOG2 7
#define LMEM_POOL_BUFLEN_BYTES 0x80
#define LMEM_POOL_BUF_COUNT 0x100
#define LMEM_POOL_HDR_SIZE 0x10

#define DDR_POOL_OFFSET 0x400000
#define DDR_POOL_SIZE 0x600000
#if 0
#define DDR_POOL_BUFLEN_LOG2 11
#define DDR_POOL_BUFLEN_BYTES 0x800
#define DDR_POOL_BUF_COUNT 0xc00
#define DDR_POOL_HDR_SIZE 0x100
#else
#define DDR_POOL_BUFLEN_LOG2 8
#define DDR_POOL_BUFLEN_BYTES 0x100
#define DDR_POOL_BUF_COUNT 0x6000
#define DDR_POOL_HDR_SIZE 0x10
#endif

#define CBUS_BASE_PE 0xc0000000
#define CBUS_BASE 0x9c000000
#define CBUS_SIZE 0x01000000
#define EMAC1_OFFSET 0x200000
#define EGPI1_OFFSET 0x210000
#define EMAC2_OFFSET 0x220000
#define EGPI2_OFFSET 0x230000
#define BMU1_OFFSET 0x240000
#define BMU2_OFFSET 0x250000
#define HIF_OFFSET 0x280000
#define HGPI_OFFSET 0x290000
#define LMEM_OFFSET 0x300000
#define LMEM_SIZE 0x00010000
#define TMU_CSR_OFFSET 0x310000
#define CLASS_CSR_OFFSET 0x320000
#define EMAC3_OFFSET 0x330000
#define EGPI3_OFFSET 0x340000
#define HIF_NOCPY_OFFSET 0x350000
#define UTIL_CSR_OFFSET 0x360000

#define CLKRESET_BASE 0x904b0000
#define CLKRESET_AXI_CLK_CNTRL_1_OFFSET 0x44
#define CLKRESET_PFE_SYS_CLK_ENABLE 0x08
#define CLKRESET_AXI_RESET_1_OFFSET 0x54
#define CLKRESET_PFE_SYS_RST 0x08
#define CLKRESET_PFE_CLK_CNTRL_OFFSET 0x100
#define CLKRESET_PFE_CORE_CLK_ENABLE 0x01

#define PE_MEM_ACCESS_ADDR_MASK 0x00fffffc
#define PE_MEM_ACCESS_WRITE (1 << 31)
#define PE_MEM_ACCESS_IMEM (1 << 15)
#define PE_MEM_ACCESS_DMEM (1 << 16)
#define PE_MEM_ACCESS_PE_ID(i) ((uint32_t)(i) << 20)
#define PE_MEM_ACCESS_BYTE_ENABLE(e) ((uint32_t)(e) << 24)

#define HIF_INTSRC_INT (1 << 0)
#define HIF_INTSRC_RXBD (1 << 1)
#define HIF_INTSRC_RXPKT (1 << 2)
#define HIF_INTSRC_TXBD (1 << 3)
#define HIF_INTSRC_TXPKT (1 << 4)

#define HIF_DESC_CTRL_BUFLEN_MASK 0x3fff
#define HIF_DESC_CTRL_CBD_INT_EN (1 << 16)
#define HIF_DESC_CTRL_PKT_INT_EN (1 << 17)
/* Last buffer in frame */
#define HIF_DESC_CTRL_LIFM (1 << 18)
#define HIF_DESC_CTRL_LAST_BD (1 << 19)
#define HIF_DESC_CTRL_DIR (1 << 20)
#define HIF_DESC_CTRL_LMEM_CPY (1 << 21)
#define HIF_DESC_CTRL_PKT_XFER (1 << 24)
#define HIF_DESC_CTRL_PARSE_DISABLE (1 << 25)
#define HIF_DESC_CTRL_BRFETCH_DISABLE (1 << 26)
#define HIF_DESC_CTRL_RTFETCH_DISABLE (1 << 27)
#define HIF_DESC_CTRL_DESC_EN (1 << 31)

struct gpi_regs {
	uint32_t version; /* 0x00 */
	uint32_t ctrl; /* 0x04 */
	uint32_t rx_config; /* 0x08 */
	uint32_t hdr_size; /* 0x0c */
	uint32_t buf_size; /* 0x10 */
	uint32_t lmem_alloc_addr; /* 0x14 */
	uint32_t lmem_free_addr; /* 0x18 */
	uint32_t ddr_alloc_addr; /* 0x1c */
	uint32_t ddr_free_addr; /* 0x20 */
	uint32_t class_addr; /* 0x24 */
	uint32_t drx_fifo; /* 0x28 */
	uint32_t trx_fifo; /* 0x2c */
	uint32_t inq_pktptr; /* 0x30 */
	uint32_t ddr_data_offset; /* 0x34 */
	uint32_t lmem_data_offset; /* 0x38 */
	uint32_t rsvd1[4];
	uint32_t tmlf_tx; /* 0x4c */
	uint32_t dtx_aseq; /* 0x50 */
	uint32_t fifo_status; /* 0x54 */
	uint32_t fifo_debug; /* 0x58 */
	uint32_t tx_pause_time; /* 0x5c */
	uint32_t lmem_sec_buf_data_offset; /* 0x60 */
	uint32_t ddr_sec_buf_data_offset; /* 0x64 */
};

struct bmu_regs {
	uint32_t version; /* 0x00 */
	uint32_t ctrl; /* 0x04 */
	uint32_t ucast_config; /* 0x08 */
	uint32_t ucast_base_addr; /* 0x0c */
	uint32_t buf_size; /* 0x10 */
	uint32_t buf_cnt; /* 0x14 */
	uint32_t thres; /* 0x18 */
	uint32_t rsvd1;
	uint32_t int_src; /* 0x20 */
	uint32_t int_enable; /* 0x24 */
	uint32_t rsvd2[2];
	uint32_t alloc_ctrl; /* 0x30 */
	uint32_t free_ctrl; /* 0x34 */
	uint32_t free_err_addr; /* 0x38 */
	uint32_t curr_buf_cnt; /* 0x3c */
	uint32_t mcast_cnt; /* 0x40 */
	uint32_t mcast_alloc_ctrl; /* 0x44 */
	uint32_t rem_buf_cnt; /* 0x48 */
	uint32_t rsvd3;
	uint32_t low_watermark; /* 0x50 */
	uint32_t high_watermark; /* 0x54 */
};

struct emac_regs {
	uint32_t network_control; /* 0x000 */
	uint32_t network_config; /* 0x004 */
	uint32_t network_status; /* 0x008 */
	uint32_t rsvd1; /* 0x00c */
	uint32_t dma_config; /* 0x010 */
	uint32_t rsvd2[8]; /* 0x014 */
	uint32_t phy_management; /* 0x034 */
	uint32_t rsvd3[18]; /* 0x038 */
	uint32_t hash_bot; /* 0x080 */
	uint32_t hash_top; /* 0x084 */
	uint32_t spec1_add_bot; /* 0x088 */
	uint32_t spec1_add_top; /* 0x08c */
	uint32_t spec2_add_bot; /* 0x090 */
	uint32_t spec2_add_top; /* 0x094 */
	uint32_t spec3_add_bot; /* 0x098 */
	uint32_t spec3_add_top; /* 0x09c */
	uint32_t spec4_add_bot; /* 0x0a0 */
	uint32_t spec4_add_top; /* 0x0a4 */
	uint32_t rsvd4[4]; /* 0x0a8 */
	uint32_t wol_enable; /* 0x0b8 */
	uint32_t rsvd5[3]; /* 0x0bc */
	uint32_t spec1_add_mask_bot; /* 0x0c8 */
	uint32_t spec1_add_mask_top; /* 0x0cc */
	uint32_t rsvd6[12]; /* 0x0d0 */
	uint32_t octets_tx_bot; /* 0x100 */ /* Lower 32-bits for number of octets tx'd */
	uint32_t octets_tx_top; /* 0x104 */ /* Upper 16-bits for number of octets tx'd */
	uint32_t frames_tx; /* 0x108 */ /* Number of frames transmitted OK */
	uint32_t broadcast_tx; /* 0x10c */ /* Number of broadcast frames transmitted */
	uint32_t multicast_tx; /* 0x110 */ /* Number of multicast frames transmitted */
	uint32_t pause_tx; /* 0x114 */ /* Number of pause frames transmitted. */
	uint32_t frame64_tx; /* 0x118 */ /* Number of 64byte frames transmitted */
	uint32_t frame65_127_tx; /* 0x11c */ /* Number of 65-127 byte frames transmitted */
	uint32_t frame128_255_tx; /* 0x120 */ /* Number of 128-255 byte frames transmitted */
	uint32_t frame256_511_tx; /* 0x124 */ /* Number of 256-511 byte frames transmitted */
	uint32_t frame512_1023_tx; /* 0x128 */ /* Number of 512-1023 byte frames transmitted */
	uint32_t frame1024_1518_tx; /* 0x12c */ /* Number of 1024-1518 byte frames transmitted*/
	uint32_t frame1519_tx; /* 0x130 */ /* Number of frames greater than 1518 bytes tx*/
	uint32_t tx_urun; /* 0x134 */ /* Transmit underrun errors due to DMA */
	uint32_t single_col; /* 0x138 */ /* Number of single collision frames */
	uint32_t multi_col; /* 0x13c */ /* Number of multi collision frames */
	uint32_t excess_col; /* 0x140 */ /* Number of excessive collision frames. */
	uint32_t late_col; /* 0x144 */ /* Collisions occuring after slot time */
	uint32_t def_tx; /* 0x148 */ /* Frames deferred due to crs */
	uint32_t crs_errors; /* 0x14c */ /* Errors caused by crs not being asserted. */
	uint32_t octets_rx_bot; /* 0x150 */ /* Lower 32-bits for number of octets rx'd */
	uint32_t octets_rx_top; /* 0x154 */ /* Upper 16-bits for number of octets rx'd */
	uint32_t frames_rx; /* 0x158 */ /* Number of frames received OK */
	uint32_t broadcast_rx; /* 0x15c */ /* Number of broadcast frames received */
	uint32_t multicast_rx; /* 0x160 */ /* Number of multicast frames received */
	uint32_t pause_rx; /* 0x164 */ /* Number of pause frames received. */
	uint32_t frame64_rx; /* 0x168 */ /* Number of 64byte frames received */
	uint32_t frame65_127_rx; /* 0x16c */ /* Number of 65-127 byte frames received */
	uint32_t frame128_255_rx; /* 0x170 */ /* Number of 128-255 byte frames received */
	uint32_t frame256_511_rx; /* 0x174 */ /* Number of 256-511 byte frames received */
	uint32_t frame512_1023_rx; /* 0x178 */ /* Number of 512-1023 byte frames received */
	uint32_t frame1024_1518_rx; /* 0x17c */ /* Number of 1024-1518 byte frames received*/
	uint32_t frame1519_rx; /* 0x180 */ /* Number of frames greater than 1518 bytes rx*/
	uint32_t usize_frames; /* 0x184 */ /* Frames received less than min of 64 bytes */
	uint32_t excess_length; /* 0x188 */ /* Number of excessive length frames rx */
	uint32_t jabbers; /* 0x18c */ /* Excessive length + crc or align errors. */
	uint32_t fcs_errors; /* 0x190 */ /* Number of frames received with crc errors */
	uint32_t length_check_errors; /* 0x194 */ /* Number of frames with incorrect length */
	uint32_t rx_symbol_errors; /* 0x198 */ /* Number of times rx_er asserted during rx */
	uint32_t align_errors; /* 0x19c */ /* Frames received without integer no. bytes */
	uint32_t rx_res_errors; /* 0x1a0 */ /* Number of times buffers ran out during rx */
	uint32_t rx_orun; /* 0x1a4 */ /* Receive overrun errors due to DMA */
	uint32_t ip_cksum; /* 0x1a8 */ /* IP header checksum errors */
	uint32_t tcp_cksum; /* 0x1ac */ /* TCP checksum errors */
	uint32_t udp_cksum; /* 0x1b0 */ /* UDP checksum errors */
	uint32_t rsvd7[83]; /* 0x1b4 */
	uint32_t spec5_add_bot; /* 0x300 */
	uint32_t spec5_add_top; /* 0x304 */
	uint32_t spec6_add_bot; /* 0x308 */
	uint32_t spec6_add_top; /* 0x30c */
	uint32_t spec7_add_bot; /* 0x310 */
	uint32_t spec7_add_top; /* 0x314 */
	uint32_t spec8_add_bot; /* 0x318 */
	uint32_t spec8_add_top; /* 0x31c */
	uint32_t spec9_add_bot; /* 0x320 */
	uint32_t spec9_add_top; /* 0x324 */
	uint32_t spec10_add_bot; /* 0x328 */
	uint32_t spec10_add_top; /* 0x32c */
	uint32_t spec11_add_bot; /* 0x330 */
	uint32_t spec11_add_top; /* 0x334 */
	uint32_t spec12_add_bot; /* 0x338 */
	uint32_t spec12_add_top; /* 0x33c */
	uint32_t spec13_add_bot; /* 0x340 */
	uint32_t spec13_add_top; /* 0x344 */
	uint32_t spec14_add_bot; /* 0x348 */
	uint32_t spec14_add_top; /* 0x34c */
	uint32_t spec15_add_bot; /* 0x350 */
	uint32_t spec15_add_top; /* 0x354 */
	uint32_t spec16_add_bot; /* 0x358 */
	uint32_t spec16_add_top; /* 0x35c */
	uint32_t spec17_add_bot; /* 0x360 */
	uint32_t spec17_add_top; /* 0x364 */
	uint32_t spec18_add_bot; /* 0x368 */
	uint32_t spec18_add_top; /* 0x36c */
	uint32_t spec19_add_bot; /* 0x370 */
	uint32_t spec19_add_top; /* 0x374 */
	uint32_t spec20_add_bot; /* 0x378 */
	uint32_t spec20_add_top; /* 0x37c */
	uint32_t spec21_add_bot; /* 0x380 */
	uint32_t spec21_add_top; /* 0x384 */
	uint32_t spec22_add_bot; /* 0x388 */
	uint32_t spec22_add_top; /* 0x38c */
	uint32_t spec23_add_bot; /* 0x390 */
	uint32_t spec23_add_top; /* 0x394 */
	uint32_t spec24_add_bot; /* 0x398 */
	uint32_t spec24_add_top; /* 0x39c */
	uint32_t spec25_add_bot; /* 0x3a0 */
	uint32_t spec25_add_top; /* 0x3a4 */
	uint32_t spec26_add_bot; /* 0x3a8 */
	uint32_t spec26_add_top; /* 0x3ac */
	uint32_t spec27_add_bot; /* 0x3b0 */
	uint32_t spec27_add_top; /* 0x3b4 */
	uint32_t spec28_add_bot; /* 0x3b8 */
	uint32_t spec28_add_top; /* 0x3bc */
	uint32_t spec29_add_bot; /* 0x3c0 */
	uint32_t spec29_add_top; /* 0x3c4 */
	uint32_t spec30_add_bot; /* 0x3c8 */
	uint32_t spec30_add_top; /* 0x3cc */
	uint32_t spec31_add_bot; /* 0x3d0 */
	uint32_t spec31_add_top; /* 0x3d4 */
	uint32_t spec32_add_bot; /* 0x3d8 */
	uint32_t spec32_add_top; /* 0x3dc */
	uint32_t rsvd8[240]; /* 0x3e0 */
	uint32_t control; /* 0x7a0 */
};

struct hif_regs {
	uint32_t version; /* 0x00 */
	uint32_t tx_ctrl; /* 0x04 */
	uint32_t tx_curr_bd_addr; /* 0x08 */
	uint32_t tx_alloc; /* 0x0c */
	uint32_t tx_bdp_addr; /* 0x10 */
	uint32_t tx_status; /* 0x14 */
	uint32_t rsvd[2];
	uint32_t rx_ctrl; /* 0x20 */
	uint32_t rx_bdp_addr; /* 0x24 */
	uint32_t rsvd2[2];
	uint32_t rx_status; /* 0x30 */
	uint32_t int_src; /* 0x34 */
	uint32_t int_enable; /* 0x38 */
	uint32_t poll_ctrl; /* 0x3c */
	uint32_t rx_curr_bd_addr; /* 0x40 */
	uint32_t rx_alloc; /* 0x44 */
	uint32_t tx_dma_status; /* 0x48 */
	uint32_t rx_dma_status; /* 0x4c */
	uint32_t int_coal; /* 0x50 */
};

struct hif_nocpy_regs {
	uint32_t version; /* 0x00 */
	uint32_t tx_ctrl; /* 0x04 */
	uint32_t tx_curr_bd_addr; /* 0x08 */
	uint32_t tx_alloc; /* 0x0c */
	uint32_t tx_bdp_addr; /* 0x10 */
	uint32_t tx_status; /* 0x14 */
	uint32_t rsvd[2];
	uint32_t rx_ctrl; /* 0x20 */
	uint32_t rx_bdp_addr; /* 0x24 */
	uint32_t rsvd2[2];
	uint32_t rx_status; /* 0x30 */
	uint32_t int_src; /* 0x34 */
	uint32_t int_enable; /* 0x38 */
	uint32_t poll_ctrl; /* 0x3c */
	uint32_t rx_curr_bd_addr; /* 0x40 */
	uint32_t rx_alloc; /* 0x44 */
	uint32_t tx_dma_status; /* 0x48 */
	uint32_t rx_dma_status; /* 0x4c */
	uint32_t rx_inq0_pktptr; /* 0x50 */
	uint32_t rx_inq1_pktptr; /* 0x54 */
	uint32_t rsvd3[2];
	uint32_t tx_port_no; /* 0x60 */
	uint32_t lmem_alloc_addr; /* 0x64 */
	uint32_t class_addr; /* 0x68 */
	uint32_t rsvd4;
	uint32_t tmu_port0_addr; /* 0x70 */
	uint32_t tmu_port1_addr; /* 0x74 */
	uint32_t rsvd5;
	uint32_t tmu_port2_addr; /* 0x7c */
	uint32_t tmu_port3_addr; /* 0x80 */
	uint32_t tmu_port4_addr; /* 0x84 */
	uint32_t rsvd6[2];
	uint32_t int_coal; /* 0x90 */
};

struct hif_desc {
	uint32_t ctrl;
	uint32_t status;
	uint32_t data;
	uint32_t next;
};

struct class_rx_hdr {
	uint32_t next_ptr; /* ptr to the start of the first DDR buffer */
	uint16_t length; /* total packet length (possibly including FCS) */
	uint16_t phyno; /* input physical port number */
	uint32_t status; /* gemac status bits bits[32:63]*/
	uint32_t status2; /* gemac status bits bits[0:31] */
};

struct tmu_csr_regs {
	uint32_t version; /* 0x000 */
	uint32_t inq_watermark; /* 0x004 */
	uint32_t phy_inq_pktptr; /* 0x008 */
	uint32_t phy_inq_pktinfo; /* 0x00c */
	uint32_t phy_inq_fifo_cnt; /* 0x010 */
	uint32_t sys_generic_control; /* 0x014 */
	uint32_t sys_generic_static; /* 0x018 */
	uint32_t sys_gen_con0; /* 0x01c */
	uint32_t sys_gen_con1; /* 0x020 */
	uint32_t sys_gen_con2; /* 0x024 */
	uint32_t sys_gen_con3; /* 0x028 */
	uint32_t sys_gen_con4; /* 0x02c */
	uint32_t teq_disable_dropchk; /* 0x030 */
	uint32_t teq_ctrl; /* 0x034 */
	uint32_t teq_qcfg; /* 0x038 */
	uint32_t teq_drop_stat; /* 0x03c */
	uint32_t teq_qavg; /* 0x040 */
	uint32_t teq_wreg_prob; /* 0x044 */
	uint32_t teq_trans_stat; /* 0x048 */
	uint32_t teq_hw_prob_cfg0; /* 0x04c */
	uint32_t teq_hw_prob_cfg1; /* 0x050 */
	uint32_t teq_hw_prob_cfg2; /* 0x054 */
	uint32_t teq_hw_prob_cfg3; /* 0x058 */
	uint32_t teq_hw_prob_cfg4; /* 0x05c */
	uint32_t teq_hw_prob_cfg5; /* 0x060 */
	uint32_t teq_hw_prob_cfg6; /* 0x064 */
	uint32_t teq_hw_prob_cfg7; /* 0x068 */
	uint32_t teq_hw_prob_cfg8; /* 0x06c */
	uint32_t teq_hw_prob_cfg9; /* 0x070 */
	uint32_t teq_hw_prob_cfg10; /* 0x074 */
	uint32_t teq_hw_prob_cfg11; /* 0x078 */
	uint32_t teq_hw_prob_cfg12; /* 0x07c */
	uint32_t teq_hw_prob_cfg13; /* 0x080 */
	uint32_t teq_hw_prob_cfg14; /* 0x084 */
	uint32_t teq_hw_prob_cfg15; /* 0x088 */
	uint32_t teq_hw_prob_cfg16; /* 0x08c */
	uint32_t teq_hw_prob_cfg17; /* 0x090 */
	uint32_t teq_hw_prob_cfg18; /* 0x094 */
	uint32_t teq_hw_prob_cfg19; /* 0x098 */
	uint32_t teq_hw_prob_cfg20; /* 0x09c */
	uint32_t teq_hw_prob_cfg21; /* 0x0a0 */
	uint32_t teq_hw_prob_cfg22; /* 0x0a4 */
	uint32_t teq_hw_prob_cfg23; /* 0x0a8 */
	uint32_t teq_hw_prob_cfg24; /* 0x0ac */
	uint32_t teq_hw_prob_cfg25; /* 0x0b0 */
	uint32_t tdq_iifg_cfg; /* 0x0b4 */
	uint32_t tdq0_sch_ctrl; /* 0x0b8 */
	uint32_t llm_ctrl; /* 0x0bc */
	uint32_t llm_base_addr; /* 0x0c0 */
	uint32_t llm_que_len; /* 0x0c4 */
	uint32_t llm_que_headptr; /* 0x0c8 */
	uint32_t llm_que_tailptr; /* 0x0cc */
	uint32_t llm_que_dropcnt; /* 0x0d0 */
	uint32_t int_en; /* 0x0d4 */
	uint32_t int_src; /* 0x0d8 */
	uint32_t inq_stat; /* 0x0dc */
	uint32_t ctrl; /* 0x0e0 */
	uint32_t mem_access_addr; /* 0x0e4 */
	uint32_t mem_access_wdata; /* 0x0e8 */
	uint32_t mem_access_rdata; /* 0x0ec */
	uint32_t phy0_inq_addr; /* 0x0f0 */
	uint32_t phy1_inq_addr; /* 0x0f4 */
	uint32_t phy2_inq_addr; /* 0x0f8 */
	uint32_t phy3_inq_addr; /* 0x0fc */
	uint32_t bmu_inq_addr; /* 0x100 */
	uint32_t tx_ctrl; /* 0x104 */
	uint32_t bus_access_wdata; /* 0x108 */
	uint32_t bus_access; /* 0x10c */
	uint32_t bus_access_rdata; /* 0x110 */
	uint32_t pe_sys_clk_ratio; /* 0x114 */
	uint32_t pe_status; /* 0x118 */
	uint32_t teq_max_threshold; /* 0x11c */
	uint32_t rsvd1[5];
	uint32_t phy4_inq_addr; /* 0x134 */
	uint32_t tdq1_sch_ctrl; /* 0x138 */
	uint32_t tdq2_sch_ctrl; /* 0x13c */
	uint32_t tdq3_sch_ctrl; /* 0x140 */
	uint32_t bmu_buf_size; /* 0x144 */
	uint32_t undocumented[12];
};

struct class_csr_regs {
	uint32_t version; /* 0x000 */
	uint32_t tx_ctrl; /* 0x004 */
	uint32_t rsvd1[2];
	uint32_t inq_pktptr; /* 0x010 */
	uint32_t hdr_size; /* 0x014 */
	uint32_t rsvd2[2];
	uint32_t pe0_qb_dm_addr0; /* 0x020 */
	uint32_t pe0_qb_dm_addr1; /* 0x024 */
	uint32_t rsvd3[14];
	uint32_t pe0_ro_dm_addr0; /* 0x060 */
	uint32_t pe0_ro_dm_addr1; /* 0x064 */
	uint32_t rsvd4[38];
	uint32_t mem_access_addr; /* 0x100 */
	uint32_t mem_access_wdata; /* 0x104 */
	uint32_t mem_access_rdata; /* 0x108 */
	uint32_t rsvd5[2];
	uint32_t tm_inq_addr; /* 0x114 */
	uint32_t pe_status; /* 0x118 */
	uint32_t phy1_rx_pkts; /* 0x11c */
	uint32_t phy1_tx_pkts; /* 0x120 */
	uint32_t phy1_lp_fail_pkts; /* 0x124 */
	uint32_t phy1_intf_fail_pkts; /* 0x128 */
	uint32_t phy1_intf_match_pkts; /* 0x12c */
	uint32_t phy1_l3_fail_pkts; /* 0x130 */
	uint32_t phy1_v4_pkts; /* 0x134 */
	uint32_t phy1_v6_pkts; /* 0x138 */
	uint32_t phy1_chksum_err_pkts; /* 0x13c */
	uint32_t phy1_ttl_err_pkts; /* 0x140 */
	uint32_t phy2_rx_pkts; /* 0x144 */
	uint32_t phy2_tx_pkts; /* 0x148 */
	uint32_t phy2_lp_fail_pkts; /* 0x14c */
	uint32_t phy2_intf_fail_pkts; /* 0x150 */
	uint32_t phy2_intf_match_pkts; /* 0x154 */
	uint32_t phy2_l3_fail_pkts; /* 0x158 */
	uint32_t phy2_v4_pkts; /* 0x15c */
	uint32_t phy2_v6_pkts; /* 0x160 */
	uint32_t phy2_chksum_err_pkts; /* 0x164 */
	uint32_t phy2_ttl_err_pkts; /* 0x168 */
	uint32_t phy3_rx_pkts; /* 0x16c */
	uint32_t phy3_tx_pkts; /* 0x170 */
	uint32_t phy3_lp_fail_pkts; /* 0x174 */
	uint32_t phy3_intf_fail_pkts; /* 0x178 */
	uint32_t phy3_intf_match_pkts; /* 0x17c */
	uint32_t phy3_l3_fail_pkts; /* 0x180 */
	uint32_t phy3_v4_pkts; /* 0x184 */
	uint32_t phy3_v6_pkts; /* 0x188 */
	uint32_t phy3_chksum_err_pkts; /* 0x18c */
	uint32_t phy3_ttl_err_pkts; /* 0x190 */
	uint32_t phy1_icmp_pkts; /* 0x194 */
	uint32_t phy1_igmp_pkts; /* 0x198 */
	uint32_t phy1_tcp_pkts; /* 0x19c */
	uint32_t phy1_udp_pkts; /* 0x1a0 */
	uint32_t phy2_icmp_pkts; /* 0x1a4 */
	uint32_t phy2_igmp_pkts; /* 0x1a8 */
	uint32_t phy2_tcp_pkts; /* 0x1ac */
	uint32_t phy2_udp_pkts; /* 0x1b0 */
	uint32_t phy3_icmp_pkts; /* 0x1b4 */
	uint32_t phy3_igmp_pkts; /* 0x1b8 */
	uint32_t phy3_tcp_pkts; /* 0x1bc */
	uint32_t phy3_udp_pkts; /* 0x1c0 */
	uint32_t phy4_icmp_pkts; /* 0x1c4 */
	uint32_t phy4_igmp_pkts; /* 0x1c8 */
	uint32_t phy4_tcp_pkts; /* 0x1cc */
	uint32_t phy4_udp_pkts; /* 0x1d0 */
	uint32_t phy4_rx_pkts; /* 0x1d4 */
	uint32_t phy4_tx_pkts; /* 0x1d8 */
	uint32_t phy4_lp_fail_pkts; /* 0x1dc */
	uint32_t phy4_intf_fail_pkts; /* 0x1e0 */
	uint32_t phy4_intf_match_pkts; /* 0x1e4 */
	uint32_t phy4_l3_fail_pkts; /* 0x1e8 */
	uint32_t phy4_v4_pkts; /* 0x1ec */
	uint32_t phy4_v6_pkts; /* 0x1f0 */
	uint32_t phy4_chksum_err_pkts; /* 0x1f4 */
	uint32_t phy4_ttl_err_pkts; /* 0x1f8 */
	uint32_t rsvd6; /* 0x1fc */
	uint32_t pe_sys_clk_ratio; /* 0x200 */
	uint32_t afull_thres; /* 0x204 */
	uint32_t gap_between_reads; /* 0x208 */
	uint32_t max_buf_cnt; /* 0x20c */
	uint32_t tsq_fifo_thres; /* 0x210 */
	uint32_t tsq_max_cnt; /* 0x214 */
	uint32_t iram_data_0; /* 0x218 */
	uint32_t iram_data_1; /* 0x21c */
	uint32_t iram_data_2; /* 0x220 */
	uint32_t iram_data_3; /* 0x224 */
	uint32_t bus_access_addr; /* 0x228 */
	uint32_t bus_access_wdata; /* 0x22c */
	uint32_t bus_access_rdata; /* 0x230 */
	uint32_t route_hash_entry_size; /* 0x234 */
	uint32_t route_table_base; /* 0x238 */
	uint32_t route_multi; /* 0x23c */
	uint32_t smem_offset; /* 0x240 */
	uint32_t lmem_buf_size; /* 0x244 */
	uint32_t vlan_id; /* 0x248 */
	uint32_t bmu1_buf_free; /* 0x24c */
	uint32_t use_tmu_inq; /* 0x250 */
	uint32_t vlan_id1; /* 0x254 */
	uint32_t bus_access_base; /* 0x258 */
	uint32_t hif_parse; /* 0x25c */
	uint32_t host_pe0_gp; /* 0x260 */
	uint32_t pe0_gp; /* 0x264 */
	uint32_t host_pe1_gp; /* 0x268 */
	uint32_t pe1_gp; /* 0x26c */
	uint32_t host_pe2_gp; /* 0x270 */
	uint32_t pe2_gp; /* 0x274 */
	uint32_t host_pe3_gp; /* 0x278 */
	uint32_t pe3_gp; /* 0x27c */
	uint32_t host_pe4_gp; /* 0x280 */
	uint32_t pe4_gp; /* 0x284 */
	uint32_t host_pe5_gp; /* 0x288 */
	uint32_t pe5_gp; /* 0x28c */
	uint32_t pe_int_src; /* 0x290 */
	uint32_t pe_int_enable; /* 0x294 */
	uint32_t tpid0_tpid1; /* 0x298 */
	uint32_t tpid2; /* 0x29c */
	uint32_t l4_chksum_addr; /* 0x2a0 */
	uint32_t pe0_debug; /* 0x2a4 */
	uint32_t pe1_debug; /* 0x2a8 */
	uint32_t pe2_debug; /* 0x2ac */
	uint32_t pe3_debug; /* 0x2b0 */
	uint32_t pe4_debug; /* 0x2b4 */
	uint32_t pe5_debug; /* 0x2b8 */
	uint32_t state; /* 0x2bc */
};

struct util_csr_regs {
	uint32_t version; /* 0x000 */
	uint32_t tx_ctrl; /* 0x004 */
	uint32_t rsvd1[2];
	uint32_t inq_pktptr; /* 0x010 */
	uint32_t hdr_size; /* 0x014 */
	uint32_t rsvd2[2];
	uint32_t pe0_qb_dm_addr0; /* 0x020 */
	uint32_t pe0_qb_dm_addr1; /* 0x024 */
	uint32_t rsvd3[14];
	uint32_t pe0_ro_dm_addr0; /* 0x060 */
	uint32_t pe0_ro_dm_addr1; /* 0x064 */
	uint32_t rsvd4[38];
	uint32_t mem_access_addr; /* 0x100 */
	uint32_t mem_access_wdata; /* 0x104 */
	uint32_t mem_access_rdata; /* 0x108 */
	uint32_t rsvd5[2];
	uint32_t tm_inq_addr; /* 0x114 */
	uint32_t pe_status; /* 0x118 */
	uint32_t rsvd6[58];
	uint32_t pe_sys_clk_ratio; /* 0x200 */
	uint32_t afull_thres; /* 0x204 */
	uint32_t gap_between_reads; /* 0x208 */
	uint32_t max_buf_cnt; /* 0x20c */
	uint32_t tsq_fifo_thres; /* 0x210 */
	uint32_t tsq_max_cnt; /* 0x214 */
	uint32_t iram_data_0; /* 0x218 */
	uint32_t iram_data_1; /* 0x21c */
	uint32_t iram_data_2; /* 0x220 */
	uint32_t iram_data_3; /* 0x224 */
	uint32_t bus_access_addr; /* 0x228 */
	uint32_t bus_access_wdata; /* 0x22c */
	uint32_t bus_access_rdata; /* 0x230 */
	uint32_t inq_afull_thres; /* 0x234 */
};
#endif
