#ifndef FS_H
#define FS_H

#define MAX_FILES 15
#define FILE_SIZE_MAX 512

typedef struct {
    char name[12];
    char ext[4];      // "txt", "sys", etc.
    char content[FILE_SIZE_MAX];
    unsigned int size;
    int active;
} VirtualFile;

// Le ramdisk est notre "disque dur" en RAM
extern VirtualFile ramdisk[MAX_FILES];

void fs_init();
void fs_create(char *name, char *extension, char *content);
void fs_delete(int idx);
// Fonction pour simuler l'enregistrement (maj de la taille et contenu)
void fs_save(int idx, char *new_content);

#endif