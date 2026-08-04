#include "mpu6050.h"





#define MPU6050_ADDR              0x68

#define MPU6050_SMPLRT_DIV        0x19
#define MPU6050_CONFIG            0x1A
#define MPU6050_GYRO_CONFIG       0x1B
#define MPU6050_ACCEL_CONFIG      0x1C

#define MPU6050_ACCEL_XOUT_H      0x3B
#define MPU6050_GYRO_XOUT_H       0x43
#define MPU6050_TEMP_OUT_H        0x41

#define MPU6050_PWR_MGMT_1        0x6B
#define MPU6050_WHO_AM_I          0x75




// static bool mpu6050_write_register(uint8_t reg, uint8_t value);

// static bool mpu6050_read_register(uint8_t reg, uint8_t *value);

// static bool mpu6050_read_buffer(uint8_t reg, uint8_t *buffer,uint8_t length);