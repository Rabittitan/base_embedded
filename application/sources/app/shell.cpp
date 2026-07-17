/**
 ******************************************************************************
 * @author: GaoKong
 * @date:   13/08/2016
 ******************************************************************************
**/

#include <stdint.h>
#include <stdlib.h>
#include <malloc.h>
#include <string.h>

#include "ak.h"
#include "task.h"
#include "timer.h"
#include "message.h"

#include "cmd_line.h"
#include "xprintf.h"

#include "sys_ctrl.h"
#include "sys_io.h"
#include "sys_dbg.h"
#include "sys_irq.h"

#include "app.h"

#include "app_dbg.h"
#include "app_data.h"

#include "task_shell.h"
#include "task_list.h"

#include "led.h"

/*****************************************************************************/
/*  local declare
 */
/*****************************************************************************/
#define STR_LIST_MAX_SIZE		8
#define STR_BUFFER_SIZE			128

static char cmd_buffer[STR_BUFFER_SIZE];
static char* str_list[STR_LIST_MAX_SIZE];
static uint8_t str_list_len;

static uint8_t str_parser(char* str);
static char* str_parser_get_attr(uint8_t);

/*****************************************************************************/
/*  command function declare
 */
/*****************************************************************************/
int32_t shell_reset(uint8_t* argv);
int32_t shell_ver(uint8_t* argv);
int32_t shell_help(uint8_t* argv);

int32_t shell_ram(uint8_t* argv);

/*****************************************************************************/
/*  command table
 */
/*****************************************************************************/
const cmd_line_t lgn_cmd_table[] = {

	/*************************************************************************/
	/* system command */
	/*************************************************************************/
	{(const int8_t*)"reset",	shell_reset,		(const int8_t*)"reset terminal"},
	{(const int8_t*)"ver",		shell_ver,			(const int8_t*)"version info"},
	{(const int8_t*)"help",		shell_help,			(const int8_t*)"help info"},
	{(const int8_t*)"ram",		shell_ram,			(const int8_t*)"ram"},

	/*************************************************************************/
	/* debug command */
	/*************************************************************************/

	/* End Of Table */
	{(const int8_t*)0,(pf_cmd_func)0,(const int8_t*)0}
};

uint8_t str_parser(char* str) {
	strcpy(cmd_buffer, str);
	str_list_len = 0;

	uint8_t i = 0;
	uint8_t str_list_index = 0;
	uint8_t flag_insert_str = 1;

	while (cmd_buffer[i] != 0 && cmd_buffer[i] != '\n' && cmd_buffer[i] != '\r') {
		if (cmd_buffer[i] == ' ') {
			cmd_buffer[i] = 0;
			flag_insert_str = 1;
		}
		else if (flag_insert_str) {
			str_list[str_list_index++] = &cmd_buffer[i];
			flag_insert_str = 0;
		}
		i++;
	}

	cmd_buffer[i] = 0;

	str_list_len = str_list_index;
	return str_list_len;
}

char* str_parser_get_attr(uint8_t index) {
	if (index < str_list_len) {
		return str_list[index];
	}
	return NULL;
}

/*****************************************************************************/
/*  command function definaion
 */
/*****************************************************************************/
int32_t shell_reset(uint8_t* argv) {
	(void)argv;
	xprintf("\033[2J\r");
	return 0;
}

int32_t shell_ver(uint8_t* argv) {
	(void)argv;

	firmware_header_t firmware_header;
	sys_ctrl_get_firmware_info(&firmware_header);

	LOGIN_PRINT("Kernel version: %s\n", AK_VERSION);
	LOGIN_PRINT("App version: %d.%d.%d.%d\n", app_info.version[0] \
			, app_info.version[1] \
			, app_info.version[2] \
			, app_info.version[3]);
	LOGIN_PRINT("Firmware checksum: %04x\n", firmware_header.checksum);
	LOGIN_PRINT("Firmware length: %d\n", firmware_header.bin_len);

	LOGIN_PRINT("\nSystem information:\n");
	LOGIN_PRINT("\tFLASH used:\t%d bytes\n", system_info.flash_used);
	LOGIN_PRINT("\tSRAM used:\t%d bytes\n", system_info.ram_used);
	LOGIN_PRINT("\t\tdata init size:\t\t%d bytes\n", system_info.data_init_size);
	LOGIN_PRINT("\t\tdata non_init size:\t%d bytes\n", system_info.data_non_init_size);
	LOGIN_PRINT("\t\tstack avail:\t\t%d bytes\n", system_info.stack_avail);
	LOGIN_PRINT("\t\theap avail:\t\t%d bytes\n", system_info.heap_avail);
	LOGIN_PRINT("\t\tother:\t\t\t%d bytes\n", system_info.ram_other);
	LOGIN_PRINT("\n");
	LOGIN_PRINT("\tcpu clock:\t%d Hz\n", system_info.cpu_clock);
	LOGIN_PRINT("\ttime tick:\t%d ms\n", system_info.tick);
	LOGIN_PRINT("\tconsole:\t%d bps\n", system_info.console_baudrate);
	LOGIN_PRINT("\n");
	LOGIN_PRINT("\tVCC:\t%d mV\n", sys_ctr_get_vbat_voltage());
	LOGIN_PRINT("\tTEMP:\t%d *C\n", sys_ctr_get_mcu_temperature());
	LOGIN_PRINT("\n\n");
	return 0;
}

