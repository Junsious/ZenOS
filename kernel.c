#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

extern void terminal_initialize(void);
extern void terminal_writestring(const char* data);
extern void terminal_readstring(char* buffer, size_t max_length);
extern void handle_input(const char* input);

void kernel_main(void) {
    terminal_initialize();
    terminal_writestring("Welcome to ZenOS!\n");
    terminal_writestring("Press any key to continue...\n");

    // Ожидание нажатия клавиши
    char wait_key = ' ';
    while (wait_key == ' ') {
        // Здесь нужно добавить код для ожидания нажатия клавиши
        // Для примера, будем использовать заглушку
        wait_key = ' '; // Замените это на код для реального получения символа
    }

    terminal_writestring("ZenOS> ");
    char input_buffer[128];
    while (true) {
        terminal_readstring(input_buffer, sizeof(input_buffer));
        handle_input(input_buffer);
    }
}
