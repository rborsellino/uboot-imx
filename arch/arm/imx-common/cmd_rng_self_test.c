/*
 * Copyright 2018, 2021 NXP
 *
 * SPDX-License-Identifier: BSD-2-Clause
 *
 * Command for RNG self test on effected chips
 */

#include <common.h>
#include <command.h>
#include <environment.h>
#include <linux/compiler.h>
#include <mapmem.h>
#include <errno.h>
#include <fsl_sec.h>

DECLARE_GLOBAL_DATA_PTR;

/*
 * do_rng_self_test() - Handle the "rng_self_test" command-line command
 * @cmdtp:  Command data struct pointer
 * @flag:   Command flag
 * @argc:   Command-line argument count
 * @argv:   Array of command-line arguments
 *
 * Returns zero on success, CMD_RET_USAGE in case of misuse and negative
 * on error.
 */
static int do_rng_self_test(cmd_tbl_t *cmdtp, int flag, int argc, char *const argv[])
{
	int ret = 0;
	uint32_t res_addr;
	uint8_t *res_ptr;

	if (argc != 2)
		return CMD_RET_USAGE;

	res_addr = simple_strtoul(argv[1], NULL, 16);
	res_ptr = map_sysmem(res_addr, 32);

	ret = rng_self_test(res_ptr);

	if (ret < 0)
		return CMD_RET_FAILURE;

	return ret;
}

/***************************************************/
static char rng_self_test_help_text[] =
	"<dest_addr>\n\n"
	"Run RNG self test before running any RNG based crypto implementation.\n"
	"Result in $dest_addr\n\n"
	"Following is the typical warning message when ROM/HAB fails rng self test\n"
	"------------+----+------+----+-------------------------------------------------\n"
	"Persistent  | T  |  L   | P  | Contents\n"
	"Memory      | a  |  e   | a  |\n"
	"Record      | g  |  n   | r  |\n"
	"Type        |    |  g   |    |\n"
	"            |    |  t   |    |\n"
	"            |    |  h   |    |\n"
	"------------+----+------+----+-------------------------------------------------\n"
	"Default Evt |0x0c|0x0014|0x42| Record Data (hex):\n"
	"            |    |      |    |  33 00 00 00 ee 00 00 00 00 00 00 00 00 00 00 00\n"
	"------------+----+------+----+-------------------------------------------------\n"
	"Engine Data |0x09|0x0008|0x42| Record Data (hex):\n"
	"            |    |      |    |  0f 36 06 00\n"
	"------------+----+------+----+-------------------------------------------------\n"
	"Event       |0xdb|0x0024|0x42| SRCE Field: 69 30 e1 1d\n"
	"            |    |      |    |             STS = HAB_WARNING (0x69)\n"
	"            |    |      |    |             RSN = HAB_ENG_FAIL (0x30)\n"
	"            |    |      |    |             CTX = HAB_CTX_ENTRY (0xE1)\n"
	"            |    |      |    |             ENG = HAB_ENG_CAAM (0x1D)\n"
	"            |    |      |    | Evt Data (hex):\n"
	"            |    |      |    |  00 08 00 02 40 00 36 06 55 55 00 03 00 00 00 00\n"
	"            |    |      |    |  00 00 00 00 00 00 00 00 00 00 00 01\n";

U_BOOT_CMD(
	rng_self_test, 2, 1, do_rng_self_test,
	"RNG self test",
	rng_self_test_help_text
);
