#ifndef GPU_H
#define GPU_H

void gpu_init(unsigned int *fb);
void gpu_render();
void draw_pixel(int x, int y, unsigned int color);
void draw_rect(int x1, int y1, int x2, int y2, unsigned int color);
void draw_string(int x, int y, char *s, unsigned int color);
void draw_window(int x, int y, int w, int h, char *title);
unsigned int gpu_get_pixel(int x, int y);

#endif