Raspberry Pi 3 - Bare Metal OS (v1.0.0-stable)

Ce projet est une implémentation d'un système d'exploitation minimaliste fonctionnant en "Bare Metal" sur Raspberry Pi 3 (Architecture ARMv8-A). Il intègre un moteur graphique de base, un système de fichiers en RAM (ramdisk) et des applications utilitaires comme un éditeur de dessin (Paint) et un explorateur de fichiers.

## 🛠 Prérequis

Pour compiler et tester ce système, les outils suivants sont nécessaires :

* **Toolchain :** `aarch64-linux-gnu` (GCC, LD, Objcopy)
* **Émulateur :** `qemu-system-aarch64`
* **Affichage :** Un client VNC (type TightVNC ou RealVNC)

## 🏗 Compilation

Le projet utilise un **Makefile** pour automatiser la génération de l'image binaire.

1. Nettoyer les anciens builds :
```bash
make clean

```


2. Compiler le noyau :
```bash
make

```


Cette commande génère le fichier `kernel8.img`.

## 🚀 Lancement (Émulation)

Pour lancer le système dans l'environnement QEMU :

```bash
make run

```

L'émulateur est configuré pour simuler une Raspberry Pi 3B (`-M raspi3b`) avec une sortie série sur le terminal (`stdio`) et un serveur d'affichage déporté via VNC.

## 📺 Accès à l'interface graphique

Le système n'ouvre pas de fenêtre native. L'affichage est accessible via le protocole VNC :

1. Lancez votre client VNC.
2. Connectez-vous à l'adresse suivante : `localhost:5901`.
3. L'interface utilisateur (Desktop) devrait apparaître.

## 📁 Structure du projet

* `boot.S` : Code de démarrage en assembleur (initialisation des coeurs et de la pile).
* `kernel.c` : Boucle principale et gestionnaire d'événements.
* `ui.c` : Moteur d'interface graphique et applications (Paint, Explorer).
* `gpu.c` : Driver de bas niveau pour le Framebuffer.
* `fs.c` : Implémentation du système de fichiers en RAM.
* `mailbox.c` : Communication avec le GPU via les canaux de messagerie Raspberry Pi.

## ⚠️ Notes techniques

* **Clavier :** Les contrôles (Z, Q, S, D pour le curseur, Espace pour cliquer, X pour quitter) sont transmis via la liaison UART de QEMU.
