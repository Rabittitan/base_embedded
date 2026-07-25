#ifndef __MENU_SCREEN_H__
#define __MENU_SCREEN_H__

#include    "fsm.h"
#include    "message.h"
#include    "task_display.h"
#include    "view_render.h"
#include    "bitmap.h"

extern view_dynamic_t dyn_view_menu;
extern view_screen_t scr_menu;
extern void scr_menu_handle(ak_msg_t* msg);

#endif