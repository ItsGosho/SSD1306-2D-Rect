#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "TwoDRObject.h"

using namespace itsgosho;

#define OLED_SDA_PIN 27
#define OLED_SCL_PIN 26

#define OLED_WIDTH 128
#define OLED_HEIGHT 32

Adafruit_SSD1306* oledDisplay;

void setup() {
    Wire.setPins(OLED_SDA_PIN, OLED_SCL_PIN);

    oledDisplay = new Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire);
    oledDisplay->begin(SSD1306_SWITCHCAPVCC, 0x3C);

    oledDisplay->clearDisplay();

    TwoDRObject topBorder = TwoDRObject(OLED_WIDTH, 1, oledDisplay);
    TwoDRObject bottomBorder = TwoDRObject(OLED_WIDTH, 1, oledDisplay);
    TwoDRObject leftBorder = TwoDRObject(1, OLED_HEIGHT, oledDisplay);
    TwoDRObject rightBorder = TwoDRObject(1, OLED_HEIGHT, oledDisplay);

    topBorder.draw({0, 0}, InnerPosition::TL);
    bottomBorder.draw({0, OLED_HEIGHT - 1}, InnerPosition::TL);
    leftBorder.draw({0, 1}, InnerPosition::TL);
    rightBorder.draw({OLED_WIDTH - 1, 1}, InnerPosition::TL);

    TwoDRObject pixelBall = TwoDRObject(5, 5, oledDisplay);
    pixelBall.draw({OLED_WIDTH / 2, OLED_HEIGHT / 2}, InnerPosition::C);

    oledDisplay->display();

    bool isDiagonalRandomTime = false;

    Direction currentDirection;

    while (true) {
        if (isDiagonalRandomTime) {
            currentDirection = static_cast<Direction>(random(4, 8));
        } else {
            currentDirection = static_cast<Direction>(random(0, 4));
        }

        while (!pixelBall.isMoveCollision(topBorder, currentDirection) && !pixelBall.isMoveCollision(bottomBorder, currentDirection) && !pixelBall.isMoveCollision(leftBorder, currentDirection) && !pixelBall.isMoveCollision(rightBorder, currentDirection)) {
            pixelBall.move(currentDirection);
            oledDisplay->display();
            delayMicroseconds(500);
        }

        isDiagonalRandomTime = !isDiagonalRandomTime;
    }
}

void loop() {

}