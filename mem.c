#define _GNU_SOURCE
#include <errno.h>
#include <fcntl.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/types.h>

#include "hw.h"
#include "mem.h"
#include "regs.h"

#define MAP_SIZE 0x10000

#define CLASS_BUS_ACCESS_BASE_MASK 0xff000000

struct mem_access_regs {
	volatile uint32_t *addr;
	volatile uint32_t *wdata;
	volatile uint32_t *rdata;
};

volatile struct bmu_regs *bmu1 = NULL;
volatile struct bmu_regs *bmu2 = NULL;
volatile uint8_t *cbus = NULL;
volatile struct class_csr_regs *class_csr = NULL;
volatile uint32_t *clkreset = NULL;
volatile uint8_t *ddr = NULL;
volatile struct emac_regs *emac1 = NULL;
volatile struct emac_regs *emac2 = NULL;
volatile struct emac_regs *emac3 = NULL;
volatile struct gpi_regs *egpi1 = NULL;
volatile struct gpi_regs *egpi2 = NULL;
volatile struct gpi_regs *egpi3 = NULL;
volatile struct gpi_regs *hgpi = NULL;
volatile struct hif_regs *hif = NULL;
volatile struct hif_desc *hif_rx_descs = NULL;
volatile struct hif_desc *hif_tx_descs = NULL;
volatile struct hif_nocpy_regs *hif_nocpy = NULL;
volatile uint8_t *lmem = NULL;
volatile struct tmu_csr_regs *tmu_csr = NULL;
volatile struct util_csr_regs *util_csr = NULL;

int map_create(void)
{
	int res = 0;
	int devmem = -1;

	devmem = open("/dev/mem", O_RDWR | O_NOCTTY | O_CLOEXEC);
	if (devmem < 0) {
		res = -errno;
		fprintf(stderr, "Failed to open /dev/mem: %m\n");
		goto fail;
	}

	clkreset = mmap(NULL, MAP_SIZE, PROT_READ | PROT_WRITE,
			MAP_SHARED, devmem, CLKRESET_BASE);
	if (clkreset == MAP_FAILED) {
		res = -errno;
		fprintf(stderr, "Failed to mmap clkreset: %m\n");
		goto fail;
	}

	cbus = mmap(NULL, CBUS_SIZE, PROT_READ | PROT_WRITE,
			MAP_SHARED, devmem, CBUS_BASE);
	if (cbus == MAP_FAILED) {
		res = -errno;
		fprintf(stderr, "Failed to mmap cbus: %m\n");
		goto fail;
	}

	ddr = mmap(NULL, PFE_DDR_SIZE, PROT_READ | PROT_WRITE,
			MAP_SHARED, devmem, PFE_DDR_BASE);
	if (ddr == MAP_FAILED) {
		res = -errno;
		fprintf(stderr, "Failed to mmap ddr: %m\n");
		goto fail;
	}

	bmu1 = (volatile struct bmu_regs *)(cbus + BMU1_OFFSET);
	bmu2 = (volatile struct bmu_regs *)(cbus + BMU2_OFFSET);
	class_csr = (volatile struct class_csr_regs *)(cbus + CLASS_CSR_OFFSET);
	hif_rx_descs = (volatile struct hif_desc *)&ddr[HIF_RX_POOL_OFFSET];
	hif_tx_descs = (volatile struct hif_desc *)&ddr[HIF_TX_POOL_OFFSET];
	emac1 = (volatile struct emac_regs *)(cbus + EMAC1_OFFSET);
	egpi1 = (volatile struct gpi_regs *)(cbus + EGPI1_OFFSET);
	emac2 = (volatile struct emac_regs *)(cbus + EMAC2_OFFSET);
	egpi2 = (volatile struct gpi_regs *)(cbus + EGPI2_OFFSET);
	emac3 = (volatile struct emac_regs *)(cbus + EMAC3_OFFSET);
	egpi3 = (volatile struct gpi_regs *)(cbus + EGPI3_OFFSET);
	hgpi = (volatile struct gpi_regs *)(cbus + HGPI_OFFSET);
	hif = (volatile struct hif_regs *)(cbus + HIF_OFFSET);
	hif_nocpy = (volatile struct hif_nocpy_regs *)(cbus + HIF_NOCPY_OFFSET);
	lmem = (volatile uint8_t *)(cbus + LMEM_OFFSET);
	tmu_csr = (volatile struct tmu_csr_regs *)(cbus + TMU_CSR_OFFSET);
	util_csr = (volatile struct util_csr_regs *)(cbus + UTIL_CSR_OFFSET);

fail:
	if (devmem != -1) {
		close(devmem);
		devmem = -1;
	}
	if (res)
		map_destroy();
	return res;
}

