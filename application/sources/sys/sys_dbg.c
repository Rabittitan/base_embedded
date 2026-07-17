/**
 ******************************************************************************
 * @author: GaoKong
 * @date:   05/09/2016
 ******************************************************************************
**/
#include "sys_dbg.h"
#include "sys_ctrl.h"
#include "sys_irq.h"
#include "sys_cfg.h"
#include "io_cfg.h"

#include "ak.h"
#include "app_eeprom.h"
#include "xprintf.h"

#define DUMP_RAM_UNIT_SIZE			256

static fatal_log_t fatal_log;

void sys_dbg_fatal(const int8_t* s, uint8_t c) {
        memset(&fatal_log, 0, sizeof(fatal_log));

        strncpy((char *)fatal_log.string,
                (const char *)s,
                sizeof(fatal_log.string) - 1);

        fatal_log.string[sizeof(fatal_log.string) - 1] = '\0';
        fatal_log.code = c;

        fatal_log.current_task = *get_current_task_info();
        fatal_log.current_active_object = *get_current_active_object();

        fatal_log.m3_core_reg.ipsr = __get_IPSR();
        fatal_log.m3_core_reg.primask = __get_PRIMASK();
        fatal_log.m3_core_reg.faultmask = __get_FAULTMASK();
        fatal_log.m3_core_reg.basepri = __get_BASEPRI();
        fatal_log.m3_core_reg.control = __get_CONTROL();

        __disable_irq();

        SYS_PRINT("\n");
        SYS_PRINT("================================================\n");
        SYS_PRINT(" AK FATAL\n");
        SYS_PRINT("================================================\n");

        SYS_PRINT("\n[FATAL]\n");
        SYS_PRINT("type      : %s\n", fatal_log.string);
        SYS_PRINT("code      : 0x%02X\n", fatal_log.code);

        SYS_PRINT("\n[AK TASK]\n");
        SYS_PRINT("id        : %d\n", fatal_log.current_task.id);
        SYS_PRINT("priority  : %d\n", fatal_log.current_task.pri);
        SYS_PRINT("entry     : 0x%08X\n", fatal_log.current_task.task);

        SYS_PRINT("\n[AK ACTIVE MESSAGE]\n");
        SYS_PRINT("task      : %d\n",
                  fatal_log.current_active_object.des_task_id);
        SYS_PRINT("signal    : %d\n",
                  fatal_log.current_active_object.sig);
        SYS_PRINT("type      : 0x%X\n",
                  get_msg_type(&fatal_log.current_active_object));
        SYS_PRINT("ref count : %d\n",
                  get_msg_ref_count(&fatal_log.current_active_object));

#if defined(AK_TASK_OBJ_LOG_ENABLE)
        SYS_PRINT("\n[AK EVENT TRACE]\n");

        ak_msg_t trace_msg;
        uint32_t trace_index = 0;

        while (log_queue_len(&log_task_dbg_object_queue)) {
                log_queue_get(
                        &log_task_dbg_object_queue,
                        &trace_msg
                );

                uint32_t wait_time;

                if (trace_msg.dbg_handler.start_exe >=
                    trace_msg.dbg_handler.start_post) {

                        wait_time =
                                trace_msg.dbg_handler.start_exe -
                                trace_msg.dbg_handler.start_post;
                }
                else {
                        wait_time =
                                trace_msg.dbg_handler.start_exe +
                                (0xFFFFFFFF -
                                 trace_msg.dbg_handler.start_post);
                }

                uint32_t exe_time;

                if (trace_msg.dbg_handler.stop_exe >=
                    trace_msg.dbg_handler.start_exe) {

                        exe_time =
                                trace_msg.dbg_handler.stop_exe -
                                trace_msg.dbg_handler.start_exe;
                }
                else {
                        exe_time =
                                trace_msg.dbg_handler.stop_exe +
                                (0xFFFFFFFF -
                                 trace_msg.dbg_handler.start_exe);
                }

                SYS_PRINT(
                        "#%d task=%d sig=%d type=0x%X ref=%d wait=%d exe=%d\n",
                        trace_index++,
                        trace_msg.des_task_id,
                        trace_msg.sig,
                        get_msg_type(&trace_msg),
                        get_msg_ref_count(&trace_msg),
                        wait_time,
                        exe_time
                );
        }
#endif

#if defined(AK_IRQ_OBJ_LOG_ENABLE)
        SYS_PRINT("\n[IRQ TRACE]\n");

        exception_info_t trace_irq;
        uint32_t irq_index = 0;

        while (log_queue_len(&log_irq_queue)) {
                log_queue_get(
                        &log_irq_queue,
                        &trace_irq
                );

                SYS_PRINT(
                        "#%d exception=%d irq=%d timestamp=%d\n",
                        irq_index++,
                        trace_irq.except_number,
                        (int32_t)trace_irq.except_number -
                        (int32_t)
                        SYS_IRQ_EXCEPTION_NUMBER_IRQ0_NUMBER_RESPECTIVE,
                        trace_irq.timestamp
                );
        }
#endif

        SYS_PRINT("\n[CPU CORE]\n");
        SYS_PRINT("IPSR      : %d\n",
                  fatal_log.m3_core_reg.ipsr);
        SYS_PRINT("PRIMASK   : 0x%08X\n",
                  fatal_log.m3_core_reg.primask);
        SYS_PRINT("FAULTMASK : 0x%08X\n",
                  fatal_log.m3_core_reg.faultmask);
        SYS_PRINT("BASEPRI   : 0x%08X\n",
                  fatal_log.m3_core_reg.basepri);
        SYS_PRINT("CONTROL   : 0x%08X\n",
                  fatal_log.m3_core_reg.control);

        SYS_PRINT("\n================================================\n");
        SYS_PRINT(" SYSTEM HALTED\n");
        SYS_PRINT("================================================\n");

        while (1) {
                led_life_on();
                sys_ctrl_delay_us(200000);

                led_life_off();
                sys_ctrl_delay_us(200000);
        }
}

