#ifndef __APP_GUI_H__
#define __APP_GUI_H__
#include "GUI.h"
#include "GUIDRV_Lin.h"
#include "BUTTON.h"
#include "emwin_support.h"

void APP_GUI_Init();
int APP_GUI_printf(const char* string, ...);

#define LCD_PRINTF      APP_GUI_printf
#define APP_GUI_FONT    GUI_FONT_6X8_ASCII
#define APP_GUI_FONT_H  8
#define APP_GUI_FONT_V  6
#define APP_GUI_MAX_CHARACTER_PER_LINE	80


#endif // _APP_GUI_H__
