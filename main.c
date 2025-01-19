#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <ctype.h>
#include "pico/stdlib.h"
#include "pico/binary_info.h"
#include "hardware/i2c.h"
#include "ssd1306_font.h"
#include "ssd1306_i2c.h"
#include "bh1750_i2c.h"

int main() {
    stdio_init_all();

    // GPIO 25 is the on-board LED
    bool led_state = false;
    const uint LED_PIN = 25;    // GPIO 25 is the on-board LED
    gpio_init(LED_PIN);
    gpio_set_dir(LED_PIN, GPIO_OUT);

#if !defined(i2c_default) || !defined(PICO_DEFAULT_I2C_SDA_PIN) || !defined(PICO_DEFAULT_I2C_SCL_PIN)
#warning i2c / SSD1306_i2d example requires a board with I2C pins
    puts("Default I2C pins were not defined");
#else
    // useful information for picotool
    bi_decl(bi_2pins_with_func(PICO_DEFAULT_I2C_SDA_PIN, PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C));
    bi_decl(bi_program_description("SSD1306 OLED driver I2C example for the Raspberry Pi Pico"));

    printf("Hello, SSD1306 OLED display! Look at my raspberries..\n");

    // I2C is "open drain", pull ups to keep signal high when no data is being
    // sent
    i2c_init(i2c_default, SSD1306_I2C_CLK * 1000);
    gpio_set_function(PICO_DEFAULT_I2C_SDA_PIN, GPIO_FUNC_I2C);
    gpio_set_function(PICO_DEFAULT_I2C_SCL_PIN, GPIO_FUNC_I2C);
    gpio_pull_up(PICO_DEFAULT_I2C_SDA_PIN);
    gpio_pull_up(PICO_DEFAULT_I2C_SCL_PIN);

    // run through the complete initialization process
    SSD1306_init();
    printf("Initialize SSD1306 done.\n");


    // Initialize render area for entire frame (SSD1306_WIDTH pixels by SSD1306_NUM_PAGES pages)
    struct render_area frame_area = {
        start_col: 0,
        end_col : SSD1306_WIDTH - 1,
        start_page : 0,
        end_page : SSD1306_NUM_PAGES - 1
        };

    calc_render_area_buflen(&frame_area);

    // zero the entire display
    uint8_t buf[SSD1306_BUF_LEN];
    memset(buf, 0, SSD1306_BUF_LEN);
    render(buf, &frame_area);

    // Initialize BH1705
    bh1705_init(i2c_default);
    printf("Initialize BH1705 done.\n");

    char lux_str[16];

    while (true) {
        // Toggle the LED
        gpio_put(LED_PIN, led_state);
        led_state = !led_state;

        // Get the luminance value
        float lux = bh1705_read_luminance(i2c0);
        memset(lux_str, 0, sizeof(lux_str));
        snprintf(lux_str, sizeof(lux_str), "  %.2f lx   ", lux);
        printf(strcat(lux_str,"\n"));
        // 16char width, 8 lines
        char *text[] = {
            lux_str,
            "",
            " SSD1306 OLED",
            "    BH1750",
            " ILLUMINANCE",
            " SENSOR TEST",
            " BY AIRNANASI",
            " 2025 V0.0.1",
        };

        int y = 0;
        for (uint i = 0 ;i < count_of(text); i++) {
            WriteString(buf, 8, y, text[i]);
            y+=8;
        }
        render(buf, &frame_area);
        sleep_ms(1000);
    }
#endif
    return 0;
}

