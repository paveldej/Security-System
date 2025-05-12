#include <TFT_eSPI.h>
#include "TerminalDisplay.h"

namespace TerminalDisplay {
    //these are the specs of the WIO seed terminal
    #define SCREEN_WIDTH 320
    #define SCREEN_HEIGHT 240
    #define TEXT_SIZE 3

    TFT_eSPI tft = TFT_eSPI();
    TFT_eSprite spr = TFT_eSprite(&tft);
    
    void initDisplay(){
        tft.setRotation(3);
        tft.fillScreen(TFT_BLACK);

        spr.createSprite(SCREEN_WIDTH, SCREEN_HEIGHT);
        spr.setTextSize(TEXT_SIZE);
    }
    //displays arming status
    void displayStatus(bool armed) {

        String status = "Armed";

        //sets armed or disarmed with differing colors
        if (armed){
            status = "Armed";
            spr.setTextColor(TFT_RED);
        } else {
            status = "Disarmed";
            spr.setTextColor(TFT_GREEN);
        }

        //displays string centered, pushes sprite to top left corner (0,0)
        spr.drawString(status, SCREEN_WIDTH/2, SCREEN_HEIGHT/2);
        spr.pushSprite(0, 0);
        spr.deleteSprite();
    }
}