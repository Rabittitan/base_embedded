#include "setting_screen.h"
#include "app_data.h"
#include "menu_screen.h"

static void view_scr_data() {

  // bool ret = mpu6050_get_data(&imu_data);

  // mpu6050_get_data(&imu_data);

  // int val =   imu_data.raw_acc_x;
  // int val_y = imu_data.raw_acc_y;
  // int val_z = imu_data.raw_acc_z;

  view_render.fillRect(0, 0, 130, 65, BLACK); // clear screen

  // draw Menu
  view_render.setTextSize(1);
  
  //  1: START
  view_render.setCursor(30, 30);
  view_render.print("get data");
  view_render.setCursor(0, 0);
  view_render.print(imu_data.raw_acc_x);

  view_render.setCursor(0, 10);
  view_render.print(imu_data.raw_acc_y);

  view_render.setCursor(0, 20);
  view_render.print(imu_data.raw_acc_z);

  // view_render.print(mpu6050_init());
};

view_dynamic_t dyn_view_data = {{
                                    .item_type = ITEM_TYPE_DYNAMIC,
                                },
                                view_scr_data};

view_screen_t scr_data = {
    &dyn_view_data,
    ITEM_NULL,
    ITEM_NULL,
    .focus_item = 0,
};

void scr_data_handle(ak_msg_t *msg) {
  switch (msg->sig) {
  case AC_DISPLAY_BUTON_MODE_PRESSED: {
    
    SCREEN_TRAN(scr_menu_handle, &scr_menu);

  } break;

  case SCREEN_ENTRY: {
    APP_DBG_SIG("buzzer call\n");
    //BUZZER_PlaySound(BUZZER_SOUND_STARTUP);
    timer_set(AC_TASK_DISPLAY_ID, AC_DISPLAY_RENDER_SCREEN, 50, TIMER_PERIODIC);


  } break;

  case SCREEN_EXIT: {
    timer_remove_attr(AC_TASK_DISPLAY_ID, AC_DISPLAY_RENDER_SCREEN);
  } break;

  default:{

  } break;

  }
}
