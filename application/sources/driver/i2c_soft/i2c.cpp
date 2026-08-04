#include "i2c.h"
#include "app_dbg.h"

// void i2c_init(){
//     pinMode(I2C_SCL_PIN, OUTPUT);
//     pinMode(I2C_SDA_PIN, OUTPUT);

//     digitalWrite(I2C_SCL_PIN, HIGH);
//     digitalWrite(I2C_SDA_PIN, HIGH);
// }
#define I2C_SCL_PIN    0x03
#define I2C_SDA_PIN    0x04


void i2c_start() {
    digitalWrite(I2C_SCL_PIN, HIGH);
    digitalWrite(I2C_SDA_PIN, HIGH);

    digitalWrite(I2C_SDA_PIN, LOW);
    digitalWrite(I2C_SCL_PIN, LOW);
}

void i2c_stop() {
    digitalWrite(I2C_SCL_PIN, LOW);
    digitalWrite(I2C_SDA_PIN, LOW);

    digitalWrite(I2C_SCL_PIN, HIGH);
    digitalWrite(I2C_SDA_PIN, HIGH);
}

bool writeByte(uint8_t data) {
    uint8_t i;
    bool ack;

    /* Send 8 bits (MSB first) */
    for (i = 0; i < 8; i++) {

        if ((data << i) & 0x80) {
            digitalWrite(I2C_SDA_PIN, HIGH);
        }
        else {
            digitalWrite(I2C_SDA_PIN, LOW);
        }

        digitalWrite(I2C_SCL_PIN, HIGH);
        digitalWrite(I2C_SCL_PIN, LOW);
    }

    /* Read ACK */
    pinMode(I2C_SDA_PIN, INPUT);

    digitalWrite(I2C_SCL_PIN, HIGH);

    ack = (digitalRead(I2C_SDA_PIN) == LOW);

    digitalWrite(I2C_SCL_PIN, LOW);

    pinMode(I2C_SDA_PIN, OUTPUT);

 //    APP_DBG("ACK=%d\r\n", digitalRead(I2C_SDA_PIN));
    return ack;
}

uint8_t readByte(bool ack) {
    uint8_t i;
    uint8_t data = 0;

    pinMode(I2C_SDA_PIN, INPUT);

    for (i = 0; i < 8; i++) {

        digitalWrite(I2C_SCL_PIN, HIGH);

        data <<= 1;

        if (digitalRead(I2C_SDA_PIN)) {
            data |= 0x01;
        }

        digitalWrite(I2C_SCL_PIN, LOW);
    }

    pinMode(I2C_SDA_PIN, OUTPUT);

    if (ack) {
        digitalWrite(I2C_SDA_PIN, LOW);     // ACK
    }
    else {
        digitalWrite(I2C_SDA_PIN, HIGH);    // NACK
    }

    digitalWrite(I2C_SCL_PIN, HIGH);
    digitalWrite(I2C_SCL_PIN, LOW);

    digitalWrite(I2C_SDA_PIN, HIGH);

    return data;
}