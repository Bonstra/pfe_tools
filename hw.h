#ifndef HW_H
#define HW_H
#include "regs.h"

#define NUM_CLASS_PE 6
#define NUM_TMU_PE 4

#define CLASS_DMEM_SIZE 0x2000
#define CLASS_IMEM_SIZE 0x8000
#define TMU_DMEM_SIZE 0x800
#define TMU_IMEM_SIZE 0x2000
#define UTIL_DMEM_SIZE 0x2000

volatile struct bmu_regs *bmu1;
volatile struct bmu_regs *bmu2;
volatile uint8_t *cbus;
volatile struct class_csr_regs *class_csr;
volatile uint32_t *clkreset;
volatile uint8_t *ddr;
volatile struct gpi_regs *egpi1;
volatile struct gpi_regs *egpi2;
volatile struct gpi_regs *egpi3;
volatile struct gpi_regs *hgpi;
volatile struct hif_regs *hif;
volatile struct hif_nocpy_regs *hif_nocpy;
volatile uint16_t *lmem;
volatile struct tmu_csr_regs *tmu_csr;
volatile struct util_csr_regs *util_csr;

void bmu_reset(volatile struct bmu_regs *bmu);
void bmu_dump_regs(volatile struct bmu_regs *bmu);
int bmu_dump(unsigned int bmuno);

void gpi_reset(volatile struct gpi_regs *gpi);
void gpi_dump_regs(volatile struct gpi_regs *gpi);
int egpi_dump(unsigned int egpino);
int hgpi_dump(void);

void hif_dump_regs(volatile struct hif_regs *hif);
void hif_dump(void);
void hif_nocpy_dump_regs(volatile struct hif_nocpy_regs *hif);
void hif_nocpy_dump(void);
void hif_nocpy_init(void);
#endif
