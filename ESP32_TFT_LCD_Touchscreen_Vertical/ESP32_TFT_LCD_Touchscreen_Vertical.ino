#include <SPI.h>
#include <Adafruit_GFX.h>

#include <Fonts/FreeMono9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSans12pt7b.h>
#include <Fonts/FreeSans18pt7b.h>
#include <Fonts/FreeSerifItalic24pt7b.h>

#include <Adafruit_ILI9341.h>  // or ST7735 depending on your display
#include <XPT2046_Touchscreen.h>
#include "WiFi.h"
#include <HTTPClient.h>

#include "variables.h"


void drawMainUI() {

  tft.fillScreen(ILI9341_BLACK);

  // tft.drawBitmap(120, 10, myBitmap_2, 64, 64, ILI9341_WHITE);

  tft.drawBitmap(60, 140, BITA_LOGO, 32, 32, ILI9341_WHITE);

  tft.setTextColor(ILI9341_WHITE);
  tft.setCursor(100, 160);
  tft.setFont(&FreeSans18pt7b);
  tft.setTextSize(1);
  tft.println("BITA");
}

void drawButtons() {
  tft.fillRect(40, 180, 55, 30, ILI9341_RED);
  tft.fillRect(90, 180, 55, 30, ILI9341_GREEN);

  write_heading("-", 50, 250, 2, ILI9341_WHITE);
  write_heading("+", 100, 250, 2, ILI9341_WHITE);
}

void adjustCFM() {
  if (cfm < 100) {
    cfm += 10;
  } else if (cfm >= 100) {
    cfm = 0;
  }
  if (cfm > 100) {
    cfm = 100;
  } else if (cfm < 0) {
    cfm = 0;
  }
  drawbox4();
  sendUpdate();
}
void toggleSeason() {
  seasonMode = (seasonMode == 0) ? 1 : 0;
  drawbox3();
  sendUpdate();
}

void toggleSwitch() {
  SwitchOn = !SwitchOn;
  drawbox2();
  sendUpdate();
}

void drawbox1() {
  tft.drawRoundRect(14, 44, 90, 120, 10, ILI9341_CYAN);
  tft.fillRoundRect(15, 45, 88, 118, 9, ILI9341_BLACK);
  write_heading(String(temp), 32, 100, 3, ILI9341_CYAN);
  write_small_text("o", 58, 88, 1, ILI9341_CYAN);
  write_heading("C", 66, 100, 3, ILI9341_CYAN);

  write_text(String(temperature), 32, 135, 3, ILI9341_CYAN);
  write_small_text("o", 55, 123, 1, ILI9341_CYAN);
  write_text("C", 68, 135, 3, ILI9341_CYAN);
}

void drawbox2() {

  if (SwitchOn) {
    tft.drawRoundRect(tft.width() / 2 + 4, 44, 90, 120, 10, ILI9341_GREEN);
    tft.fillRoundRect(tft.width() / 2 + 5, 45, 88, 118, 9, ILI9341_BLACK);
    write_text("ON", 155, 105, 2, ILI9341_GREEN);

  } else {
    tft.drawRoundRect(tft.width() / 2 + 4, 44, 90, 120, 10, ILI9341_RED);
    tft.fillRoundRect(tft.width() / 2 + 5, 45, 88, 118, 9, ILI9341_BLACK);
    write_text("OFF", 155, 105, 2, ILI9341_RED);
  }
}

void drawbox3() {
  if (seasonMode) {
    tft.drawRoundRect(14, 174, 90, 120, 10, ILI9341_CYAN);
    tft.fillRoundRect(15, 175, 88, 118, 9, ILI9341_BLACK);
    write_text("Winter", 35, 240, 2, ILI9341_CYAN);
  } else {
    tft.drawRoundRect(14, 174, 90, 120, 10, ILI9341_ORANGE);
    tft.fillRoundRect(15, 175, 88, 118, 9, ILI9341_BLACK);
    write_text("Summer", 25, 240, 2, ILI9341_ORANGE);
  }
}

