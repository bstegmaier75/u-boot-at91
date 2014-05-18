/*
 * (C) Copyright 2014
 * Bernhard Stegmaier <stegmaier@sw-systems.de>
 *
 * based on AT91SAM9260EK and Olimex patches
 *
 * SPDX-License-Identifier:	GPL-2.0+
 */

#include <common.h>
#include <asm/io.h>
#include <asm/arch/at91sam9260_matrix.h>
#include <asm/arch/at91sam9_smc.h>
#include <asm/arch/at91_common.h>
#include <asm/arch/at91_pmc.h>
#include <asm/arch/gpio.h>

#if defined(CONFIG_RESET_PHY_R) && defined(CONFIG_MACB)
# include <net.h>
#endif
#include <netdev.h>

#ifdef CONFIG_GENERIC_ATMEL_MCI
# include <mmc.h>
# include <atmel_mci.h>
#endif


DECLARE_GLOBAL_DATA_PTR;


/******************************************************************************
 * NAND initializations
 *****************************************************************************/
#ifdef CONFIG_CMD_NAND
static void sam9l9260_nand_hw_init(void)
{
	struct at91_smc *smc = (struct at91_smc *)ATMEL_BASE_SMC;
	struct at91_matrix *matrix = (struct at91_matrix *)ATMEL_BASE_MATRIX;
	unsigned long csa;

	/* Assign CS3 to NAND/SmartMedia Interface */
	csa = readl(&matrix->ebicsa);
	csa |= AT91_MATRIX_CS3A_SMC_SMARTMEDIA;
	writel(csa, &matrix->ebicsa);

	/* Configure SMC CS3 for NAND/SmartMedia */
	writel(AT91_SMC_SETUP_NWE(1) | AT91_SMC_SETUP_NCS_WR(0) |
		AT91_SMC_SETUP_NRD(1) | AT91_SMC_SETUP_NCS_RD(0),
		&smc->cs[3].setup);
	writel(AT91_SMC_PULSE_NWE(3) | AT91_SMC_PULSE_NCS_WR(3) |
		AT91_SMC_PULSE_NRD(3) | AT91_SMC_PULSE_NCS_RD(3),
		&smc->cs[3].pulse);
	writel(AT91_SMC_CYCLE_NWE(5) | AT91_SMC_CYCLE_NRD(5),
		&smc->cs[3].cycle);
	writel(AT91_SMC_MODE_RM_NRD | AT91_SMC_MODE_WM_NWE |
		AT91_SMC_MODE_EXNW_DISABLE |
#ifdef CONFIG_SYS_NAND_DBW_16
		AT91_SMC_MODE_DBW_16 |
#else /* CONFIG_SYS_NAND_DBW_8 */
		AT91_SMC_MODE_DBW_8 |
#endif
		AT91_SMC_MODE_TDF_CYCLE(2),
		&smc->cs[3].mode);

	/* Configure RDY/BSY */
	at91_set_gpio_input(CONFIG_SYS_NAND_READY_PIN, 1);

	/* Enable NandFlash */
	at91_set_gpio_output(CONFIG_SYS_NAND_ENABLE_PIN, 1);

}
#endif


/******************************************************************************
 * Ethernet initializations
 *****************************************************************************/
