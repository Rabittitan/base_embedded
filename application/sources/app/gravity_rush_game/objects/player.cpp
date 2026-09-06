
#include "player.h"
#include "physics.h"

core_t static_core = {
  .x = 20.0f,
  .y = 20.0f,
  .vx = 0.0f,
  .vy = 0.0f,
  .hp = 3,
  .combo = 0,
  .ultimate = 0,
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
    static_core.x = 20.0f;
    static_core.y = 20.0f;

    static_core.vx = 0.0f;
    static_core.vy = 0.0f;

    static_core.hp = 3;

    static_core.combo = 0;
    static_core.ultimate = 0;

    static_core.state = CORE_STATE_NORMAL;

    static_physics.ax = 0.0f;
    static_physics.ay = 0.0f;
  } break;

  case CORE_UPDATE_SIG: {
    static_physics.ax =  imu_data.acc_x ;
    static_physics.ay =  imu_data.acc_y;

    physics_update(&static_core,&static_physics);
    APP_DBG("ax=%d vx=%d x=%d  acc_x=%d\n",
        (int)(static_physics.ax * 100),
        (int)(static_core.vx * 100),
        (int)(static_core.x * 100),
        (int)(imu_data.acc_x )
      );

    APP_DBG("ay=%d vy=%d x=%d  acc_y=%d\n",
        (int)(static_physics.ay * 100),
        (int)(static_core.vy * 100),
        (int)(static_core.y * 100),
        (int)(imu_data.acc_y )
      );
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