void map_destroy(void)
{
	bmu1 = NULL;
	bmu2 = NULL;
	class_csr = NULL;
	emac1 = NULL;
	egpi1 = NULL;
	emac2 = NULL;
	egpi2 = NULL;
	emac3 = NULL;
	egpi3 = NULL;
	hgpi = NULL;
	hif = NULL;
	hif_nocpy = NULL;
	lmem = NULL;
	tmu_csr = NULL;
	util_csr = NULL;
	if (clkreset) {
		munmap((void*)clkreset, MAP_SIZE);
		clkreset = NULL;
	}
	if (cbus) {
		munmap((void*)cbus, CBUS_SIZE);
		cbus = NULL;
	}
	if (ddr) {
		munmap((void*)ddr, MAP_SIZE);
		ddr = NULL;
	}
}

bool is_ddr_addr(uint32_t addr)
{
	const uint32_t ddr_start = PFE_DDR_BASE;
	const uint32_t ddr_end = PFE_DDR_BASE + PFE_DDR_SIZE;
	return (addr >= ddr_start) && (addr < ddr_end);
}

bool is_lmem_addr(uint32_t addr)
{
	const uint32_t lmem_start = CBUS_BASE_PE + LMEM_OFFSET;
	const uint32_t lmem_end = CBUS_BASE_PE + LMEM_OFFSET + LMEM_SIZE;
	return (addr >= lmem_start) && (addr < lmem_end);
}

void copy_from_lmem(void *dst, uint32_t lsrc, size_t len)
{
	uint8_t *dstb = dst;
	volatile uint32_t *srcw;
	if (!is_lmem_addr(lsrc))
		return;
	srcw = (uint32_t *)(lmem + (lsrc - (CBUS_BASE_PE + LMEM_OFFSET)));
	/* Read first bytes */
	if (lsrc & 3) {
		uint32_t i = lsrc & 3;
		uint32_t val = *srcw;
		if (i == 1) {
			*dstb = val >> 16;
			dstb++;
			i++;
			len--;
		}
		if (i == 2) {
			*dstb = val >> 8;
			dstb++;
			i++;
			len--;
		}
		if (i == 3) {
			*dstb = val & 0xff;
			dstb++;
			i++;
			len--;
		}
	}
	/* Read as many words as possible */
	while (len >= 4) {
		uint32_t val = *srcw;
		dstb[0] = val >> 24;
		dstb[1] = val >> 16;
		dstb[2] = val >> 8;
		dstb[3] = val;
		srcw++;
		dstb += 4;
		len -= 4;
	}
	/* Add remaining bytes */
	if (len > 0) {
		uint32_t val = *srcw;
		if (len == 3) {
			*dstb = val >> 24;
			dstb++;
			len--;
		}
		if (len == 2) {
			*dstb = val >> 16;
			dstb++;
			len--;
		}
		if (len == 1) {
			*dstb = val >> 8;
			dstb++;
			len--;
		}
	}
}