int32_t shell_help(uint8_t* argv) {
	uint32_t idx = 0;
	switch (*(argv + 4)) {
	default:
		LOGIN_PRINT("\nCOMMANDS INFORMATION:\n\n");
		while(lgn_cmd_table[idx].cmd != (const int8_t*)0) {
			LOGIN_PRINT("%s\t-> %s\n\n", lgn_cmd_table[idx].cmd, lgn_cmd_table[idx].info);
			idx++;
		}
		break;
	}
	return 0;
}










/* https://www.charbase.com */

int32_t shell_ram(uint8_t* argv) {
	extern uint32_t _start_ram;
	extern uint32_t _estack;

	char* str_start_addr = NULL;
	char* str_stop_addr = NULL;
	uint8_t* start_addr = NULL;
	uint8_t* stop_addr = NULL;

	uint8_t len = str_parser((char*)argv);

	LOGIN_PRINT("RAM start: 0x%x\n", ((uint32_t)&_start_ram));
	LOGIN_PRINT("RAM   end: 0x%x\n", ((uint32_t)&_estack));

	/* "ram x 0x1000 0xA000" */
	switch (*(argv + 4)) {
	case 'r': {
		if (len == 4) {
			str_start_addr	= str_parser_get_attr(2);
			str_stop_addr	= str_parser_get_attr(3);
			start_addr	= (uint8_t*)((uint32_t)strtol(str_start_addr, NULL, 0));
			stop_addr 	= (uint8_t*)((uint32_t)strtol(str_stop_addr, NULL, 0));

			LOGIN_PRINT("start_addr: 0x%x\n", start_addr);
			LOGIN_PRINT("stop_addr: 0x%x\n", stop_addr);

			if ((uint32_t)start_addr >= ((uint32_t)&_start_ram) && (uint32_t)stop_addr <= ((uint32_t)&_estack)) {

				/* start dump ram */
				LOGIN_PRINT("\n");

				for (uint8_t* addr_index = start_addr; addr_index < stop_addr; addr_index++) {
					if (!((uint32_t)addr_index % 4)) {
						/* reset watchdog */
						sys_ctrl_independent_watchdog_reset();
						sys_ctrl_soft_watchdog_reset();

						LOGIN_PRINT("\n0x%x\t", addr_index);
					}

					*addr_index = 0;
					LOGIN_PRINT("%d\t", *addr_index);
				}

				LOGIN_PRINT("\n");
				/* end dump ram */
			}
		}
		else {
			LOGIN_PRINT("syntax error\n");
		}
	}
		break;

	case 'd': { /* ram d 0x20000000 0x20004000 */
		if (len == 4) {
			str_start_addr	= str_parser_get_attr(2);
			str_stop_addr	= str_parser_get_attr(3);
			start_addr	= (uint8_t*)((uint32_t)strtol(str_start_addr, NULL, 0));
			stop_addr 	= (uint8_t*)((uint32_t)strtol(str_stop_addr, NULL, 0));

			LOGIN_PRINT("start_addr: 0x%x\n", start_addr);
			LOGIN_PRINT("stop_addr: 0x%x\n", stop_addr);

			if ((uint32_t)start_addr >= ((uint32_t)&_start_ram) && (uint32_t)stop_addr <= ((uint32_t)&_estack)) {

				/* start dump ram */
				LOGIN_PRINT("\n");

				for (uint8_t* addr_index = start_addr; addr_index < stop_addr; addr_index++) {
					if (!((uint32_t)addr_index % 4)) {
						/* reset watchdog */
						sys_ctrl_independent_watchdog_reset();
						sys_ctrl_soft_watchdog_reset();

						LOGIN_PRINT("\n0x%x\t", addr_index);
					}

					LOGIN_PRINT("%d\t", *addr_index);
				}

				LOGIN_PRINT("\n");
				/* end dump ram */
			}
		}
		else {
			LOGIN_PRINT("syntax error\n");
		}
	}
		break;

	case 'h': {
		if (len == 4) {
			str_start_addr	= str_parser_get_attr(2);
			str_stop_addr	= str_parser_get_attr(3);
			start_addr	= (uint8_t*)((uint32_t)strtol(str_start_addr, NULL, 0));
			stop_addr 	= (uint8_t*)((uint32_t)strtol(str_stop_addr, NULL, 0));

			LOGIN_PRINT("start_addr: 0x%x\n", start_addr);
			LOGIN_PRINT("stop_addr: 0x%x\n", stop_addr);

			if ((uint32_t)start_addr >= ((uint32_t)&_start_ram) && (uint32_t)stop_addr <= ((uint32_t)&_estack)) {

				/* start dump ram */
				LOGIN_PRINT("\n");

				for (uint8_t* addr_index = start_addr; addr_index < stop_addr; addr_index++) {
					if (!((uint32_t)addr_index % 4)) {
						/* reset watchdog */
						sys_ctrl_independent_watchdog_reset();
						sys_ctrl_soft_watchdog_reset();

						LOGIN_PRINT("\n0x%x\t", addr_index);
					}

					LOGIN_PRINT("0x%x\t", *addr_index);
				}

				LOGIN_PRINT("\n");
				/* end dump ram */
			}
		}
		else {
			LOGIN_PRINT("syntax error\n");
		}
	}
		break;

	case 's': {
		LOGIN_PRINT("Stack Size: %d\n", sys_stack_get_size());
		LOGIN_PRINT("Stack Usage: %d\n", sys_stack_usage());
		sys_dbg_stack_space_dump();
	}
		break;

	case 'c': {
		sys_dbg_cpu_dump();
	}
		break;

	default: {
		LOGIN_PRINT("unknown option\n");
	}
		break;
	}

	return 0;
}





