#ifndef BH1750_I2C_H
#define BH1750_I2C_H

#include "hardware/i2c.h"

// Function prototypes
void bh1705_init(i2c_inst_t *i2c);
float bh1705_read_luminance(i2c_inst_t *i2c);


#endif // BH1750_I2C_H