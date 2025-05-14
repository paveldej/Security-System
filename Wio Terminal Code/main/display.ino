#include "display.h"
#include "buttons.h"
#include "keyboard.h"
#include "rpcWiFi.h"

extern PubSubClient client;
extern bool armed;
extern byte batteryLevel;
extern String pinInput;
extern const String pin;

TFT_eSPI tft;
// current screen frame to display
ScreenState screen = MAIN_MENU;

/**
* needed to map main menu options to appropriate screen frames
*/
const std::map<String, ScreenState> optionToScreen = {
  {"connect to wifi", WIFI_LIST},
  {"arm/disarm system", PIN_ENTRY}
};

//for main menu
std::vector<String> mainMenuOptions = {"connect to wifi", "arm/disarm system"};
int selectedMainOption = 0;
int mainOptionCount = mainMenuOptions.size();

//for wifi list menu
std::vector<String> ssids;
int selectedSSID = 0;
int ssidCount = 0;

bool prevStatus = armed;
bool prevIsConnected = isOnline();

extern const char** keyboard;
extern const char* keyboardPinEntry[];

void initializeDisplay() {
  tft.begin();
  tft.setRotation(3);
  tft.fillScreen(TFT_BLACK);
}

void drawMainMenu(const std::vector<String>& mainMenuOptions, int selected) {
  // you can paste your code to display general information here:
  // Empty space available from 0 to 160th pixel.
  tft.setTextSize(2);
  tft.fillScreen(TFT_BLACK);

  tft.setTextColor(TFT_WHITE);

  tft.setCursor(10, 180);
  tft.println("Choose option:");
  tft.fillRect(0, 210, 320, 30, TFT_YELLOW);
  tft.setCursor(10, 220);

  tft.setTextColor(TFT_BLACK);
  tft.print("<< "+ mainMenuOptions[selected] + " >>");
}


void drawWiFiList(const std::vector<String>& ssids, int selected) {
  const int itemsPerPage = 7;
  int page = selected / itemsPerPage;
  int start = page * itemsPerPage;
  int end = min(start + itemsPerPage, ssids.size());
  int totalPages = (ssids.size() + itemsPerPage - 1) / itemsPerPage;

  tft.fillScreen(TFT_BLACK);
  tft.fillRect(0,0,320,25, TFT_GREEN);
  tft.setTextSize(2);
  tft.setCursor(10, 5);
  tft.setTextColor(TFT_BLACK);
  tft.println("Select Wi-Fi:       " + String(page + 1) + "/" + String(totalPages));
  tft.setTextColor(TFT_WHITE);

  for (int i = start; i < end; i++) {
    int displayIndex = i - start;
    int y = 30 + displayIndex * 30;
    
    if (i == selected) {
      tft.fillRect(0, y, 320, 30, TFT_YELLOW);
      tft.setTextColor(TFT_BLACK);
    } else {
      tft.setTextColor(TFT_WHITE);
    }

    tft.setCursor(10, y + 5);
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
  tft.fillRect(0, 195, 320, 240, TFT_GREEN);
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
  tft.setCursor(10,200);
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.print("\"UPRIGHT\"- connect");
  tft.setCursor(10,220);
  tft.print("\"UPMID\"- change layout");
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

void drawFailed(const String& ssid) {
  tft.fillScreen(TFT_RED);
  tft.setCursor(10, 100);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.print("Fail to Connect" + ssid);
}

void drawPin(const String& pin) {
  tft.fillRect(0, 0, 320, 40, TFT_DARKGREY);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 10);
  tft.setTextSize(2);
  tft.print("PIN: " + pin);
}

void drawPinEntry(int row, int col) {
  tft.fillRect(0, 40, 320, 200, TFT_BLACK);
  tft.fillRect(0, 195, 320, 240, TFT_GREEN);
  for (int r = 0; r < 4; r++) {
    for (int c = 0; c < 3; c++) {
      int x = c * 60 + 70;
      int y = r * 35 + 50;
      char ch = keyboardPinEntry[r][c];
      if (r == row && c == col) {
        tft.fillRect(x - 25, y - 10, 60, 35, TFT_BLUE);
        tft.setTextColor(TFT_WHITE, TFT_BLUE);
      } else {
        tft.setTextColor(TFT_WHITE, TFT_BLACK);
      }
      tft.setCursor(x, y);
      tft.print(ch);
    }
  } 
  tft.setCursor(10,200);
  tft.setTextColor(TFT_BLACK, TFT_GREEN);
  tft.print("\"UPRIGHT\"- confirm");
}

void drawWrongPin() {
  tft.fillScreen(TFT_RED);
  tft.setCursor(10, 100);
  tft.setTextColor(TFT_BLACK);
  tft.setTextSize(2);
  tft.print("Incorrect PIN!\nPlease try again");
}

/**
 * Handles the current screen frame based on the screen state.
 * Maps the given screen state to the appropriate handler function and executes it.
 */
void handleScreen(ScreenState screen) {
  if (screenHandlers.find(screen) != screenHandlers.end()) {
    screenHandlers[screen]();
  }
}

void handleMainMenu() {
  drawConnectionStatus();
  drawStatus(armed);
  drawBatteryLevel(batteryLevel);
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
    screen = optionToScreen.at(mainMenuOptions[selectedMainOption]);
    delay(200);
    if (screen == WIFI_LIST) {
      scanNetworks();
      drawWiFiList(ssids, selectedSSID);
    } else if(screen == PIN_ENTRY) {
      drawPin(pinInput);
      drawPinEntry(pinRow, pinCol);
    }
  }
}

