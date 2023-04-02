#include <Wire.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>
#include <Fonts/FreeSans9pt7b.h>
#include "display.h"

struct Display::Private
{
    static void draw_sun_icon(Adafruit_SSD1306 &display, int center_x, int center_y, int size)
    {
        display.drawCircle(center_x, center_y, size, WHITE);

        display.drawPixel(center_x, center_y - size - 2, WHITE);
        display.drawPixel(center_x, center_y + size + 2, WHITE);
        display.drawPixel(center_x - size - 2, center_y, WHITE);
        display.drawPixel(center_x + size + 2, center_y, WHITE);

        int hr = (size / 2);
        display.drawPixel(center_x + hr + 3, center_y + hr + 3, WHITE);
        display.drawPixel(center_x - hr - 3, center_y - hr - 3, WHITE);
        display.drawPixel(center_x + hr + 3, center_y - hr - 3, WHITE);
        display.drawPixel(center_x - hr - 3, center_y + hr + 3, WHITE);
    }

    static void draw_house_icon(Adafruit_SSD1306 &display, int center_x, int center_y, int size)
    {
        // Roof left
        display.drawLine(center_x, center_y - size, center_x - size - 2, center_y, WHITE);

        // Roof right
        display.drawLine(center_x, center_y - size, center_x + size + 2, center_y, WHITE);

        // Chimney
        display.drawLine(center_x + (size / 2) + 2, center_y - (size / 2) - 1, center_x + (size / 2) + 2, center_y - size, WHITE);

        // Door
        display.drawRect(center_x - (size * 0.5), center_y + 1, (size * 0.5) + 1, size, WHITE);

        // Bottom line
        display.drawLine(center_x - size, center_y + size, center_x + size, center_y + size, WHITE);

        // Left line
        display.drawLine(center_x - size, center_y, center_x - size, center_y + size, WHITE);

        // Right line
        display.drawLine(center_x + size, center_y, center_x + size, center_y + size, WHITE);
    }

    static void draw_battery_icon(Adafruit_SSD1306 &display, int center_x, int center_y, int size)
    {
        // Outer Top line
        display.drawLine(center_x - size, center_y - size + 2, center_x + size, center_y - size + 2, WHITE);

        // Outer Bottom line
        display.drawLine(center_x - size, center_y + size - 2, center_x + size, center_y + size - 2, WHITE);

        // Outer Left line
        display.drawLine(center_x - size, center_y + size - 2, center_x - size, center_y - size + 2, WHITE);

        // Outer Right line
        display.drawLine(center_x + size, center_y + size - 2, center_x + size, center_y - size + 2, WHITE);

        // Plus pole
        display.drawLine(center_x + size + 1, center_y - (int)(size * 0.3), center_x + size + 1, center_y + (int)(size * 0.3), WHITE);

        // Fill capacity
        display.fillRect(center_x - size + 2, center_y - size + 4, size, (size * 2) - 7, WHITE);
    }

    static void draw_indicator_battery(Adafruit_SSD1306 &display, int start_x, int starty_y, int width, int height, int loadPercent)
    {
        // Outer border
        display.drawRect(start_x, starty_y, width - 5, height, WHITE);

        // Plus pole
        display.fillRect(start_x + width - 5, starty_y + (height * 1/3), 5, (height * 1/3), WHITE);

        // Draw capacity
        int fullWidth = width - 5 - 4;
        display.fillRect(start_x + 2, starty_y + 2, fullWidth * loadPercent / 100, height - 4, WHITE);
    }
};

Display::Display(int width, int height)
{
    display = Adafruit_SSD1306(width, height, &Wire, -1);
}

void Display::setup()
{
    display.begin(SSD1306_SWITCHCAPVCC, 0x3C);
    display.clearDisplay();
    display.setTextColor(WHITE);
    display.setFont(&FreeSans9pt7b);
    display.setTextSize(1);
    display.ssd1306_command(SSD1306_SETCONTRAST);
    display.ssd1306_command(255);
}

void Display::show_connection_info(String ip)
{
    display.clearDisplay();
    display.setCursor(boundage_x, boundage_y);
    display.print("Success!");
    display.setCursor(boundage_x, boundage_y + line_height + 2);
    display.print(ip);
    display.display();

    Serial.print("WiFi connected: ");
    Serial.println(ip);
}

void Display::show_full_dataset(int battery, double batteryLoad, double load, double grid, double yield)
{
    int iconsLeft = 28;
    int fontLeft = boundage_x + 32;

    display.clearDisplay();

    Private::draw_battery_icon(display, iconsLeft, 16, 6);
    display.setCursor(fontLeft, boundage_y);
    display.print(battery);
    display.println(" %");

    Private::draw_sun_icon(display, iconsLeft, 34, 5);
    display.setCursor(fontLeft, boundage_y + line_height);
    display.print(yield, 1);
    display.println(" kW");

    Private::draw_house_icon(display, iconsLeft, 52, 5);
    display.setCursor(fontLeft, boundage_y + line_height * 2);
    display.print(load, 1);
    display.println(" kW");

    display.display();
}

void Display::show_battery(int battery)
{
    display.clearDisplay();
    
    Private::draw_indicator_battery(display, 10, 15, 108, 44, battery);

    display.display();
}

void Display::show_timestamp(String date, String time)
{
    display.clearDisplay();
    display.setCursor(boundage_x, boundage_y + 5);
    display.print(date);

    display.setCursor(boundage_x, boundage_y + line_height + 10);
    display.print(time);

    display.display();
}

void Display::show_message(String message)
{
    display.clearDisplay();
    display.setCursor(boundage_x, boundage_y);
    display.print(message);

    Serial.println(message);

    display.display();
}
