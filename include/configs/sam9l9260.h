/*
 * (C) Copyright 2014
 * Bernhard Stegmaier <stegmaier@sw-systems.de>
 *
 * Configuation settings for the Olimex SAM9L9260 board based on AT91SAM9260EK
 * and Olimex patches.
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#ifndef __CONFIG_H
#define __CONFIG_H

/*
 * SoC must be defined first, before hardware.h is included.
 * In this case SoC is defined in boards.cfg.
 */
#include <asm/hardware.h>

/*
 * Warning: changing CONFIG_SYS_TEXT_BASE requires
 * adapting the initial boot program.
 * Since the linker has to swallow that define, we must use a pure
 * hex number here!
 */
#define CONFIG_SYS_TEXT_BASE		0x21F00000

/* Command line configuration */
#include <config_cmd_default.h>
#undef CONFIG_CMD_BDI
#undef CONFIG_CMD_FPGA
#undef CONFIG_CMD_IMI
#undef CONFIG_CMD_IMLS
#undef CONFIG_CMD_LOADS
#undef CONFIG_CMD_SOURCE
#define CONFIG_CMD_PING		1
#define CONFIG_CMD_DHCP		1
#define CONFIG_CMD_NAND		1
#ifndef CONFIG_SYS_USE_DATAFLASH_CS1    
#define CONFIG_CMD_MMC		1
#define CONFIG_CMD_FAT		1
#endif
#define CONFIG_CMD_USB		1

/* Clocks */
#define CONFIG_SYS_AT91_SLOW_CLOCK	32768		/* slow clock xtal */
#define CONFIG_SYS_AT91_MAIN_CLOCK	18432000	/* main clock xtal */
#define CONFIG_SYS_HZ			1000

/* Misc CPU and board related */
#define CONFIG_AT91SAM9260		1	/* Atmel AT91SAM9260 SoC */
#define CONFIG_ARCH_CPU_INIT		1	/* arch_cpu_init for clock setup */
#define CONFIG_SKIP_LOWLEVEL_INIT	1	/* Skip lowlevel_init (mem setup) */
#define CONFIG_BOARD_EARLY_INIT_F	1	/* Early init used to enable GPIO */
#define CONFIG_DISPLAY_CPUINFO		1	/* Show CPU info */

/* Linux related stuff */
#define CONFIG_CMDLINE_TAG		1	/* Enable passing of ATAGs */
#define CONFIG_SETUP_MEMORY_TAGS	1
#define CONFIG_INITRD_TAG		1
#define CONFIG_CMD_BOOTZ		1	/* Enable zImage booting */
#define CONFIG_OF_LIBFDT		1	/* Device tree support */

/* general purpose I/O */
#define CONFIG_ATMEL_LEGACY		1	/* required until (g)pio is fixed */
#define CONFIG_AT91_GPIO		1	/* Use GPIO implementation */
#define CONFIG_AT91_GPIO_PULLUP		1	/* keep pullups on peripheral pins */

/* serial console */
#define CONFIG_ATMEL_USART
#define CONFIG_USART_BASE		ATMEL_BASE_DBGU
#define	CONFIG_USART_ID			ATMEL_ID_SYS
#define CONFIG_BAUDRATE			115200

/* LED */
#define CONFIG_AT91_LED
#define	CONFIG_RED_LED			AT91_PIN_PA9	/* this is the power led */
#define	CONFIG_GREEN_LED		AT91_PIN_PA6	/* this is the status led */

/* BOOTP options */
#define CONFIG_BOOTP_BOOTFILESIZE	1
#define CONFIG_BOOTP_BOOTPATH		1
#define CONFIG_BOOTP_GATEWAY		1
#define CONFIG_BOOTP_HOSTNAME		1

/*
 * SDRAM: 1 bank, 64MB
 * Initialized before u-boot gets started.
 */
#define CONFIG_NR_DRAM_BANKS		1
#define CONFIG_SYS_SDRAM_BASE		ATMEL_BASE_CS1
#define CONFIG_SYS_SDRAM_SIZE		0x04000000

/* Linux mach type */
#define CONFIG_MACH_TYPE		MACH_TYPE_SAM9_L9260

