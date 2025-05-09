#include <TFT_eSPI.h>
#include "TerminalDisplay.h"

namespace TerminalDisplay {
    //these are the specs of the WIO seed terminal
    #define SCREEN_WIDTH 320
    #define SCREEN_HEIGHT 240
    #define TEXT_SIZE 3

    TFT_eSPI tft = TFT_eSPI();
    TFT_eSprite spr = TFT_eSprite(&tft);
    tft.setRotation(3);
    tft.fillScreen(TFT_BLACK);

    sprite.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
    sprite.setTextSize(TEXT_SIZE);
    //displays arming status
    void displayStatus(bool armed) {

        String status = "Armed";

        //sets armed or disarmed with differing colors
        if (armed){
            status = "Armed";
            sprite.setTextColor(TFT_RED);
        } else {
            status = "Disarmed"
            sprite.setTextColor(TFT_GREEN);
        }

        //displays string centered, pushes sprite to top left corner (0,0)
        sprite.drawString(status, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        sprite.pushSprite(0, 0);
        sprite.deleteSprite();
    }
}