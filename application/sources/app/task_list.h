#ifndef __TASK_LIST_H__
#define __TASK_LIST_H__

#include "ak.h"
#include "task.h"

/*****************************************************************************/
/* TASK TABLE */
/*****************************************************************************/

extern const task_t app_task_table[];
extern const task_polling_t app_task_polling_table[];

/*****************************************************************************/
/* INTERNAL TASK ID
 *
 * IMPORTANT:
 * Task ID must remain in increasing order.
 */
/*****************************************************************************/

enum {
        TASK_TIMER_TICK_ID,

        AC_TASK_SYSTEM_ID,
        AC_TASK_SHELL_ID,
        AC_TASK_LIFE_ID,
        AC_TASK_DBG_ID,
        AC_TASK_DISPLAY_ID,
        AC_TASK_MPU_ID,



        AK_TASK_EOT_ID,



};






/*****************************************************************************/
/* POLLING TASK ID */
/*****************************************************************************/

enum {
        AC_TASK_POLLING_CONSOLE_ID,

        AK_TASK_POLLING_EOT_ID,
};

/*****************************************************************************/
/* TASK ENTRY POINT */
/*****************************************************************************/

extern void task_system(ak_msg_t*);
extern void task_shell(ak_msg_t*);
extern void task_life(ak_msg_t*);
extern void task_dbg(ak_msg_t*);
extern void task_game(ak_msg_t*);
extern void task_display(ak_msg_t*);
extern void task_mpu(ak_msg_t*);
/*****************************************************************************/
/* POLLING ENTRY POINT */
/*****************************************************************************/

extern void task_polling_console();

#endif
