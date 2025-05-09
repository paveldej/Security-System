#include "keyboard.h"
#include <TFT_eSPI.h>

extern TFT_eSPI tft;

KeyboardMode kbMode = UPPER;
//set of keyboard characters to display 
const char** keyboard = getKeyboardLayout();
String passwordInput = "";
int row = 0, col = 0;

// Keyboard layouts
const char* keyboardUpper[] = {"ABCDEF", "GHIJKL", "MNOPQR", "STUVWX", "YZ <- "};
const char* keyboardLower[] = {"abcdef", "ghijkl", "mnopqr", "stuvwx", "yz <- "};
const char* keyboardDigits[] = {"123456", "7890+-", "!@#$%^", "&*()[]", "{}_=< "};
const char* keyboardSymbols[] = {":;\"',.", "/?|\\~` ", "<>-+= ", "      ", "     <-"};

const char** getKeyboardLayout() {
  switch (kbMode) {
    case UPPER: return keyboardUpper;
    case LOWER: return keyboardLower;
    case DIGITS: return keyboardDigits;
    case SYMBOLS: return keyboardSymbols;
  }
  return keyboardUpper;
}

void switchKeyboardLayout() {
  kbMode = static_cast<KeyboardMode>((kbMode + 1) % 4);
  keyboard = getKeyboardLayout();
}
