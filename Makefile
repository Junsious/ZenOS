# Указываем имя выходного файла
OSNAME := ZenOS
ISOFILE := $(OSNAME).iso
BINARY := $(OSNAME).bin

# Указываем компилятор и ассемблер
AS := i386-elf-as
CC := i386-elf-gcc

# Флаги для компилятора и линковщика
CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS := -ffreestanding -O2 -nostdlib

# Файлы
KERNEL_SRC := kernel.c
BOOT_SRC := boot.asm
LINKER_SCRIPT := linker.ld
GRUB_CFG := grub.cfg

# Объектные файлы
OBJS := boot.o kernel.o

# Директории
ISO_DIR := isodir
BOOT_DIR := $(ISO_DIR)/boot
GRUB_DIR := $(BOOT_DIR)/grub

# Цель по умолчанию
all: $(ISOFILE)

# Сборка ISO
$(ISOFILE): $(BINARY) $(GRUB_CFG)
	mkdir -p $(GRUB_DIR)
	cp $(BINARY) $(BOOT_DIR)/$(BINARY)
	cp $(GRUB_CFG) $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $(ISOFILE) $(ISO_DIR)

# Сборка бинарного файла
$(BINARY): $(OBJS) $(LINKER_SCRIPT)
	$(CC) -T $(LINKER_SCRIPT) -o $(BINARY) $(LDFLAGS) $(OBJS) -lgcc
	grub-file --is-x86-multiboot $(BINARY)

# Сборка объектных файлов
boot.o: $(BOOT_SRC)
	$(AS) $(BOOT_SRC) -o boot.o

kernel.o: $(KERNEL_SRC)
	$(CC) -c $(KERNEL_SRC) -o kernel.o $(CFLAGS)

# Очистка
clean:
	rm -rf $(OBJS) $(BINARY) $(ISOFILE) $(ISO_DIR)

.PHONY: all clean
