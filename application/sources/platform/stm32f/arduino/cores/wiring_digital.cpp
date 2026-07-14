// #include "Arduino.h"
// #include "io_cfg.h"
// #include "sys_dbg.h"

// void pinMode(uint8_t pin, uint8_t mode) {
// 	switch (pin) {
// 	case OLED_CLK_PIN:
// 		if (mode == INPUT) {
// 			oled_clk_input_mode();
// 		}
// 		else if (mode == OUTPUT) {
// 			oled_clk_output_mode();
// 		}
// 		else if (mode == INPUT_PULLUP) {
// 			oled_clk_input_mode();
// 		}
// 		else {
// 			FATAL("AR", 0x01);
// 		}
// 		break;

// 	case OLED_DATA_PIN:
// 		if (mode == INPUT) {
// 			oled_data_input_mode();
// 		}
// 		else if (mode == OUTPUT) {
// 			oled_data_output_mode();
// 		}
// 		else if (mode == INPUT_PULLUP) {
// 			oled_data_input_mode();
// 		}
// 		else {
// 			FATAL("AR", 0x01);
// 		}
// 		break;

// 	case OLED_RES_PIN:
// 		if (mode == INPUT) {
// 			oled_res_input_mode();
// 		}
// 		else if (mode == OUTPUT) {
// 			oled_res_output_mode();
// 		}
// 		else if (mode == INPUT_PULLUP) {
// 			oled_res_input_mode();
// 		}
// 		else {
// 			FATAL("AR", 0x01);
// 		}
// 		break;

// 	default:
// 		FATAL("AR", 0xF1);
// 		break;
// 	}
// }

// void digitalWrite(uint8_t pin, uint8_t val) {
// 	switch (pin) {
// 	case OLED_CLK_PIN:
// 		if (val == HIGH) {
// 			oled_clk_digital_write_high();
// 		}
// 		else if (val == LOW) {
// 			oled_clk_digital_write_low();
// 		}
// 		else {
// 			FATAL("AR", 0x02);
// 		}
// 		break;

// 	case OLED_DATA_PIN:
// 		if (val == HIGH) {
// 			oled_data_digital_write_high();
// 		}
// 		else if (val == LOW) {
// 			oled_data_digital_write_low();
// 		}
// 		else {
// 			FATAL("AR", 0x02);
// 		}
// 		break;

// 	case OLED_RES_PIN:
// 		if (val == HIGH) {
// 			oled_res_digital_write_high();
// 		}
// 		else if (val == LOW) {
// 			oled_res_digital_write_low();
// 		}
// 		else {
// 			FATAL("AR", 0x02);
// 		}
// 		break;

// 	default:
// 		FATAL("AR", 0xF2);
// 		break;
// 	}
// }

// int digitalRead(uint8_t pin) {
// 	int val = 0;
// 	switch (pin) {
// 	case OLED_CLK_PIN: {
// 		val = oled_clk_digital_read();
// 	}
// 		break;

// 	case OLED_DATA_PIN: {
// 		val = oled_data_digital_read();
// 	}
// 		break;

// 	case OLED_RES_PIN: {
// 		val = oled_res_digital_read();
// 	}
// 		break;
// 	default:
// 		FATAL("AR", 0xF3);
// 		break;
// 	}
// 	return val;
// }
#include "Arduino.h"
#include "io_cfg.h"
#include "sys_dbg.h"

/* * Định nghĩa lại Virtual Pin cho Arduino Layer 
 * Đảm bảo các định nghĩa này trùng với file io_cfg.h của bạn
 */
#define OLED_SCL_PIN                 (0x03)
#define OLED_SDA_PIN                 (0x04)

void pinMode(uint8_t pin, uint8_t mode) {
    GPIO_InitTypeDef GPIO_InitStructure;
    GPIO_TypeDef* gpio_port = NULL;
    uint16_t          gpio_pin  = 0;
    uint32_t          gpio_clk  = 0;

    // Bước 1: Trích xuất phần cứng dựa trên Virtual Pin (Rất gọn, tiết kiệm Flash)
    switch (pin) {
    case OLED_SCL_PIN:
        gpio_port = OLED_SCL_IO_PORT;
        gpio_pin  = OLED_SCL_IO_PIN;
        gpio_clk  = OLED_SCL_IO_CLOCK;
        break;

    case OLED_SDA_PIN:
        gpio_port = OLED_SDA_IO_PORT;
        gpio_pin  = OLED_SDA_IO_PIN;
        gpio_clk  = OLED_SDA_IO_CLOCK;
        break;

    default:
        FATAL("AR", 0xF1);
        return;
    }

    // Bước 2: Cấp clock cho Port tương ứng (Lưu ý: F103 dùng APB2 cho GPIO)
    RCC_APB2PeriphClockCmd(gpio_clk, ENABLE);

    GPIO_InitStructure.GPIO_Pin   = gpio_pin;
    GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

    // Bước 3: Cấu hình Mode cho I2C phần mềm
    if (mode == INPUT) {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING; // Chân nghe tín hiệu tự do
    }
    else if (mode == OUTPUT) {
        /* * BẮT BUỘC DÙNG GPIO_Mode_Out_OD (Open-Drain) cho I2C.
         * Nó giúp bảo vệ phần cứng khi cả Master và Slave cùng kéo bus,
         * đồng thời cho phép đọc ngược trạng thái chân mà không cần đổi pinMode liên tục.
         */
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_OD; 
    }
    else if (mode == INPUT_PULLUP) {
        GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; // Bật trở kéo lên nội bộ nếu mạch thiếu trở treo
    }
    else {
        FATAL("AR", 0x01);
        return;
    }

    // Thực thi cấu hình (Chỉ gọi đúng 1 lần duy nhất ở cuối hàm)
    GPIO_Init(gpio_port, &GPIO_InitStructure);
}

void digitalWrite(uint8_t pin, uint8_t val) {
    GPIO_TypeDef* gpio_port = NULL;
    uint16_t      gpio_pin  = 0;

    switch (pin) {
    case OLED_SCL_PIN:
        gpio_port = OLED_SCL_IO_PORT;
        gpio_pin  = OLED_SCL_IO_PIN;
        break;

    case OLED_SDA_PIN:
        gpio_port = OLED_SDA_IO_PORT;
        gpio_pin  = OLED_SDA_IO_PIN;
        break;

    default:
        FATAL("AR", 0xF2);
        return;
    }

    if (val == HIGH) {
        GPIO_SetBits(gpio_port, gpio_pin);
    }
    else if (val == LOW) {
        GPIO_ResetBits(gpio_port, gpio_pin);
    }
    else {
        FATAL("AR", 0x02);
    }
}

int digitalRead(uint8_t pin) {
    GPIO_TypeDef* gpio_port = NULL;
    uint16_t      gpio_pin  = 0;

    switch (pin) {
    case OLED_SCL_PIN:
        gpio_port = OLED_SCL_IO_PORT;
        gpio_pin  = OLED_SCL_IO_PIN;
        break;

    case OLED_SDA_PIN:
        gpio_port = OLED_SDA_IO_PORT;
        gpio_pin  = OLED_SDA_IO_PIN;
        break;

    default:
        FATAL("AR", 0xF3);
        return 0;
    }

    return (int)GPIO_ReadInputDataBit(gpio_port, gpio_pin);
}


