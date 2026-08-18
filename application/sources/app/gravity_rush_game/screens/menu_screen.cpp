#include "menu_screen.h"
#include "setting_screen.h"
#include "game_screen.h" 

static uint8_t menu_index = 0; 
// ================ (DRAW)========================
static void view_scr_menu()
{
    
    view_render.fillRect(0,0,130,65,BLACK); // clear screen
    // draw 124x60 bitmap 
    //view_render.drawBitmap(0, 0, bitmap_banner_game, 124, 60, WHITE);


    // draw Menu
    view_render.setTextSize(1);
    
    //  1: STARTS
    view_render.setCursor(50, 25);
    view_render.print("START");
    //  2: TOP SCORE
    view_render.setCursor(50, 37);
    view_render.print("TEST");
    
    //  3: EXIT
    view_render.setCursor(50, 49);
    view_render.print("EXIT");

    // (Cursor) on menu_index '>'
    int8_t cursor_y = 25 + (menu_index * 12); 
    view_render.setCursor(40, cursor_y);
    view_render.print(">");
    
};


view_dynamic_t dyn_view_menu = {
    {
        .item_type = ITEM_TYPE_DYNAMIC,
    },
    view_scr_menu 
};


view_screen_t scr_menu = {
    &dyn_view_menu,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};


// ================logic (Event Handler)========================
void scr_menu_handle(ak_msg_t* msg){
    //msg -> sig:  (signal) from (Active Kernel).
    switch (msg -> sig){
        //when this screen active
        case SCREEN_ENTRY:
           menu_index = 0; 
            APP_DBG(">> Entered BANNER GAME Screen!\n");
            BUZZER_PlaySound(BUZZER_SOUND_STARTUP);

            // CLEAR ALL TIMER UPDATE 
            timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_SHOW_CORE_MOVING_UPDATE);
            break;

       
        case AC_DISPLAY_SHOW_CORE_MOVING_UPDATE:
        //     break;

        //when button "MODE" released
        case AC_DISPLAY_BUTON_MODE_PRESSED: 
            if (menu_index == 0) {
                // START GAME: (scr_game)
              //  SCREEN_TRAN(scr_game_handle, &scr_game); 
              SCREEN_TRAN(game_screen_handle, &game_screen);
            }
            else if (menu_index == 1) {
                // goto TOP SCORE 
                SCREEN_TRAN(scr_data_handle, &scr_data);
            }
            else if (menu_index == 2) {
                // EXIT game
                SCREEN_TRAN(scr_startup_handle, &scr_startup);
            }
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            break;
        //when button UP released
        case AC_DISPLAY_BUTON_UP_PRESSED:
            if (menu_index > 0) menu_index--;
            else menu_index = 2; 
            BUZZER_PlaySound(BUZZER_SOUND_CLICK); 
            break;
        //when button DOWN released
        case AC_DISPLAY_BUTON_DOWN_PRESSED:
            if (menu_index < 2) menu_index++;
            else menu_index = 0; 
            BUZZER_PlaySound(BUZZER_SOUND_CLICK);
            break;

        
        default:
            break;
    }
}

