/* ppu.c - Implementação da unidade de processamento de imagens */
#include "../headers.h"

// Palete de cores (R0, G0, B0, R1, G1, B1...)
static uint8_t palette[] = {
    0x46, 0x46, 0x46, 0x00, 0x06, 0x5A, 0x00, 0x06, 0x78, 0x02, 0x06, 0x73, 0x35, 0x03, 0x4C, 0x57,
    0x00, 0x0E, 0x5A, 0x00, 0x00, 0x41, 0x00, 0x00, 0x12, 0x02, 0x00, 0x00, 0x14, 0x00, 0x00, 0x1E,
    0x00, 0x00, 0x1E, 0x00, 0x00, 0x15, 0x21, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0x9D, 0x9D, 0x9D, 0x00, 0x4A, 0xB9, 0x05, 0x30, 0xE1, 0x57, 0x18, 0xDA, 0x9F, 0x07, 0xA7, 0xCC,
    0x02, 0x55, 0xCF, 0x0B, 0x00, 0xA4, 0x23, 0x00, 0x5C, 0x3F, 0x00, 0x0B, 0x58, 0x00, 0x00, 0x66,
    0x00, 0x00, 0x67, 0x13, 0x00, 0x5E, 0x6E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFE, 0xFF, 0xFF, 0x1F, 0x9E, 0xFF, 0x53, 0x76, 0xFF, 0x98, 0x65, 0xFF, 0xFC, 0x67, 0xFF, 0xFF,
    0x6C, 0xB3, 0xFF, 0x74, 0x66, 0xFF, 0x80, 0x14, 0xC4, 0x9A, 0x00, 0x71, 0xB3, 0x00, 0x28, 0xC4,
    0x21, 0x00, 0xC8, 0x74, 0x00, 0xBF, 0xD0, 0x2B, 0x2B, 0x2B, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
    0xFE, 0xFF, 0xFF, 0x9E, 0xD5, 0xFF, 0xAF, 0xC0, 0xFF, 0xD0, 0xB8, 0xFF, 0xFE, 0xBF, 0xFF, 0xFF,
    0xC0, 0xE0, 0xFF, 0xC3, 0xBD, 0xFF, 0xCA, 0x9C, 0xE7, 0xD5, 0x8B, 0xC5, 0xDF, 0x8E, 0xA6, 0xE6,
    0xA3, 0x94, 0xE8, 0xC5, 0x92, 0xE4, 0xEB, 0xA7, 0xA7, 0xA7, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
};

#define RED(color) palette[(color)*3 + 0]
#define GREEN(color) palette[(color)*3 + 1]
#define BLUE(color) palette[(color)*3 + 2]

// Renderiza um frame
void ppu(State *st) {
    for (int i = 0; i < 256*240; i++) {
        draw_point(i & 0xFF, i >> 8, 0x85, 0x76, 0xFF);
    }

    graphics_update();
}
