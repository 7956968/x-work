#include <linux/platform_device.h>

#include <linux/input.h>
#include <mach/jzgpio_keys.h>
#include <linux/input/matrix_keypad.h>
#include "board.h"

#ifdef CONFIG_KEYBOARD_MATRIX

#define KEY_R0 GPIO_PA(8)//GPIO_PB(3)
#define KEY_R1 GPIO_PA(9)//GPIO_PB(2)
#define KEY_R2 GPIO_PA(10)//GPIO_PB(1)
#define KEY_R3 GPIO_PA(11)//GPIO_PB(0)

#define KEY_C0 GPIO_PA(12)//GPIO_PB(24)
#define KEY_C1 GPIO_PA(13)//GPIO_PB(23)
#define KEY_C2 GPIO_PA(14)//GPIO_PB(4)

static const unsigned int matrix_keypad_cols[] = {KEY_C0, KEY_C1, KEY_C2};
static const unsigned int matrix_keypad_rows[] = {KEY_R0, KEY_R1,KEY_R2, KEY_R3};
static uint32_t canna_keymap[] =
{
	//KEY(row, col, keycode)
	KEY(0,0, KEY_MODE),		/* AP/STA */
	KEY(0,1, KEY_F1),		/* AIRKISS */
	KEY(0,2, KEY_WAKEUP),
	KEY(1,0, KEY_VOLUMEDOWN),
	KEY(1,1, KEY_VOLUMEUP),
	KEY(1,2, KEY_PLAYPAUSE),
	KEY(2,0, KEY_PREVIOUSSONG),
	KEY(2,1, KEY_NEXTSONG),
	KEY(2,2, KEY_MENU),
	KEY(3,0, KEY_F3), 		/* music shortcut key 1 */
	KEY(3,1, KEY_BLUETOOTH),
	KEY(3,2, KEY_RECORD),
};

static struct matrix_keymap_data canna_keymap_data =
{
	.keymap = canna_keymap,
	.keymap_size =ARRAY_SIZE(canna_keymap),
};

struct matrix_keypad_platform_data canna_keypad_data  =
{
	.keymap_data = &canna_keymap_data,
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
		.platform_data  = &canna_keypad_data,
	}
};
#endif /* CONFIG_KEYBOARD_MATRIX */

#ifdef CONFIG_KEYBOARD_JZGPIO
static struct jz_gpio_keys_button board_longbuttons[] = {

#ifdef GPIO_NW5027_P3_KEY//JJJHHH
	{//RESET
                .gpio                           = GPIO_NW5027_P3_KEY0,
                .code = {
                        .shortpress_code        = KEY_F1,
                        .longpress_code         = KEY_MODE,
                },
                .desc                           = "RESET KEY OF NW5027 P3",
                .active_low                     = ACTIVE_LOW_NW5027_P3_KEY,
                .longpress_interval             = 2000,//1s 
                .debounce_interval              = 2,
    },
	{
                .gpio                           = GPIO_KEY_PALYPAUSE,
                .code = {
                        .shortpress_code        = KEY_PLAYPAUSE,
                        .longpress_code         = KEY_NEXTSONG,
                },
                .desc                           = "KEY1 OF NW5027 P3",
                .active_low                     = ACTIVE_LOW_NW5027_P3_KEY,
                .longpress_interval             = 1000,//1s 
                .debounce_interval              = 2,
    },
    {
                .gpio                           = GPIO_KEY_LIKE,
                .code = {
                        .shortpress_code        = KEY_F3,
                        .longpress_code         = KEY_RESERVED,
                },
                .desc                           = "KEY2 OF NW5027 P3",
                .active_low                     = ACTIVE_LOW_NW5027_P3_KEY,
                .longpress_interval             = 0,
                .debounce_interval              = 2,
    },
    {//KEY3
                .gpio                           = GPIO_KEY_RECORD,
                .code = {
                        .shortpress_code        = KEY_RECORD,
                        .longpress_code         = KEY_RESERVED,
                },
                .desc                           = "KEY3 OF NW5027 P3",
                .active_low                     = ACTIVE_LOW_NW5027_P3_KEY,
                .longpress_interval             = 0,
    },
          
    {//VOL-
                .gpio                           = GPIO_KEY_VOLDOWN,
                .code = {
                        .shortpress_code        = KEY_VOLUMEDOWN,
                        .longpress_code         = KEY_RESERVED,
                },
                .desc                           = "key voldown",
                .active_low                     = ACTIVE_LOW_NW5027_P3_KEY,
                .longpress_interval             = 0,
                .debounce_interval              = 2,
    },
    {//VOL+
                .gpio                           = GPIO_KEY_VOLUP,
                .code = {
                        .shortpress_code        = KEY_VOLUMEUP,
                        .longpress_code         = KEY_RESERVED,
                },
                .desc                           = "key volup",
                .active_low                     = ACTIVE_LOW_NW5027_P3_KEY,
                .longpress_interval             = 0,
                .debounce_interval              = 2,
    },
    {//MODE
                .gpio                           = GPIO_KEY_MODE,
                .code = {
                        .shortpress_code        = KEY_MENU,
                        .longpress_code         = KEY_RESERVED,
                },
                .desc                           = "key mode",
                .active_low                     = ACTIVE_LOW_NW5027_P3_KEY,
                .longpress_interval             = 0,
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
