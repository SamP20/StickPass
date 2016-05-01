#include "Menu.h"

#include <stdlib.h>
#include <string.h>
#include "Board/Joystick.h"
#include "Board/Display.h"
#include "Data/menus.h"
#include "Data/font6x8.h"

#include "Crypto.h"

static int8_t menuID = 0;
static uint8_t menuSelected = 0;
static menu_data_t activeMenu;
static char* entryBuffer = 0;
static uint8_t entryLen = 0;
static uint8_t entryCursor = 0;

void Menu_Init(void) {
    Menu_Setmenu(0);
}

void Menu_UpdateTask(void) {
    uint8_t joyStatus = Joystick_GetKeyRepeat();

    if(menuID >= 0) {
        Menu_MenuNav(joyStatus);
    }else{
        switch(menuID) {
            case MENU_SEARCH_ID:
            Menu_TextEntryNav(joyStatus);
            break;
        }
    }

    if(menuID >= 0) {
        Menu_MenuDraw();
    }else{
        switch(menuID) {
            case MENU_SEARCH_ID:
            Menu_TextEntryDraw();
            break;
        }
    }
}

void Menu_Setmenu(int8_t id) {
    menuID = id;
    if(menuID >= 0) {
        Menu_MenuInit(id);
    } else {
        switch(menuID) {
            case MENU_SEARCH_ID:
            Menu_TextEntryInit(15);
            break;
        }
    }
    Display_ResetScroll();
}

void Menu_MenuInit(int8_t id) {
    menuSelected = 0;
    memcpy_P(&activeMenu, &menus[id], sizeof(menu_data_t));
}

void Menu_MenuNav(uint8_t joyStatus) {
    if(joyStatus & JOY_UP) {
        if(menuSelected > 0) {
            menuSelected--;
        }
        Display_ResetScroll();
    }else if(joyStatus & JOY_DOWN) {
        if(menuSelected < activeMenu.itemcount-1) {
            menuSelected++;
        }
        Display_ResetScroll();
    }else if(joyStatus & JOY_LEFT) {
        Menu_Setmenu(activeMenu.parent_menu);
    }else if(joyStatus & JOY_RIGHT) {
        Menu_Setmenu(activeMenu.next_menus[menuSelected]);
    }else if(joyStatus & JOY_PRESS) {
        Menu_Setmenu(activeMenu.next_menus[menuSelected]);
    }
}

void Menu_MenuDraw(void) {
    Display_Clear();
    uint8_t i;
    for(i=0; i<activeMenu.itemcount;i++) {
        Display_WritePos_P(i,6, activeMenu.strings[i], menuSelected==i);
    }
    Display_WritePos(menuSelected, 0, "\x1F", false);
    Display_Update();
}

void Menu_TextEntryInit(uint8_t maxlen) {
    if(entryBuffer) free(entryBuffer);
    entryBuffer = malloc(maxlen+2);
    memset(entryBuffer, 0, maxlen+1);
    entryBuffer[0] = 'A';
    entryBuffer[1] = '\x7f';
    entryCursor = 0;
    entryLen = maxlen;
}

void Menu_TextEntryNav(uint8_t joyStatus) {
    if(joyStatus & JOY_UP) {
        if(entryBuffer[entryCursor] == ' ') {
            entryBuffer[entryCursor] = '\x7f';
        }else{
            entryBuffer[entryCursor]--;
        }

        // Edge case when entire string has been deleted
        if(entryBuffer[entryCursor+1] == '\0') {
            entryBuffer[entryCursor+1] = '\x7f';
            entryBuffer[entryCursor+2] = '\0';
        }
    }else if(joyStatus & JOY_DOWN) {
        if(entryBuffer[entryCursor] == '\x7f') {
            entryBuffer[entryCursor] = ' ';
        }else{
            entryBuffer[entryCursor]++;
        }

        // Edge case when entire string has been deleted
        if(entryBuffer[entryCursor+1] == '\0') {
            entryBuffer[entryCursor+1] = '\x7f';
            entryBuffer[entryCursor+2] = '\0';
        }
    }else if(joyStatus & JOY_LEFT) {
        if(entryBuffer[entryCursor] == '\x7f') {
            entryBuffer[entryCursor+1] = '\0';
        }
        if(entryCursor > 0) {
            entryCursor--;
        }
    }else if(joyStatus & JOY_RIGHT) {
        if(entryBuffer[entryCursor] == '\x7f') return;
        if(entryCursor < entryLen-1) entryCursor++;
        if(entryBuffer[entryCursor] == '\x7f') {
            entryBuffer[entryCursor] = entryBuffer[entryCursor-1];
            entryBuffer[entryCursor+1] = '\x7f';
            entryBuffer[entryCursor+2] = '\0';
        }
    }else if(joyStatus & JOY_PRESS) {
        if(entryBuffer[entryCursor] == '\x7f') {
            entryBuffer[entryCursor] = '\0';
        }
        Menu_TextEntryConfirm();
    }
}

void Menu_TextEntryConfirm(void) {
    switch(menuID) {
        case MENU_SEARCH_ID:
        Menu_Setmenu(0);
        break;
    }
}

void Menu_TextEntryDraw(void) {
    char digit[2] = {0x00};
    Display_Clear();
    uint8_t skip = 0;
    if(entryCursor > 5) {
        skip = entryCursor-5;
        if(skip+9 > entryLen) {
            skip = entryLen-9;
        }
    }
    Display_WritePos(3,0, entryBuffer+skip, false);

    digit[0] = entryBuffer[entryCursor];
    digit[0]--;
    if(digit[0] < ' ') digit[0] = '\x7f';
    Display_WritePos(2,(entryCursor-skip)*FONTWIDTH, digit, false);
    digit[0]--;
    if(digit[0] < ' ') digit[0] = '\x7f';
    Display_WritePos(1,(entryCursor-skip)*FONTWIDTH, digit, false);

    digit[0] = entryBuffer[entryCursor];
    digit[0]++;
    if(digit[0] > '\x7f') digit[0] = ' ';
    Display_WritePos(4,(entryCursor-skip)*FONTWIDTH, digit, false);
    digit[0]++;
    if(digit[0] > '\x7f') digit[0] = ' ';
    Display_WritePos(5,(entryCursor-skip)*FONTWIDTH, digit, false);

    Display_Update();
}
