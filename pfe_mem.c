#define _GNU_SOURCE
#include <assert.h>
#include <errno.h>
#include <fcntl.h>
#include <inttypes.h>
#include <libgen.h>
#include <limits.h>
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

char *argv0;

static bool is_aligned(uint32_t offset, unsigned int width)
{
	switch(width) {
	case 1:
		return true;
	case 2:
		return (offset & 0x1) == 0;
	case 4:
		return (offset & 0x3) == 0;
	default:
		break;
	}
	return false;
}

static int check_cbus_constraints(uint32_t offset, unsigned int width)
{
	if (!is_cbus_access_ok()) {
		printf("Accessing the CBUS would lock the system.\n");
		return -EFAULT;
	}
	if (offset >= CBUS_SIZE) {
		printf("Offset 0x"PRIx32" is outside CBUS range.\n", offset);
		return -EINVAL;
	}
	if (!is_aligned(offset, width)) {
		printf("0x%"PRIx32" is not aligned on a %u bytes boundary.\n",
				offset, width);
		return -EINVAL;
	}
	if (width == 1 && (offset & 0x0000ffff) == 0xffff) {
		printf("Cannot perform byte access at offset 0x"PRIx32".\n");
		return -EINVAL;
	}
	return 0;
}

static int cbus_read(uint32_t offset, unsigned int width)
{
	int res;

	res = check_cbus_constraints(offset, width);
	if (res)
		return res;

	switch(width) {
	case 1:
	{
		uint8_t val;
		res = cbus_read_byte(offset, &val);
		if (res)
			return res;
		printf("CBUS 0x%.8" PRIx32 ": 0x%.2" PRIx8 "\n", offset, val);
		break;
	}
	case 2:
	{
		uint16_t val;
		res = cbus_read_hword(offset, &val);
		if (res)
			return res;
		printf("CBUS 0x%.8" PRIx32 ": 0x%.4" PRIx16 "\n", offset, val);
		break;
	}
	case 4:
	{
		uint32_t val;
		res = cbus_read_word(offset, &val);
		if (res)
			return res;
		printf("CBUS 0x%.8" PRIx32 ": 0x%.8" PRIx32 "\n", offset, val);
		break;
	}
	default:
		return -EINVAL;
	}
	return 0;
}

static int cbus_write(uint32_t offset, uint32_t value, unsigned int width)
{
	int res;

	res = check_cbus_constraints(offset, width);
	if (res)
		return res;

	switch(width) {
	case 1:
	{
		uint8_t val = (uint8_t)value;
		res = cbus_write_byte(offset, val);
		if (res)
			return res;
		break;
	}
	case 2:
	{
		uint16_t val = (uint16_t)value;
		res = cbus_write_hword(offset, val);
		if (res)
			return res;
		break;
	}
	case 4:
	{
		uint32_t val = (uint32_t)value;
		res = cbus_write_word(offset, val);
		if (res)
			return res;
		break;
	}
	default:
		return -EINVAL;
	}
	return 0;
}

static int class_bus_read(uint32_t offset, unsigned int width)
{
	int res;

	if (!is_class_bus_access_ok()) {
		printf("Accessing the Class bus would lock the system.\n");
		return -EFAULT;
	}

	switch(width) {
	case 1:
	{
		uint8_t val;
		res = class_bus_read_byte(offset, &val);
		if (res)
			break;
		printf("Class bus 0x%.8" PRIx32 ": 0x%.2" PRIx8 "\n", offset, val);
		break;
	}
	case 2:
	{
		uint16_t val;
		res = class_bus_read_hword(offset, &val);
		if (res)
			break;
		printf("Class bus 0x%.8" PRIx32 ": 0x%.4" PRIx16 "\n", offset, val);
		break;
	}
	case 4:
	{
		uint32_t val;
		res = class_bus_read_word(offset, &val);
		if (res)
			break;
		printf("Class bus 0x%.8" PRIx32 ": 0x%.8" PRIx32 "\n", offset, val);
		break;
	}
	default:
		res = -EINVAL;
		break;
	}
	if (res)
		printf("Read from Class bus failed: %s\n", strerror(-res));
	return res;
}

static int class_bus_write(uint32_t offset, uint32_t value, unsigned int width)
{
	int res;

	if (!is_class_bus_access_ok()) {
		printf("Accessing the Class bus would lock the system.\n");
		return -EFAULT;
	}

	switch(width) {
	case 1:
		res = class_bus_write_byte(offset, (uint8_t)value);
		break;
	case 2:
		res = class_bus_write_hword(offset, (uint16_t)value);
		break;
	case 4:
		res = class_bus_write_word(offset, value);
		break;
	default:
		res = -EINVAL;
		break;
	}
	if (res)
		printf("Write to Class bus failed: %s\n", strerror(-res));
	return res;
}

