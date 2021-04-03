#define _GNU_SOURCE
#include <errno.h>
#include <inttypes.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <arpa/inet.h>

#include "hw.h"
#include "mem.h"
#include "regs.h"

static void enable_pfe_sys_clk(void)
{
	clkreset[CLKRESET_AXI_CLK_CNTRL_1_OFFSET >> 2] |=
		CLKRESET_PFE_SYS_CLK_ENABLE;
}

/* Perform a hexdump of memory region using only 16-bit accesses. */
static void hexdump16_bytes(volatile void *base, intptr_t phys_base, off_t offset, size_t len)
{
	intptr_t p_off = phys_base + offset;
	size_t i;
	uint16_t hword;
	for (i = 0; i < len; i++, p_off++) {
		uint8_t byte;
		bool odd = (p_off & 1 == 1);
		if (i == 0 || !odd) {
			intptr_t v_off;
			v_off = ((uintptr_t)base + offset + i) & ~1;
			hword = ntohs(*(volatile uint16_t*)v_off);
		}
		byte = odd ? hword & 0xff : hword >> 8;
		if (i % 16 == 0) {
			printf("0x%.8"PRIxPTR":", p_off);
		}
		printf(" %.2x", byte);
		if (i % 16 == 15) {
			printf("\n");
		}
	}
	if (i % 16 > 0) {
		printf("\n");
	}
}

static void hexdump_lmem_bytes(off_t offset, size_t len)
{
	intptr_t p_off = CBUS_BASE + LMEM_OFFSET + offset;
	size_t i;
	offset += LMEM_OFFSET;
	for (i = 0; i < len; i++, p_off++) {
		int res;
		uint8_t byte;
		uint32_t readoff = offset + i;
		/* Byte swap */
		readoff = (readoff & ~3) | (3 - (readoff & 3));
		res = cbus_read_byte(readoff, &byte);
		if (res) {
			printf("Failed to read byte from CBUS: %s\n", strerror(-res));
			return;
		}
		if (i % 16 == 0) {
			printf("0x%.8"PRIxPTR":", p_off);
		}
		printf(" %.2x", byte);
		if (i % 16 == 15) {
			printf("\n");
		}
	}
	if (i % 16 > 0) {
		printf("\n");
	}
}

static int lmem_hexdump(off_t offset, size_t len)
{
	if (offset < 0 ||
	    offset > LMEM_SIZE ||
	    offset + len > LMEM_SIZE) {
		return -EINVAL;
	}
	printf("=== LMEM dump from 0x%.4x to 0x%.4x ===\n",
			offset, offset + (len - 1));
	hexdump_lmem_bytes(offset, len);
	return 0;
}

static int ddr_hexdump(off_t offset, size_t len)
{
	if (offset < 0 ||
	    offset > PFE_DDR_SIZE ||
	    offset + len > PFE_DDR_SIZE) {
		return -EINVAL;
	}
	printf("=== PFE DDR dump from 0x%.4x to 0x%.4x ===\n",
			offset, offset + (len - 1));
	hexdump16_bytes(ddr, PFE_DDR_BASE, offset, len);
	return 0;
}

static void lmem_clear(uint8_t pattern)
{
	size_t i;
	size_t imax = LMEM_SIZE >> 1;
	/* Cannot perform byte access at index (LMEM_SIZE - 1)
	 * We must use 16 bit writes to work around this issue.
	 */
	volatile uint16_t *lmem16 = (volatile uint16_t *)lmem;
	uint16_t hword = pattern;
	hword = hword << 8 | pattern;
	for (i = 0; i < imax; i++)
		lmem16[i] = hword;
}

static void ddr_clear(uint8_t pattern)
{
	size_t i;
	for (i = 0; i < PFE_DDR_SIZE; i++)
		ddr[i] = pattern;
}

static int parse_bmu_opts(int argc, char *argv[], int argidx,
		unsigned int bmuno)
{
	char *cmd;
	if (argc - argidx < 1) {
		fprintf(stderr, "Missing arguments.\n");
		return -EINVAL;
	}
	if (bmuno < 1 || bmuno > 2) {
		fprintf(stderr, "Invalid BMU number: %u\n", bmuno);
		return -EINVAL;
	}

	cmd = argv[argidx];
	if (strcmp(cmd, "dump") == 0) {
		int res = bmu_dump(bmuno);
		if (res)
			return res;
	} else if (strcmp(cmd, "reset") == 0) {
		if (bmuno == 1) {
			bmu_reset(bmu1);
		} else if (bmuno == 2) {
			bmu_reset(bmu2);
		} else {
			return -EINVAL;
		}
	} else {
		fprintf(stderr, "Invalid command: %s\n", cmd);
		return -EINVAL;
	}
	return 0;
}

