#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Wire.h"
#include "app.h"
#include "app_dbg.h"
#include "bitmap.h"
#include "fsm.h"
#include "game_over_screen.h"
#include "message.h"
#include "mpu6050.h"
#include "mpu6050.h"
#include "screen_manager.h"
#include "task_display.h"
#include "view_render.h"



struct ak_msg_t;

typedef enum {
  CORE_STATE_NORMAL,
  CORE_STATE_FREEZE,
  CORE_STATE_ULTIMATE,
  CORE_STATE_DEAD
} core_state_t;

typedef struct core_t {
  float x;
  float y;

  float vx;
  float vy;

  // float ax;
  // float ay;

  /* Gameplay */
  uint8_t hp;

  uint8_t combo;
  uint8_t ultimate;

  /* State */
  core_state_t state;
} core_t;




extern mpu6050_data_t imu_data;

#ifdef __cplusplus
extern "C" {
#endif
void task_core_handle(struct ak_msg_t *msg);
void core_draw();

#ifdef __cplusplus
}
#endif

#endif