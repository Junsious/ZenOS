# Makefile

# Установите переменные для вашего кросс-компилятора
AS = nasm
CC = x86_64-elf-gcc
LD = x86_64-elf-gcc
GRUB = grub-mkrescue

# Файлы
ASM_SRC = boot.asm
ASM_OBJ = boot.o
C_SRC = kernel.c keyboard_driver.c
C_OBJ = kernel.o keyboard_driver.o
LINKER_SCRIPT = linker.ld
OUTPUT_BIN = ZenOS.bin
ISO_DIR = isodir
ISO_IMG = ZenOS.iso

# Цели сборки
all: $(ISO_IMG)

# Сборка ассемблерного кода
$(ASM_OBJ): $(ASM_SRC)
	$(AS) -f elf64 $(ASM_SRC) -o $(ASM_OBJ)

# Сборка C-кода
$(C_OBJ): $(C_SRC)
	$(CC) -c -std=gnu99 -ffreestanding -O2 -Wall -Wextra $(C_SRC)

# Линковка
$(OUTPUT_BIN): $(ASM_OBJ) $(C_OBJ)
	$(LD) -T $(LINKER_SCRIPT) -ffreestanding -O2 -nostdlib -lgcc -o $(OUTPUT_BIN) $(ASM_OBJ) $(C_OBJ) -e _start

# Создание ISO-образа
$(ISO_IMG): $(OUTPUT_BIN)
	mkdir -p $(ISO_DIR)/boot/grub
	cp $(OUTPUT_BIN) $(ISO_DIR)/boot/ZenOS.bin
	cp grub.cfg $(ISO_DIR)/boot/grub/grub.cfg
	$(GRUB) -o $(ISO_IMG) $(ISO_DIR)

# Очистка
clean:
	rm -f $(ASM_OBJ) $(C_OBJ) $(OUTPUT_BIN) $(ISO_IMG)
	rm -rf $(ISO_DIR)
