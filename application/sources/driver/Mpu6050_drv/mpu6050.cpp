#include  "mpu6050.h"
#include  "mpu6050_reg.h"
#include "i2c.h"
#include "app_dbg.h"

static bool mpu6050_write_register(uint8_t reg, uint8_t value)
{
    i2c_start();

    if (!writeByte(MPU6050_ADDR << 1))
    {
        APP_DBG("FAIL 1\r\n");
        i2c_stop();
        return false;
    }

    if (!writeByte(reg))
    {
        APP_DBG("FAIL 2\r\n");
        i2c_stop();
        return false;
    }

    if (!writeByte(value))
    {
        APP_DBG("FAIL 3\r\n");
        i2c_stop();
        return false;
    }

    i2c_stop();

    return true;
}


static bool mpu6050_read_register(uint8_t reg, uint8_t *value)
{
    i2c_start();

    if (!writeByte(MPU6050_ADDR << 1))
    {
        APP_DBG("FAIL 1\r\n");
        i2c_stop();
        return false;
    }

    if (!writeByte(reg))
    {
        APP_DBG("FAIL 2\r\n");
        i2c_stop();
        return false;
    }

    i2c_start();

    if (!writeByte((MPU6050_ADDR << 1) | 0x01))
    {
        APP_DBG("FAIL 3\r\n");
        i2c_stop();
        return false;
    }

    *value = readByte(false);

    i2c_stop();

    return true;
}


static bool mpu6050_read_buffer(uint8_t reg, uint8_t *buffer,uint8_t length)
{
    uint8_t i;

    i2c_start();

    if (!writeByte(MPU6050_ADDR << 1))
    {
        APP_DBG("FAIL 1\r\n");
        i2c_stop();
        return false;
    }

    if (!writeByte(reg))
    {
        APP_DBG("FAIL 2\r\n");
        i2c_stop();
        return false;
    }

    i2c_start();

    if (!writeByte((MPU6050_ADDR << 1) | 0x01))
    {
        APP_DBG("FAIL 3\r\n");
        i2c_stop();
        return false;
    }

    for (i = 0; i < length; i++)
    {
        if (i == (length - 1))
        {
            buffer[i] = readByte(false);     // NACK byte cuối
        }
        else
        {
            buffer[i] = readByte(true);      // ACK
        }
    }

    i2c_stop();

    return true;
}


bool mpu6050_init(void){
    uint8_t who_am_i;

    /* Check device ID */
    if (!mpu6050_read_register(MPU6050_WHO_AM_I, &who_am_i))
    {
        return false;
    }

    volatile uint8_t debug_id = who_am_i;


    if (who_am_i != MPU6050_ADDR)
    {
        return false;
    }

    /* Wake up device */
    if (!mpu6050_write_register(MPU6050_PWR_MGMT_1, 0x00))
    {
        return false;
    }

    /* Sample Rate = 1KHz */
    if (!mpu6050_write_register(MPU6050_SMPLRT_DIV, 0x07))
    {
        return false;
    }

    /* DLPF = 42Hz */
    if (!mpu6050_write_register(MPU6050_CONFIG, 0x03))
    {
        return false;
    }

    /* Gyroscope ±250°/s */
    if (!mpu6050_write_register(MPU6050_GYRO_CONFIG, 0x00))
    {
        return false;
    }

    /* Accelerometer ±2g */
    if (!mpu6050_write_register(MPU6050_ACCEL_CONFIG, 0x00))
    {
        return false;
    }

    return true;
}

bool mpu6050_get_data(mpu6050_data_t *data)
{
    uint8_t buffer[14];

    if (!mpu6050_read_buffer(MPU6050_ACCEL_XOUT_H, buffer, 14))
    {
        return false;
    }

    data->raw_acc_x = (int16_t)((buffer[0] << 8) | buffer[1]);
    data->raw_acc_y = (int16_t)((buffer[2] << 8) | buffer[3]);
    data->raw_acc_z = (int16_t)((buffer[4] << 8) | buffer[5]);

    data->raw_temp = (int16_t)((buffer[6] << 8) | buffer[7]);

    data->raw_gyro_x = (int16_t)((buffer[8] << 8) | buffer[9]);
    data->raw_gyro_y = (int16_t)((buffer[10] << 8) | buffer[11]);
    data->raw_gyro_z = (int16_t)((buffer[12] << 8) | buffer[13]);

    data->acc_x = (float)data->raw_acc_x / 16384.0f;
    data->acc_y = (float)data->raw_acc_y / 16384.0f;
    data->acc_z = (float)data->raw_acc_z / 16384.0f;

    data->gyro_x = (float)data->raw_gyro_x / 131.0f;
    data->gyro_y = (float)data->raw_gyro_y / 131.0f;
    data->gyro_z = (float)data->raw_gyro_z / 131.0f;

    data->roll = 0.0f;
    data->pitch = 0.0f;


    return true;
}