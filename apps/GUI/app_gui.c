#include <stdlib.h>
#include <stdio.h>
#include <stdarg.h>
#include "app_gui.h"
#include "fsl_debug_console.h"
#include "string.h"
#include "fsl_clock.h"

#define CLEAR_BUTTON_ID (GUI_ID_BUTTON0)
#define COLOR_BUTTONS 8
#define COLOR_BUTTON_FIRST_ID (GUI_ID_USER)
#define COLOR_BUTTON_LAST_ID (COLOR_BUTTON_FIRST_ID + COLOR_BUTTONS - 1)

static GUI_COLOR button_color[COLOR_BUTTONS] = {GUI_WHITE,   GUI_YELLOW, GUI_ORANGE, GUI_RED,
                                                GUI_MAGENTA, GUI_BLUE,   GUI_GREEN,  GUI_BLACK};


static void cbBackgroundWin(WM_MESSAGE *pMsg)
{
    int widget_id;

    switch (pMsg->MsgId)
    {
        case WM_NOTIFY_PARENT:
            widget_id = WM_GetId(pMsg->hWinSrc);
            if (widget_id >= COLOR_BUTTON_FIRST_ID && widget_id <= COLOR_BUTTON_LAST_ID)
            {
                GUI_SetColor(button_color[widget_id - COLOR_BUTTON_FIRST_ID]);
            }
            else if (widget_id == CLEAR_BUTTON_ID && pMsg->Data.v == WM_NOTIFICATION_CLICKED)
            {
                GUI_Clear();
            }
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}
/*
static void cbCanvasWin(WM_MESSAGE *pMsg)
{
    GUI_PID_STATE *pid_state;
    static GUI_PID_STATE pid_state_0;

    switch (pMsg->MsgId)
    {
        case WM_TOUCH:
            pid_state = (GUI_PID_STATE *)pMsg->Data.p;
            if (pid_state->Pressed)
            {
                if (pid_state_0.Pressed)
                {
                    GUI_DrawLine(pid_state_0.x, pid_state_0.y, pid_state->x, pid_state->y);
                }
                else
                {
                    GUI_DrawPoint(pid_state->x, pid_state->y);
                }
            }
            pid_state_0 = *pid_state;
            break;
        default:
            WM_DefaultProc(pMsg);
    }
}
*/
void APP_GUI_Init()
{
	CLOCK_AttachClk(kMCLK_to_LCD_CLK);
	CLOCK_SetClkDiv(kCLOCK_DivLcdClk, 1, true);
	APP_LCDC_Init();
	/* emWin start */
	GUI_Init();
	/* Set size and default color for the background window */
	WM_SetSize(WM_HBKWIN, LCD_WIDTH, LCD_HEIGHT);
	WM_SetDesktopColor(GUI_BLACK);

    /* Set callback for the backgroung window */
    WM_SetCallback(WM_HBKWIN, cbBackgroundWin);

    /* Solid color display */
    GUI_SetBkColor(GUI_BLACK);
    GUI_Clear();
    GUI_SetColor(GUI_YELLOW);
    GUI_SetDrawMode(GUI_DM_NORMAL);
    GUI_SetPenSize(6);
    GUI_DispStringHCenterAt("ChauNM Pilot Project - LPC54608\n", LCD_WIDTH / 2, 0);
    GUI_DispNextLine();
    WM_Exec();
}

int APP_GUI_printf(const char* string, ...)
{
    char* displayString;
    char* lcdString;
    int i, line, lineCount, dispLength, pos;
    memset(displayString, 0, 1024);
    va_list ap;
    va_start(ap, string);
    vsprintf(displayString, string, ap);
    va_end(ap);
    dispLength = strlen(displayString);
    if (dispLength > 1024)
    	return -1;
    displayString = (char*)malloc(1024);
    line = GUI_GetDispPosY();
    if (dispLength <= GUI_BIDI_MAX_CHARS_PER_LINE)
    {
    	if (line >= LCD_HEIGHT - APP_GUI_FONT_H)
    	{
    		WM_Exec();
    		GUI_Clear();
    		GUI_GotoXY(0,0);
    	}
    	GUI_DispString(displayString);
    	WM_Exec();
    }
    else
    {
    	lineCount = dispLength / GUI_BIDI_MAX_CHARS_PER_LINE;
    	if ((dispLength % GUI_BIDI_MAX_CHARS_PER_LINE) > 0)
    		dispLength++;
    	if (line >= LCD_HEIGHT - (APP_GUI_FONT_H * lineCount))
    	{
    		WM_Exec();
    		GUI_Clear();
    		GUI_GotoXY(0,0);
    	}
    	lcdString = (char*)malloc(GUI_BIDI_MAX_CHARS_PER_LINE + 1);
    	pos = 0;
    	while (dispLength > 0)
    	{
    		i = 0;
    		memset(lcdString, 0, GUI_BIDI_MAX_CHARS_PER_LINE + 1);
    		while ((i < GUI_BIDI_MAX_CHARS_PER_LINE) && (dispLength > 0))
    		{
    			lcdString[i] = displayString[i + pos];
    			i++;
    			dispLength--;
    		}
    		pos += i;
    		GUI_DispString(lcdString);
    		if (dispLength > 0)
    			GUI_DispNextLine();
    	}
    	WM_Exec();
    	free(lcdString);
    }
    free(displayString);
    return 0;
}
