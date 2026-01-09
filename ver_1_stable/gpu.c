#include "gpu.h"
#include "font.h"

static unsigned int *framebuffer;
static unsigned int *backbuffer = (unsigned int *)0x800000; 

void gpu_init(unsigned int *fb) {
    framebuffer = fb;
    for (int i = 0; i < (1024 * 768); i++) backbuffer[i] = 0xFF2980B9;
}

void draw_pixel(int x, int y, unsigned int color) {
    if (x >= 0 && x < 1024 && y >= 0 && y < 768) {
        backbuffer[y * 1024 + x] = color;
    }
}

void draw_rect(int x1, int y1, int x2, int y2, unsigned int color) {
    if (x1 < 0) { x1 = 0; }
    if (y1 < 0) { y1 = 0; }
    if (x2 > 1024) { x2 = 1024; }
    if (y2 > 768) { y2 = 768; }

    for (int y = y1; y < y2; y++) {
        unsigned int *dest = &backbuffer[y * 1024 + x1];
        for (int x = x1; x < x2; x++) {
            *dest++ = color;
        }
    }
}

void gpu_render() {
    if (!framebuffer) return;
    unsigned int *src = backbuffer;
    unsigned int *dst = framebuffer;
    for (int i = 0; i < (1024 * 768); i++) {
        *dst++ = *src++;
    }
}
// ... garde tes fonctions draw_string, etc. à la suite ...

void draw_string(int x, int y, char *s, unsigned int color) {
    int cx = x; int cy = y;
    while (*s) {
        if (*s == '\n') { cy += 12; cx = x; }
        else {
            for (int i = 0; i < 8; i++) {
                unsigned char line = font[(unsigned char)*s][i];
                for (int j = 0; j < 8; j++) {
                    if ((line >> (7 - j)) & 1) draw_pixel(cx + j, cy + i, color);
                }
            }
            cx += 9;
        }
        s++;
    }
}

void draw_window(int x, int y, int w, int h, char *title) {
    draw_rect(x, y, x + w, y + h, 0xFFEEEEEE); // Corps
    draw_rect(x, y, x + w, y + 30, 0xFF2C3E50); // Barre titre
    draw_string(x + 10, y + 10, title, 0xFFFFFFFF);
    draw_rect(x + w - 25, y + 5, x + w - 5, y + 25, 0xFFE74C3C); // Bouton Fermer
}

unsigned int gpu_get_pixel(int x, int y) {
    if (x >= 0 && x < 1024 && y >= 0 && y < 768) return backbuffer[y * 1024 + x];
    return 0;
}