void sys_dbg_func_stack_dump(uint32_t* args) {
	/**
	Stack frame contains:
	r0, r1, r2, r3, r12, r14, the return address and xPSR
	- Stacked R0	<-> args[0]
	- Stacked R1	<-> args[1]
	- Stacked R2	<-> args[2]
	- Stacked R3	<-> args[3]
	- Stacked R12	<-> args[4]
	- Stacked LR	<-> args[5]
	- Stacked PC	<-> args[6]
	- Stacked xPSR	<-> args[7]
	*/
	(void)args;
	SYS_PRINT("[st]R0\t0x%08X\n",		args[0]);
	SYS_PRINT("[st]R1\t0x%08X\n",		args[1]);
	SYS_PRINT("[st]R2\t0x%08X\n",		args[2]);
	SYS_PRINT("[st]R3\t0x%08X\n",		args[3]);
	SYS_PRINT("[st]R12\t0x%08X\n",	args[4]);
	SYS_PRINT("[st]LR\t0x%08X\n",		args[5]);
	SYS_PRINT("[st]PC\t0x%08X\n",		args[6]);
	SYS_PRINT("[st]PSR\t0x%08X\n",	args[7]);
}

void sys_dbg_cpu_dump() {
	SYS_PRINT("[cr]IPSR\t%d\n", __get_IPSR());
	SYS_PRINT("[cr]PRIMASK\t0x%08X\n", __get_PRIMASK());
	SYS_PRINT("[cr]FAULTMASK\t0x%08X\n", __get_FAULTMASK());
	SYS_PRINT("[cr]BASEPRI\t0x%08X\n", __get_BASEPRI());
	SYS_PRINT("[cr]CONTROL\t0x%08X\n", __get_CONTROL());
}

void sys_dbg_stack_space_dump() {
	extern uint32_t _estack;
	uint32_t* start_addr = (uint32_t*)((uint32_t)&_estack) - sizeof(uint32_t);
	uint32_t* end_addr = (uint32_t*)((uint32_t)__get_MSP());
	SYS_PRINT("--- sys_dbg_stack_space_dump ---\n");
	for (uint32_t* i = start_addr; i > end_addr ; i--) {
		SYS_PRINT("[%08X] %08X\n", i, *i);
	}
}
