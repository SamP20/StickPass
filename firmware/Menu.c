#include "Menu.h"

#include "Board/Joystick.h"
#include "Board/Display.h"
#include "Data/strings.h"

#include "Crypto.h"

static uint8_t menuID = 0;
static uint8_t menuSelected = 0;
static uint8_t count = 0;

void Menu_UpdateTask(void) {
    uint8_t joyStatus = Joystick_GetKeyRepeat();
    if(joyStatus & JOY_UP) {
        Menu_ScrollUp();
    }else if(joyStatus & JOY_DOWN) {
        Menu_ScrollDown();
    }else if(joyStatus & JOY_LEFT) {
        Menu_Back();
    }else if(joyStatus & JOY_RIGHT) {
        Menu_Enter();
    }
}

void Menu_ScrollUp(void) {
    Display_Clear();
    char s[5];
    snprintf(s, sizeof(s), "%.2x", count++);
    Display_WritePos(0,0, s);
    Display_Update();
    //Menu_Redraw();
}

void Menu_ScrollDown(void) {
    Display_Clear();
    Display_WritePos_P(0,0, str_down);
    Display_Update();
    //Menu_Redraw();
}

void Menu_Enter(void) {
    Display_Clear();
    Display_WritePos_P(0,0, str_right);
    Display_Update();
    //Menu_Redraw();
}

void Menu_Back(void) {
    Display_Clear();
    Display_WritePos_P(0,0, str_left);
    Display_Update();
    //Menu_Redraw();
}

void Menu_Redraw(void) {
    Display_Clear();
    switch (menuID) {
        case MENU_HOME_MENU:
            Display_WritePos_P(0,0, str_login);
    }
    Display_Update();
}