void copy_to_lmem(uint32_t ldst, const void *src, size_t len)
{
	const uint8_t *srcb = src;
	volatile uint32_t *dstw;
	if (!is_lmem_addr(ldst))
		return;
	dstw = (uint32_t *)(lmem + ((ldst - (CBUS_BASE_PE + LMEM_OFFSET)) >> 1));
	/* Read first bytes */
	if (ldst & 3) {
		uint32_t i = ldst & 3;
		uint32_t val = *dstw;
		if (i == 1) {
			val = (val & ~0x00ff0000) | ((uint32_t)(*srcb) << 16);
			srcb++;
			i++;
			len--;
		}
		if (i == 2) {
			val = (val & ~0x0000ff00) | ((uint32_t)(*srcb) << 8);
			srcb++;
			i++;
			len--;
		}
		if (i == 3) {
			val = (val & ~0x0000ff00) | ((uint32_t)(*srcb));
			srcb++;
			i++;
			len--;
		}
		*dstw = val;
	}
	/* Read as many words as possible */
	while (len >= 4) {
		uint32_t val = 0;
		val |= (uint32_t)srcb[0] << 24;
		val |= (uint32_t)srcb[1] << 16;
		val |= (uint32_t)srcb[2] << 8;
		val |= (uint32_t)srcb[3];
		*dstw = val;
		dstw++;
		srcb += 4;
		len -= 4;
	}
	/* Add remaining bytes */
	if (len > 0) {
		uint32_t val = *dstw;
		if (len == 3) {
			val = (val & ~0xff000000) | ((uint32_t)*srcb << 24);
			srcb++;
			len--;
		}
		if (len == 2) {
			val = (val & ~0x00ff0000) | ((uint32_t)*srcb << 16);
			srcb++;
			len--;
		}
		if (len == 1) {
			val = (val & ~0x0000ff00) | ((uint32_t)*srcb << 8);
			srcb++;
			len--;
		}
		*dstw = val;
	}
}

void copy_from_ddr(void *dst, uint32_t dsrc, size_t len)
{
	uint8_t *dstb = dst;
	volatile uint8_t *srcb;
	if (!is_ddr_addr(dsrc))
		return;
	srcb = ddr + (dsrc - PFE_DDR_BASE);
	while(len > 0) {
		*dstb = *srcb;
		dstb++;
		srcb++;
		len--;
	}
}

void copy_to_ddr(uint32_t ddst, const void *src, size_t len)
{
	volatile uint8_t *dstb;
	const uint8_t *srcb = src;
	if (!is_ddr_addr(ddst))
		return;
	dstb = ddr + (ddst - PFE_DDR_BASE);
	while(len > 0) {
		*dstb = *srcb;
		dstb++;
		srcb++;
		len--;
	}
}

bool is_cbus_access_ok(void)
{
	uint32_t axiclk1;
	if (!clkreset || !cbus)
		return false;
	axiclk1 = clkreset[CLKRESET_AXI_CLK_CNTRL_1_OFFSET >> 2];
	if ((axiclk1 & CLKRESET_PFE_SYS_CLK_ENABLE) != 0)
		return true;
	return false;
}

static bool is_pfe_core_clk_enabled(void)
{
	uint32_t pfe_ctrl;
	if (!clkreset)
		return false;
	pfe_ctrl = clkreset[CLKRESET_PFE_CLK_CNTRL_OFFSET >> 2];
	return ((pfe_ctrl & CLKRESET_PFE_CORE_CLK_ENABLE) != 0);
}

bool is_class_bus_access_ok(void)
{
	if (!is_cbus_access_ok())
		return false;
	if (!class_csr)
		return false;
	return true;
}

bool is_class_mem_access_ok(void)
{
	if (!is_cbus_access_ok())
		return false;
	if (!is_pfe_core_clk_enabled())
		return false;
	if (!class_csr)
		return false;
	return true;
}

bool is_tmu_mem_access_ok(void)
{
	uint32_t ctrl;
	if (!is_cbus_access_ok())
		return false;
	if (!is_pfe_core_clk_enabled())
		return false;
	if (!tmu_csr)
		return false;
	ctrl = tmu_csr->ctrl;
	if (ctrl & 1)
		return false;
	if ((ctrl & 0x80) == 0)
		return false;
	return true;
}

bool is_util_mem_access_ok(void)
{
	if (!is_cbus_access_ok())
		return false;
	if (!is_pfe_core_clk_enabled())
		return false;
	if (!util_csr)
		return false;
	return true;
}

