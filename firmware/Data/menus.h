#ifndef MENUS_H
#define MENUS_H

#include <avr/pgmspace.h>
#include "strings.h"

#define MENU_SEARCH_ID -1
#define MENU_TESTMEMORY_ID -2


typedef struct {
    uint8_t itemcount;
    uint8_t parent_menu;
    PGM_P strings[6];
    int8_t next_menus[6];
} menu_data_t;

const menu_data_t menus[] PROGMEM =
{
    // Home menu ID=0
    {
        .itemcount = 5,
        .parent_menu = 0,
        .strings =    {str_search_logins, str_browse_logins, str_add_login, str_settings, str_testmem},
        .next_menus = {MENU_SEARCH_ID, 1, 2, 3, MENU_TESTMEMORY_ID}
    },
    // Browse menu ID=1
    {
        .itemcount = 6,
        .parent_menu = 0,
        .strings = {str_sym, str_ae, str_fj, str_ko, str_pt, str_uz},
        .next_menus = {0}
    },
    // Add login menu ID=2
    {
        .itemcount = 1,
        .parent_menu = 0,
        .strings = {str_placeholder},
        .next_menus = {0}
    },
    // Settings menu ID=3
    {
        .itemcount = 1,
        .parent_menu = 0,
        .strings = {str_placeholder},
        .next_menus = {0}
    },
    // Settings menu ID=4
    {
        .itemcount = 1,
        .parent_menu = 0,
        .strings = {str_placeholder},
        .next_menus = {0}
    },
    // Test Pass menu ID=5
    {
        .itemcount = 1,
        .parent_menu = 0,
        .strings = {str_pass},
        .next_menus = {0}
    },
    // Test Fail menu ID=6
    {
        .itemcount = 1,
        .parent_menu = 0,
        .strings = {str_fail},
        .next_menus = {0}
    },

};

#endif
