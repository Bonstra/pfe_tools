#include <errno.h>
#include <stdio.h>

#include "hw.h"

void emac_init(volatile struct emac_regs *emac)
{
	if (!emac)
		return;

	emac->control = 0x00000004;
	emac->network_config = 0x01282003;
	emac->dma_config = 0x00020004;
	emac->network_control = 0x0000000c;
	emac->spec1_add_bot = 0x009b0800;
	emac->spec1_add_top = 0x00000100;
}

void emac_dump_regs(volatile struct emac_regs *emac)
{
	printf("[0x000] NETWORK_CONTROL: 0x%.8x\n", emac->network_control);
	printf("[0x004] NETWORK_CONFIG: 0x%.8x\n", emac->network_config);
	printf("[0x008] NETWORK_STATUS: 0x%.8x\n", emac->network_status);
	printf("[0x010] DMA_CONFIG: 0x%.8x\n", emac->dma_config);
	printf("[0x034] PHY_MANAGEMENT: 0x%.8x\n", emac->phy_management);
	printf("[0x080] HASB_BOT: 0x%.8x\n", emac->hash_bot);
	printf("[0x084] HASH_TOP: 0x%.8x\n", emac->hash_top);
	printf("[0x088] SPEC1_ADD_BOT: 0x%.8x\n", emac->spec1_add_bot);
	printf("[0x08c] SPEC1_ADD_TOP: 0x%.8x\n", emac->spec1_add_top);
	printf("[0x090] SPEC2_ADD_BOT: 0x%.8x\n", emac->spec2_add_bot);
	printf("[0x094] SPEC2_ADD_TOP: 0x%.8x\n", emac->spec2_add_top);
	printf("[0x098] SPEC3_ADD_BOT: 0x%.8x\n", emac->spec3_add_bot);
	printf("[0x09c] SPEC3_ADD_TOP: 0x%.8x\n", emac->spec3_add_top);
	printf("[0x0a0] SPEC4_ADD_BOT: 0x%.8x\n", emac->spec4_add_bot);
	printf("[0x0a4] SPEC4_ADD_TOP: 0x%.8x\n", emac->spec4_add_top);
	printf("[0x0b8] WOL_ENABLE: 0x%.8x\n", emac->wol_enable);
	printf("[0x0c8] SPEC1_ADD_MASK_BOT: 0x%.8x\n", emac->spec1_add_mask_bot);
	printf("[0x0cc] SPEC1_ADD_MASK_TOP: 0x%.8x\n", emac->spec1_add_mask_top);
	printf("[0x100] OCTETS_TX_BOT: 0x%.8x\n", emac->octets_tx_bot);
	printf("[0x104] OCTETS_TX_TOP: 0x%.8x\n", emac->octets_tx_top);
	printf("[0x108] FRAMES_TX: 0x%.8x\n", emac->frames_tx);
	printf("[0x10c] BROADCAST_TX: 0x%.8x\n", emac->broadcast_tx);
	printf("[0x110] MULTICAST_TX: 0x%.8x\n", emac->multicast_tx);
	printf("[0x114] PAUSE_TX: 0x%.8x\n", emac->pause_tx);
	printf("[0x118] FRAME64_TX: 0x%.8x\n", emac->frame64_tx);
	printf("[0x11c] FRAME65_127_TX: 0x%.8x\n", emac->frame65_127_tx);
	printf("[0x120] FRAME128_255_TX: 0x%.8x\n", emac->frame128_255_tx);
	printf("[0x124] FRAME256_511_TX: 0x%.8x\n", emac->frame256_511_tx);
	printf("[0x128] FRAME512_1023_TX: 0x%.8x\n", emac->frame512_1023_tx);
	printf("[0x12c] FRAME1024_1518_TX: 0x%.8x\n", emac->frame1024_1518_tx);
	printf("[0x130] FRAME1519_TX: 0x%.8x\n", emac->frame1519_tx);
	printf("[0x134] TX_URUN: 0x%.8x\n", emac->tx_urun);
	printf("[0x138] SINGLE_COL: 0x%.8x\n", emac->single_col);
	printf("[0x13c] MULTI_COL: 0x%.8x\n", emac->multi_col);
	printf("[0x140] EXCESS_COL: 0x%.8x\n", emac->excess_col);
	printf("[0x144] LATE_COL: 0x%.8x\n", emac->late_col);
	printf("[0x148] DEF_TX: 0x%.8x\n", emac->def_tx);
	printf("[0x14c] CRS_ERRORS: 0x%.8x\n", emac->crs_errors);
	printf("[0x150] OCTETS_RX_BOT: 0x%.8x\n", emac->octets_rx_bot);
	printf("[0x154] OCTETS_RX_TOP: 0x%.8x\n", emac->octets_rx_top);
	printf("[0x158] FRAMES_RX: 0x%.8x\n", emac->frames_rx);
	printf("[0x15c] BROADCAST_RX: 0x%.8x\n", emac->broadcast_rx);
	printf("[0x160] MULTICAST_RX: 0x%.8x\n", emac->multicast_rx);
	printf("[0x164] PAUSE_RX: 0x%.8x\n", emac->pause_rx);
	printf("[0x168] FRAME64_RX: 0x%.8x\n", emac->frame64_rx);
	printf("[0x16c] FRAME65_127_RX: 0x%.8x\n", emac->frame65_127_rx);
	printf("[0x170] FRAME128_255_RX: 0x%.8x\n", emac->frame128_255_rx);
	printf("[0x174] FRAME256_511_RX: 0x%.8x\n", emac->frame256_511_rx);
	printf("[0x178] FRAME512_1023_RX: 0x%.8x\n", emac->frame512_1023_rx);
	printf("[0x17c] FRAME1024_1518_RX: 0x%.8x\n", emac->frame1024_1518_rx);
	printf("[0x180] FRAME1519_RX: 0x%.8x\n", emac->frame1519_rx);
	printf("[0x184] USIZE_FRAMES: 0x%.8x\n", emac->usize_frames);
	printf("[0x188] EXCESS_LENGTH: 0x%.8x\n", emac->excess_length);
	printf("[0x18c] JABBERS: 0x%.8x\n", emac->jabbers);
	printf("[0x190] FCS_ERRORS: 0x%.8x\n", emac->fcs_errors);
	printf("[0x194] LENGTH_CHECK_ERRORS: 0x%.8x\n", emac->length_check_errors);
	printf("[0x198] RX_SYMBOL_ERRORS: 0x%.8x\n", emac->rx_symbol_errors);
	printf("[0x19c] ALIGN_ERRORS: 0x%.8x\n", emac->align_errors);
	printf("[0x1a0] RX_RES_ERRORS: 0x%.8x\n", emac->rx_res_errors);
	printf("[0x1a4] RX_ORUN: 0x%.8x\n", emac->rx_orun);
	printf("[0x1a8] IP_CKSUM: 0x%.8x\n", emac->ip_cksum);
	printf("[0x1ac] TCP_CKSUM: 0x%.8x\n", emac->tcp_cksum);
	printf("[0x1b0] UDP_CKSUM: 0x%.8x\n", emac->udp_cksum);
	printf("[0x300] SPEC5_ADD_BOT: 0x%.8x\n", emac->spec5_add_bot);
	printf("[0x304] SPEC5_ADD_TOP: 0x%.8x\n", emac->spec5_add_top);
	printf("[0x308] SPEC6_ADD_BOT: 0x%.8x\n", emac->spec6_add_bot);
	printf("[0x30c] SPEC6_ADD_TOP: 0x%.8x\n", emac->spec6_add_top);
	printf("[0x310] SPEC7_ADD_BOT: 0x%.8x\n", emac->spec7_add_bot);
	printf("[0x314] SPEC7_ADD_TOP: 0x%.8x\n", emac->spec7_add_top);
	printf("[0x318] SPEC8_ADD_BOT: 0x%.8x\n", emac->spec8_add_bot);
	printf("[0x31c] SPEC8_ADD_TOP: 0x%.8x\n", emac->spec8_add_top);
	printf("[0x320] SPEC9_ADD_BOT: 0x%.8x\n", emac->spec9_add_bot);
	printf("[0x324] SPEC9_ADD_TOP: 0x%.8x\n", emac->spec9_add_top);
	printf("[0x328] SPEC10_ADD_BOT: 0x%.8x\n", emac->spec10_add_bot);
	printf("[0x32c] SPEC10_ADD_TOP: 0x%.8x\n", emac->spec10_add_top);
	printf("[0x330] SPEC11_ADD_BOT: 0x%.8x\n", emac->spec11_add_bot);
	printf("[0x334] SPEC11_ADD_TOP: 0x%.8x\n", emac->spec11_add_top);
	printf("[0x338] SPEC12_ADD_BOT: 0x%.8x\n", emac->spec12_add_bot);
	printf("[0x33c] SPEC12_ADD_TOP: 0x%.8x\n", emac->spec12_add_top);
	printf("[0x340] SPEC13_ADD_BOT: 0x%.8x\n", emac->spec13_add_bot);
	printf("[0x344] SPEC13_ADD_TOP: 0x%.8x\n", emac->spec13_add_top);
	printf("[0x348] SPEC14_ADD_BOT: 0x%.8x\n", emac->spec14_add_bot);
	printf("[0x34c] SPEC14_ADD_TOP: 0x%.8x\n", emac->spec14_add_top);
	printf("[0x350] SPEC15_ADD_BOT: 0x%.8x\n", emac->spec15_add_bot);
	printf("[0x354] SPEC15_ADD_TOP: 0x%.8x\n", emac->spec15_add_top);
	printf("[0x358] SPEC16_ADD_BOT: 0x%.8x\n", emac->spec16_add_bot);
	printf("[0x35c] SPEC16_ADD_TOP: 0x%.8x\n", emac->spec16_add_top);
	printf("[0x360] SPEC17_ADD_BOT: 0x%.8x\n", emac->spec17_add_bot);
	printf("[0x364] SPEC17_ADD_TOP: 0x%.8x\n", emac->spec17_add_top);
	printf("[0x368] SPEC18_ADD_BOT: 0x%.8x\n", emac->spec18_add_bot);
	printf("[0x36c] SPEC18_ADD_TOP: 0x%.8x\n", emac->spec18_add_top);
	printf("[0x370] SPEC19_ADD_BOT: 0x%.8x\n", emac->spec19_add_bot);
	printf("[0x374] SPEC19_ADD_TOP: 0x%.8x\n", emac->spec19_add_top);
	printf("[0x378] SPEC20_ADD_BOT: 0x%.8x\n", emac->spec20_add_bot);
	printf("[0x37c] SPEC20_ADD_TOP: 0x%.8x\n", emac->spec20_add_top);
	printf("[0x380] SPEC21_ADD_BOT: 0x%.8x\n", emac->spec21_add_bot);
	printf("[0x384] SPEC21_ADD_TOP: 0x%.8x\n", emac->spec21_add_top);
	printf("[0x388] SPEC22_ADD_BOT: 0x%.8x\n", emac->spec22_add_bot);
	printf("[0x38c] SPEC22_ADD_TOP: 0x%.8x\n", emac->spec22_add_top);
	printf("[0x390] SPEC23_ADD_BOT: 0x%.8x\n", emac->spec23_add_bot);
	printf("[0x394] SPEC23_ADD_TOP: 0x%.8x\n", emac->spec23_add_top);
	printf("[0x398] SPEC24_ADD_BOT: 0x%.8x\n", emac->spec24_add_bot);
	printf("[0x39c] SPEC24_ADD_TOP: 0x%.8x\n", emac->spec24_add_top);
	printf("[0x3a0] SPEC25_ADD_BOT: 0x%.8x\n", emac->spec25_add_bot);
	printf("[0x3a4] SPEC25_ADD_TOP: 0x%.8x\n", emac->spec25_add_top);
	printf("[0x3a8] SPEC26_ADD_BOT: 0x%.8x\n", emac->spec26_add_bot);
	printf("[0x3ac] SPEC26_ADD_TOP: 0x%.8x\n", emac->spec26_add_top);
	printf("[0x3b0] SPEC27_ADD_BOT: 0x%.8x\n", emac->spec27_add_bot);
	printf("[0x3b4] SPEC27_ADD_TOP: 0x%.8x\n", emac->spec27_add_top);
	printf("[0x3b8] SPEC28_ADD_BOT: 0x%.8x\n", emac->spec28_add_bot);
	printf("[0x3bc] SPEC28_ADD_TOP: 0x%.8x\n", emac->spec28_add_top);
	printf("[0x3c0] SPEC29_ADD_BOT: 0x%.8x\n", emac->spec29_add_bot);
	printf("[0x3c4] SPEC29_ADD_TOP: 0x%.8x\n", emac->spec29_add_top);
	printf("[0x3c8] SPEC30_ADD_BOT: 0x%.8x\n", emac->spec30_add_bot);
	printf("[0x3cc] SPEC30_ADD_TOP: 0x%.8x\n", emac->spec30_add_top);
	printf("[0x3d0] SPEC31_ADD_BOT: 0x%.8x\n", emac->spec31_add_bot);
	printf("[0x3d4] SPEC31_ADD_TOP: 0x%.8x\n", emac->spec31_add_top);
	printf("[0x3d8] SPEC32_ADD_BOT: 0x%.8x\n", emac->spec32_add_bot);
	printf("[0x3dc] SPEC32_ADD_TOP: 0x%.8x\n", emac->spec32_add_top);
	printf("[0x3dc] SPEC32_ADD_TOP: 0x%.8x\n", emac->spec32_add_top);
	printf("[0x7a0] CONTROL: 0x%.8x\n", emac->control);
}

int emac_dump(unsigned int emacno)
{
	uintptr_t base;
	volatile struct emac_regs *regs;
	switch(emacno) {
	case 1:
		base = CBUS_BASE + EMAC1_OFFSET;
		regs = emac1;
		break;
	case 2:
		base = CBUS_BASE + EMAC2_OFFSET;
		regs = emac2;
		break;
	case 3:
		base = CBUS_BASE + EMAC3_OFFSET;
		regs = emac3;
		break;
	default:
		fprintf(stderr, "Invalid EMAC number: %u\n", emacno);
		return -EINVAL;
	}
	printf("==== EMAC%u (0x%.8x) ====\n", emacno, base);
	emac_dump_regs(regs);
	return 0;
}
