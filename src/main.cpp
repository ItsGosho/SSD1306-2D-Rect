#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "TwoDRObject.h"

#define OLED_SDA_PIN 27
#define OLED_SCL_PIN 26

#define OLED_WIDTH 128
#define OLED_HEIGHT 32

Adafruit_SSD1306 oledDisplay;

/*
 * TODO: Следващото нещо, което трябва да направя е да сложа пример за това рисуване спрямо част от обекта в README-то :)
 * move с предварително зададени брой движения
 * директно при създаването на object-a да сложа кой е дисплея вместо всеки път да го подавам
 * */
void setup() {
    Direction direction = Direction::RIGHT;
    Serial.begin(9600);

    Wire.setPins(OLED_SDA_PIN, OLED_SCL_PIN);

    oledDisplay = Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire);
    oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    oledDisplay.clearDisplay();

    //firstObject = TwoDRObject(10, 10);
    //secondObject = TwoDRObject(10, 10);

    /*  Point centerLeftPoint = Point{0, (OLED_HEIGHT / 2) - 1};
      Point centerRightPoint = Point{OLED_WIDTH - 1, (OLED_HEIGHT / 2) - 1};

      firstObject.draw(oledDisplay, centerLeftPoint, LC);
      secondObject.draw(oledDisplay, centerRightPoint, RC);
      oledDisplay.display();
      delay(2000);*/

    /* while (true) {
         firstObject.moveRight(oledDisplay);
         oledDisplay.display();
         if (firstObject.isRightMoveCollision(secondObject)) {

             secondObject.redraw(oledDisplay);
             firstObject.moveLeft(oledDisplay);
             oledDisplay.display();
             Serial.println("Collision1!");
             break;
         }
         secondObject.moveLeft(oledDisplay);
         oledDisplay.display();
         if (firstObject.checkCollision(secondObject)) {
             Serial.println("Collision2!");
             secondObject.moveRight(oledDisplay);
             oledDisplay.display();
             break;
         }
         delay(100);
     }*/

    TwoDRObject tileLeft = TwoDRObject(3, 11, oledDisplay);
    TwoDRObject tileRight = TwoDRObject(3, 11, oledDisplay);

    tileLeft.draw({15, OLED_HEIGHT / 2 - 1}, InnerPosition::C);
    tileRight.draw({OLED_WIDTH - 15 - 1, OLED_HEIGHT / 2 - 1}, InnerPosition::C);

    TwoDRObject topBorder = TwoDRObject(OLED_WIDTH - 1, 1, oledDisplay);
    TwoDRObject bottomBorder = TwoDRObject(OLED_WIDTH - 1, 1, oledDisplay);
    TwoDRObject leftBorder = TwoDRObject(1, OLED_HEIGHT - 2 - 1, oledDisplay);
    TwoDRObject rightBorder = TwoDRObject(1, OLED_HEIGHT - 2 - 1, oledDisplay);

    topBorder.draw({0, 0}, InnerPosition::TL);
    bottomBorder.draw({0, OLED_HEIGHT - 1}, InnerPosition::TL);
    leftBorder.draw({0, 1}, InnerPosition::TL);
    rightBorder.draw({OLED_WIDTH - 1, 1}, InnerPosition::TL);

    TwoDRObject pixelBall = TwoDRObject(5, 5, oledDisplay);
    pixelBall.draw({OLED_WIDTH / 2, OLED_HEIGHT / 2}, InnerPosition::C);

    oledDisplay.display();

    bool isDiagonalRandomTime = false;

    Direction currentDirection;

    while (true) {
        if (isDiagonalRandomTime) {
            currentDirection = static_cast<Direction>(random(4, 8));
        } else {
            currentDirection = static_cast<Direction>(random(0, 4));
        }

        while (!pixelBall.isMoveCollision(topBorder, currentDirection) && !pixelBall.isMoveCollision(bottomBorder, currentDirection) && !pixelBall.isMoveCollision(leftBorder, currentDirection) && !pixelBall.isMoveCollision(rightBorder, currentDirection) && !pixelBall.isMoveCollision(tileLeft, currentDirection) && !pixelBall.isMoveCollision(tileRight, currentDirection)) {

            if (!pixelBall.isFront(tileLeft) && !pixelBall.isBehind(tileLeft)) {
                Serial.println("Left lose!");
                return;
            }

            if (!tileRight.isFront(pixelBall) && !tileRight.isBehind(pixelBall)) {
                Serial.println("Right lose!");
                return;
            }

            pixelBall.move(currentDirection);
            oledDisplay.display();
            delayMicroseconds(500);
        }

        isDiagonalRandomTime = !isDiagonalRandomTime;
    }

    //twoDrObject.checkCollision();


    /*oledDisplay.writePixel(0,0, WHITE);
    oledDisplay.display();*/
}

void loop() {

    /*  twoDrObject.moveRightDown(oledDisplay);
      oledDisplay.display();*/
    //delayMicroseconds(1000);

}