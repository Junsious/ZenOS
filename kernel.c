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
            terminal_putentryat(' ', terminal_color, x, y);  // Очищаем видеопамять пробелами
        }
    }
}

static void terminal_hidecursor(void) {
    terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
}

static void terminal_showcursor(void) {
    terminal_putentryat('_', terminal_color, terminal_column, terminal_row);
}

static void terminal_scroll(void) {
    for (size_t y = 1; y < VGA_HEIGHT; y++) {
        for (size_t x = 0; x < VGA_WIDTH; x++) {
            terminal_buffer[(y - 1) * VGA_WIDTH + x] = terminal_buffer[y * VGA_WIDTH + x];
        }
    }

    // Очищаем последнюю строку
    for (size_t x = 0; x < VGA_WIDTH; x++) {
        terminal_putentryat(' ', terminal_color, x, VGA_HEIGHT - 1);
    }

    terminal_row = VGA_HEIGHT - 1;
}

static void terminal_putchar(char c) {
    terminal_hidecursor();
    if (c == '\n') {
        terminal_column = 0;
        if (++terminal_row == VGA_HEIGHT) {
            terminal_scroll();  // Прокрутка экрана, если достигли конца
        }
    } else {
        if (terminal_column >= VGA_WIDTH) {
            terminal_column = 0;
            if (++terminal_row == VGA_HEIGHT) {
                terminal_scroll();  // Прокрутка экрана
            }
        }
        terminal_putentryat(c, terminal_color, terminal_column, terminal_row);
        terminal_column++;
    }
    terminal_showcursor();
}

static void terminal_writestring(const char* data) {
    size_t len = strlen(data);
    for (size_t i = 0; i < len; i++) {
        terminal_putchar(data[i]);
    }
}
static void delay() {
    for (volatile int i = 0; i < 1000000000; i++) {
        // Пустой цикл для задержки ~10 секунд
    }
}

static void terminal_backspace(void) {
    terminal_hidecursor();
    if (terminal_column > 0) {
        terminal_column--;
    } else if (terminal_row > 0) {
        terminal_row--;
        terminal_column = VGA_WIDTH - 1;
    }
    terminal_putentryat(' ', terminal_color, terminal_column, terminal_row);
    terminal_showcursor();
}


static void welcome_message(void) {
    terminal_writestring("Welcome To ZenOS\n");
    delay();
    terminal_initialize();
}
static void terminal_readstring(char* buffer, size_t max_length) {
    size_t length = 0;
    char c = ' ';

    while (length < max_length - 1) {
        c = keyboard_read();

        if (c == '\n') {
            terminal_putchar('\n');
            break;
        } else if (c == '\b') {
            if (length > 0) {
                length--;
                terminal_backspace();
            }
        } else if (c >= ' ') {  // Только отображаемые символы
            buffer[length++] = c;
            terminal_putchar(c);
        }
    }
    buffer[length] = '\0';  // Завершаем строку
}


