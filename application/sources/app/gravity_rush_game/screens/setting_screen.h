#ifndef __SETTING_SCREEN_H__
#define __SETTING_SCREEN_H__

#include    "fsm.h"
#include    "message.h"
#include    "task_display.h"
#include    "view_render.h"
#include    "bitmap.h"
#include    "screen_manager.h"
#include    "Wire.h"
#include    "mpu6050.h"

extern mpu6050_data_t imu_data;

extern view_dynamic_t dyn_view_data;
extern view_screen_t scr_data;
extern void scr_data_handle(ak_msg_t* msg);

#endif