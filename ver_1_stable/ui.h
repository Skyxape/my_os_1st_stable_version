#ifndef UI_H
#define UI_H

void ui_draw_background();
void ui_draw_taskbar(int sur_menu_btn, unsigned int ticks); // Ajout du paramètre ticks
void ui_draw_menu(int mx, int my);
void ui_draw_explorer();
void ui_draw_file_view(int idx);
void ui_draw_keyboard(int mx, int my);
void ui_draw_cursor(int x, int y);
void draw_window(int x, int y, int w, int h, char *title);

#endif