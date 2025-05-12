#ifndef BUTTONS_H
#define BUTTONS_H

enum Button { UP, DOWN, LEFT, RIGHT, CENTER, A, B, C, NONE };

void initializeButtons();

Button readButton();

#endif