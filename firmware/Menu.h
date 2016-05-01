#ifndef MENU_H
#define MENU_H

#include <stdint.h>

void Menu_Init(void);
void Menu_UpdateTask(void);
void Menu_Setmenu(int8_t id);

void Menu_MenuInit(int8_t id);
void Menu_MenuNav(uint8_t joyStatus);
void Menu_MenuDraw(void);

void Menu_TextEntryInit(uint8_t maxlen);
void Menu_TextEntryNav(uint8_t joyStatus);
void Menu_TextEntryConfirm(void);
void Menu_TextEntryDraw(void);




#endif