void handleWiFiList() {
  if (readButton() == UP) {
    selectedSSID = (selectedSSID - 1 + ssidCount) % ssidCount;
    drawWiFiList(ssids, selectedSSID);
    delay(100);
  }
  if (readButton() == DOWN) {
    selectedSSID = (selectedSSID + 1) % ssidCount;
    drawWiFiList(ssids, selectedSSID);
    delay(100);
  }
  if (readButton() == CENTER) {
    screen = PASSWORD_ENTRY;
    drawKeyboard(keyboard,row,col);
    drawPassword(passwordInput);
    delay(200);
  }

  if (readButton() == C) {
    screen = MAIN_MENU;
    drawMainMenu(mainMenuOptions, selectedMainOption);
    delay(200);
  }
}

// Creates a list with available wifi ssid's
void scanNetworks() {
  tft.fillScreen(TFT_BLACK);
  tft.setCursor(10, 10);
  tft.setTextColor(TFT_WHITE);
  tft.setTextSize(2);
  tft.println("Scanning Wi-Fi...");

  ssids.clear();

  int found = WiFi.scanNetworks();
  for (int i = 0; i < found && ssids.size() < 20; i++) {
    String ssid = WiFi.SSID(i);
    if (ssid.length() == 0) continue;

    bool duplicate = false;
    for (const auto& existing : ssids) {
      if (existing == ssid) {
        duplicate = true;
        break;
      }
    }
    if (!duplicate) {
      ssids.push_back(ssid);
      Serial.println(ssid);
    }
  }

  ssidCount = ssids.size();
}


void handlePasswordEntry() {
  if (readButton() == UP) {
    row = (row + 4) % 5;
    drawKeyboard(keyboard,row,col);
    delay(100);
  }
  if (readButton() == DOWN) {
    row = (row + 1) % 5;
    drawKeyboard(keyboard,row,col);
    delay(100);
  }
  if (readButton() == LEFT) {
    col = (col + 5) % 6;
    drawKeyboard(keyboard,row,col);
    delay(100);
  }
  if (readButton() == RIGHT) {
    col = (col + 1) % 6;
    drawKeyboard(keyboard,row,col);
    delay(100);
  }

  if (readButton() == CENTER) {
    char c = keyboard[row][col];
    if (c == '<') {
      if (passwordInput.length() > 0) passwordInput.remove(passwordInput.length() - 1);
    } else if (c != ' ') {
      passwordInput += c;
    }
    drawPassword(passwordInput);
    delay(200);
  }

  if (readButton() == A) {
    screen = CONNECTING;
    drawConnecting(ssids[selectedSSID]);
    delay(200);
  }

  if (readButton() == B) {
    switchKeyboardLayout();
    drawKeyboard(keyboard,row,col);
    delay(200);
  }

  if (readButton() == C) {
    screen = WIFI_LIST;
    drawWiFiList(ssids, selectedSSID);
    delay(200);
  }
}

