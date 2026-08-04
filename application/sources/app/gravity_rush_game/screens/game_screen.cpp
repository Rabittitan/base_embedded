#include "game_screen.h"

static void view_game_screen()
{
   
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
    
}