int cbus_read_byte(uint32_t offset, uint8_t *val)
{
	if (!val)
		return -EINVAL;
	*val = cbus[offset];
	return 0;
}

int cbus_read_hword(uint32_t offset, uint16_t *val)
{
	if (!val)
		return -EINVAL;
	*val = ((volatile uint16_t*)cbus)[offset >> 1];
	return 0;
}

int cbus_read_word(uint32_t offset, uint32_t *val)
{
	if (!val)
		return -EINVAL;
	*val = ((volatile uint32_t*)cbus)[offset >> 2];
	return 0;
}

int cbus_write_byte(uint32_t offset, uint8_t val)
{
	cbus[offset] = val;
	return 0;
}

int cbus_write_hword(uint32_t offset, uint16_t val)
{
	((volatile uint16_t*)cbus)[offset >> 1] = val;
	return 0;
}

int cbus_write_word(uint32_t offset, uint32_t val)
{
	((volatile uint32_t*)cbus)[offset >> 2] = val;
	return 0;
}

int class_bus_read_byte(uint32_t offset, uint8_t *val)
{
	volatile uint32_t *bus = (volatile uint32_t *)cbus;
	uint32_t addr;
	uint32_t word;
	if (!val)
		return -EINVAL;
	class_csr->bus_access_base = offset & CLASS_BUS_ACCESS_BASE_MASK;
	addr = (offset & ~CLASS_BUS_ACCESS_BASE_MASK) | (1 << 24);
	class_csr->bus_access_addr = addr;
	word = be32_to_cpu(class_csr->bus_access_rdata);
	/* Shift by correct amount to compensate for non word-aligned offset. */
	word >>= 8 * (offset & 3);
	*val = (uint8_t)word;
	return 0;
}

int class_bus_read_hword(uint32_t offset, uint16_t *val)
{
	volatile uint32_t *bus = (volatile uint32_t *)cbus;
	uint32_t addr;
	uint32_t word;
	if (!val)
		return -EINVAL;
	if (offset & 1)
		return -EINVAL;
	class_csr->bus_access_base = offset & CLASS_BUS_ACCESS_BASE_MASK;
	addr = (offset & ~CLASS_BUS_ACCESS_BASE_MASK) | (2 << 24);
	class_csr->bus_access_addr = addr;
	word = be32_to_cpu(class_csr->bus_access_rdata);
	/* For non word-aligned offset, adjust by appropriate amount. */
	if (offset & 2)
		word >>= 16;
	*val = (uint16_t)word;
	return 0;
}

int class_bus_read_word(uint32_t offset, uint32_t *val)
{
	volatile uint32_t *bus = (volatile uint32_t *)cbus;
	uint32_t addr;
	if (offset & 3)
		return -EINVAL;
	if (!val)
		return -EINVAL;
	class_csr->bus_access_base = offset & CLASS_BUS_ACCESS_BASE_MASK;
	addr = (offset & ~CLASS_BUS_ACCESS_BASE_MASK) | (4 << 24);
	class_csr->bus_access_addr = addr;
	*val = be32_to_cpu(class_csr->bus_access_rdata);
	return 0;
}

int class_bus_write_word(uint32_t offset, uint32_t val)
{
	volatile uint32_t *bus = (volatile uint32_t *)cbus;
	uint32_t addr;
	if (offset & 3)
		return -EINVAL;
	class_csr->bus_access_base = offset & CLASS_BUS_ACCESS_BASE_MASK;
	addr = (offset & ~CLASS_BUS_ACCESS_BASE_MASK) | (1 << 31) | (4 << 24);
	class_csr->bus_access_wdata = cpu_to_be32(val);
	class_csr->bus_access_addr = addr;
	return 0;
}

int class_bus_write_hword(uint32_t offset, uint16_t val)
{
	volatile uint32_t *bus = (volatile uint32_t *)cbus;
	uint32_t addr;
	uint32_t word = val;

	if (offset & 1)
		return -EINVAL;
	/* For non word-aligned offset, we must shift value by correct amount. */
	if (offset & 2)
		word <<= 16;
	class_csr->bus_access_base = offset & CLASS_BUS_ACCESS_BASE_MASK;
	addr = (offset & ~CLASS_BUS_ACCESS_BASE_MASK) | (1 << 31) | (2 << 24);
	class_csr->bus_access_wdata = cpu_to_be32(word);
	class_csr->bus_access_addr = addr;
	return 0;
}