void handleConnecting() {
  connectToWiFi();
  if (WiFi.isConnected()) {
    screen = CONNECTED;
    drawConnected(ssids[selectedSSID]);
  } else {
    screen = FAILED;
    drawFailed(ssids[selectedSSID]);
  }
  delay(2000);
}

void handleConnected() {
  screen = MAIN_MENU;
  drawMainMenu(mainMenuOptions, selectedMainOption);
}

void handleConnectionFailed() {
  screen = MAIN_MENU;
  drawMainMenu(mainMenuOptions, selectedMainOption);
}

void handlePinEntry() {
  if (readButton() == UP) {
    pinRow = (pinRow + 3) % 4;
    drawPinEntry(pinRow,pinCol);
    delay(100);
  }
  if (readButton() == DOWN) {
    pinRow = (pinRow + 1) % 4;
    drawPinEntry(pinRow,pinCol);
    delay(100);
  }
  if (readButton() == LEFT) {
    pinCol = (pinCol + 5) % 3;
    drawPinEntry(pinRow,pinCol);
    delay(100);
  }
  if (readButton() == RIGHT) {
    pinCol = (pinCol + 1) % 3;
    drawPinEntry(pinRow,pinCol);
    delay(100);
  }

  if (readButton() == CENTER) {
    char c = keyboardPinEntry[pinRow][pinCol];
    if (c == '<') {
      if (pinInput.length() > 0) pinInput.remove(pinInput.length() - 1);
    } else if (c != ' ') {
      pinInput += c;
    }
    drawPin(pinInput);
    delay(200);
  }

  if (readButton() == A) {
    if (isPinCorrect()) {
      armed = toggleState(armed);
      if (WiFi.isConnected() && client.connected()) {
        updateStatus();
      }
      screen = MAIN_MENU;
      drawMainMenu(mainMenuOptions, selectedMainOption);
    } else {
      screen = WRONG_PIN;
      drawWrongPin();
    }
    pinInput = "";
    delay(200);
  }

  if (readButton() == C) {
    screen = MAIN_MENU;
    drawMainMenu(mainMenuOptions, selectedMainOption);
    delay(200);
  }
}

void handleWrongPin() {
  delay(2000);
  screen = MAIN_MENU;
  drawMainMenu(mainMenuOptions, selectedMainOption);
}

bool isPinCorrect() {
  return pin == pinInput ? true : false;
}

bool toggleState(bool armed) {
  return armed == true ? false : true;
}


void drawStatus(bool armed) {
  if (prevStatus != armed) {
    tft.fillRect(20,20,320,60,TFT_BLACK);
    prevStatus = armed;
  }
  String status = "Armed";
  //sets armed or disarmed with differing colors
  if (armed){
    status = "Armed";
    tft.setTextColor(TFT_RED, TFT_BLACK);
  } else {
    status = "Disarmed";
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
  }
  tft.setTextSize(2);
  tft.drawString(status, 20, 40);
}

void drawConnectionStatus() {
  bool isConnected = isOnline();
  if (prevIsConnected != isConnected) {
    tft.fillRect(20,0,320,40,TFT_BLACK);
    prevIsConnected = isConnected;
  }
  tft.setTextSize(2);
  if (isConnected) {
    tft.setTextColor(TFT_GREEN, TFT_BLACK);
    tft.drawString("Online", 20, 5);
  } else {
    tft.setTextColor(TFT_RED, TFT_BLACK);
    tft.drawString("Offline", 20, 5);
  }
}

bool isOnline() {
  return client.connected() && WiFi.isConnected() ? true : false; 
}

void drawBatteryLevel(byte bateryLevel){
      if (bateryLevel<20){
      tft.setTextColor(TFT_RED, TFT_BLACK);
    } else {
      tft.setTextColor(TFT_GREEN, TFT_BLACK);
    }
    tft.setTextSize(2);
    String text = "Batery: " + String(bateryLevel);
    tft.drawString(text, 20, 70);
}


