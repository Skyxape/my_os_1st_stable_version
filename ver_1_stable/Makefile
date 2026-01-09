TARGET = kernel8.img
CC = aarch64-linux-gnu-gcc
LD = aarch64-linux-gnu-ld
OBJCOPY = aarch64-linux-gnu-objcopy
CFLAGS = -Wall -O2 -ffreestanding -nostdlib -nostartfiles

OBJS = boot.o mailbox.o gpu.o ui.o kernel.o sd.o fs.o

all: $(TARGET)

boot.o: boot.S
	$(CC) $(CFLAGS) -c boot.S -o boot.o

%.o: %.c
	$(CC) $(CFLAGS) -c $< -o $@

$(TARGET): $(OBJS)
	$(LD) -nostdlib $(OBJS) -T linker.ld -o kernel8.elf
	$(OBJCOPY) -O binary kernel8.elf $(TARGET)

clean:
	rm -f *.o *.elf $(TARGET)

run:
	qemu-system-aarch64 -M raspi3b -kernel kernel8.img -serial stdio -vnc :1