void drawbox4() {
  tft.drawRoundRect(tft.width() / 2 + 4, 174, 90, 120, 10, ILI9341_YELLOW);
  tft.fillRoundRect(tft.width() / 2 + 5, 175, 88, 118, 9, ILI9341_BLACK);
  write_text(String(cfm) + "%", 150, 240, 2, ILI9341_YELLOW);
}

void draw_temp() {
  currentPage = "temp";

  Serial.println("Temp Page");
  tft.fillScreen(ILI9341_DARKBROWN);

  write_heading("Temperature", 100, 45, 2, ILI9341_LIGHTGREY);
  write_heading(String(temp), 145, 95, 4, ILI9341_CYAN);
  write_small_text("o", 178, 70, 1, ILI9341_CYAN);
  write_heading("C", 190, 95, 3, ILI9341_CYAN);


  write_heading("Setpoint", 120, 130, 2, ILI9341_LIGHTGREY);
  write_heading(String(temperature), 145, 175, 4, ILI9341_CYAN);
  write_small_text("o", 178, 150, 1, ILI9341_CYAN);
  write_heading("C", 190, 175, 3, ILI9341_CYAN);

  tft.fillRoundRect(10, 20, 70, 30, 5, ILI9341_DARKGREEN);
  write_text("Back", 20, 42, 2, ILI9341_WHITE);

  drawButtons();
}





void drawHomePage() {
  currentPage = "home";
  Serial.println("Home Page");
  // tft.fillScreen(ILI9341_BLACK);
  tft.fillScreen(ILI9341_DARKBROWN);

  tft.drawRoundRect(0, 0, tft.width(), 40, 10, ILI9341_BLUE);
  tft.fillRoundRect(1, 1, tft.width() - 2, 38, 9, ILI9341_DARKGREY);

  if (wifi_connected) {
    write_heading(device_name, 40, 30, 3, ILI9341_WHITE);
  } else {
    write_heading("ZONE MASTER", 30, 30, 3, ILI9341_WHITE);
  }
  drawbox1();
  drawbox2();
  drawbox3();
  drawbox4();
}


void write_heading(String text, int16_t pos_x, int16_t pos_y, uint8_t font_size, uint16_t color) {
  tft.setTextColor(color);
  tft.setCursor(pos_x, pos_y);
  tft.setFont(&FreeSans12pt7b);
  tft.setTextSize(1);
  tft.println(text);
}


void write_text(String text, int16_t pos_x, int16_t pos_y, uint8_t font_size, uint16_t color) {
  tft.setTextColor(color);
  tft.setCursor(pos_x, pos_y);
  tft.setFont(&FreeSans9pt7b);
  tft.setTextSize(1);
  tft.println(text);
}


void write_small_text(String text, int16_t pos_x, int16_t pos_y, uint8_t font_size, uint16_t color) {
  tft.setTextColor(color);
  tft.setCursor(pos_x, pos_y);
  tft.setFont(&FreeSans9pt7b);
  tft.setTextSize(1);
  tft.println(text);
}

void sendUpdate() {
  if (WiFi.status() == WL_CONNECTED) {
    HTTPClient http;
    String value = String(temperature);
    String switchState = String(SwitchOn);
    http.begin("http://192.168.4.1/send_by_tft");
    http.addHeader("Content-Type", "application/x-www-form-urlencoded");
    String postData = "value1=" + value + "&value2=" + switchState + "&value3=" + String(cfm) + "&value4=" + String(seasonMode);
    Serial.println(postData);
    int httpCode = http.POST(postData);
    if (httpCode > 0) Serial.println("Server: " + http.getString());
    else Serial.println("Request failed");
    http.end();
  }
}