#ifdef CONFIG_MACB
static void sam9l9260_macb_hw_init(void)
{
	struct at91_pmc *pmc = (struct at91_pmc *)ATMEL_BASE_PMC;
	struct at91_port *pioa = (struct at91_port *)ATMEL_BASE_PIOA;

	/* Enable EMAC clock */
	writel(1 << ATMEL_ID_EMAC0, &pmc->pcer);

	/*
	 * Disable pull-up on:
	 *	RXDV (PA17) => PHY normal mode (not Test mode)
	 *	ERX0 (PA14) => PHY ADDR0
	 *	ERX1 (PA15) => PHY ADDR1
	 *	ERX2 (PA25) => PHY ADDR2
	 *	ERX3 (PA26) => PHY ADDR3
	 *	ECRS (PA28) => PHY ADDR4  => PHYADDR = 0x0
	 *
	 * PHY has internal pull-down
	 */
	writel(pin_to_mask(AT91_PIN_PA14) |
		pin_to_mask(AT91_PIN_PA15) |
		pin_to_mask(AT91_PIN_PA17) |
		pin_to_mask(AT91_PIN_PA25) |
		pin_to_mask(AT91_PIN_PA26) |
		pin_to_mask(AT91_PIN_PA28),
		&pioa->pudr);

	/* Reset PHY */
	at91_phy_reset();

	/* Re-enable pull-up */
	writel(pin_to_mask(AT91_PIN_PA14) |
		pin_to_mask(AT91_PIN_PA15) |
		pin_to_mask(AT91_PIN_PA17) |
		pin_to_mask(AT91_PIN_PA25) |
		pin_to_mask(AT91_PIN_PA26) |
		pin_to_mask(AT91_PIN_PA28),
		&pioa->puer);

	/* Initialize EMAC=MACB hardware */
	at91_macb_hw_init();
}
#endif

#ifdef CONFIG_RESET_PHY_R
void reset_phy(void)
{
}
#endif

int board_eth_init(bd_t *bis)
{
	int rc = 0;
#ifdef CONFIG_MACB
	rc = macb_eth_initialize(0, (void *)ATMEL_BASE_EMAC0, 0x01);
#endif
	return rc;
}


/******************************************************************************
 * MMC/SDCard initialisations (override weak defines from drivers/mmc/mmc.c)
 *****************************************************************************/
#ifdef CONFIG_GENERIC_ATMEL_MCI
int board_mmc_init(bd_t *bd)
{
	/* Setup MCI port pins */
	at91_mci_hw_init();

        /* This calls the atmel_mci_init in gen_atmel_mci.c */
	return atmel_mci_init((void *)ATMEL_BASE_MCI);
}

int board_mmc_getcd(struct mmc *mmc)
{
        return !at91_get_gpio_value(CONFIG_SYS_MMC_CD_PIN);
}
#endif


/******************************************************************************
 * Board specific initialisations
 *****************************************************************************/
/* Setup peripheral clocks in board_early_init_f */
int board_early_init_f(void)
{
	struct at91_pmc *pmc = (struct at91_pmc *)ATMEL_BASE_PMC;

	/* Enable clocks for all PIOs */
	writel((1 << ATMEL_ID_PIOA) | 
		(1 << ATMEL_ID_PIOB) |
		(1 << ATMEL_ID_PIOC),
		&pmc->pcer);

#ifdef CONFIG_GENERIC_ATMEL_MCI
	/* Enable clock fror MCI */
        writel(1 << ATMEL_ID_MCI, &pmc->pcer);
#endif

	return 0;
}

/* Setup peripherals */
int board_init(void)
{
        /* arch number of SAM9-L9260-Board */
        gd->bd->bi_arch_number = MACH_TYPE_SAM9_L9260;
	/* adress of boot parameters */
	gd->bd->bi_boot_params = CONFIG_SYS_SDRAM_BASE + 0x100;

	at91_seriald_hw_init();
#ifdef CONFIG_HAS_DATAFLASH
	at91_spi0_hw_init((1 << 0) | (1 << 1));
#endif
#ifdef CONFIG_CMD_NAND
	sam9l9260_nand_hw_init();
#endif
#ifdef CONFIG_MACB
	sam9l9260_macb_hw_init();
#endif

	return 0;
}

/* Setup DRAM */
int dram_init(void)
{
	gd->ram_size = get_ram_size(
		(void *)CONFIG_SYS_SDRAM_BASE,
		CONFIG_SYS_SDRAM_SIZE);

	return 0;
}


