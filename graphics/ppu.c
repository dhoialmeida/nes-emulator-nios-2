/* ppu.c - Implementação da unidade de processamento de imagens */

#include "../execution/state.h"
#include "../mappers/mappers.h"
#include "../util/types.h"

#include "ppu.h"

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

uint8_t reverse_bits(uint8_t x){
    x = (((x & 0xaaaaaaaa) >> 1) | ((x & 0x55555555) << 1));
    x = (((x & 0xcccccccc) >> 2) | ((x & 0x33333333) << 2));
    x = (((x & 0xf0f0f0f0) >> 4) | ((x & 0x0f0f0f0f) << 4));
    return x;
}

void render_sprites(State *st, Mapper *mapper) {
    uint16_t start_y, index, attr, start_x;
    uint16_t pattern_low, pattern_high, color, color_set;
    uint16_t pattern_addr;

    for (uint16_t i = 0; i < 255;) {
        start_y = st->ppu.oam[i++];
        index = st->ppu.oam[i++];
        attr = st->ppu.oam[i++];
        start_x = st->ppu.oam[i++];

        color_set = attr & 0x3;

        if (start_y < 240) {
            for (uint8_t y = start_y; (y < (start_y+8)) && (y < 240); y++) {
                // Obtém os bits da tabela pattern (CHR)
                pattern_addr = ((index << 4) | (y & 0x7));
                pattern_low = ppu_get(mapper, pattern_addr);
                pattern_high = ppu_get(mapper, pattern_addr + 8);

                if (attr & 64) {
                    pattern_high = reverse_bits(pattern_high);
                    pattern_low = reverse_bits(pattern_low);
                }

                for (uint8_t x = start_x; (x < 255) && (x < start_x+8);) {
                    // Obtém o número da cor através da pattern
                    color = (pattern_high >> 6) & 0x2;
                    color = color | (pattern_low >> 7);

                    // Obtém a cor da tabela de cores
                    if (color != 0) {
                        color = st->ppu.palette[16 + color_set*4 + color];
                        // Desenha o pixel
                        draw_point(x, y, RED(color), GREEN(color), BLUE(color));
                    }

                    pattern_high <<= 1;
                    pattern_low <<= 1;
                    x++;
                }
            }
        }
    }
}

// Renderiza um frame
void ppu(State *st, Mapper *mapper) {
    uint16_t tile = 0; // Número to tile atual
    uint8_t tx = 0, ty = 0; // x e y to tile atual
    uint8_t name_entry; // entrada na nametable
    uint8_t attr, attr_addr = 0; // atributo do bloco atual
    uint8_t color_set; // palete selecionado
    uint16_t pattern_addr; // endereço da pattern table
    uint8_t pattern_low; // parte baixa do pattern table
    uint8_t pattern_high; // parte alta do pattern table (chr)
    uint8_t color; // cor do pixel

    for (uint16_t y = 0; y < 240; y++) {
        tile = (y >> 3) << 5; // int(y / 8) * 32
        ty = y & 16; // bit 4
        for (uint16_t x = 0; x < 256;) {
            // Obtém a entrada no nametable e atributos
            tx = x & 16; // bit 4
            name_entry = st->ppu.vram[tile];
            attr_addr = ((y >> 5) << 3) + (x >> 5); // int(y / 32) * 8 + int(x/32)
            attr = st->ppu.vram[0x3C0 + attr_addr];

            // Obtém a paleta de acordo com o quadrante do tile
            if ((!tx) & (!ty)) {
                // Top left
                color_set = attr;
            } else if (tx & (!ty)) {
                // Top right
                color_set = attr >> 2;
            } else if ((!tx) & ty) {
                // Bottom left
                color_set = attr >> 4;
            } else {
                // Bottom right
                color_set = attr >> 6;
            }
            color_set = color_set & 0x3;

            // Obtém os bits da tabela pattern (CHR)
            pattern_addr = ((name_entry << 4) | (y & 0x7)) + 0x1000;
            pattern_low = ppu_get(mapper, pattern_addr);
            pattern_high = ppu_get(mapper, pattern_addr + 8);

            for (uint8_t i = 0; i < 8; i++) {
                // Obtém o número da cor através da pattern
                color = (pattern_high >> 6) & 0x2;
                color = color | (pattern_low >> 7);

                // Obtém a cor da tabela de cores
                if (color == 0) {
                    color = st->ppu.palette[0];
                } else {
                    color = st->ppu.palette[color_set*4 + color];
                }

                // Desenha o pixel
                draw_point(x, y, RED(color), GREEN(color), BLUE(color));

                pattern_high <<= 1;
                pattern_low <<= 1;
                x++;
            }

            tile++;
        }
    }
    render_sprites(st, mapper);
    graphics_update();
}
