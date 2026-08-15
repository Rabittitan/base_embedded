
#include "player.h"
#include "physics.h"

core_t static_core = {
    .x = 20,
    .y = 20,
    .hp = 3,
    .combo = 0,
    .state = CORE_STATE_NORMAL,
};

physics_t static_physics = {

  .ax = 0,
  .ay = 0,

};


void core_draw() {
    view_render.drawBitmap((int16_t)static_core.x, (int16_t)static_core.y, bitmap_ball, 16, 16, WHITE);
}

void task_core_handle(struct ak_msg_t *msg) {
  switch (msg->sig) {
  case CORE_SETUP_SIG: {

  } break;

  case CORE_UPDATE_SIG: {
    static_physics.ax =  imu_data.acc_x ;
    static_physics.ay =  imu_data.acc_y;

    physics_update(&static_core,&static_physics);


  } break;

  case CORE_RESET_SIG: {

  } break;

  // case CORE_MOVE_UP_SIG: {
  //   APP_DBG(">> Move up Success!\n");
  //   static_core.y += 10;

  // } break;

  // case CORE_MOVE_DOWN_SIG: {
  //   APP_DBG(">> Move Down Success!\n");
  //   static_core.y -= 10;
  // } break;

  // case CORE_MOVE_LEFT_SIG: {

  // } break;

  // case CORE_MOVE_RIGHT_SIG: {

  // } break;

  default:
    break;
  }
}