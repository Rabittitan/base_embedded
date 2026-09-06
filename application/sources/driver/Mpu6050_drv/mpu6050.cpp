/**
 ******************************************************************************
 * @file    mpu6050.cpp
 * @author  Your Name
 * @date    Current Date
 ******************************************************************************
**/

#include "mpu6050.h"
#include "mpu6050_reg.h"
#include "i2c.h"
#include "app_dbg.h"
#include "timer.h"
#include <math.h>

// Kalman filter variables
static float kalman_angle_roll = 0.0f;
static float kalman_angle_pitch = 0.0f;
static float kalman_bias_roll = 0.0f;
static float kalman_bias_pitch = 0.0f;
static float kalman_P[2][2] = {{0.0f, 0.0f}, {0.0f, 0.0f}};
static float kalman_P_pitch[2][2] = {{0.0f, 0.0f}, {0.0f, 0.0f}};

// Kalman filter parameters - Cân bằng giữa ổn định và phản hồi
static float Q_angle = 0.001f;
static float Q_bias = 0.003f;
static float R_measure = 0.03f;

// Complementary filter variables
static float comp_angle_roll = 0.0f;
static float comp_angle_pitch = 0.0f;
static float alpha = 0.95f;  // 95% gyro, 5% accel - phản hồi nhanh hơn

// Moving average filter - Nhỏ hơn để phản hồi nhanh
#define MOVING_AVG_SIZE 5  // Chỉ 5 mẫu
static float roll_buffer[MOVING_AVG_SIZE] = {0.0f};
static float pitch_buffer[MOVING_AVG_SIZE] = {0.0f};

// Spike filter variables
static float last_gyro_x = 0.0f;
static float last_gyro_y = 0.0f;
static float last_gyro_z = 0.0f;
#define GYRO_SPIKE_THRESHOLD 30.0f  // Ngưỡng phát hiện spike

// Gyro low-pass filter - Nhẹ để phản hồi nhanh
static float filtered_gyro_x = 0.0f;
static float filtered_gyro_y = 0.0f;
static float filtered_gyro_z = 0.0f;
static float gyro_lp_alpha = 0.5f;  // Lọc nhẹ

// Deadzone cho game - Vùng chết ở giữa
#define DEADZONE_ANGLE 5.0f  // ±5 độ là vùng chết
#define MAX_ANGLE 45.0f      // Góc tối đa để điều khiển

static bool kalman_initialized = false;

// Calibration offsets
static float calib_roll_offset = 0.0f;
static float calib_pitch_offset = 0.0f;

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

static bool mpu6050_read_buffer(uint8_t reg, uint8_t *buffer, uint8_t length)
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

// Spike filter - Loại bỏ giá trị đột biến
static float spike_filter(float new_value, float *last_value, float threshold)
{
    float diff = fabsf(new_value - *last_value);
    
    if (diff > threshold) {
        return *last_value;
    }
    
    *last_value = new_value;
    return new_value;
}

// Low-pass filter cho gyro
static float low_pass_filter(float input, float *output, float alpha_lp)
{
    *output = alpha_lp * input + (1.0f - alpha_lp) * (*output);
    return *output;
}

// Kalman filter function
static float kalman_filter(float accel_angle, float gyro_rate, float dt, 
                          float *kalman_angle, float *kalman_bias, float P[2][2])
{
    float rate;
    float S, K[2], y;
    
    // Predict
    rate = gyro_rate - *kalman_bias;
    *kalman_angle += dt * rate;
    
    // Update error covariance matrix
    P[0][0] += dt * (dt * P[1][1] - P[0][1] - P[1][0] + Q_angle);
    P[0][1] -= dt * P[1][1];
    P[1][0] -= dt * P[1][1];
    P[1][1] += Q_bias * dt;
    
    // Update
    S = P[0][0] + R_measure;
    if (S > 0.0001f) {
        K[0] = P[0][0] / S;
        K[1] = P[1][0] / S;
    } else {
        K[0] = 0.0f;
        K[1] = 0.0f;
    }
    
    y = accel_angle - *kalman_angle;
    *kalman_angle += K[0] * y;
    *kalman_bias += K[1] * y;
    
    // Update covariance matrix
    P[0][0] -= K[0] * P[0][0];
    P[0][1] -= K[0] * P[0][1];
    P[1][0] -= K[1] * P[0][0];
    P[1][1] -= K[1] * P[0][1];
    
    return *kalman_angle;
}

