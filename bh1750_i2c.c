#include "pico/stdlib.h"
#include "bh1750_i2c.h"

// BH1705 I2C address
#define BH1705_I2C_ADDR 0x23

// BH1705 command codes
#define BH1705_POWER_ON  0x01
#define BH1705_RESET     0x07
#define BH1705_CONT_HRES_MODE 0x10 // Continuous H-Resolution Mode

// Initialize
void bh1705_init(i2c_inst_t *i2c) {
    uint8_t buffer[1];
    buffer[0] = BH1705_POWER_ON;
    i2c_write_blocking(i2c, BH1705_I2C_ADDR, buffer, 1, false);

    buffer[0] = BH1705_RESET;
    i2c_write_blocking(i2c, BH1705_I2C_ADDR, buffer, 1, false);
}

// Get luminance
float bh1705_read_luminance(i2c_inst_t *i2c) {
    uint8_t buffer[2];
    uint16_t raw_lux;

    // Start measurement command
    buffer[0] = BH1705_CONT_HRES_MODE;
    i2c_write_blocking(i2c, BH1705_I2C_ADDR, buffer, 1, false);

    // data 2bytes read
    i2c_read_blocking(i2c, BH1705_I2C_ADDR, buffer, 2, false);

    // change to 16bit value
    raw_lux = (buffer[0] << 8) | buffer[1];

    // change lux (1 lx = 1 raw value for BH1705 in this mode)
    return raw_lux / 1.2; // Continuous H-Resolution Mode divide by 1.2
}

