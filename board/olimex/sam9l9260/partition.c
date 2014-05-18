/*
 * (C) Copyright 2014
 * Bernhard Stegmaier <stegmaier@sw-systems.de>
 *
 * based on AT91SAM9260EK and Olimex patches
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <config.h>
#include <asm/hardware.h>
#include <dataflash.h>

AT91S_DATAFLASH_INFO dataflash_info[CONFIG_SYS_MAX_DATAFLASH_BANKS];

struct dataflash_addr cs[CONFIG_SYS_MAX_DATAFLASH_BANKS] = {
	{CONFIG_SYS_DATAFLASH_LOGIC_ADDR_CS1, 1}	/* Logical adress, CS */
};

/* 
 * Define the area offsets of the DataFlash.
 * 
 * Note: SAM9L9260 uses a AT45DB161 DataFlash with 4096 pages and a page size 
 * of 528 bytes (2.162.688 bytes).
 *
 * Dataflash layout:
 * # pages	size (bytes)	Usage
 *    8		    4.224	Bootstrap
 *   32		   16.896	U-Boot Environment
 *  745		  393.360	U-Boot
 * 3311		1.748.208	Kernel
 */
dataflash_protect_t area_list[NB_DATAFLASH_AREA] = {
	{0x00000000, 0x0000107F, FLAG_PROTECT_SET,   0, "Bootstrap"},
	{0x00001080, 0x0000527F, FLAG_PROTECT_CLEAR, 0, "Environment"},
	{0x00005280, 0x0006530F, FLAG_PROTECT_SET,   0, "U-Boot"},
	{0x00065310, 0x0020FFFF, FLAG_PROTECT_CLEAR, 0,	"Kernel"}
};

