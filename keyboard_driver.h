#ifndef KEYBOARD_DRIVER_H
#define KEYBOARD_DRIVER_H

#include <stddef.h>
#include <stdint.h>

// Инициализация клавиатуры
void keyboard_initialize(void);

// Считывание символов с клавиатуры
char keyboard_read(void);

#endif // KEYBOARD_DRIVER_H
