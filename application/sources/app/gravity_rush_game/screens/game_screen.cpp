#include "game_screen.h"

static void view_game_screen()
{
   core_draw();
};

view_dynamic_t dyn_view_game = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_game_screen};

view_screen_t game_screen = {
    &dyn_view_game,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

// ================LOGIC (EVENT HANDLER)======================== //
void game_screen_handle(ak_msg_t *msg)
{
    
    switch (msg->sig)
    {
     case SCREEN_ENTRY:{
        APP_DBG(">> Entered game Screen Success!\n");
        APP_DBG(">> step1 !\n");
        task_post_pure_msg(TG_CORE_TASK_ID,CORE_SETUP_SIG);
        APP_DBG(">> step2 !\n");
        timer_set(AC_TASK_DISPLAY_ID , AC_DISPLAY_SHOW_TANK_MOVING_UPDATE, 60, TIMER_PERIODIC);
    
    } break;
    
    case AC_DISPLAY_SHOW_TANK_MOVING_UPDATE:{
        task_post_pure_msg(TG_CORE_TASK_ID,CORE_UPDATE_SIG);


    }break;

    // case AC_DISPLAY_BUTON_UP_PRESSED:{
    //     task_post_pure_msg(TG_CORE_TASK_ID,CORE_MOVE_UP_SIG);
    // }break;

    // case AC_DISPLAY_BUTON_DOWN_PRESSED:{
    //     task_post_pure_msg(TG_CORE_TASK_ID,CORE_MOVE_DOWN_SIG);

    // }break;

    default:
        break;
    }
}