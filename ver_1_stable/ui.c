#include "ui.h"
#include "gpu.h"
#include "fs.h"

// DEFINITIONS DES VARIABLES (La "maison" des variables)
int menu_ouvert = 0;
int menu_hauteur = 0;
int mode_renommer = 0;

char *keyboard_layout[] = {
    "1234567890",
    "AZERTYUIOP",
    "QSDFGHJKLM",
    "WXCVBN"
};

// --- FONCTIONS DE DESSIN ---

void itoa_simple(int n, char *s) {
    s[0] = (n / 10) + '0';
    s[1] = (n % 10) + '0';
    s[2] = '\0';
}

void draw_folder_icon(int x, int y) {
    draw_rect(x, y, x + 20, y + 10, 0xFFD4AC0D); 
    draw_rect(x, y + 6, x + 50, y + 40, 0xFFF1C40F); 
    draw_rect(x + 2, y + 36, x + 48, y + 38, 0xFFB7950B);
}

void ui_draw_cursor(int x, int y) {
    draw_rect(x - 1, y - 1, x + 7, y + 7, 0xFF000000);
    draw_rect(x, y, x + 6, y + 6, 0xFFFFFFFF);
}

void ui_draw_background() {
    draw_rect(0, 0, 1024, 768, 0xFF2980B9);
}

void ui_draw_taskbar(int sur_menu_btn, unsigned int ticks) {
    draw_rect(0, 710, 1024, 768, 0xFF2C3E50);
    draw_rect(412, 720, 612, 750, (sur_menu_btn ? 0xFFE67E22 : 0xFFD35400));
    draw_string(475, 728, "SYSTEME", 0xFFFFFFFF);

    // Horloge
    unsigned int total_sec = ticks / 1000000;
    int mins = (total_sec / 60) % 100;
    int secs = total_sec % 60;
    char m_str[3], s_str[3];
    itoa_simple(mins, m_str);
    itoa_simple(secs, s_str);

    draw_rect(910, 720, 1010, 750, 0xFF34495E);
    draw_string(925, 728, m_str, 0xFFFFFFFF);
    draw_string(945, 728, ":", 0xFFFFFFFF);
    draw_string(955, 728, s_str, 0xFFFFFFFF);
}

void ui_draw_keyboard(int mx, int my) {
    draw_rect(50, 565, 974, 765, 0xFF34495E); 
    for(int r = 0; r < 4; r++) {
        char *line = keyboard_layout[r];
        for(int i = 0; line[i] != '\0'; i++) {
            int kx = 80 + (i * 60) + (r * 20);
            int ky = 575 + (r * 45);
            unsigned int kcol = (mx > kx && mx < kx+50 && my > ky && my < ky+40) ? 0xFFE67E22 : 0xFF2C3E50;
            draw_rect(kx, ky, kx + 50, ky + 40, kcol);
            char key[2] = {line[i], '\0'};
            draw_string(kx + 20, ky + 12, key, 0xFFFFFFFF);
        }
    }
    // Espace
    int sp_x = 480, sp_y = 710;
    unsigned int sp_col = (mx > sp_x && mx < sp_x + 240 && my > sp_y && my < sp_y + 40) ? 0xFFE67E22 : 0xFF2C3E50;
    draw_rect(sp_x, sp_y, sp_x + 240, sp_y + 40, sp_col);
    draw_string(sp_x + 80, sp_y + 12, "ESPACE", 0xFFFFFFFF);

    // Effacer
    int bk_x = 730, bk_y = 710;
    unsigned int bk_col = (mx > bk_x && mx < bk_x + 160 && my > bk_y && my < bk_y + 40) ? 0xFFC0392B : 0xFF2C3E50;
    draw_rect(bk_x, bk_y, bk_x + 160, bk_y + 40, bk_col);
    draw_string(bk_x + 40, bk_y + 12, "EFFACER", 0xFFFFFFFF);
}

void ui_draw_menu(int mx, int my) {
    if (menu_hauteur <= 5) return;
    int m_y = 705 - menu_hauteur;
    draw_rect(402, m_y, 622, 705, 0xFF34495E);
    if (menu_hauteur > 150) draw_string(415, m_y + 20, "> EXPLORATEUR", 0xFFFFFFFF);
}

void ui_draw_explorer() {
    draw_window(50, 40, 924, 520, " EXPLORATEUR");
    draw_rect(60, 80, 964, 550, 0xFFFFFFFF); 
    draw_rect(900, 90, 950, 120, 0xFF2ECC71);
    draw_string(918, 98, "+", 0xFFFFFFFF);

    int count = 0;
    for (int i = 0; i < MAX_FILES; i++) {
        if(ramdisk[i].active) {
            int col = count % 6; int row = count / 6;
            int fx = 100 + (col * 140); int fy = 120 + (row * 100);
            draw_folder_icon(fx, fy);
            draw_string(fx - 10, fy + 45, ramdisk[i].name, 0xFF2C3E50);
            count++;
        }
    }
}

void ui_draw_file_view(int idx) {
    if (idx < 0) return;
    draw_window(80, 30, 864, 500, ramdisk[idx].name);
    draw_rect(90, 70, 934, 520, 0xFFFCF3CF); 
    draw_string(110, 90, ramdisk[idx].content, 0xFF2C3E50);
    draw_rect(880, 80, 925, 110, 0xFFE74C3C); 
    draw_string(895, 88, "X", 0xFFFFFFFF);
}