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
#include <asm/arch/gpio.h>

void coloured_LED_init(void)
{
	/* Clock is enabled in board_early_init_f() */
	at91_set_gpio_output(CONFIG_RED_LED, 1);
	at91_set_gpio_output(CONFIG_GREEN_LED, 1);
	at91_set_gpio_value(CONFIG_RED_LED, 0);
	at91_set_gpio_value(CONFIG_GREEN_LED, 1);
}