static int class_dmem_read(unsigned int pe, uint32_t offset, unsigned int width)
{
	int res;

	if (!is_class_mem_access_ok()) {
		printf("Accessing the Class memory would lock the system.\n");
		return -EFAULT;
	}
	if (offset >= CLASS_DMEM_SIZE) {
		printf("Offset 0x%" PRIx32 " is too large. Maximum is 0x%x\n",
				offset, CLASS_DMEM_SIZE - 1);
		return -EINVAL;
	}
	switch(width) {
	case 1:
	{
		uint8_t val;
		res = class_dmem_read_byte((uint8_t)pe, offset, &val);
		if (res)
			break;
		printf("Class%u DMEM 0x%.8" PRIx32 ": 0x%.2" PRIx8 "\n", pe, offset, val);
		break;
	}
	case 2:
	{
		uint16_t val;
		res = class_dmem_read_hword((uint8_t)pe, offset, &val);
		if (res)
			break;
		printf("Class%u DMEM 0x%.8" PRIx32 ": 0x%.4" PRIx16 "\n", pe, offset, val);
		break;
	}
	case 4:
	{
		uint32_t val;
		res = class_dmem_read_word((uint8_t)pe, offset, &val);
		if (res)
			break;
		printf("Class%u DMEM 0x%.8" PRIx32 ": 0x%.8" PRIx32 "\n", pe, offset, val);
		break;
	}
	default:
		res = -EINVAL;
		break;
	}
	if (res)
		printf("Read from Class%u DMEM failed: %s\n", pe, strerror(-res));
	return res;
}

static int class_dmem_write(unsigned int pe, uint32_t offset, uint32_t value,
		unsigned int width)
{
	int res;

	if (!is_class_mem_access_ok()) {
		printf("Accessing the Class memory would lock the system.\n");
		return -EFAULT;
	}
	if (offset >= CLASS_DMEM_SIZE) {
		printf("Offset 0x%" PRIx32 " is too large. Maximum is 0x%x\n",
				offset, CLASS_DMEM_SIZE - 1);
		return -EINVAL;
	}
	switch(width) {
	case 1:
		res = class_dmem_write_byte(pe, offset, (uint8_t)value);
		break;
	case 2:
		res = class_dmem_write_hword(pe, offset, (uint16_t)value);
		break;
	case 4:
		res = class_dmem_write_word(pe, offset, value);
		break;
	default:
		res = -EINVAL;
		break;
	}
	if (res)
		printf("Write to Class DMEM failed: %s\n", strerror(-res));
	return res;
}

static int class_imem_read(unsigned int pe, uint32_t offset, unsigned int width)
{
	int res;

	if (!is_class_mem_access_ok()) {
		printf("Accessing the Class memory would lock the system.\n");
		return -EFAULT;
	}
	if (offset >= CLASS_IMEM_SIZE) {
		printf("Offset 0x%" PRIx32 " is too large. Maximum is 0x%x\n",
				offset, CLASS_IMEM_SIZE - 1);
		return -EINVAL;
	}
	switch(width) {
	case 1:
	{
		uint8_t val;
		res = class_imem_read_byte((uint8_t)pe, offset, &val);
		if (res)
			break;
		printf("Class%u IMEM 0x%.8" PRIx32 ": 0x%.2" PRIx8 "\n", pe, offset, val);
		break;
	}
	case 2:
	{
		uint16_t val;
		res = class_imem_read_hword((uint8_t)pe, offset, &val);
		if (res)
			break;
		printf("Class%u IMEM 0x%.8" PRIx32 ": 0x%.4" PRIx16 "\n", pe, offset, val);
		break;
	}
	case 4:
	{
		uint32_t val;
		res = class_imem_read_word((uint8_t)pe, offset, &val);
		if (res)
			break;
		printf("Class%u IMEM 0x%.8" PRIx32 ": 0x%.8" PRIx32 "\n", pe, offset, val);
		break;
	}
	default:
		res = -EINVAL;
		break;
	}
	if (res)
		printf("Read from Class%u IMEM failed: %s\n", pe, strerror(-res));
	return res;
}

static int class_imem_write(unsigned int pe, uint32_t offset, uint32_t value,
		unsigned int width)
{
	int res;

	if (!is_class_mem_access_ok()) {
		printf("Accessing the Class memory would lock the system.\n");
		return -EFAULT;
	}
	if (offset >= CLASS_IMEM_SIZE) {
		printf("Offset 0x%" PRIx32 " is too large. Maximum is 0x%x\n",
				offset, CLASS_IMEM_SIZE - 1);
		return -EINVAL;
	}
	switch(width) {
	case 1:
		res = class_imem_write_byte(pe, offset, (uint8_t)value);
		break;
	case 2:
		res = class_imem_write_hword(pe, offset, (uint16_t)value);
		break;
	case 4:
		res = class_imem_write_word(pe, offset, value);
		break;
	default:
		res = -EINVAL;
		break;
	}
	if (res)
		printf("Write to Class IMEM failed: %s\n", strerror(-res));
	return res;
}

static int parse_index(const char *str, unsigned int max)
{
	long idx;
	char *endptr = NULL;

	if (!str || max > INT_MAX)
		return -EINVAL;

	idx = strtol(str, &endptr, 10);
	if (str[0] == '\0' || endptr[0] != '\0')
		return -EINVAL;

	if (idx < 0)
		return -ERANGE;
	if (idx > INT_MAX)
		return -ERANGE;
	if (idx > (int)max)
		return -ERANGE;

	return (int)idx;
}