/* DataFlash (only when not using MMC) */
#ifndef CONFIG_SYS_USE_MMC    
#define CONFIG_HAS_DATAFLASH			1
#define CONFIG_ATMEL_DATAFLASH_SPI		1
#define CONFIG_SYS_SPI_WRITE_TOUT		(5*CONFIG_SYS_HZ)
#define CONFIG_SYS_MAX_DATAFLASH_BANKS		1
#define CONFIG_SYS_DATAFLASH_LOGIC_ADDR_CS1	0xD0000000	/* CS1 */
#define AT91_SPI_CLK				33000000
#define DATAFLASH_TCSS				(0x1a << 16)
#define DATAFLASH_TCHS				(0x1 << 24)
#endif

/* NAND flash */
#define CONFIG_NAND_ATMEL		1
#define CONFIG_SYS_MAX_NAND_DEVICE	1
#define CONFIG_SYS_NAND_BASE		ATMEL_BASE_CS3
#define CONFIG_SYS_NAND_DBW_8		1
#define CONFIG_SYS_NAND_MASK_ALE	(1 << 21)
#define CONFIG_SYS_NAND_MASK_CLE	(1 << 22)
#define CONFIG_SYS_NAND_ENABLE_PIN	AT91_PIN_PC14
#define CONFIG_SYS_NAND_READY_PIN	AT91_PIN_PC13

/* NOR flash - no real flash on this board */
#define CONFIG_SYS_NO_FLASH		1

/* SDCard (only when not using DataFlash) */
#ifndef CONFIG_SYS_USE_DATAFLASH_CS1    
#define CONFIG_MMC			1
#define CONFIG_GENERIC_MMC		1
#define CONFIG_GENERIC_ATMEL_MCI	1
#define CONFIG_SYS_MMC_MAX_BLK_COUNT	1       /* other values seem to hang MCI on AT91 */
#define CONFIG_ATMEL_MCI_PORTB		1
#define CONFIG_SYS_MMC_CD_PIN		AT91_PIN_PC8
#endif

/* Ethernet */
#define CONFIG_MACB			1
#undef  CONFIG_RMII
#define CONFIG_NET_MULTI		1
#define CONFIG_NET_RETRY_COUNT		20
#define CONFIG_AT91_WANTS_COMMON_PHY	1

/* USB */
#define CONFIG_USB_ATMEL			1
#define CONFIG_USB_ATMEL_CLK_SEL_PLLB		1
#define CONFIG_USB_OHCI_NEW			1
#define CONFIG_USB_STORAGE			1
#define CONFIG_SYS_USB_OHCI_CPU_INIT		1
#define CONFIG_SYS_USB_OHCI_REGS_BASE		0x00500000	/* AT91SAM9260_UHP_BASE */
#define CONFIG_SYS_USB_OHCI_SLOT_NAME		"at91sam9260"
#define CONFIG_SYS_USB_OHCI_MAX_ROOT_PORTS	2

/* FAT */
#ifdef CONFIG_CMD_FAT
#define CONFIG_DOS_PARTITION
#endif

/* Destination to load data/images */
#define CONFIG_SYS_LOAD_ADDR                    0x22000000

/* 
 * Memtest configuration
 * Start from physical SDRAM start to CONFIG_SYS_TEXT_BASE-1
 * to avoid memtest destroying u-boot code.
 */
#define CONFIG_SYS_MEMTEST_START                CONFIG_SYS_SDRAM_BASE
#define CONFIG_SYS_MEMTEST_END                  0x21efffff

#ifdef CONFIG_SYS_USE_DATAFLASH_CS1
/* 
 * bootstrap + u-boot + env + linux in dataflash on CS1
 * Dataflash layout (see board/olimex/sam9l9260/partition.c):
 *   0x00000000	Bootstrap
 *   0x00001080	Environment
 *   0x00005280	U-Boot
 *   0x00065310	Kernel
 * Nandflash layout:
 *   see CONFIG_SYS_USE_NANDFLASH
 */
#define CONFIG_ENV_IS_IN_DATAFLASH	1
#define CONFIG_SYS_MONITOR_BASE		(CONFIG_SYS_DATAFLASH_LOGIC_ADDR_CS1 + 0x5280)
#define CONFIG_ENV_OFFSET		0x1080
#define CONFIG_ENV_ADDR			(CONFIG_SYS_DATAFLASH_LOGIC_ADDR_CS1 + CONFIG_ENV_OFFSET)
#define CONFIG_ENV_SIZE			0x4200
#define CONFIG_BOOTCOMMAND		"cp.b 0xD0065310 0x22000000 0x1AACF0; bootm"
#define CONFIG_BOOTARGS							\
	"console=ttyS0,115200 earlyprintk "				\
	"mtdparts=atmel_nand:128k(bootstrap)ro,640k(uboot)ro,"		\
	"128k(env1)ro,128k(env2)ro,8M(kernel),-(root) "			\
	"root=/dev/mtdblock5 "						\
	"rootfstype=jffs2 rootwait"

