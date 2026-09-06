#include "task_mpu.h"
#include "mpu6050.h"
#include "app_data.h"

#include "fsm.h"
#include "port.h"
#include "message.h"
#include "timer.h"

#include "app.h"
#include "app_dbg.h"

#include "task_list.h"

void task_mpu(ak_msg_t* msg){

    
    switch(msg->sig)
    {
        case AC_MPU6050_INIT:
        {
            //i2c_init();
            mpu6050_init();

        }
        break;

        case AC_MPU6050_UPDATE:
        {
            //mpu6050_get_data(&imu_data);

            bool ret = mpu6050_get_data(&imu_data);

            if (!ret)
            {
                APP_DBG("MPU Read Failed\n");
            }


            
            // Lấy tín hiệu điều khiển game
            float control_x = mpu6050_get_game_control_x();  // -1.0 đến 1.0
            float control_y = mpu6050_get_game_control_y();  // -1.0 đến 1.0
            

            APP_DBG("dataa x: %.2f | y: %.2f\n", control_x, control_y);

            // Áp dụng vào game
            if (control_x > 0.1f) {
                // Di chuyển nhân vật sang phải
                // game_move_right(control_x);
            } else if (control_x < -0.1f) {
                // Di chuyển nhân vật sang trái
                // game_move_left(-control_x);
            }
            
            if (control_y > 0.1f) {
                // Di chuyển nhân vật lên
                // game_move_up(control_y);
            } else if (control_y < -0.1f) {
                // Di chuyển nhân vật xuống
                // game_move_down(-control_y);
            }


           // sys_ctrl_delay_ms(500);

        }
        break;




    
    }
}