#ifndef DISPLAY_H
#define DISPLAY_H

#include <TFT_eSPI.h>
#include <vector>
#include <map>

// Forward declaration of shared TFT object
extern TFT_eSPI tft;
enum ScreenState { MAIN_MENU, WIFI_LIST, PASSWORD_ENTRY, CONNECTING, CONNECTED, FAILED, PIN_ENTRY, WRONG_PIN};

typedef void (*ScreenHandler)(); 

void initializeDisplay();

void handleScreen(ScreenState screen);
void handleMainMenu();
void handleWiFiList();
void handlePasswordEntry();
void handleConnecting();
void handleConnected();
void handleConnectionFailed();
void handlePinEntry();
void handleWrongPin();

void drawMainMenu(const std::vector<String>& mainMenuOptions, int selected);
void drawWiFiList(const std::vector<String>& ssids, int selected);
void drawPassword(const String& password);
void drawKeyboard(const char** layout, int row, int col);
void drawConnecting(const String& ssid);
void drawConnected(const String& ssid);
void drawFailed(const String& ssid);
void drawPin();
void drawPinEntry();
void drawWrongPin();
void drawStatus(bool armed);
void drawConnectionStatus();
void drawBatteryLevel(byte bateryLevel);

std::map<ScreenState, ScreenHandler> screenHandlers = {
  { MAIN_MENU, handleMainMenu },
  { WIFI_LIST, handleWiFiList },
  { PASSWORD_ENTRY, handlePasswordEntry },
  { CONNECTING, handleConnecting },
  { CONNECTED, handleConnected },
  { FAILED, handleConnectionFailed },
  { PIN_ENTRY, handlePinEntry},
  { WRONG_PIN, handleWrongPin}
};

#endif
