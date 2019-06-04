/* ppu.c - Implementação da unidade de processamento de imagens */

#include "../execution/state.h"
#include "../mappers/mappers.h"
#include "../util/queue.h"
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

// Renderiza um frame
void ppu(State *st, Mapper *mapper) {
    uint16_t tile; // Número to tile atual
    uint8_t tx, ty; // x e y to tile atual
    uint8_t name_entry; // entrada na nametable
    uint8_t attr; // atributo do bloco atual
    uint8_t color_set; // palete selecionado
    uint16_t pattern_addr; // endereço da pattern table
    uint8_t pattern_low; // parte baixa do pattern table
    uint8_t pattern_high; // parte alta do pattern table (chr)
    uint8_t color; // cor do pixel

    // Colors: 2 (back ou front) X 4 (numero de palettes) X 4 (cores por palette)
    uint8_t colors[2][4][4];

    reset_queue(st);
    for (uint16_t y = 0; y < 240; y++) {
        for (uint16_t x = 0; x < 256; x++) {
            if ((x & 0x7) == 0) {
                // Calcula o tile e sua posição x y
                tile = (y>>2) + (x & 0x7);
                tx = (tile / 32) & 1;
                ty = (tile % 32) & 1;

                // Obtém a entrada no nametable e atributos
                name_entry = ppu_get(mapper, 0x2000 + tile);
                attr = ppu_get(mapper, 0x23C0);

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
                pattern_addr = (name_entry << 4) | (x & 0x7);
                pattern_low = ppu_get(mapper, pattern_addr);
                pattern_high = ppu_get(mapper, pattern_addr + 8);

                // Preenche colors com as cores na memória da PPU
                for (uint8_t i = 0; i < 2; i++)
                    for (uint8_t pal = 0; pal < 4; pal++)
                        for (uint8_t col = 0; col < 4; col++) {
                            colors[i][pal][col] = ppu_get(mapper, 0x3F01 + i*2*4 + pal*4 + col);
                        }
            }

            // Obtém o número da cor através da pattern
            color = (pattern_high >> 6) & 0x2;
            color = color | (pattern_low >> 7);

            // Obtém a cor da tabela de cores
            color = colors[0][color_set][color];

            // Desenha o pixel
            draw_point(x, y, RED(color), GREEN(color), BLUE(color));

            pattern_high <<= 1;
            pattern_low <<= 1;
        }
    }

    graphics_update();
}