// Complementary filter function
static float complementary_filter(float accel_angle, float gyro_rate, float dt, float *comp_angle)
{
    *comp_angle = alpha * (*comp_angle + gyro_rate * dt) + (1.0f - alpha) * accel_angle;
    return *comp_angle;
}

// Moving average filter
static float moving_average_filter(float new_value, float *buffer, uint8_t size)
{
    float sum = 0.0f;
    uint8_t i;
    
    // Shift buffer
    for (i = size - 1; i > 0; i--)
    {
        buffer[i] = buffer[i - 1];
    }
    buffer[0] = new_value;
    
    // Calculate average
    for (i = 0; i < size; i++)
    {
        sum += buffer[i];
    }
    
    return sum / size;
}

// Deadzone filter - Chuyển đổi góc thành tín hiệu điều khiển game
static float game_deadzone(float angle, float deadzone, float max_angle)
{
    float abs_angle = fabsf(angle);
    
    // Nếu trong vùng chết, trả về 0 (đứng yên)
    if (abs_angle < deadzone) {
        return 0.0f;
    }
    
    // Nếu vượt quá góc tối đa, giới hạn lại
    if (abs_angle > max_angle) {
        return (angle > 0) ? 1.0f : -1.0f;
    }
    
    // Map góc từ deadzone đến max_angle thành -1.0 đến 1.0
    float normalized = (abs_angle - deadzone) / (max_angle - deadzone);
    
    return (angle > 0) ? normalized : -normalized;
}

bool mpu6050_init(void)
{
    uint8_t who_am_i;

    /* Check device ID */
    if (!mpu6050_read_register(MPU6050_WHO_AM_I, &who_am_i))
    {
        return false;
    }

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

    // Initialize filters
    kalman_angle_roll = 0.0f;
    kalman_angle_pitch = 0.0f;
    kalman_bias_roll = 0.0f;
    kalman_bias_pitch = 0.0f;
    
    kalman_P[0][0] = 0.0f;
    kalman_P[0][1] = 0.0f;
    kalman_P[1][0] = 0.0f;
    kalman_P[1][1] = 0.0f;
    
    kalman_P_pitch[0][0] = 0.0f;
    kalman_P_pitch[0][1] = 0.0f;
    kalman_P_pitch[1][0] = 0.0f;
    kalman_P_pitch[1][1] = 0.0f;
    
    comp_angle_roll = 0.0f;
    comp_angle_pitch = 0.0f;
    
    // Initialize gyro filter variables
    last_gyro_x = 0.0f;
    last_gyro_y = 0.0f;
    last_gyro_z = 0.0f;
    filtered_gyro_x = 0.0f;
    filtered_gyro_y = 0.0f;
    filtered_gyro_z = 0.0f;
    
    // Clear moving average buffers
    for (uint8_t i = 0; i < MOVING_AVG_SIZE; i++)
    {
        roll_buffer[i] = 0.0f;
        pitch_buffer[i] = 0.0f;
    }
    
    kalman_initialized = false;

    return true;
}

// Calibration function - Gọi khi sensor ở vị trí cân bằng
void mpu6050_calibrate(void)
{
    // Đọc 100 mẫu và tính trung bình
    float sum_roll = 0.0f;
    float sum_pitch = 0.0f;
    mpu6050_data_t temp_data;
    
    for (int i = 0; i < 100; i++) {
        if (mpu6050_get_data(&temp_data)) {
            sum_roll += temp_data.roll;
            sum_pitch += temp_data.pitch;
        }
        // Delay 10ms
        for (volatile int j = 0; j < 10000; j++);
    }
    
    calib_roll_offset = sum_roll / 100.0f;
    calib_pitch_offset = sum_pitch / 100.0f;
    
    APP_DBG("Calib: roll_offset=%.2f, pitch_offset=%.2f\r\n", 
            calib_roll_offset, calib_pitch_offset);
}