static int parse_emac_opts(int argc, char *argv[], int argidx,
		unsigned int emacno)
{
	char *cmd;
	if (argc - argidx < 1) {
		fprintf(stderr, "Missing arguments.\n");
		return -EINVAL;
	}
	if (emacno < 1 || emacno > 3) {
		fprintf(stderr, "Invalid EMAC number: %u\n", emacno);
		return -EINVAL;
	}

	cmd = argv[argidx];
	if (strcmp(cmd, "dump") == 0) {
		int res = emac_dump(emacno);
		if (res)
			return res;
	} else {
		fprintf(stderr, "Invalid command: %s\n", cmd);
		return -EINVAL;
	}
	return 0;
}

static int parse_egpi_opts(int argc, char *argv[], int argidx,
		unsigned int egpino)
{
	char *cmd;
	if (argc - argidx < 1) {
		fprintf(stderr, "Missing arguments.\n");
		return -EINVAL;
	}
	if (egpino < 1 || egpino > 3) {
		fprintf(stderr, "Invalid EGPI number: %u\n", egpino);
		return -EINVAL;
	}

	cmd = argv[argidx];
	if (strcmp(cmd, "dump") == 0) {
		int res = egpi_dump(egpino);
		if (res)
			return res;
	} else if (strcmp(cmd, "reset") == 0) {
		if (egpino == 1) {
			gpi_reset(egpi1);
		} else if (egpino == 2) {
			gpi_reset(egpi2);
		} else if (egpino == 3) {
			gpi_reset(egpi3);
		} else {
			return -EINVAL;
		}
	} else {
		fprintf(stderr, "Invalid command: %s\n", cmd);
		return -EINVAL;
	}
	return 0;
}

static int parse_hgpi_opts(int argc, char *argv[], int argidx)
{
	char *cmd;
	if (argc - argidx < 1) {
		fprintf(stderr, "Missing arguments.\n");
		return -EINVAL;
	}

	cmd = argv[argidx];
	if (strcmp(cmd, "dump") == 0) {
		int res = hgpi_dump();
		if (res)
			return res;
	} else if (strcmp(cmd, "reset") == 0) {
		gpi_reset(hgpi);
	} else {
		fprintf(stderr, "Invalid command: %s\n", cmd);
		return -EINVAL;
	}
	return 0;
}

static int parse_hif_opts(int argc, char *argv[], int argidx)
{
	char *cmd;
	if (argc - argidx < 1) {
		fprintf(stderr, "Missing arguments.\n");
		return -EINVAL;
	}

	cmd = argv[argidx];
	if (strcmp(cmd, "dump") == 0) {
		hif_dump();
	} else {
		fprintf(stderr, "Invalid command: %s\n", cmd);
		return -EINVAL;
	}
	return 0;
}

static int parse_hif_nocpy_opts(int argc, char *argv[], int argidx)
{
	char *cmd;
	if (argc - argidx < 1) {
		fprintf(stderr, "Missing arguments.\n");
		return -EINVAL;
	}

	cmd = argv[argidx];
	if (strcmp(cmd, "dump") == 0) {
		hif_nocpy_dump();
	} else if (strcmp(cmd, "init") == 0) {
		hif_nocpy_init();
	} else {
		fprintf(stderr, "Invalid command: %s\n", cmd);
		return -EINVAL;
	}
	return 0;
}

static int parse_mem_hexdump(int argc, char *argv[], int argidx, bool ddr)
{
	long offset, length;
	char *offset_s, *length_s;
	int res;
	if (argc - argidx != 2) {
		fprintf(stderr, "Invalid number of arguments.\n");
		fprintf(stderr, "Usage: %s hexdump OFFSET LENGTH\n",
				ddr ? "ddr" : "lmem");
		return -EINVAL;
	}
	offset_s = argv[argidx];
	length_s = argv[argidx + 1];
	offset = strtol(argv[argidx], &offset_s, 0);
	if (*offset_s != '\0' || argv[argidx][0] == '\0' || offset < 0) {
		fprintf(stderr, "Invalid offset: %s\n", argv[argidx]);
		return -EINVAL;
	}
	length = strtol(argv[argidx + 1], &length_s, 0);
	if (*length_s != '\0' || argv[argidx + 1][0] == '\0' || length < 0) {
		fprintf(stderr, "Invalid length: %s\n", argv[argidx + 1]);
		return -EINVAL;
	}
	if (ddr) {
		res = ddr_hexdump(offset, length);
	} else {
		res = lmem_hexdump(offset, length);
	}
	if (res == -EINVAL) {
		fprintf(stderr, "Invalid offset and length.\n");
		return res;
	} else if (res) {
		return res;
	}
	return 0;
}

