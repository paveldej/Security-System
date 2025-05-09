#include "display.h"
#include "buttons.h"

// extern std::vector<String> mainMenuOptions;
// extern int selectedMainOption;
// extern int mainOptionCount;
TFT_eSPI tft;

const std::map<String, ScreenState> optionToScreen = {
  {"connect to wifi", WIFI_LIST}
};

//for main menu
extern std::vector<String> mainMenuOptions;  // NO assignment here
int selectedMainOption = 0;
int mainOptionCount = mainMenuOptions.size();

//for wifi list menu
std::vector<String> ssids;
int selectedSSID = 0;
int ssidCount = 0;

void initializeDisplay() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
}

void drawMainMenu(const std::vector<String>& mainMenuOptions, int selected) {
  // you can paste your code to display general information here:
  // Empty space available from 0 to 160th pixel.
  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 180);
  tft.println("Choose option:");
  tft.fillRect(0, 210, 320, 30, TFT_YELLOW);
  tft.setCursor(10, 220);

  tft.setTextColor(TFT_BLACK);
  tft.print("<< "+ mainMenuOptions[selected] + " >>");
}

void drawWiFiList(const std::vector<String>& ssids, int selected) {
  tft.fillScreen(TFT_BLACK);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  for (int i = 0; i < ssids.size(); i++) {
    if (i == selected) tft.setTextColor(TFT_GREEN);
    else tft.setTextColor(TFT_WHITE);
    tft.setCursor(10, 30 + i * 30);
    tft.print(ssids[i]);
  }
}

void drawPassword(const String& password) {
  tft.fillRect(0, 0, 320, 40, TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 10);
  tft.setTextSize(2);
  tft.print("PWD: " + password);
}

void drawKeyboard(const char** layout, int row, int col) {
  tft.fillRect(0, 40, 320, 200, TFT_BLACK);
  for (int r = 0; r < 5; r++) {
    for (int c = 0; c < 6; c++) {
      int x = c * 50 + 10;
      int y = r * 30 + 50;
      char ch = layout[r][c];
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
}

void drawConnecting(const String& ssid) {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 100);
  tft.setTextColor(TFT_YELLOW);
  tft.setTextSize(2);
  tft.print("Connecting to: " + ssid);
}

void drawConnected(const String& ssid) {
  tft.fillScreen(TFT_GREEN);
  tft.setCursor(10, 100);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.print("Connected: " + ssid);
}

void handleScreen(ScreenState screen) {
  if (screenHandlers.find(screen) != screenHandlers.end()) {
    screenHandlers[screen]();
  }
}

void handleMainMenu() {
  if (readButton() == LEFT) {
    selectedMainOption = (selectedMainOption - 1 + mainOptionCount) % mainOptionCount;
    drawMainMenu(mainMenuOptions, selectedMainOption);
    delay(100);
  }
  if (readButton() == RIGHT) {
    selectedMainOption = (selectedMainOption + 1) % mainOptionCount;
    drawMainMenu(mainMenuOptions, selectedMainOption);
    delay(100);
  }
  if (readButton() == CENTER) {
    Serial.print("hello");
    delay(100);
  }
}

void handleWiFiList() {

}

void handlePasswordEntry() {

}

void handleConnecting() {

}

void handleConnected() {

}

void handleConnectionFailed() {

}