void getUpdate() {


  HTTPClient http;
  http.begin("http://192.168.4.1/update_tft");
  int code_by_http_get = http.GET();


  Serial.print("code_by_http_get: ");
  Serial.println(code_by_http_get);

  if (code_by_http_get > 0) {
    String payload = http.getString();

    Serial.print("payload: ");
    Serial.println(payload);

    payload.replace("{", "");
    payload.replace("}", "");
    payload.replace("\"", "");

    int start = 0;
    while (start < payload.length()) {
      int end = payload.indexOf(',', start);
      if (end == -1) end = payload.length();

      String pair = payload.substring(start, end);
      int sep = pair.indexOf(':');
      if (sep != -1) {
        String key = pair.substring(0, sep);
        String value = pair.substring(sep + 1);

        key.trim();
        value.trim();

        Serial.print(key);
        Serial.print(" = ");
        Serial.println(value);

        // You can store the value based on key
        if (key == "seasonsw") {
          if (value.toInt() == 0 || value.toInt() == 2) {
            seasonMode = 0;
          } else {
            seasonMode = 1;
          }
        } else if (key == "dmptemp") {
          temp = value.toInt();
        } else if (key == "dmptempsp") {
          temperature = value.toInt();
        } else if (key == "dampertsw") {
          SwitchOn = value.toInt();
        } else if (key == "supcfm") {
          cfm = value.toInt();
        }
      }

      start = end + 1;
    }
  }  //
  else {
    Serial.println("Error on HTTP request");
  }
  http.end();
}

void connect_to_wifi(String ssid) {
  tft.fillScreen(ILI9341_BLACK);
  write_heading("Connecting to:", 10, 40, 2, ILI9341_WHITE);
  write_heading(ssid, 10, 75, 2, ILI9341_YELLOW);

  Serial.println();
  Serial.print("Connecting to SSID: ");
  Serial.println(ssid);

  device_name = ssid;

  WiFi.begin(ssid.c_str(), "bitahomes");
  int retry = 0;

  while (WiFi.status() != WL_CONNECTED && retry < 20) {
    delay(500);
    tft.print(".");
    Serial.print(".");
    retry++;
  }

  Serial.println();

  if (WiFi.status() == WL_CONNECTED) {
    write_heading("Connected!", 10, 145, 2, ILI9341_GREEN);
    Serial.println("WiFi Connected!");
    Serial.print("IP Address: ");
    Serial.println(WiFi.localIP());
    wifi_connected = true;
    delay(2000);
    drawHomePage();
  } else {
    write_heading("Failed", 10, 90, 2, ILI9341_RED);
    Serial.println("WiFi Connection Failed.");
    wifi_connected = false;
    delay(2000);
    drawHomePage();
  }
}

void handle_wifi_touch() {
  if (!ts.touched()) return;

  TS_Point p = ts.getPoint();
  x = map(p.x, 200, 3800, 0, 320);
  y = map(p.y, 200, 3800, 0, 240);

  // Back button
  if (x > 10 && x < 80 && y > 20 && y < 50) {
    drawHomePage();
    return;
  }

  // WiFi list area
  for (int i = 0; i < 4; i++) {
    int itemY = 70 + i * 40;
    if (y > itemY - 20 && y < itemY + 10) {
      selectedNetwork = scrollOffset + i;
      draw_wifi_list();
      delay(300);
      device_name = ssidList[selectedNetwork];
      Serial.print("Connecting to ");
      Serial.println(device_name);
      connect_to_wifi(ssidList[selectedNetwork]);
      return;
    }
  }
}

void scan_wifi() {
  currentPage = "scan_wifi";
  Serial.println("Scan Wifi Page");

  tft.fillScreen(ILI9341_BLACK);
  WiFi.mode(WIFI_STA);
  WiFi.disconnect();
  wifi_connected = false;
  delay(100);

  write_heading("Searching Devices...", 10, 80, 2, ILI9341_LIGHTGREY);
  tft.fillRoundRect(10, 20, 70, 30, 5, ILI9341_DARKGREEN);
  write_text("Back", 20, 42, 2, ILI9341_WHITE);

  int n = WiFi.scanNetworks();
  tft.fillScreen(ILI9341_BLACK);

  totalNetworks = 0;
  if (n == 0) {
    write_heading("No devices found", 10, 75, 2, ILI9341_LIGHTGREY);
  } else {
    tft.setTextColor(ILI9341_LIGHTGREY);
    tft.setFont(&FreeSans12pt7b);  // Slightly smaller for better spacing
    tft.setTextSize(1);

    int y = 60;
    for (int i = 0; i < n && totalNetworks < MAX_NETWORKS; ++i) {
      String ssid = WiFi.SSID(i);
      if (ssid.indexOf("ZMB") < 0) continue;
      ssidList[totalNetworks] = ssid;
      totalNetworks++;
    }

    draw_wifi_list();
    if (totalNetworks == 0) {
      write_heading("No devices found", 10, 85, 2, ILI9341_LIGHTGREY);
    }
  }

  tft.fillRoundRect(10, 20, 70, 30, 5, ILI9341_DARKGREEN);
  write_text("Back", 20, 42, 2, ILI9341_WHITE);
}

