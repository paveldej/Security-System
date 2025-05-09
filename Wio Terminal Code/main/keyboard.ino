#include "keyboard.h"
#include <TFT_eSPI.h>

extern TFT_eSPI tft; // Use from main file

KeyboardMode kbMode = UPPER;
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

void drawKeyboard() {
  const char** keyboard = getKeyboardLayout();
  tft.fillRect(0, 80, 320, 160, TFT_BLACK);
  tft.setTextSize(2);

  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 6; c++) {
      int x = c * 50 + 10;
      int y = r * 30 + 90;
      char ch = keyboard[r][c];

      if (r == row && c == col) {
        tft.fillRect(x - 5, y - 5, 40, 30, TFT_BLUE);
        tft.setTextColor(TFT_WHITE, TFT_BLUE);
      } else {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
      }

      tft.setCursor(x, y);
      tft.print(ch);
    }
  }

  tft.setTextColor(TFT_CYAN);
  tft.setCursor(10, 250);
  tft.setTextSize(2);
  tft.print("[B] Mode  ");
  tft.print("[A] Connect");
}

void drawPassword() {
  tft.fillRect(0, 0, 320, 80, TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 10);
  tft.setTextSize(2);
  tft.print("PWD: ");
  tft.print(passwordInput);
}