static int parse_lmem_opts(int argc, char *argv[], int argidx)
{
	char *cmd;
	if (argc - argidx < 1) {
		fprintf(stderr, "Missing arguments.\n");
		return -EINVAL;
	}
	cmd = argv[argidx];
	if (strcmp(cmd, "clear") == 0) {
		lmem_clear(0xaa);
	} else if (strcmp(cmd, "hexdump") == 0) {
		return parse_mem_hexdump(argc, argv, argidx + 1, false);
	} else {
		fprintf(stderr, "Invalid command: %s\n", cmd);
		return -EINVAL;
	}
	return 0;
}

static int parse_ddr_opts(int argc, char *argv[], int argidx)
{
	char *cmd;
	if (argc - argidx < 1) {
		fprintf(stderr, "Missing arguments.\n");
		return -EINVAL;
	}
	cmd = argv[argidx];
	if (strcmp(cmd, "clear") == 0) {
		ddr_clear(0xaa);
	} else if (strcmp(cmd, "hexdump") == 0) {
		return parse_mem_hexdump(argc, argv, argidx + 1, true);
	} else {
		fprintf(stderr, "Invalid command: %s\n", cmd);
		return -EINVAL;
	}
	return 0;
}

int main(int argc, char *argv[])
{
	const char *cmd;
	if (argc < 2) {
		fprintf(stderr, "Missing arguments.\n");
		return EXIT_FAILURE;
	}
	if (map_create())
		return EXIT_FAILURE;

	enable_pfe_sys_clk();

	cmd = argv[1];
	if (strncmp(cmd, "bmu", 3) == 0) {
		char bmuno = cmd[3] - '0';
		if (bmuno < 1 || bmuno > 2) {
			fprintf(stderr, "Invalid BMU number: '%c'.\n",
					cmd[3]);
			return EXIT_FAILURE;
		}
		if (parse_bmu_opts(argc, argv, 2, bmuno))
			return EXIT_FAILURE;
	} else if (strcmp(cmd, "ddr") == 0) {
		if (parse_ddr_opts(argc, argv, 2))
			return EXIT_FAILURE;
	} else if (strncmp(cmd, "egpi", 4) == 0) {
		char egpino = cmd[4] - '0';
		if (egpino < 0 || egpino > 9) {
			fprintf(stderr, "Invalid EGPI number: '%c'.\n",
					cmd[4]);
			return EXIT_FAILURE;
		}
		if (parse_egpi_opts(argc, argv, 2, egpino))
			return EXIT_FAILURE;
	} else if (strncmp(cmd, "emac", 4) == 0) {
		char emacno = cmd[4] - '0';
		if (emacno < 0 || emacno > 9) {
			fprintf(stderr, "Invalid EMAC number: '%c'.\n",
					cmd[4]);
			return EXIT_FAILURE;
		}
		if (parse_emac_opts(argc, argv, 2, emacno))
			return EXIT_FAILURE;
	} else if (strcmp(cmd, "hgpi") == 0) {
		if (parse_hgpi_opts(argc, argv, 2))
			return EXIT_FAILURE;
	} else if (strcmp(cmd, "hif") == 0) {
		if (parse_hif_opts(argc, argv, 2))
			return EXIT_FAILURE;
	} else if (strcmp(cmd, "hif_nocpy") == 0) {
		if (parse_hif_nocpy_opts(argc, argv, 2))
			return EXIT_FAILURE;
	} else if (strcmp(cmd, "lmem") == 0) {
		if (parse_lmem_opts(argc, argv, 2))
			return EXIT_FAILURE;
	} else {
		fprintf(stderr, "Unrecognized command: %s\n", cmd);
		return EXIT_FAILURE;
	}

	map_destroy();
	return EXIT_SUCCESS;
}
