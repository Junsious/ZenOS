# Указываем имя выходного файла
OSNAME := ZenOS
ISOFILE := $(OSNAME).iso
BINARY := $(OSNAME).bin

# Указываем компиляторы и ассемблер
AS := i386-elf-as
CC := i386-elf-gcc

# Флаги для компилятора и линковщика
CFLAGS := -std=gnu99 -ffreestanding -O2 -Wall -Wextra
LDFLAGS := -ffreestanding -O2 -nostdlib

# Файлы
KERNEL_C_SRC := kernel.c
LIB_C_SRC := lib.c
BOOT_SRC := boot.asm
LINKER_SCRIPT := linker.ld
GRUB_CFG := grub.cfg

# Объектные файлы
OBJS := boot.o kernel.o lib.o

# Директории
ISO_DIR := isodir
BOOT_DIR := $(ISO_DIR)/boot
GRUB_DIR := $(BOOT_DIR)/grub

# Прогресс-бар цвета
define print_progress
	@printf "\033[1;32m[ %-20s ]\033[0m\r" $(1)
endef

# Цель по умолчанию
all: $(ISOFILE)

# Сборка ISO
$(ISOFILE): $(BINARY) $(GRUB_CFG)
	@$(call print_progress, "ISO Creation")
	mkdir -p $(GRUB_DIR)
	cp $(BINARY) $(BOOT_DIR)/$(BINARY)
	cp $(GRUB_CFG) $(GRUB_DIR)/grub.cfg
	grub-mkrescue -o $(ISOFILE) $(ISO_DIR)
	@echo "ISO created successfully"

# Сборка бинарного файла
$(BINARY): $(OBJS) $(LINKER_SCRIPT)
	@$(call print_progress, "Linking")
	$(CC) -T $(LINKER_SCRIPT) -o $(BINARY) $(LDFLAGS) $(OBJS) -lgcc
	grub-file --is-x86-multiboot $(BINARY)
	@echo "Binary created successfully"

# Сборка объектных файлов
boot.o: $(BOOT_SRC)
	@$(call print_progress, "Assembling boot.asm")
	$(AS) $(BOOT_SRC) -o boot.o

kernel.o: $(KERNEL_C_SRC)
	@$(call print_progress, "Compiling kernel.c")
	$(CC) -c $(KERNEL_C_SRC) -o kernel.o $(CFLAGS)

lib.o: $(LIB_C_SRC)
	@$(call print_progress, "Compiling lib.c")
	$(CC) -c $(LIB_C_SRC) -o lib.o $(CFLAGS)

# Очистка
clean:
	@echo "Cleaning up..."
	rm -rf $(OBJS) $(BINARY) $(ISOFILE) $(ISO_DIR)
	@echo "Cleaned successfully"

.PHONY: all clean
