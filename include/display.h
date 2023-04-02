#include <Arduino.h>
#include <Adafruit_SSD1306.h>

#ifndef DISPLAY_H
#define DISPLAY_H

class Display
{

private:
    Adafruit_SSD1306 display;
    int line_height = 18;
    int boundage_x = 12;
    int boundage_y = 22;
    struct Private;

public:
    enum DisplayMode
    {
        All,
        Battery,
        Timestamp
    };
    Display(int width, int height);
    void setup();
    void show_full_dataset(int battery, double batteryLoad, double load, double grid, double yield);
    void show_battery(int battery);
    void show_timestamp(String date, String time);
    void show_message(String message);
    void show_connection_info(String ip);
};

#endif