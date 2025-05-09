#ifndef KEYBOARD_H
#define KEYBOARD_H

// Enums, constants
enum KeyboardMode { UPPER, LOWER, DIGITS, SYMBOLS };
extern KeyboardMode kbMode;

const char** getKeyboardLayout();
void drawKeyboard();
void drawPassword();

extern String passwordInput;
extern int row, col;

#endif
