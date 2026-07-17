#include "task_game.h"
#include "app.h"
#include "task_life.h"
#include "button.h"
#include "app_bsp.h"
#include "app_dbg.h"
#include "view_render.h"
#include "timer.h"
#include "task_list.h"

void task_game(ak_msg_t *msg)
{
    int i =0;
    switch (msg->sig)
    {
        case AC_GAME_INIT:
           
            timer_set(
               
                AC_TASK_GAME_ID,
                AC_GAME_TICK,
                AC_GAME_TASK_TIMER_TICK_INTERVAL,
                TIMER_PERIODIC
            );
            led_life_on();
            break; 
        static uint32_t counter = 0;

        case AC_GAME_TICK:
            view_render.initialize();
            view_render.clear();
            led_life_on();
            view_render.setTextSize(2);

            view_render.setCursor(55, 25);

            view_render.print(counter++);

            view_render.update();

            break;   

        // case AC_GAME_BUTTON_UP:
        //     led_toggle(&led_life);
        //     break;

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