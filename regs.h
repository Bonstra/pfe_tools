#ifndef REGS_H
#define REGS_H

#include <stdint.h>

#define PFE_DDR_BASE 0x03400000
#define PFE_DDR_SIZE 0x00c00000

#define HIF_RX_POOL_OFFSET 0xa80000
#define HIF_RX_POOL_SIZE 0x400
#define HIF_TX_POOL_OFFSET 0xa80400
#define HIF_TX_POOL_SIZE 0x400

#define CBUS_BASE_PE 0xc0000000
#define CBUS_BASE 0x9c000000
#define CBUS_SIZE 0x01000000
#define EGPI1_OFFSET 0x210000
#define EGPI2_OFFSET 0x230000
#define BMU1_OFFSET 0x240000
#define BMU2_OFFSET 0x250000
#define HIF_OFFSET 0x280000
#define HGPI_OFFSET 0x290000
#define LMEM_OFFSET 0x300000
#define LMEM_SIZE 0x00010000
#define TMU_CSR_OFFSET 0x310000
#define CLASS_CSR_OFFSET 0x320000
#define EGPI3_OFFSET 0x340000
#define HIF_NOCPY_OFFSET 0x350000
#define UTIL_CSR_OFFSET 0x360000

#define CLKRESET_BASE 0x904b0000
#define CLKRESET_AXI_CLK_CNTRL_1_OFFSET 0x44
#define CLKRESET_PFE_SYS_CLK_ENABLE 0x08
#define CLKRESET_PFE_CLK_CNTRL_OFFSET 0x100
#define CLKRESET_PFE_CORE_CLK_ENABLE 0x01

#define PE_MEM_ACCESS_ADDR_MASK 0x00fffffc
#define PE_MEM_ACCESS_WRITE (1 << 31)
#define PE_MEM_ACCESS_IMEM (1 << 15)
#define PE_MEM_ACCESS_DMEM (1 << 16)
#define PE_MEM_ACCESS_PE_ID(i) ((uint32_t)(i) << 20)
#define PE_MEM_ACCESS_BYTE_ENABLE(e) ((uint32_t)(e) << 24)

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
