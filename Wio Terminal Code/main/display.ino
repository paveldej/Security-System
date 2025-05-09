#include "display.h"
#include "buttons.h"
#include "rpcWiFi.h"

TFT_eSPI tft;
ScreenState screen = MAIN_MENU;

const std::map<String, ScreenState> optionToScreen = {
  {"connect to wifi", WIFI_LIST},
  {"connect", PASSWORD_ENTRY}
};

//for main menu
std::vector<String> mainMenuOptions = {"connect to wifi","new option", "one more"};
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
  const int itemsPerPage = 7;
  int page = selected / itemsPerPage;
  int start = page * itemsPerPage;
  int end = min(start + itemsPerPage, ssids.size());
  int totalPages = (ssids.size() + itemsPerPage - 1) / itemsPerPage;

  tft.fillScreen(TFT_BLACK);
  tft.setTextSize(2);
  tft.setTextColor(TFT_WHITE);
  tft.setCursor(10, 5);
  tft.println("Select Wi-Fi:       " + String(page + 1) + "/" + String(totalPages));

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
    screen = optionToScreen.at(mainMenuOptions[selectedMainOption]);
    delay(200);
    if (screen == WIFI_LIST) {
      scanNetworks();
      drawWiFiList(ssids, selectedSSID);
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
    screen = optionToScreen.at("connect");
    delay(200);
  }
}

// Creates a list with all available wifi ssid's
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
  const char** keyboard = getKeyboardLayout();

  if (readButton() == UP) {
    row = (row + 4) % 5;
    drawKeyboard();
    delay(100);
  }
  if (readButton() == DOWN) {
    row = (row + 1) % 5;
    drawKeyboard();
    delay(100);
  }
  if (readButton() == LEFT) {
    col = (col + 5) % 6;
    drawKeyboard();
    delay(100);
  }
  if (readButton() == RIGHT) {
    col = (col + 1) % 6;
    drawKeyboard();
    delay(100);
  }

  if (readButton() == CENTER) {
    char c = keyboard[row][col];
    if (c == '<') {
      if (passwordInput.length() > 0) passwordInput.remove(passwordInput.length() - 1);
    } else if (c != ' ') {
      passwordInput += c;
    }
    drawPassword();
    delay(200);
  }

  if (readButton() == A) {
    screen = CONNECTING;
    drawConnecting();
    connectToWiFi();
    delay(200);
  }

  if (readButton() == B) {
    kbMode = static_cast<KeyboardMode>((kbMode + 1) % 4);
    drawKeyboard();
    delay(200);
  }
}

void handleConnecting() {

}

void handleConnected() {

}

void handleConnectionFailed() {

}