int class_bus_write_byte(uint32_t offset, uint8_t val)
{
	volatile uint32_t *bus = (volatile uint32_t *)cbus;
	uint32_t addr;
	/* For non word-aligned offset, we must shift value by correct amount. */
	uint32_t word = (uint32_t)val << (8 * (offset & 3));
	class_csr->bus_access_base = offset & CLASS_BUS_ACCESS_BASE_MASK;
	addr = (offset & ~CLASS_BUS_ACCESS_BASE_MASK) | (1 << 31) | (1 << 24);
	class_csr->bus_access_wdata = cpu_to_be32(word);
	class_csr->bus_access_addr = addr;
	return 0;
}

static int pe_mem_read_byte(struct mem_access_regs *regs, bool imem, uint8_t pe,
		uint16_t offset, uint8_t *val)
{
	uint32_t addr;
	uint32_t off = offset & 0x3;
	uint32_t byte_en;
	if (!regs || !val)
		return -EINVAL;
	if (pe > 0xf)
		return -EINVAL;
	byte_en = 0x8 >> off;
	addr = (offset & PE_MEM_ACCESS_ADDR_MASK) |
		PE_MEM_ACCESS_BYTE_ENABLE(byte_en) | PE_MEM_ACCESS_PE_ID(pe);
	addr |= imem ? PE_MEM_ACCESS_IMEM : PE_MEM_ACCESS_DMEM;
	*regs->addr = addr;
	*val = (uint8_t)(*regs->rdata >> (off * 8));
	return 0;
}

static int pe_mem_read_hword(struct mem_access_regs *regs, bool imem, uint8_t pe,
		uint16_t offset, uint16_t *val)
{
	uint32_t addr;
	uint32_t off = offset & 0x3;
	uint32_t byte_en;
	if (!regs || !val)
		return -EINVAL;
	if (pe > 0xf)
		return -EINVAL;
	if (off == 0) {
		byte_en = 0xc;
	} else if (off == 2) {
		byte_en = 0x3;
	} else {
		return -EINVAL;
	}
	addr = (offset & PE_MEM_ACCESS_ADDR_MASK) |
		PE_MEM_ACCESS_BYTE_ENABLE(byte_en) | PE_MEM_ACCESS_PE_ID(pe);
	addr |= imem ? PE_MEM_ACCESS_IMEM : PE_MEM_ACCESS_DMEM;
	*regs->addr = addr;
	*val = (uint16_t)(*regs->rdata >> (off * 8));
	return 0;
}

static int pe_mem_read_word(struct mem_access_regs *regs, bool imem, uint8_t pe,
		uint16_t offset, uint32_t *val)
{
	uint32_t addr;
	if (!val)
		return -EINVAL;
	if (pe > 0xf)
		return -EINVAL;
	if ((offset & 3) != 0)
		return -EINVAL;
	addr = (offset & PE_MEM_ACCESS_ADDR_MASK) |
		PE_MEM_ACCESS_BYTE_ENABLE(0xf);
	addr |= imem ? PE_MEM_ACCESS_IMEM : PE_MEM_ACCESS_DMEM;
	*regs->addr = addr;
	*val = *regs->rdata;
	return 0;
}

static int pe_mem_write_byte(struct mem_access_regs *regs, bool imem,
		uint8_t pe, uint16_t offset, uint8_t val)
{
	uint32_t addr;
	uint32_t off = offset & 0x3;
	uint32_t byte_en;
	if (!regs)
		return -EINVAL;
	if (pe > 0xf)
		return -EINVAL;
	byte_en = 0x8 >> off;
	addr = (offset & PE_MEM_ACCESS_ADDR_MASK) | PE_MEM_ACCESS_WRITE |
		PE_MEM_ACCESS_BYTE_ENABLE(byte_en) | PE_MEM_ACCESS_PE_ID(pe);
	addr |= imem ? PE_MEM_ACCESS_IMEM : PE_MEM_ACCESS_DMEM;
	*regs->wdata = ((uint32_t)val) << (off * 8);
	*regs->addr = addr;
	return 0;
}

