#include "headers.h"
#define VIDEO_ADDR 0x08000000
#define FRONT_BUFFER_ADDR 0x10003020;
#define BACK_BUFFER_ADDR 0x10003024;

void clear_screen(uint16_t color){
	volatile short int* px;
	for (px = VIDEO_ADDR; px < VIDEO_ADDR + 2*512*240; px++) {
		*px = color;
	}
}

void graphcs_init() {
    clear_screen(0x0000);
}

void draw_pixel(uint16_t x, uint16_t y, uint16_t color){
	volatile short int* px = VIDEO_ADDR + (x << 1) + (y << 10);
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
    draw_pixel(x,y,map(r,g,b));
}

void graphics_update() {
    
}

void graphics_finish() {
    clear_screen(0x0000);
}