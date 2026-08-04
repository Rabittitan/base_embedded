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
            mpu6050_get_data(&imu_data);

            bool ret = mpu6050_get_data(&imu_data);

            if (!ret)
            {
                APP_DBG("MPU Read Failed\n");
            }
        }
        break;
    }
}