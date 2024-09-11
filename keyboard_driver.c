#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>

#define KEYBOARD_DATA_PORT 0x60
#define KEYBOARD_STATUS_PORT 0x64

static const char keyboard_map[256] = {
    0, 27, '1', '2', '3', '4', '5', '6', '7', '8', '9', '0', '-', '=', '\b', '\t',
    'q', 'w', 'e', 'r', 't', 'y', 'u', 'i', 'o', 'p', '[', ']', '\n', 0,
    'a', 's', 'd', 'f', 'g', 'h', 'j', 'k', 'l', ';', '\'', '`', 0,
    '\\', 'z', 'x', 'c', 'v', 'b', 'n', 'm', ',', '.', '/', 0,
    '*', 0, ' ', 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
};

// Модификаторы клавиш
#define SHIFT_LEFT 0x2A
#define SHIFT_RIGHT 0x36
#define CTRL_LEFT 0x1D
#define CTRL_RIGHT 0x1E
#define ALT_LEFT 0x38
#define ALT_RIGHT 0x39

static bool shift_pressed = false;
static bool ctrl_pressed = false;
static bool alt_pressed = false;

static inline void outb(uint16_t port, uint8_t value) {
    asm volatile ("outb %0, %1" : : "a"(value), "Nd"(port));
}

static inline uint8_t inb(uint16_t port) {
    uint8_t ret;
    asm volatile ("inb %1, %0" : "=a"(ret) : "Nd"(port));
    return ret;
}

char keyboard_read(void) {
    uint8_t scan_code;
    char ch = 0;

    while ((inb(KEYBOARD_STATUS_PORT) & 0x01) == 0) {
        // Ожидание, пока в порту появится новый символ
    }

    scan_code = inb(KEYBOARD_DATA_PORT);

    // Обработка модификаторов
    switch (scan_code) {
        case SHIFT_LEFT:
        case SHIFT_RIGHT:
            shift_pressed = true;
            break;
        case CTRL_LEFT:
        case CTRL_RIGHT:
            ctrl_pressed = true;
            break;
        case ALT_LEFT:
        case ALT_RIGHT:
            alt_pressed = true;
            break;
        case 0xAA: // Left Shift release
        case 0xB6: // Right Shift release
            shift_pressed = false;
            break;
        case 0x9D: // Left Ctrl release
        case 0x9E: // Right Ctrl release
            ctrl_pressed = false;
            break;
        case 0xB8: // Left Alt release
        case 0xBA: // Right Alt release
            alt_pressed = false;
            break;
        default:
            // Преобразование в символ при условии, что это правильный скан-код
            ch = keyboard_map[scan_code];
            if (shift_pressed) {
                // Преобразование в верхний регистр или спецсимволы
                if (ch >= 'a' && ch <= 'z') {
                    ch -= 32; // Преобразование в верхний регистр
                }
            }
            break;
    }

    return ch;
}
void keyboard_initialize(void) {
    // Сброс клавиатурного контроллера
    outb(KEYBOARD_STATUS_PORT, 0xFF);
}