static void usage()
{
	printf("Usage: %s TARGET OFFSET [WIDTH [VALUE]]\n", argv0);
	printf("  TARGET  one of cbus, classbus, classdmemID, classimemID,\n"
	       "          tmubus, tmudmemID, tmuimemID, utilbus or utilmem,\n"
	       "          where ID is the index of the target PE instance\n"
	       "  OFFSET  offset in TARGET where to perform the access\n"
	       "  WIDTH   access width: [b]yte, [h]alfword or [w]ord for \n"
	       "          8, 16 and 32 bit access, respectively. Default is\n"
	       "          32 bit access.\n"
	       "  VALUE   value to write. If not specified, a read operation\n"
	       "          is performed.\n");
}

int main(int argc, char *argv[])
{
	const char *target;
	char *endptr;
	long long offset;
	long long value;
	unsigned int width = 4;
	bool write = false;

	argv0 = basename(argv[0]);

	if (argc < 3 || argc > 5) {
		printf("Invalid number of arguments.\n");
		usage();
		return EXIT_FAILURE;
	}

	target = argv[1];
	offset = strtoll(argv[2], &endptr, 0);
	if (argv[2][0] == '\0' || endptr[0] != '\0' || offset < 0) {
		printf("Invalid offset: %s\n", argv[2]);
		return EXIT_FAILURE;
	}
	if (offset > UINT32_MAX) {
		printf("Offset %lld too large.\n", offset);
		return EXIT_FAILURE;
	}

	if (map_create())
		return EXIT_FAILURE;

	if (argc >= 4) {
		if (strlen(argv[3]) != 1) {
			printf("Invalid width: %s\n", argv[3]);
			return EXIT_FAILURE;
		}
		switch(argv[3][0]) {
		case 'b':
			width = 1;
			break;
		case 'h':
			width = 2;
			break;
		case 'w':
			width = 4;
			break;
		default:
			printf("Invalid width: %c\n", *argv[3]);
			return EXIT_FAILURE;
		}
	}

	if (argc == 5) {
		value = strtoll(argv[4], &endptr, 0);
		if (argv[4][0] == '\0' || endptr[0] != '\0' || value < 0) {
			printf("Invalid value: %s\n", argv[4]);
			return EXIT_FAILURE;
		}
		if ((width == 1 && value > UINT8_MAX) ||
		    (width == 2 && value > UINT16_MAX) ||
		    (width == 4 && value > UINT32_MAX)) {
			printf("Value %lld too large.\n", value);
			return EXIT_FAILURE;
		}
		write = true;
	}

	if (strcmp(target, "cbus") == 0) {
		int res;
		if (write) {
			res = cbus_write(offset, value, width);
		} else {
			res = cbus_read(offset, width);
		}
		if (res)
			return EXIT_FAILURE;
	} else if (strcmp(target, "classbus") == 0) {
		int res;
		if (write) {
			res = class_bus_write(offset, value, width);
		} else {
			res = class_bus_read(offset, width);
		}
		if (res)
			return EXIT_FAILURE;
	} else if (strncmp(target, "classdmem", 9) == 0) {
		int idx;
		int res;
		idx = parse_index(&target[9], 5);
		if (idx < 0) {
			printf("Invalid index. Must be between 0 and 5.\n");
			return EXIT_FAILURE;
		}
		if (write) {
			res = class_dmem_write(idx, offset, value, width);
		} else {
			res = class_dmem_read(idx, offset, width);
		}
		if (res)
			return EXIT_FAILURE;
	} else if (strncmp(target, "classimem", 9) == 0) {
		int idx;
		int res;
		idx = parse_index(&target[9], 5);
		if (idx < 0) {
			printf("Invalid index. Must be between 0 and 5.\n");
			return EXIT_FAILURE;
		}
		if (write) {
			res = class_imem_write(idx, offset, value, width);
		} else {
			res = class_imem_read(idx, offset, width);
		}
		if (res)
			return EXIT_FAILURE;
	} else if (strcmp(target, "tmubus") == 0) {
		return EXIT_FAILURE;
	} else if (strncmp(target, "tmuimem", 7) == 0) {
		int idx;
		idx = parse_index(&target[7], 3);
		if (idx < 0) {
			printf("Invalid index. Must be between 0 and 3.\n");
			return EXIT_FAILURE;
		}
		return EXIT_FAILURE;
	} else if (strncmp(target, "tmudmem", 7) == 0) {
		int idx;
		idx = parse_index(&target[7], 3);
		if (idx < 0) {
			printf("Invalid index. Must be between 0 and 3.\n");
			return EXIT_FAILURE;
		}
		return EXIT_FAILURE;
	} else if (strcmp(target, "utilbus") == 0) {
		return EXIT_FAILURE;
	} else if (strcmp(target, "utilmem") == 0) {
	} else {
		printf("Invalid target \"%s\"\n", target);
		return EXIT_FAILURE;
	}

	map_destroy();

	return EXIT_SUCCESS;
}