void draw_wifi_list() {
  tft.fillRect(0, 60, 320, 180, ILI9341_BLACK);  // clear list area

  int y = 70;
  for (int i = scrollOffset; i < totalNetworks && i < scrollOffset + 4; ++i) {
    if (i == selectedNetwork) {
      tft.fillRoundRect(5, y - 20, 310, 30, 5, ILI9341_DARKCYAN);
      tft.setTextColor(ILI9341_BLACK);
    } else {
      tft.setTextColor(ILI9341_LIGHTGREY);
    }
    tft.setCursor(20, y);
    tft.print(String(i + 1) + ") " + ssidList[i]);
    y += 40;
  }
}



void setup() {
  Serial.begin(115200);
  delay(500);

  SPI.begin(18, 19, 23);  // SCK, MISO, MOSI — explicitly define for VSPI

  ts.begin();
  ts.setRotation(2);  // adjust if needed based on your screen orientation

  tft.begin();
  tft.setRotation(0);  // adjust if needed based on your screen orientation
  tft.fillScreen(ILI9341_WHITE);
  drawMainUI();

  delay(5000);
  drawHomePage();
}

void loop() {
  if (ts.touched()) {
    TS_Point p = ts.getPoint();

    x = map(p.x, 140, 3710, 0, 320);
    y = map(p.y, 220, 3770, 0, 240);

    Serial.print("x: ");
    Serial.println(x);
    Serial.print("y: ");
    Serial.println(y);

    if (currentPage == "scan_wifi") {
      if (x > 15 && x < 75 && y > 20 && y < 55) { drawHomePage(); }
      handle_wifi_touch();
    } else if (currentPage == "home") {
      if (x > 10 && x < 315 && y > 10 && y < 40) {
        scan_wifi();
      }  //
      else if (x > 20 && x < 150 && y > 50 && y < 135) {
        draw_temp();
      }  //
      else if (x > 170 && x < 300 && y > 150 && y < 235) {

        adjustCFM();
      }  //
      else if (x > 20 && x < 150 && y > 150 && y < 235) {
        toggleSeason();
      }  //
      else if (x > 170 && x < 300 && y > 50 && y < 135) {
        toggleSwitch();
      }
    } else if (currentPage == "temp") {
      if (x > 15 && x < 75 && y > 20 && y < 55) { drawHomePage(); }
      if (x > 95 && x < 146 && y > 185 && y < 210) {
        Serial.println("Minus button hit");
        if (temperature > 5) {
          temperature--;
          draw_temp();
          sendUpdate();
        }
      } else if (x > 195 && x < 245 && y > 185 && y < 210) {
        Serial.println("Plus button hit");
        if (temperature < 35) {
          temperature++;
          draw_temp();
          sendUpdate();
        }
      }
    }
  }


  /*--------------- This code is for Get data from Device -------------*/
  if (millis() - previousMillis_1 >= 5000) {
    if (WiFi.status() == WL_CONNECTED) {

      getUpdate();
      if (currentPage == "home") {
        drawHomePage();
      }  //
      else if (currentPage == "temp") {
        draw_temp();
      }
    }
    previousMillis_1 = millis();
  } else {
  }
  /* ----------------------------------------------------------------------------------------------- */

  delay(10);
}
