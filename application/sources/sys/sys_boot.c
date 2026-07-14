#include <string.h>

#include "sys_boot.h"
#include "sys_io.h"
#include "sys_dbg.h"

static sys_boot_t sys_boot_obj;

void sys_boot_init() {
    // Khi chạy standalone (không có bootloader), không đọc từ BSF.
    // Khởi tạo sys_boot_obj với giá trị mặc định.
    memset(&sys_boot_obj, 0, sizeof(sys_boot_t));
}

void sys_boot_get(sys_boot_t* obj) {
    memcpy((uint8_t*)obj, (uint8_t*)&sys_boot_obj, sizeof(sys_boot_t));
}

uint8_t sys_boot_set(sys_boot_t* sys_boot) {
    // Khi chạy standalone, chỉ cập nhật RAM object, không ghi Flash.
    memcpy((uint8_t*)&sys_boot_obj, (uint8_t*)sys_boot, sizeof(sys_boot_t));
    return SYS_BOOT_OK;
}