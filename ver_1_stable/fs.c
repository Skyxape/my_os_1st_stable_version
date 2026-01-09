#include "fs.h"
#include "virtual_disk.h"

VirtualFile ramdisk[MAX_FILES];

void fs_init() {
    for(int i = 0; i < MAX_FILES; i++) {
        ramdisk[i].active = 0;
    }

    // Chargement des fichiers initiaux
    for(int i = 0; i < DISK_SEED_COUNT; i++) {
        fs_create(initial_disk[i].name, initial_disk[i].ext, initial_disk[i].content);
    }
}

void fs_create(char *name, char *extension, char *content) {
    for(int i = 0; i < MAX_FILES; i++) {
        if(!ramdisk[i].active) {
            // Copie du nom
            int n = 0;
            while(name[n] && n < 11) { ramdisk[i].name[n] = name[n]; n++; }
            ramdisk[i].name[n] = '\0';

            // Copie de l'extension
            int e = 0;
            while(extension[e] && e < 3) { ramdisk[i].ext[e] = extension[e]; e++; }
            ramdisk[i].ext[e] = '\0';

            // Copie du contenu
            int c = 0;
            while(content[c] && c < FILE_SIZE_MAX - 1) {
                ramdisk[i].content[c] = content[c];
                c++;
            }
            ramdisk[i].content[c] = '\0';
            ramdisk[i].size = c;
            ramdisk[i].active = 1;
            return;
        }
    }
}

void fs_delete(int idx) {
    if(idx >= 0 && idx < MAX_FILES) {
        ramdisk[idx].active = 0;
    }
}

void fs_rename(int idx, char *new_name) {
    if (idx >= 0 && idx < MAX_FILES && ramdisk[idx].active) {
        int i = 0;
        while (new_name[i] && i < 11) {
            ramdisk[idx].name[i] = new_name[i];
            i++;
        }
        ramdisk[idx].name[i] = '\0';
    }
}