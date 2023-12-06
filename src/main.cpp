#include <Arduino.h>
#include <WiFiNINA.h>
#include <SPI.h>

#include "board.hpp"
#include "network.hpp"

#define BTNLED_PIN 2    // D2
#define BUTTON_PIN 3    //  D2
#define LED_RING_PIN 16 // D4
#define BUZZER_PIN 7    // D6

#define JOYSTICK_X_PIN 0
#define JOYSTICK_Y_PIN 1

void printNetworkInformation();
Board *board;

void setup()
{
  Serial.begin(9600);

  // // check for the WiFi module:
  // if (WiFi.status() == WL_NO_MODULE) {
  //   Serial.println("Communication with WiFi module has failed...!");
  // }

  // // attempt to connect to WiFi network:
  // int wifiStatus = WL_IDLE_STATUS;
  // while (wifiStatus != WL_CONNECTED) {
  //   char ssid[] = SSID;
  //   char pass[] = PASS;

  //   Serial.print("Attempting to connect to WPA SSID: ");
  //   Serial.println(ssid);

  //   wifiStatus = WiFi.begin(ssid, pass);
  //   delay(10000);
  // }

  // Serial.println("Connected to Wi-Fi network...");
  // printNetworkInformation();

  board = new Board(BUTTON_PIN, BTNLED_PIN, BUZZER_PIN, JOYSTICK_X_PIN, JOYSTICK_Y_PIN, LED_RING_PIN, 24);
  board->setTimer(50 * 60, 10 * 60);
  board->startTimer();
}

void loop()
{
  board->updateButtonState();

  auto buttonState = board->getButtonState();
  if (buttonState == ButtonType::DOUBLE_PRESS)
  {
    board->solidLedRing(0xFF0000);
  }
  else if (buttonState == ButtonType::SINGLE_PRESS)
  {
    board->solidLedRing(0x0000ff);
  }
  else if (buttonState == ButtonType::LONG_PRESS)
  {
    board->solidLedRing(0x00ff00);
  }
  else if (buttonState == ButtonType::NO_PRESS)
  {
    board->solidLedRing(0x000000);
  }
  else if (buttonState == ButtonType::RESET)
  {
    board->reboot();
  }
}

void printNetworkInformation()
{
  Serial.print("IPv4 address: ");
  Serial.println(WiFi.localIP());

  Serial.print("Subnet Mask: ");
  Serial.println(WiFi.subnetMask());

  Serial.print("Gateway IP: ");
  Serial.println(WiFi.gatewayIP());

  Serial.print("Signal Strength: ");
  Serial.println(WiFi.RSSI());

  byte routerMac[6];
  WiFi.BSSID(routerMac);
  Serial.print("Router MAC: ");
  for (int i = 5; i >= 0; i--)
  {
    if (routerMac[i] < 0x10)
    {
      Serial.print("0");
    }
    Serial.print(routerMac[i], HEX);
    if (i > 0)
    {
      Serial.print(":");
    }
  }
  Serial.println();
}