bool mpu6050_get_data(mpu6050_data_t *data)
{
    uint8_t buffer[14];
    float accel_roll, accel_pitch;
    float kalman_roll, kalman_pitch;
    float comp_roll, comp_pitch;
    float final_roll, final_pitch;
    float dt = 0.01f;  // 10ms
    float gyro_x_filtered, gyro_y_filtered, gyro_z_filtered;

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

    // Lọc spike cho gyro
    gyro_x_filtered = spike_filter(data->gyro_x, &last_gyro_x, GYRO_SPIKE_THRESHOLD);
    gyro_y_filtered = spike_filter(data->gyro_y, &last_gyro_y, GYRO_SPIKE_THRESHOLD);
    gyro_z_filtered = spike_filter(data->gyro_z, &last_gyro_z, GYRO_SPIKE_THRESHOLD);
    
    // Lọc low-pass nhẹ cho gyro
    gyro_x_filtered = low_pass_filter(gyro_x_filtered, &filtered_gyro_x, gyro_lp_alpha);
    gyro_y_filtered = low_pass_filter(gyro_y_filtered, &filtered_gyro_y, gyro_lp_alpha);
    gyro_z_filtered = low_pass_filter(gyro_z_filtered, &filtered_gyro_z, gyro_lp_alpha);
    
    data->gyro_x = gyro_x_filtered;
    data->gyro_y = gyro_y_filtered;
    data->gyro_z = gyro_z_filtered;

    // Calculate angles from accelerometer
    accel_roll = atan2f(data->acc_y, data->acc_z) * 180.0f / PI;
    accel_pitch = atan2f(-data->acc_x, sqrtf(data->acc_y * data->acc_y + data->acc_z * data->acc_z)) * 180.0f / PI;
    
    if (!kalman_initialized)
    {
        // Initialize filters with first measurement
        kalman_angle_roll = accel_roll;
        kalman_angle_pitch = accel_pitch;
        comp_angle_roll = accel_roll;
        comp_angle_pitch = accel_pitch;
        kalman_bias_roll = 0.0f;
        kalman_bias_pitch = 0.0f;
        kalman_initialized = true;
        
        data->roll = accel_roll - calib_roll_offset;
        data->pitch = accel_pitch - calib_pitch_offset;
    }
    else
    {
        // Apply Kalman filter
        kalman_roll = kalman_filter(accel_roll, gyro_y_filtered, dt, 
                                   &kalman_angle_roll, &kalman_bias_roll, kalman_P);
        kalman_pitch = kalman_filter(accel_pitch, gyro_x_filtered, dt, 
                                    &kalman_angle_pitch, &kalman_bias_pitch, kalman_P_pitch);
        
        // Apply Complementary filter
        comp_roll = complementary_filter(accel_roll, gyro_y_filtered, dt, &comp_angle_roll);
        comp_pitch = complementary_filter(accel_pitch, gyro_x_filtered, dt, &comp_angle_pitch);
        
        // Combine Kalman and Complementary
        final_roll = (kalman_roll + comp_roll) / 2.0f;
        final_pitch = (kalman_pitch + comp_pitch) / 2.0f;
        
        // Apply moving average filter (nhỏ)
        final_roll = moving_average_filter(final_roll, roll_buffer, MOVING_AVG_SIZE);
        final_pitch = moving_average_filter(final_pitch, pitch_buffer, MOVING_AVG_SIZE);
        
        // Trừ offset calibration
        final_roll -= calib_roll_offset;
        final_pitch -= calib_pitch_offset;
        
        data->roll = final_roll;
        data->pitch = final_pitch;
    }

    return true;
}

// Hàm lấy tín hiệu điều khiển game đã qua deadzone
// Trả về: -1.0 đến 1.0 cho điều khiển trái/phải
float mpu6050_get_game_control_x(void)
{
    mpu6050_data_t data;
    if (mpu6050_get_data(&data)) {
        return game_deadzone(data.roll, DEADZONE_ANGLE, MAX_ANGLE);
    }
    return 0.0f;
}

// Hàm lấy tín hiệu điều khiển game đã qua deadzone
// Trả về: -1.0 đến 1.0 cho điều khiển lên/xuống
float mpu6050_get_game_control_y(void)
{
    mpu6050_data_t data;
    if (mpu6050_get_data(&data)) {
        return game_deadzone(data.pitch, DEADZONE_ANGLE, MAX_ANGLE);
    }
    return 0.0f;
}