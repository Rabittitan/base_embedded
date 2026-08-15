#ifndef __GAME_SCREEN_H__
#define __GAME_SCREEN_H__

#include "fsm.h"
#include "message.h"
#include "task_display.h"
#include "player.h"
#include "app.h"


extern view_dynamic_t dyn_view_game;
extern view_screen_t game_screen;
extern void game_screen_handle(ak_msg_t* msg);

#endif 