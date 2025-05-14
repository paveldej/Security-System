#ifndef KEYBOARD_H
#define KEYBOARD_H

// Enums, constants
enum KeyboardMode { UPPER, LOWER, DIGITS, SYMBOLS };
extern KeyboardMode kbMode;

extern String passwordInput;
extern int row, col;
extern int pinRow, pinCol;

const char** getKeyboardLayout();

void switchKeyboardLayout();


#endif