static void handle_input(char* input) {
    if (strcmp(input, "!ZenOS") == 0) {
        terminal_writestring("                              -=====+=---::. :%@%##%@%+.            \n");
        terminal_writestring("                             +@@@@%*######%%%%#+++++**#%+           \n");
        terminal_writestring("                            :@@@@@%++++++++++++*%%%%#+++#%=         \n");
        terminal_writestring("                            *@@@@@%++++++++++#%@@**#@%*++*%#.       \n");
        terminal_writestring("           =*=            :=@@@@@@%+++++++++@@#*++++*%@#+++#%=      \n");
        terminal_writestring("        .*@@@@@+-     :+#@@@@@@@@@%+++++++++%@*+++++++*%%*++*@%:    \n");
        terminal_writestring("        #@@@@@@@@@#+#@@@@@@@@@@@@@%+++++++*##@@+++++++++*%#+++#@:   \n");
        terminal_writestring("         %@@@@@@@@@@@@@@@@@@@@@@@@%====++*@@#%@#+++++++++++++++*@.  \n");
        terminal_writestring("         .@@@@@@@@@@@@@@@@@@@@@@@@%======*@@+*@@++++++++++++++++%%  \n");
        terminal_writestring("          =@@@@@@@@@@@@@@@@@@@@@@@%======%@@*+%@*+++++++++++++++*@- \n");
        terminal_writestring("          -@@@@@@@@@@@@@@@@@@@@@@@%====*%@%@%=#@%++++++++++++++++%# \n");
        terminal_writestring("          #@@@@@@@@@@@@@@@@@@@@@@@%*#%%*#@#%@++@@+=++++++++++++++#@ \n");
        terminal_writestring("         +@@@@@@@@@@@@@@@@@@@@@@@@@@#:.=@%#%@+=#@+==+++++++++++++*@.\n");
        terminal_writestring("      .-#@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@%%%%@%==+@#===++++++++++++#% \n");
        terminal_writestring(" :+#%@@@@@@@@@@@@@@@@@@@@@@@@@%%@@@++*#####*====@%====++++++++++*@# \n");
        terminal_writestring("-@@@@@@@@@@@@@@@@@@@@@@@@@@@@#: -@@%=============%%=====++++++++*@* \n");
        terminal_writestring("+@@@@@@@@@@@@@@@@@@@@@@@@@@@#   -@@%=============%@=====++++++++*@: \n");
        terminal_writestring("*@@@@@@@@@@@@@@@@@@@@@@@@@@@=   -@@%======*%+====%@+====+++++++++#@:\n");
        terminal_writestring("*@@@@@@@@@@@@@@@@@@@@@@@@@@@*   -@@%======+@@====#@=====++++++++++@%\n");
        terminal_writestring("=@@@@@@@@@@@@@@@@@@@@@@@@@@@@+  -@@%=======%@*===%@=====++++++++++@*\n");
        terminal_writestring(" :-+#%@@@@@@@@@@@@@@@@@@@@@@@@@##@@%=======*@%==+@%====++++++++++%@-\n");
        terminal_writestring("      .-%@@@@@@@@@@@@@@@@@@@@@@@@@%=======*@%==*@*====++++++++*@%:  \n");
        terminal_writestring("         *@@@@@@@@@@@@@@@@@@@@@@@@%=======*@%=+@%+===++++++++@@-    \n");
        terminal_writestring("          #@@@@@@@@@@@@@@@@@@@@@@@%=======*@@#%@*===+++++++++@@.    \n");
        terminal_writestring("          -@@@@@@@@@@@@@@@@@@@@@@@%=======*@@@@#==+++++++++++#@+    \n");
        terminal_writestring("          :@@@@@@@@@@@@@@@@@@@@@@@%=======*@@@@*=+++++++++++++@@    \n");
        terminal_writestring("          %@@@@@@@@@@@@@@@@@@@@@@@%=======#@#*@@*+++++++++++++#@-   \n");
        terminal_writestring("         #@@@@@@@@@@@@@@@@@@@@@@@@%=======#@# *@@+++++++++**++#@-   \n");
        terminal_writestring("        *@@@@@@@@@@#%@@@@@@@@@@@@@%+++++++*@#  #@@%%@@%%#*#%*#@*    \n");
        terminal_writestring("        :#@@@@@@*-   :=*@@@@@@@@@@%++++++++@@#%%##**+***#@@@@%=     \n");
        terminal_writestring("          :*%+:          :-*@@@@@@%++++++++%@*+++++++++++*@+        \n");
        terminal_writestring("                            #@@@@@%++++++++*@@*+***+++++++@*        \n");
        terminal_writestring("                            -@@@@@%+++++++++*@@@@@@++++++#@=:.      \n");
        terminal_writestring("                             *@@@@@*++++++++++##%%*+++++#@@@%=      \n");
        terminal_writestring("                              =+++#%@%##*************##%@%*-        \n");
    } else if (strcmp(input, "!clear") == 0) {
        terminal_initialize();  // Очистка экрана
    } else if (strcmp(input, "!about") == 0) {
        terminal_writestring("ZenOS: A simple custom operating system.\n");
    } else if (strcmp(input, "!reboot") == 0) {
        terminal_writestring("Rebooting...\n");
        terminal_initialize();
        welcome_message();
    } else if (strcmp(input, "!date") == 0) {
        terminal_writestring("The current date is 01/01/2024.\n");  // Здесь можно заменить на реальную дату, если добавить часы реального времени (RTC).
    } else if (strcmp(input, "!help") == 0) {
        terminal_writestring("Available commands:\n");
        terminal_writestring("!ZenOS   - Display ZenOS logo\n");
        terminal_writestring("!clear   - Clear the screen\n");
        terminal_writestring("!about   - Display information about ZenOS\n");
        terminal_writestring("!reboot  - Reboot ZenOS (reset the environment)\n");
        terminal_writestring("!date    - Display the current date\n");
        terminal_writestring("!help    - Show this help message\n");
    } else if (strcmp(input, "!echo") == 0) {
        terminal_writestring("Echo mode is now active. Type something:\n");
        char buffer[256];
        terminal_readstring(buffer, sizeof(buffer));
        terminal_writestring("You entered: ");
        terminal_writestring(buffer);
        terminal_writestring("\n");
    } else if (strcmp(input, "!shutdown") == 0) {
        terminal_writestring("Shutting down the system...\n");
        // Остановка или перезагрузка системы, если это реализовано
        // Зависание системы после завершения команд
        while (true) {
            asm volatile("hlt");
        }
    } else {
        terminal_writestring("Unknown command. Type !help for a list of available commands.\n");
    }
}

static void set_prompt_color(void) {
    terminal_hidecursor();  // Скрываем курсор перед выводом строки
    uint8_t old_color = terminal_color;
    terminal_color = vga_entry_color(VGA_COLOR_GREEN, VGA_COLOR_BLACK);  // Установка зелёного цвета для подсказки
    terminal_writestring("ZenOS> ");
    terminal_color = old_color;  // Возврат к старому цвету
    terminal_showcursor();  // Отображаем курсор после строки
}

void kernel_main(void) {
    terminal_initialize();
    welcome_message();

    while (true) {
        set_prompt_color();

        char input[256];
        terminal_readstring(input, sizeof(input));

        if (strlen(input) > 0) {
            if (input[0] == '!') {
                handle_input(input);  // Обработка команд, начинающихся с "!"
            } else {
                terminal_writestring("\n");  // Просто перенос строки, если команда не начинается с "!"
            }
        }
    }
}
