#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include "keyboard_driver.h"

// Глобальные переменные
static size_t terminal_row;
static size_t terminal_column;
static uint8_t terminal_color;
static uint16_t* terminal_buffer;

enum vga_color {
    VGA_COLOR_BLACK = 0,
    VGA_COLOR_BLUE = 1,
    VGA_COLOR_GREEN = 2,
    VGA_COLOR_CYAN = 3,
    VGA_COLOR_RED = 4,
    VGA_COLOR_MAGENTA = 5,
    VGA_COLOR_BROWN = 6,
    VGA_COLOR_LIGHT_GREY = 7,
    VGA_COLOR_DARK_GREY = 8,
    VGA_COLOR_LIGHT_BLUE = 9,
    VGA_COLOR_LIGHT_GREEN = 10,
    VGA_COLOR_LIGHT_CYAN = 11,
    VGA_COLOR_LIGHT_RED = 12,
    VGA_COLOR_LIGHT_MAGENTA = 13,
    VGA_COLOR_LIGHT_BROWN = 14,
    VGA_COLOR_WHITE = 15,
};

static const size_t VGA_WIDTH = 80;
static const size_t VGA_HEIGHT = 25;

static inline uint8_t vga_entry_color(enum vga_color fg, enum vga_color bg) {
    return fg | (bg << 4);
}

static inline uint16_t vga_entry(unsigned char uc, uint8_t color) {
    return (uint16_t)uc | ((uint16_t)color << 8);
}

static size_t strlen(const char* str) {
    size_t len = 0;
    while (str[len])
        len++;
    return len;
}

static int strcmp(const char* str1, const char* str2) {
    while (*str1 && (*str1 == *str2)) {
        str1++;
        str2++;
    }
    return *(unsigned char*)str1 - *(unsigned char*)str2;
}
static void terminal_putentryat(char c, uint8_t color, size_t x, size_t y) {
    const size_t index = y * VGA_WIDTH + x;
    terminal_buffer[index] = vga_entry(c, color);
}

static void terminal_initialize(void) {
    terminal_row = 0;
    terminal_column = 0;
    terminal_color = vga_entry_color(VGA_COLOR_LIGHT_GREY, VGA_COLOR_BLACK);
    terminal_buffer = (uint16_t*)0xB8000;  // Адрес видеопамяти

    for (size_t y = 0; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_putentryat(' ', terminal_color, x, y);
        }
    }
}

static void terminal_putchar(char c) {
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_row = 0;  // Перенос строки, если вышли за пределы экрана
        }
    } else {
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        terminal_column++;  // Обновляем столбец после вставки символа
        if (terminal_column == VGA_WIDTH) {  // Проверяем, не достигли ли конца строки
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {  // Проверяем, не достигли ли конца экрана
                terminal_row = 0;
            }
        }
    }
}


static void terminal_writestring(const char* data) {
    size_t len = strlen(data);
    for (size_t i = 0; i < len; i++) {
        terminal_putchar(data[i]);
    }
}

static void handle_input(char* input) {
    if (strcmp(input, "!help") == 0) {
        terminal_writestring("Available commands:\n");
        terminal_writestring("!help: Display this help message\n");
    } else {
        terminal_writestring("Unknown command: ");
        terminal_writestring(input);
        terminal_putchar('\n');
    }
}

static void terminal_backspace(void) {
    if (terminal_column > 0) {
        terminal_column--;
    } else if (terminal_row > 0) {
        terminal_row--;
        terminal_column = VGA_WIDTH - 1;
    }
    terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);  // Удаление символа
}

static void terminal_readstring(char* buffer, size_t max_length) {
    size_t length = 0;
    char c = ' ';

    while (length < max_length - 1) {
        c = keyboard_read();  // Считываем символ с клавиатуры

        if (c == '\n') {
            terminal_putchar('\n');
            break;
        } else if (c == '\b') {  // Обработка backspace
            if (length > 0) {
                length--;
                terminal_backspace();  // Стереть предыдущий символ
            }
        } else {
            buffer[length++] = c;
            terminal_putchar(c);
        }
    }
    buffer[length] = '\0';  // Завершение строки
}

static void set_prompt_color(void) {
    uint8_t old_color = terminal_color;
    terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);  // Установка зелёного цвета для подсказки
    terminal_writestring("ZenOS> ");
    terminal_color = old_color;  // Возврат к старому цвету
}

void kernel_main(void) {
    terminal_initialize();

    while (true) {
        set_prompt_color();

        char input[256];
        terminal_readstring(input, sizeof(input));
        handle_input(input);
    }
}
