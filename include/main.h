#ifndef MAIN_H
#define MAIN_H

#include <ESP8266WiFi.h>

void setup();
void loop();
void fetch_data();
void init_display_mode();
void toggle_display_mode();
void render_screen();
bool good_time_to_waste_power();
IPAddress wait_for_connection();

#endif