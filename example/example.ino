#include <Arduino.h>
#include "Adafruit_SharpMem.h"

#define PIN_CS 2
#define PIN_MOSI 75
#define PIN_CLK 76

#define BLACK 0
#define WHITE 1

Adafruit_SharpMem display(PIN_CLK, PIN_MOSI, PIN_CS, 230, 303, 2000000);

void setup()
{
    // enable DISP pin
    pinMode(3, OUTPUT);
    digitalWrite(3, HIGH);

    display.begin();
    display.clearDisplay();
}

void loop()
{
    display.clearDisplayBuffer();

    display.drawRect(100, 100, 100, 100, BLACK);
    display.drawLine(0,0,229,302, BLACK);
    display.drawCircle(50, 230, 30, BLACK);

    display.setTextSize(3);
    display.setTextColor(BLACK);
    display.setCursor(0, 0);
    display.println("HELLO WORLD");

    display.refresh();
    delay(1000);
}

