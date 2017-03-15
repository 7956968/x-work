#include <linux/platform_device.h>

#include <linux/input.h>
#include <mach/jzgpio_keys.h>
#include <linux/input/matrix_keypad.h>
#include "board.h"

#ifdef CONFIG_KEYBOARD_MATRIX

#define KEY_R0 GPIO_PB(6)
#define KEY_R1 GPIO_PB(7)
#define KEY_R2 GPIO_PB(8)
#define KEY_R3 GPIO_PB(9)

#define KEY_C0 GPIO_PB(0)
#define KEY_C1 GPIO_PB(1)
#define KEY_C2 GPIO_PB(2)
#define KEY_C3 GPIO_PB(3)
#define KEY_C4 GPIO_PB(4)

static const unsigned int matrix_keypad_cols[] = {KEY_C0, KEY_C1, KEY_C2, KEY_C3, KEY_C4};
static const unsigned int matrix_keypad_rows[] = {KEY_R0, KEY_R1, KEY_R2, KEY_R3};
static uint32_t js1_keymap[] =
{
	//KEY(row, col, keycode)
	KEY(0,0, KEY_MODE),
	KEY(0,1, KEY_F1),
	KEY(0,2, KEY_WAKEUP),
	KEY(0,3, KEY_1),
	KEY(0,4, KEY_2),

	KEY(1,0, KEY_VOLUMEDOWN),
	KEY(1,1, KEY_VOLUMEUP),
	KEY(1,2, KEY_PLAYPAUSE),
	KEY(1,3, KEY_3),
	KEY(1,4, KEY_4),

	KEY(2,0, KEY_PREVIOUSSONG),
	KEY(2,1, KEY_NEXTSONG),
	KEY(2,2, KEY_MENU),
	KEY(2,3, KEY_5),
	KEY(2,4, KEY_6),

	KEY(3,0, KEY_F3),
	KEY(3,1, KEY_BLUETOOTH),
	KEY(3,2, KEY_RECORD),
	KEY(3,3, KEY_7),
	KEY(3,4, KEY_8),
};

static struct matrix_keymap_data js1_keymap_data =
{
	.keymap = js1_keymap,
	.keymap_size =ARRAY_SIZE(js1_keymap),
};

struct matrix_keypad_platform_data js1_keypad_data  =
{
	.keymap_data = &js1_keymap_data,
	.col_gpios = matrix_keypad_cols,
	.row_gpios = matrix_keypad_rows,
	.num_col_gpios = ARRAY_SIZE(matrix_keypad_cols),
	.num_row_gpios = ARRAY_SIZE(matrix_keypad_rows),
	.col_scan_delay_us = 10,
	.debounce_ms =100,
	.wakeup  = 1,
	.active_low  = 1,
	.no_autorepeat = 1,
};

struct platform_device jz_matrix_kdb_device = {
	.name		= "matrix-keypad",
	.id		= 0,
	.dev		= {
		.platform_data  = &js1_keypad_data,
	}
};
#endif /* CONFIG_KEYBOARD_MATRIX */

#ifdef CONFIG_KEYBOARD_JZGPIO
static struct jz_gpio_keys_button board_longbuttons[] = {
#ifdef GPIO_POWERDOWN
        {
                .gpio                           = GPIO_POWERDOWN,
                .code = {
                        .longpress_code         = KEY_POWER,
                },
                .desc                           = "power down & wakeup",
                .active_low                     = ACTIVE_LOW_POWERDOWN,
                .longpress_interval             = 3000,
		.wakeup                         = 1,
                .debounce_interval              = 2,
        },
#endif
};

static struct jz_gpio_keys_platform_data board_longbutton_data = {
        .buttons        = board_longbuttons,
        .nbuttons       = ARRAY_SIZE(board_longbuttons),
};

struct platform_device jz_longbutton_device = {
        .name           = "jz-gpio-keys",
        .id             = -1,
        .num_resources  = 0,
        .dev            = {
                .platform_data  = &board_longbutton_data,
        }
};
#endif /* CONFIG_KEYBOARD_JZGPIO */
