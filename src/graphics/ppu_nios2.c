/* ppu_nios2.c - Implementação das primitivas gráficas para Nios II */

#ifdef PLACA

#include <stdio.h>
#include "../util/types.h"

#define FRONT_BUFFER_ADDR (volatile int *) 0x10003020
#define BACK_BUFFER_ADDR (volatile int *) 0x10003024

uint8_t frame_count = 0;

volatile short int *front = (volatile short int *) 0x08000000;
volatile short int *back = (volatile short int *) 0x08040000;

void clear_screen(uint16_t color){
    volatile short int* px;
    for (px = back; px < back + 2*512*240; px++) {
        *px = color;
    }
}

void graphics_init() {
    *(BACK_BUFFER_ADDR) = 0x08040000;
    clear_screen(0x0000);
}

void draw_pixel(uint16_t x, uint16_t y, uint16_t color){
    volatile short int* px = 30 + back + x + (y << 9);
    *px = color;
}

uint16_t mapColor(uint8_t r, uint8_t g, uint8_t b){
    uint16_t color = 0;
    r = r >> 3;
    g = g >> 2;
    b = b >> 3;
    color |= b;
    color |= g << 5;
    color |= r << 11;

    return color;
}

void draw_point(uint16_t x, uint16_t y, uint8_t r, uint8_t g, uint8_t b) {
    //TODO:upscale
    draw_pixel(x,y,mapColor(r,g,b));
}

void graphics_update() {
    frame_count++;

    if(frame_count > 5){
        printf("FRAME!\n");
        frame_count = 0;
    }

    volatile int *tmp = (volatile int *) front;
    front = back;
    back = (volatile short int *) tmp;

    tmp = (volatile int *) 0x10003020;
    *tmp = 1;
    tmp = (volatile int *) 0x1000302C;
    while (((*tmp) & 0x1) != 0);
}

void graphics_finish() {
    clear_screen(0x0000);
}

#endif
