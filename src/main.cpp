#include <Arduino.h>
#include <ESP8266WiFi.h>
#include <ESP8266HTTPClient.h>
#include <WiFiClient.h>
#include <SPI.h>
#include <EEPROM.h>
#include "main.h"
#include "inverter-data-provider.h"
#include "loop-manager.h"
#include "display.h"
#include "credentials.h"

#define DISPLAY_MODE_EEPROM_ADDR 1
#define SWITCH D6
#define HIGH_GAIN_LED D7
#define MESSAGE_LED D5

LoopManager loop_manager;
Display display(128, 64);
InverterDataProvider data_provider("http://rpi2-node-1/inverter-data");
InverterDataProvider::Response current;
Display::DisplayMode display_mode;

int led_state = LOW;

void setup()
{
  pinMode(SWITCH, INPUT);
  pinMode(MESSAGE_LED, OUTPUT);
  pinMode(HIGH_GAIN_LED, OUTPUT);

  loop_manager.register_handler(fetch_data, 4000);

  EEPROM.begin(1024);
  Serial.begin(115200);

  delay(2000);

  init_display_mode();

  WiFi.begin(ssid, password);

  display.setup();

  IPAddress ip = wait_for_connection();
  display.show_connection_info(ip.toString());
  delay(1500);
}

void loop()
{
  loop_manager.process();

  int state = digitalRead(SWITCH);
  if (state == HIGH && !current.message_only)
  {
    toggle_display_mode();
    render_screen();
    delay(300);
  }
}

/**
 * Fetches inverter data from the specified API endpoint.
 */
void fetch_data()
{
  if (WiFi.status() == WL_CONNECTED)
  {
    current = data_provider.provide();

    if (current.message_only)
    {
      digitalWrite(MESSAGE_LED, HIGH);
      digitalWrite(HIGH_GAIN_LED, LOW);
      display.show_message(current.message);
    }
    else
    {
      digitalWrite(HIGH_GAIN_LED, current.is_high_gain);
      digitalWrite(MESSAGE_LED, LOW);
      render_screen();
    }
  }
  else
  {
    display.show_message("Not connected.");
  }
}

/**
 * Immediate screen rendering depending on current display mode
 */
void render_screen()
{
  switch (display_mode)
  {

  case Display::DisplayMode::All:
    display.show_full_dataset(
        current.data.battery,
        current.data.batteryLoad,
        current.data.load,
        current.data.grid,
        current.data.yield);
    break;

  case Display::DisplayMode::Battery:
    display.show_battery(current.data.battery);
    break;

  case Display::Display::Timestamp:
    String date, time;

    date += current.data.timestamp.substring(8, 10);
    date += ".";
    date += current.data.timestamp.substring(5, 7);
    date += ".";
    date += current.data.timestamp.substring(0, 4);

    time += current.data.timestamp.substring(11, 19);

    display.show_timestamp(date, time);
    break;
  }
}

/**
 * Initialize the display mode
 */
void init_display_mode()
{
  int stored = EEPROM.read(DISPLAY_MODE_EEPROM_ADDR);
  display_mode = stored == 1
                     ? Display::DisplayMode::Battery
                 : stored == 2
                     ? Display::DisplayMode::Timestamp
                     : Display::DisplayMode::All;
}

/**
 * Toggles the display mode
 */
void toggle_display_mode()
{
  switch (display_mode)
  {
  case Display::DisplayMode::All:
    display_mode = Display::DisplayMode::Battery;
    break;
  case Display::DisplayMode::Battery:
    display_mode = Display::DisplayMode::Timestamp;
    break;
  case Display::DisplayMode::Timestamp:
    display_mode = Display::DisplayMode::All;
    break;
  default:
    break;
  }

  EEPROM.write(DISPLAY_MODE_EEPROM_ADDR, (uint8_t)display_mode);
  EEPROM.commit();
}

/**
 * Waits until the WLAN connection is established
 */
IPAddress wait_for_connection()
{
  String connectString = "Connecting";

  while (WiFi.status() != WL_CONNECTED)
  {
    if (connectString.length() == 13)
    {
      connectString = "Connecting";
    }
    else
    {
      connectString += ".";
    }

    display.show_message(connectString);

    delay(1000);
  }

  return WiFi.localIP();
}