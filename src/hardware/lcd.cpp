#include "lcd.h"

#include <stdio.h>
#include <string.h>
#include "pico/stdlib.h"
#include "src/hardware/sd.h"
#include "src/sprites.h"

#include "drivers/st7789/st7789.hpp"
#include "libraries/pico_graphics/pico_graphics.hpp"

namespace LCD {
    const int SCREEN_WIDTH = 240;
    const int SCREEN_HEIGHT = 240;
    constexpr uint16_t CHROMA_KEY = 0x1FF8; // RGB565 magenta, treated as transparent

    static uint16_t buffer[SCREEN_WIDTH * SCREEN_HEIGHT];

    pimoroni::ST7789 st7789(SCREEN_WIDTH, SCREEN_HEIGHT, pimoroni::ROTATE_0, false, {spi1, 9U, 10U, 11U, pimoroni::PIN_UNUSED, 8U, 13U});
    pimoroni::PicoGraphics_PenRGB565 graphics(st7789.width, st7789.height, buffer);

    void write_cmd(uint8_t cmd) {
        gpio_put(8, 0); // DC low for commands
        gpio_put(9, 0); // CS low
        spi_write_blocking(spi1, &cmd, 1);
        gpio_put(9, 1); // CS high
    }
    void write_cmd(uint8_t cmd, uint8_t data) {
        write_cmd(cmd);
        gpio_put(8, 1); // DC high for data
        gpio_put(9, 0); // CS low
        spi_write_blocking(spi1, &data, 1);
        gpio_put(9, 1); // CS high
    }

    int init() {
        // SPI setup
        spi_init(spi1, 75000000);
        
        // Hardware reset
        gpio_init(7);
        gpio_set_dir(7, GPIO_OUT);
        gpio_put(7, 0);
        sleep_ms(50);
        gpio_put(7, 1);
        sleep_ms(150);

        // Software reset
        write_cmd(0x01);
        sleep_ms(150);

        write_cmd(0x11); // Sleep out
        sleep_ms(150);

        // Color mode = RGB565
        write_cmd(0x3A, 0x55);
        // sleep_ms(10);

        write_cmd(0x36, 0x00); // Memory access control
        write_cmd(0x21); // Inversion on
        // write_cmd(0x13); // Normal display mode
        write_cmd(0x29); // Display on
        sleep_ms(100);

        graphics.clear();
        st7789.update(&graphics);
        return 0;
    }

    void set_backlight(int brightness) {
        st7789.set_backlight(brightness);
    }

    void set_pen_color(uint8_t r, uint8_t g, uint8_t b) {
        graphics.set_pen(r, g, b);
    }

    void clear_screen() {
        graphics.set_pen(0, 0, 0);
        graphics.clear();
    }

    void update_screen() {
        st7789.update(&graphics);
    }

    void update_screen(uint16_t x, uint16_t y, uint8_t w, uint8_t h) {
        // For some reason this method is unimplemented, need to create it based off of st7789.update()
        st7789.partial_update(&graphics, pimoroni::Rect(x, y, w, h));
    }

    void draw_sprite(SD::Sprite *sprite, uint8_t posx, uint8_t posy, bool transparent, uint8_t frame, uint8_t scale) {
        uint16_t *frame_data = (uint16_t*)sprite->data + frame * sprite->width * sprite->width;

        if(!transparent && scale == 1) {
            if(sprite->width == SCREEN_WIDTH) {
                memcpy(buffer, frame_data, sprite->width * sprite->width * sizeof(uint16_t));
                return;
            }
            for(int i = 0; i < sprite->width; i++){
                memcpy(&buffer[(posx+i)*SCREEN_WIDTH+posy], &frame_data[i*sprite->width], sprite->width * sizeof(uint16_t));
            }
            return;
        }

        for(int i = 0; i < sprite->width; i++){
            for(int j = 0; j < sprite->width; j++){
                uint16_t pixel = frame_data[i*sprite->width + j];
                if(!transparent || pixel != CHROMA_KEY) {
                    for(int si = 0; si < scale; si++) {
                        for(int sj = 0; sj < scale; sj++) {
                            buffer[(posx + i*scale + si)*SCREEN_WIDTH + posy + j*scale + sj] = pixel;
                        }
                    }
                }
            }
        }
    }

    void draw_text(std::string msg, uint8_t posx, uint8_t posy, uint8_t wrap) {
        graphics.text(msg, pimoroni::Point(posx, posy), wrap);
    }

    void draw_rect(int32_t x, int32_t y, int32_t w, int32_t h) {
        graphics.rectangle(pimoroni::Rect(x, y, w, h));
    }

    void draw_line(int32_t x1, int32_t y1, int32_t x2, int32_t y2) {
        graphics.line(pimoroni::Point(x1, y1), pimoroni::Point(x2, y2));
    }
};
