#include "task_game.h"
#include "app.h"
#include "task_life.h"
#include "button.h"
#include "app_bsp.h"
#include "app_dbg.h"
#include "view_render.h"
#include "timer.h"
#include "task_list.h"
#include "startup_screen.h"
#include "menu_screen.h"


void task_game(ak_msg_t *msg)
{
   
    switch (msg->sig)
    {

        case AC_GAME_INIT:
            
            timer_set(
               
                AC_TASK_GAME_ID,
                AC_GAME_TICK,
                AC_LIFE_TASK_TIMER_LED_LIFE_INTERVAL,
                TIMER_PERIODIC
            );
            
            break; 
        static uint32_t counter = 0;
        
        case AC_GAME_TICK:
            // led_toggle(&led_life);
            view_render.initialize();
            view_render.clear();
            
            view_render.setTextSize(2);

            view_render.setCursor(55, 25);

            view_render.print(counter++);

            view_render.update();

            
            scr_mng_dispatch(msg);
            //SCREEN_TRAN(scr_info_handle, &scr_menu);
            SCREEN_TRAN(scr_menu_handle, &scr_menu);
            break;   

        case AC_GAME_BUTTON_UP:
             //SCREEN_TRAN(scr_info_handle, &scr_info);

            //SCREEN_TRAN(view_scr_menu, &scr_menu);
            led_toggle(&led_life);
            break;

        // case AC_GAME_BUTTON_DOWN:
        //     led_toggle(&led_life);
        //     break;

        // case AC_GAME_BUTTON_MODE:
        //     led_toggle(&led_life);
        //     break;

        default:
            
            break;
    }
}