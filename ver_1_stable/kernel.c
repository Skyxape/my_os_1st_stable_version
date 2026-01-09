#include "mailbox.h"
#include "gpu.h"
#include "ui.h"
#include "fs.h"

// Adresses matérielles pour le Timer et l'UART
#define TIMER_CLO    ((volatile unsigned int*)(0x3F003004))
#define UART0_DR     ((volatile unsigned int*)(0x3F201000))
#define UART0_FR     ((volatile unsigned int*)(0x3F201018))

// Variables partagées avec ui.c
extern int menu_ouvert;
extern int menu_hauteur;
extern char *keyboard_layout[];
extern int mode_renommer; 

void kernel_main() {
    // Initialisation du Framebuffer via Mailbox
    mbox[0] = 35*4; mbox[1] = 0;
    mbox[2] = 0x48003; mbox[3] = 8; mbox[4] = 8; mbox[5] = 1024; mbox[6] = 768;
    mbox[7] = 0x48004; mbox[8] = 8; mbox[9] = 8; mbox[10] = 1024; mbox[11] = 768;
    mbox[12] = 0x48005; mbox[13] = 4; mbox[14] = 4; mbox[15] = 32;
    mbox[16] = 0x40001; mbox[17] = 8; mbox[18] = 8; mbox[19] = 0; mbox[20] = 0;
    mbox[21] = 0;

    if (mbox_call(8)) {
        unsigned int *fb = (unsigned int *)((unsigned long)mbox[19] & 0x3FFFFFFF);
        gpu_init(fb);
        fs_init();

        int mx = 512, my = 384;
        int explorer = 0, open_idx = -1;
        unsigned int last_f = 0;

        while (1) {
            // Gestion du timing pour la fluidité (environ 60 FPS)
            unsigned int current_ticks = *TIMER_CLO;
            if (current_ticks - last_f < 15000) continue;
            last_f = current_ticks;

            // Animation du menu système
            if (menu_ouvert && menu_hauteur < 200) menu_hauteur += 20;
            if (!menu_ouvert && menu_hauteur > 0) menu_hauteur -= 20;

            // --- DESSIN DE L'INTERFACE ---
            ui_draw_background();

            // Affichage de la barre des tâches et de l'horloge en continu
            ui_draw_taskbar(mx > 412 && mx < 612 && my > 710, current_ticks);

            if (explorer) {
                ui_draw_explorer();
                if (open_idx != -1) {
                    if (mode_renommer) {
                        ui_draw_keyboard(mx, my);
                        draw_string(110, 530, "MODE RENOMMER :", 0xFFFFFFFF);
                        draw_string(250, 530, ramdisk[open_idx].name, 0xFFF1C40F);
                    } else {
                        ui_draw_file_view(open_idx);
                        ui_draw_keyboard(mx, my);
                    }
                }
            }
            
            ui_draw_menu(mx, my);
            ui_draw_cursor(mx, my);

            gpu_render();

            // --- GESTION DES ENTRÉES (UART) ---
            if (!(*UART0_FR & 0x10)) {
                char k = (char)(*UART0_DR);
                
                // Mouvements fluides
                if (k == 'z') { my -= 10; } 
                if (k == 's') { my += 10; }
                if (k == 'q') { mx -= 10; } 
                if (k == 'd') { mx += 10; }
                
                if (k == ' ') {
                    // Clic sur bouton système
                    if (!explorer && mx > 412 && mx < 612 && my > 710) {
                        menu_ouvert = !menu_ouvert;
                    }
                    // Clic sur Explorateur dans le menu
                    else if (menu_ouvert && mx > 402 && mx < 622 && my > 500) {
                        explorer = 1; menu_ouvert = 0;
                    } 
                    else if (explorer) {
                        if (open_idx != -1) {
                            // --- CLAVIER VIRTUEL ---
                            for(int r = 0; r < 4; r++) {
                                char *l = keyboard_layout[r];
                                for(int i = 0; l[i] != '\0'; i++) {
                                    int kx_f = 80 + (i * 60) + (r * 20);
                                    int ky_f = 575 + (r * 45);
                                    if(mx > kx_f && mx < kx_f+50 && my > ky_f && my < ky_f+40) {
                                        if (mode_renommer) {
                                            int len = 0; while(ramdisk[open_idx].name[len]) len++;
                                            if(len < 10) { ramdisk[open_idx].name[len] = l[i]; ramdisk[open_idx].name[len+1] = '\0'; }
                                        } else {
                                            int len = 0; while(ramdisk[open_idx].content[len]) len++;
                                            if(len < 400) { ramdisk[open_idx].content[len] = l[i]; ramdisk[open_idx].content[len+1] = '\0'; }
                                        }
                                    }
                                }
                            }
                            // Bouton Espace (uniquement pour le contenu)
                            if(mx > 480 && mx < 720 && my > 710 && my < 750 && !mode_renommer) {
                                int len = 0; while(ramdisk[open_idx].content[len]) len++;
                                if(len < 400) { ramdisk[open_idx].content[len] = ' '; ramdisk[open_idx].content[len+1] = '\0'; }
                            }
                            // Bouton Effacer
                            if(mx > 730 && mx < 890 && my > 710 && my < 750) {
                                if (mode_renommer) {
                                    int len = 0; while(ramdisk[open_idx].name[len]) len++;
                                    if(len > 0) ramdisk[open_idx].name[len-1] = '\0';
                                } else {
                                    int len = 0; while(ramdisk[open_idx].content[len]) len++;
                                    if(len > 0) ramdisk[open_idx].content[len-1] = '\0';
                                }
                            }
                            // Fermer fichier (Bouton X)
                            if (!mode_renommer && mx > 880 && mx < 925 && my > 80 && my < 110) {
                                open_idx = -1;
                            }
                        } else {
                            // --- NAVIGATION EXPLORATEUR ---
                            if (mx > 900 && mx < 950 && my > 90 && my < 120) {
                                fs_create("NOUVEAU", "txt", "");
                            } else {
                                int c = 0;
                                for(int i=0; i<MAX_FILES; i++) {
                                    if(ramdisk[i].active) {
                                        int col = c % 6; int row = c / 6;
                                        int fx = 100 + (col * 140); int fy = 120 + (row * 100);
                                        // Clic icône = ouvrir
                                        if(mx > fx && mx < fx + 50 && my > fy && my < fy + 40) { 
                                            open_idx = i; mode_renommer = 0; 
                                        }
                                        // Clic texte = renommer
                                        else if(mx > fx - 10 && mx < fx + 80 && my > fy + 40 && my < fy + 60) { 
                                            open_idx = i; mode_renommer = 1; 
                                        }
                                        c++;
                                    }
                                }
                            }
                        }
                    }
                }
                
                // Touche 'x' pour quitter/fermer
                if (k == 'x') { 
                    if (mode_renommer) mode_renommer = 0; 
                    else if (open_idx != -1) open_idx = -1;
                    else { explorer = 0; menu_ouvert = 0; }
                }
            }

            // --- BORNES DE L'ÉCRAN ---
            if (mx < 0) { mx = 0; }
            if (mx > 1014) { mx = 1014; }
            if (my < 0) { my = 0; }
            if (my > 758) { my = 758; }
        }
    }
}