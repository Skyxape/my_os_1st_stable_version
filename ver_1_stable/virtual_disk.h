#ifndef VIRTUAL_DISK_H
#define VIRTUAL_DISK_H

#define DISK_SEED_COUNT 2

static struct {
    char name[12];
    char ext[4];
    char content[100];
} initial_disk[DISK_SEED_COUNT] = {
    {"SYSTEM", "sys", "EOS KERNEL V1.0 - STABLE"},
    {"LISEZMOI", "txt", "BIENVENUE DANS VOTRE OS BARE METAL !"}
};

#endif