#ifndef MEM_H
#define MEM_H

#include <byteswap.h>

#include "hw.h"

#if __BYTE_ORDER == __LITTLE_ENDIAN
#define be32_to_cpu(x)  bswap_32(x)
#define cpu_to_be32(x)  bswap_32(x)
#define be16_to_cpu(x)  bswap_16(x)
#define cpu_to_be16(x)  bswap_16(x)
#elif __BYTE_ORDER == __BIG_ENDIAN
#define be32_to_cpu(x)  ((__u32)(x))
#define cpu_to_be32(x)  ((__u32)(x))
#define be16_to_cpu(x)  ((__u16)(x))
#define cpu_to_be16(x)  ((__u16)(x))
#endif

int map_create(void);
void map_destroy(void);

bool is_ddr_addr(uint32_t addr);
bool is_lmem_addr(uint32_t addr);

void copy_from_lmem(void *dst, uint32_t lsrc, size_t len);
void copy_to_lmem(uint32_t ldst, const void *src, size_t len);

void copy_from_ddr(void *dst, uint32_t dsrc, size_t len);
void copy_to_ddr(uint32_t ddst, const void *src, size_t len);

bool is_cbus_access_ok(void);
bool is_class_bus_access_ok(void);
bool is_class_mem_access_ok(void);
bool is_tmu_mem_access_ok(void);
bool is_util_mem_access_ok(void);

int cbus_read_byte(uint32_t offset, uint8_t *val);
int cbus_read_hword(uint32_t offset, uint16_t *val);
int cbus_read_word(uint32_t offset, uint32_t *val);

int cbus_write_byte(uint32_t offset, uint8_t val);
int cbus_write_hword(uint32_t offset, uint16_t val);
int cbus_write_word(uint32_t offset, uint32_t val);

int class_bus_read_byte(uint32_t offset, uint8_t *val);
int class_bus_read_hword(uint32_t offset, uint16_t *val);
int class_bus_read_word(uint32_t offset, uint32_t *val);

int class_bus_write_byte(uint32_t offset, uint8_t val);
int class_bus_write_hword(uint32_t offset, uint16_t val);
int class_bus_write_word(uint32_t offset, uint32_t val);

int class_dmem_read_word(uint8_t pe, uint16_t offset, uint32_t *val);
int class_dmem_read_hword(uint8_t pe, uint16_t offset, uint16_t *val);
int class_dmem_read_byte(uint8_t pe, uint16_t offset, uint8_t *val);

int class_dmem_write_word(uint8_t pe, uint16_t offset, uint32_t val);
int class_dmem_write_hword(uint8_t pe, uint16_t offset, uint16_t val);
int class_dmem_write_byte(uint8_t pe, uint16_t offset, uint8_t val);

int class_imem_read_word(uint8_t pe, uint16_t offset, uint32_t *val);
int class_imem_read_hword(uint8_t pe, uint16_t offset, uint16_t *val);
int class_imem_read_byte(uint8_t pe, uint16_t offset, uint8_t *val);

int class_imem_write_word(uint8_t pe, uint16_t offset, uint32_t val);
int class_imem_write_hword(uint8_t pe, uint16_t offset, uint16_t val);
int class_imem_write_byte(uint8_t pe, uint16_t offset, uint8_t val);

#endif
