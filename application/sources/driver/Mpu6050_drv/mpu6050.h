#ifndef __MPU6050_H__
#define __MPU6050_H__

#include <stdint.h>


typedef struct
{
    int16_t raw_acc_x;
    int16_t raw_acc_y;
    int16_t raw_acc_z;

    int16_t raw_gyro_x;
    int16_t raw_gyro_y;
    int16_t raw_gyro_z;

    int16_t raw_temp;

    float acc_x;
    float acc_y;
    float acc_z;

    float gyro_x;
    float gyro_y;
    float gyro_z;

    float roll;
    float pitch;

} mpu6050_data_t;

bool mpu6050_init(void);

bool mpu6050_get_data(mpu6050_data_t* data);
void mpu6050_timestamp_update(void);
float mpu6050_get_game_control_x(void);
float mpu6050_get_game_control_y(void);
#endif