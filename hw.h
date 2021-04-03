#ifndef HW_H
#define HW_H
#include <stdbool.h>

#include "regs.h"

#define NUM_CLASS_PE 6
#define NUM_TMU_PE 4

#define CLASS_DMEM_SIZE 0x2000
#define CLASS_IMEM_SIZE 0x8000
#define TMU_DMEM_SIZE 0x800
#define TMU_IMEM_SIZE 0x2000
#define UTIL_DMEM_SIZE 0x2000

extern volatile struct bmu_regs *bmu1;
extern volatile struct bmu_regs *bmu2;
extern volatile uint8_t *cbus;
extern volatile struct class_csr_regs *class_csr;
extern volatile uint32_t *clkreset;
extern volatile uint8_t *ddr;
extern volatile struct gpi_regs *egpi1;
extern volatile struct gpi_regs *egpi2;
extern volatile struct gpi_regs *egpi3;
extern volatile struct emac_regs *emac1;
extern volatile struct emac_regs *emac2;
extern volatile struct emac_regs *emac3;
extern volatile struct gpi_regs *hgpi;
extern volatile struct hif_regs *hif;
extern volatile struct hif_desc *hif_rx_descs;
extern volatile struct hif_desc *hif_tx_descs;
extern volatile struct hif_nocpy_regs *hif_nocpy;
extern volatile uint8_t *lmem;
extern volatile struct tmu_csr_regs *tmu_csr;
extern volatile struct util_csr_regs *util_csr;

uint32_t bmu_alloc(volatile struct bmu_regs *bmu);
void bmu_disable(volatile struct bmu_regs *bmu);
void bmu_dump_regs(volatile struct bmu_regs *bmu);
int bmu_dump(unsigned int bmuno);
void bmu_enable(volatile struct bmu_regs *bmu);
void bmu_free(volatile struct bmu_regs *bmu, uint32_t addr);
void bmu_init(volatile struct bmu_regs *bmu, uint32_t base, uint16_t num,
		uint16_t log2_len);
bool bmu_is_enabled(volatile struct bmu_regs *bmu);
void bmu_reset(volatile struct bmu_regs *bmu);
void bmu1_init(void);
void bmu2_init(void);

void emac_dump_regs(volatile struct emac_regs *emac);
int emac_dump(unsigned int emacno);
void emac_init(volatile struct emac_regs *emac);

void gpi_disable(volatile struct gpi_regs *gpi);
void gpi_dump_regs(volatile struct gpi_regs *gpi);
void gpi_enable(volatile struct gpi_regs *gpi);
void gpi_init(volatile struct gpi_regs *gpi);
bool gpi_is_enabled(volatile struct gpi_regs *gpi);
void gpi_reset(volatile struct gpi_regs *gpi);
int egpi_dump(unsigned int egpino);
int hgpi_dump(void);

void hif_dump_regs(volatile struct hif_regs *hif);
void hif_dump(void);
void hif_init_rx_bds(void);
void hif_init_tx_bds(void);
void hif_nocpy_dump_regs(volatile struct hif_nocpy_regs *hif);
void hif_nocpy_dump(void);
void hif_nocpy_init(void);
void hif_rx_ack_bd(volatile struct hif_desc *bd);

#endif
