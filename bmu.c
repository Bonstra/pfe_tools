#include <errno.h>
#include <stdbool.h>
#include <stdio.h>

#include "hw.h"

void bmu_reset(volatile struct bmu_regs *bmu)
{
	bmu->ctrl |= 0x02;
}

void bmu_enable(volatile struct bmu_regs *bmu)
{
	bmu->ctrl = 0x01;
}

void bmu_disable(volatile struct bmu_regs *bmu)
{
	bmu->ctrl = 0x00;
}

bool bmu_is_enabled(volatile struct bmu_regs *bmu)
{
	return !!(bmu->ctrl & 1);
}

uint32_t bmu_alloc(volatile struct bmu_regs *bmu)
{
	if (!bmu_is_enabled(bmu))
		return 0;
	return bmu->alloc_ctrl;
}

void bmu_free(volatile struct bmu_regs *bmu, uint32_t addr)
{
	if (!bmu_is_enabled(bmu))
		return;
	bmu->free_ctrl = addr;
}

void bmu_dump_regs(volatile struct bmu_regs *bmu)
{
	uint32_t ctrl = bmu->ctrl;
	printf("[0x00] VERSION: 0x%.8x\n", bmu->version);
	printf("[0x04] CTRL: 0x%.8x\n", ctrl);
	if ((ctrl & 0x1) == 0) {
		printf("BMU is disabled. Dumping further registers is unsafe.\n");
		return;
	}
	printf("[0x08] UCAST_CONFIG: 0x%.8x\n", bmu->ucast_config);
	printf("[0x0c] UCAST_BASE_ADDR: 0x%.8x\n", bmu->ucast_base_addr);
	printf("[0x10] BUF_SIZE: 0x%.8x\n", bmu->buf_size);
	printf("[0x14] BUF_CNT: 0x%.8x\n", bmu->buf_cnt);
	printf("[0x18] THRES: 0x%.8x\n", bmu->thres);
	printf("[0x20] INT_SRC: 0x%.8x\n", bmu->int_src);
	printf("[0x24] INT_ENABLE: 0x%.8x\n", bmu->int_enable);
	printf("[0x30] ALLOC_CTRL: (omitted because of side effet)\n");
	printf("[0x34] FREE_CTRL: (omitted because of side effect)\n");
	printf("[0x38] FREE_ERR_ADDR: 0x%.8x\n", bmu->free_err_addr);
	printf("[0x3c] CURR_BUF_CNT: 0x%.8x\n", bmu->curr_buf_cnt);
	printf("[0x40] MCAST_CNT: 0x%.8x\n", bmu->mcast_cnt);
	printf("[0x44] MCAST_ALLOC_CTRL: 0x%.8x\n", bmu->mcast_alloc_ctrl);
	printf("[0x48] REM_BUF_CNT: 0x%.8x\n", bmu->rem_buf_cnt);
	printf("[0x50] LOW_WATERMARK: 0x%.8x\n", bmu->low_watermark);
	printf("[0x54] HIGH_WATERMARK: 0x%.8x\n", bmu->high_watermark);
}

void bmu_init(volatile struct bmu_regs *bmu, uint32_t base, uint16_t num,
		uint16_t log2_len)
{
	if (!bmu)
		return;

	bmu_enable(bmu);

	bmu->ucast_base_addr = base;
	bmu->ucast_config = num;
	bmu->buf_size = log2_len;

	bmu_reset(bmu);
}

void bmu1_init(void)
{
	bmu_init(bmu1, CBUS_BASE_PE + LMEM_OFFSET + LMEM_POOL_OFFSET,
			LMEM_POOL_BUF_COUNT, LMEM_POOL_BUFLEN_LOG2);
}

void bmu2_init(void)
{
	bmu_init(bmu2, PFE_DDR_BASE + DDR_POOL_OFFSET, DDR_POOL_BUF_COUNT,
			DDR_POOL_BUFLEN_LOG2);
}

int bmu_dump(unsigned int bmuno)
{
	uintptr_t base;
	volatile struct bmu_regs *regs;
	switch(bmuno) {
	case 1:
		base = CBUS_BASE + BMU1_OFFSET;
		regs = bmu1;
		break;
	case 2:
		base = CBUS_BASE + BMU2_OFFSET;
		regs = bmu2;
		break;
	default:
		fprintf(stderr, "Invalid BMU number: %u\n", bmuno);
		return -EINVAL;
	}
	printf("==== BMU%u (0x%.8x) ====\n", bmuno, base);
	bmu_dump_regs(regs);
	return 0;
}