#elif defined(CONFIG_SYS_USE_NANDFLASH)
/* 
 * bootstrap + u-boot + env + linux in nandflash 
 * Nandflash layout (128kB sector size)
 *   0x00000000	128kB	AT91Bootstrap
 *   0x00020000 640kB	U-Boot
 *   0x000C0000 128kB	U-Boot Environment
 *   0x000E0000 128kB	U-Boot Environment (Backup)
 *   0x00100000 8M	Kernel
 *   0x00900000 	FS 
 */
#define CONFIG_ENV_IS_IN_NAND		1
#define CONFIG_ENV_OFFSET		0xC0000
#define CONFIG_ENV_OFFSET_REDUND	0xE0000
#define CONFIG_ENV_SIZE			0x20000		/* 1 sector = 128 kB */
#define CONFIG_BOOTCOMMAND		"nand read 0x22000000 0x100000 0x800000; bootm"
#define CONFIG_BOOTARGS							\
	"console=ttyS0,115200 earlyprintk "				\
	"mtdparts=atmel_nand:128k(bootstrap)ro,640k(uboot)ro,"		\
	"128k(env1)ro,128k(env2)ro,8M(kernel),-(root) "			\
	"root=/dev/mtdblock5 "						\
	"rootfstype=jffs2 rootwait"

#elif CONFIG_SYS_USE_MMC
/* 
 * bootstrap + u-boot + env in nandflash, linux and rootfs on sdcard
 * Nandflash layout:
 *   see CONFIG_SYS_USE_NANDFLASH
 * SDCard layout
 *   Part1: FAT         kernel (uImage)
 *   Part2: EXT4        rootfs 
 */
#define CONFIG_ENV_IS_IN_NAND		1
#define CONFIG_ENV_OFFSET		0xC0000
#define CONFIG_ENV_OFFSET_REDUND	0xE0000
#define CONFIG_ENV_SIZE			0x20000         /* 1 sector = 128 kB */
#define CONFIG_BOOTCOMMAND		"mmc rescan 0; ext2load mmc 0:1 0x22000000 uImage; bootm"
#define CONFIG_BOOTARGS							\
	"console=ttyS0,115200 earlyprintk "				\
	"mtdparts=atmel_nand:128k(bootstrap)ro,640k(uboot)ro,"		\
	"128k(env1)ro,128k(env2)ro,8M(kernel),-(root) "			\
	"root=/dev/mmcblk0p2 "						\
	"rootfstype=jffs2 rootwait"

#else
/* Unknown */
#error Boot config not supported for SAM9-L9260
#endif

/* Misc */
#define CONFIG_BOOTDELAY	2
#define CONFIG_SYS_PROMPT	"U-Boot> "
#define CONFIG_SYS_CBSIZE	256
#define CONFIG_SYS_MAXARGS	16
#define CONFIG_SYS_PBSIZE	(CONFIG_SYS_CBSIZE + sizeof(CONFIG_SYS_PROMPT) + 16)
#define CONFIG_SYS_LONGHELP	1
#define CONFIG_CMDLINE_EDITING	1
#define CONFIG_AUTO_COMPLETE

/* Size of malloc() pool */

#define CONFIG_SYS_MALLOC_LEN	ROUND(3 * CONFIG_ENV_SIZE + 128*1024, 0x1000)
/* Regular stack */

#define CONFIG_STACKSIZE	(32*1024)       
/*
 * Initial stack pointer: 4k - GENERATED_GBL_DATA_SIZE in internal SRAM,
 * leaving the correct space for initial global data structure above
 * that address while providing maximum stack area below.
 */
#define CONFIG_SYS_INIT_SP_ADDR	(ATMEL_BASE_SRAM1 + 4*1024 - GENERATED_GBL_DATA_SIZE)

/* IRQ/FIQ stuff not needed */
#ifdef CONFIG_USE_IRQ
#error CONFIG_USE_IRQ not supported
#endif

#endif
