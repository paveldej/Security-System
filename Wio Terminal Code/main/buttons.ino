#include "buttons.h"

void initializeButtons() {
    pinMode(WIO_5S_UP, INPUT_PULLUP);
    pinMode(WIO_5S_DOWN, INPUT_PULLUP);
    pinMode(WIO_5S_LEFT, INPUT_PULLUP);
    pinMode(WIO_5S_RIGHT, INPUT_PULLUP);
    pinMode(WIO_5S_PRESS, INPUT_PULLUP);
    pinMode(WIO_KEY_A, INPUT_PULLUP);
    pinMode(WIO_KEY_B, INPUT_PULLUP);
    pinMode(WIO_KEY_C, INPUT_PULLUP);
}

Button readButton() { 
    if (digitalRead(WIO_5S_UP) == LOW) return UP;
    if (digitalRead(WIO_5S_DOWN) == LOW) return DOWN;
    if (digitalRead(WIO_5S_LEFT) == LOW) return LEFT;
    if (digitalRead(WIO_5S_RIGHT) == LOW) return RIGHT;
    if (digitalRead(WIO_5S_PRESS) == LOW) return CENTER;
    if (digitalRead(WIO_KEY_A) == LOW) return A;
    if (digitalRead(WIO_KEY_B) == LOW) return B;
    if (digitalRead(WIO_KEY_C) == LOW) return C;
    return NONE;
}
