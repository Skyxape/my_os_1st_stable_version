.section ".text.boot"

.global _start

_start:
    // Lire l'ID du processeur, si ce n'est pas le coeur 0, on l'endort
    mrs     x0, mpidr_el1
    and     x0, x0, #3
    cbz     x0, master
    b       hang

master:
    // Initialiser la pile (Stack Pointer)
    ldr     x0, =_stack_top
    mov     sp, x0

    // Sauter vers le code C
    bl      kernel_main

hang: 
    wfe
    b       hang