static int pe_mem_write_hword(struct mem_access_regs *regs, bool imem,
		uint8_t pe, uint16_t offset, uint16_t val)
{
	uint32_t addr;
	uint32_t off = offset & 0x3;
	uint32_t byte_en;
	if (!regs)
		return -EINVAL;
	if (pe > 0xf)
		return -EINVAL;
	if (off == 0) {
		byte_en = 0xc;
	} else if (off == 2) {
		byte_en = 0x3;
	} else {
		return -EINVAL;
	}
	addr = (offset & PE_MEM_ACCESS_ADDR_MASK) | PE_MEM_ACCESS_WRITE |
		PE_MEM_ACCESS_BYTE_ENABLE(byte_en) | PE_MEM_ACCESS_PE_ID(pe);
	addr |= imem ? PE_MEM_ACCESS_IMEM : PE_MEM_ACCESS_DMEM;
	*regs->wdata = ((uint32_t)val) << (off * 8);
	*regs->addr = addr;
	return 0;
}

static int pe_mem_write_word(struct mem_access_regs *regs, bool imem,
		uint8_t pe, uint16_t offset, uint32_t val)
{
	uint32_t addr;
	if (pe > 0xf)
		return -EINVAL;
	if ((offset & 3) != 0)
		return -EINVAL;
	addr = (offset & PE_MEM_ACCESS_ADDR_MASK) | PE_MEM_ACCESS_WRITE |
		PE_MEM_ACCESS_BYTE_ENABLE(0xf);
	addr |= imem ? PE_MEM_ACCESS_IMEM : PE_MEM_ACCESS_DMEM;
	*regs->wdata = (uint32_t)val;
	*regs->addr = addr;
	return 0;
}


int class_dmem_read_byte(uint8_t pe, uint16_t offset, uint8_t *val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_read_byte(&regs, false, pe, offset, val);
}

int class_dmem_read_hword(uint8_t pe, uint16_t offset, uint16_t *val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_read_hword(&regs, false, pe, offset, val);
}

int class_dmem_read_word(uint8_t pe, uint16_t offset, uint32_t *val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_read_word(&regs, false, pe, offset, val);
}

int class_dmem_write_byte(uint8_t pe, uint16_t offset, uint8_t val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_write_byte(&regs, false, pe, offset, val);
}

int class_dmem_write_hword(uint8_t pe, uint16_t offset, uint16_t val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_write_hword(&regs, false, pe, offset, val);
}

int class_dmem_write_word(uint8_t pe, uint16_t offset, uint32_t val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_write_word(&regs, false, pe, offset, val);
}


int class_imem_read_byte(uint8_t pe, uint16_t offset, uint8_t *val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_read_byte(&regs, true, pe, offset, val);
}

int class_imem_read_hword(uint8_t pe, uint16_t offset, uint16_t *val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_read_hword(&regs, true, pe, offset, val);
}

int class_imem_read_word(uint8_t pe, uint16_t offset, uint32_t *val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_read_word(&regs, true, pe, offset, val);
}

int class_imem_write_byte(uint8_t pe, uint16_t offset, uint8_t val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_write_byte(&regs, true, pe, offset, val);
}

int class_imem_write_hword(uint8_t pe, uint16_t offset, uint16_t val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_write_hword(&regs, true, pe, offset, val);
}

int class_imem_write_word(uint8_t pe, uint16_t offset, uint32_t val)
{
	struct mem_access_regs regs = {
		.addr = &class_csr->mem_access_addr,
		.wdata = &class_csr->mem_access_wdata,
		.rdata = &class_csr->mem_access_rdata
	};
	if (pe >= NUM_CLASS_PE)
		return -EINVAL;
	return pe_mem_write_word(&regs, true, pe, offset, val);
}
