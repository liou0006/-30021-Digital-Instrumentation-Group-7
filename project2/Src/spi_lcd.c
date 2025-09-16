// spi_lcd.c
#include <string.h>
#include "stm32f30x_conf.h"
#include "30010_io.h"
#include "lcd.h"
#include "spi_lcd.h"
#include "charset.h"


// --- Temporary weak fallback: 5x7 ASCII font table ---
// If the real charset.c is linked (strong definition), it will override this.
// This keeps the linker happy and lets you see "Hello, DTU!" immediately.

__attribute__((weak)) const uint8_t font5x7[96][5] = {
    // All blank by default:
    [0 ... 95] = {0x00,0x00,0x00,0x00,0x00},

    // Fill only the glyphs we actually use now: ' ', 'H','e','l','o',',','D','T','U','!'
    // Index = char - 0x20
    // Each entry is 5 columns, bit0 = top pixel.

    // ' ' (0x20)
    [0x20 - 0x20] = {0x00,0x00,0x00,0x00,0x00},

    // '!' (0x21)
    [0x21 - 0x20] = {0x00,0x00,0x5F,0x00,0x00},

    // ',' (0x2C)
    [0x2C - 0x20] = {0x00,0x02,0x1C,0x00,0x00},

    // 'D' (0x44)
    [0x44 - 0x20] = {0x7F,0x41,0x41,0x22,0x1C},

    // 'H' (0x48)
    [0x48 - 0x20] = {0x7F,0x08,0x08,0x08,0x7F},

    // 'T' (0x54)
    [0x54 - 0x20] = {0x01,0x01,0x7F,0x01,0x01},

    // 'U' (0x55)
    [0x55 - 0x20] = {0x3F,0x40,0x40,0x40,0x3F},

    // 'e' (0x65)
    [0x65 - 0x20] = {0x3E,0x49,0x49,0x49,0x2E},

    // 'l' (0x6C)
    [0x6C - 0x20] = {0x00,0x41,0x7F,0x40,0x00},

    // 'o' (0x6F)
    [0x6F - 0x20] = {0x3E,0x41,0x41,0x41,0x3E},
};

extern const uint8_t font5x7[96][5];
#define charset font5x7

#define CHAR_W 5
#define CHAR_H 7

static uint8_t fbuffer[512];

void initLCD(void) {
    printf("Initializing LCD\r\n");
    init_spi_lcd();
    memset(fbuffer, 0x00, sizeof(fbuffer));
    lcd_reset();
    lcd_push_buffer(fbuffer);
    printf("Initializing LCD...done\r\n");
}


void transmit_bytes(uint8_t *data, uint16_t length)
{
    if (!data || length == 0) return;
    for (uint16_t i = 0; i < length; i++) {
        lcd_transmit_byte(data[i]);
    }
}

void push_lcd_buffer(uint8_t *buffer)
{
    if (buffer == NULL) {
        lcd_push_buffer(fbuffer);
    } else {
        lcd_push_buffer(buffer);
    }
}

void reset_lcd_screen(void)
{
    lcd_reset();
    lcd_push_buffer(fbuffer);
}

// ---------------------- Drawing helpers ----------------------

void lcd_fill(uint8_t pattern)
{
    memset(fbuffer, pattern, sizeof(fbuffer));
}

void lcd_draw_pixel(uint8_t x, uint8_t y, uint8_t on)
{
    if (x >= 128 || y >= 32) return;

    uint8_t page = y / 8;           // 0..3
    uint8_t bit  = y % 8;           // 0..7 (LSB is top pixel)
    uint16_t idx = page * 128 + x;  // 512-byte linear index

    if (on)  fbuffer[idx] |=  (1U << bit);
    else     fbuffer[idx] &= ~(1U << bit);
}

void lcd_draw_char(uint8_t x, uint8_t y, char c)
{
    if (c < 0x20 || c > 0x7F) c = '?';
    const uint8_t *glyph = charset[c - 0x20];

    for (uint8_t col = 0; col < CHAR_W; col++) {
        uint8_t col_bits = glyph[col];
        for (uint8_t row = 0; row < CHAR_H; row++) {
            uint8_t on = (col_bits >> row) & 0x01;
            lcd_draw_pixel(x + col, y + row, on);
        }
    }
}

void lcd_draw_string(uint8_t x, uint8_t y, const char *s)
{
    uint8_t cursor_x = x;
    while (*s) {
        lcd_draw_char(cursor_x, y, *s++);
        cursor_x += (CHAR_W + 1);       // 6px advance
        if (cursor_x + CHAR_W >= 128)   // simple wrap guard
            break;
    }
}
