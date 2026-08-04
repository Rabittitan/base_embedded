#include "Arduino.h"
#include "io_cfg.h"
#include "sys_dbg.h"


#define I2C_SCL_PIN                 (0x03)
#define I2C_SDA_PIN                 (0x04)

void pinMode(uint8_t pin, uint8_t mode) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_TypeDef* gpio_port = NULL;
    uint16_t          gpio_pin  = 0;
    uint32_t          gpio_clk  = 0;

    switch (pin) {
        case I2C_SCL_PIN:
            gpio_port = I2C_SCL_IO_PORT;
            gpio_pin  = I2C_SCL_IO_PIN;
            gpio_clk  = I2C_SCL_IO_CLOCK;
            break;

        case I2C_SDA_PIN:
            gpio_port = I2C_SDA_IO_PORT;
            gpio_pin  = I2C_SDA_IO_PIN;
            gpio_clk  = I2C_SDA_IO_CLOCK;
            break;

        default:
            FATAL("AR", 0xF1);
            return;
    }

    
    RCC_APB2PeriphClockCmd(gpio_clk, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = gpio_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

   
    if (mode == INPUT) {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
    } else if (mode == OUTPUT) {
       
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
    }else if (mode == INPUT_PULLUP) {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; 
    }
    else {
        FATAL("AR", 0x01);
        return;
    }

   
    GPIO_Init(gpio_port, &GPIO_InitStructure);
}

void digitalWrite(uint8_t pin, uint8_t val) {
    GPIO_TypeDef* gpio_port = NULL;
    uint16_t      gpio_pin  = 0;

    switch (pin) {
        case I2C_SCL_PIN:
            gpio_port = I2C_SCL_IO_PORT;
            gpio_pin  = I2C_SCL_IO_PIN;
            break;

        case I2C_SDA_PIN:
            gpio_port = I2C_SDA_IO_PORT;
            gpio_pin  = I2C_SDA_IO_PIN;
            break;

        default:
            FATAL("AR", 0xF2);
            return;
    }

    if (val == HIGH) {
        GPIO_SetBits(gpio_port, gpio_pin);
    } else if (val == LOW) {
        GPIO_ResetBits(gpio_port, gpio_pin);
    } else {
        FATAL("AR", 0x02);
    }
}

int digitalRead(uint8_t pin) {
    GPIO_TypeDef* gpio_port = NULL;
    uint16_t      gpio_pin  = 0;

    switch (pin) {
        case I2C_SCL_PIN:
            gpio_port = I2C_SCL_IO_PORT;
            gpio_pin  = I2C_SCL_IO_PIN;
            break;

        case I2C_SDA_PIN:
            gpio_port = I2C_SDA_IO_PORT;
            gpio_pin  = I2C_SDA_IO_PIN;
            break;

        default:
            FATAL("AR", 0xF3);
            return 0;
    }

    return (int)GPIO_ReadInputDataBit(gpio_port, gpio_pin);
}


