#include "headers.h"

int main(void) {
    State st;
    char *ROM_START = 0x123456576;
    cpu(ROM_START, &st);
}