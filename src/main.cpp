#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA_PIN 27
#define OLED_SCL_PIN 26

#define OLED_WIDTH 128
#define OLED_HEIGHT 32

Adafruit_SSD1306 oledDisplay;

#define UP 0
#define DOWN 1
#define LEFT 2
#define RIGHT 3

#define OP_TL 0
#define OP_TC 1
#define OP_TR 2
#define OP_RC 3
#define OP_C 4
#define OP_LC 5
#define OP_BL 6
#define OP_BC 7
#define OP_BR 8

struct TwoDPoint {
    uint8_t x;
    uint8_t y;
};

struct TwoDRInnerPoint {
    TwoDPoint topLeft;
    TwoDPoint topRight;
    TwoDPoint bottomLeft;
    TwoDPoint bottomRight;
};

class TwoDRObject {

public:

    TwoDRObject() {
    }

    /*
     * @param width can be only odd number. 1, 3, 5, 7 ...
     * @param height can be only odd number. 1, 3, 5, 7 ...
     * */
    TwoDRObject(uint8_t width, uint8_t height) {
        this->width = width;
        this->height = height;
        this->isDraw = false;
    }

    void draw(Adafruit_SSD1306& ssd1306, const TwoDPoint& point, const uint8_t& relative) {

        if (this->isDraw)
            return;

        TwoDPoint topLeft = this->getTopLeft(point, relative);

        for (uint8_t y = topLeft.y; y < (topLeft.y + this->height); ++y) {
            for (uint8_t x = topLeft.x; x < (topLeft.x + this->width); ++x) {
                ssd1306.drawPixel(x, y, WHITE);
            }
        }

        ssd1306.display();

        uint8_t bottomRightX = topLeft.x + (width - 1);
        uint8_t bottomRightY = topLeft.y + (height - 1);
        TwoDPoint bottomRight{bottomRightX, bottomRightY};

        uint8_t bottomLeftX = topLeft.x;
        uint8_t bottomLeftY = topLeft.y + (height - 1);
        TwoDPoint bottomLeft{bottomLeftX, bottomLeftY};

        uint8_t topRightX = topLeft.x + (width - 1);
        uint8_t topRightY = topLeft.y;
        TwoDPoint topRight{topRightX, topRightY};

        this->innerPoint = TwoDRInnerPoint{topLeft, topRight, bottomLeft, bottomRight};
        this->isDraw = true;
    }

    void moveLeft(Adafruit_SSD1306& ssd1306) {

        if (!this->isDraw)
            return;

        for (uint8_t y = this->innerPoint.topLeft.y; y < (this->innerPoint.topLeft.y + this->height); ++y) {
            ssd1306.drawPixel(this->innerPoint.topLeft.x - 1, y, WHITE);
        }

        ssd1306.display();
        this->innerPoint.topLeft.x = this->innerPoint.topLeft.x - 1;
        this->innerPoint.bottomLeft.x = this->innerPoint.bottomLeft.x - 1;

        for (uint8_t y = this->innerPoint.topRight.y; y < (this->innerPoint.topRight.y + this->height); ++y) {
            ssd1306.drawPixel(this->innerPoint.topRight.x, y, BLACK);
        }

        ssd1306.display();
        this->innerPoint.topRight.x = this->innerPoint.topRight.x - 1;
        this->innerPoint.bottomRight.x = this->innerPoint.bottomRight.x - 1;
    }

    void moveRight(Adafruit_SSD1306& ssd1306) {

        if (!this->isDraw)
            return;

        for (uint8_t y = this->innerPoint.topRight.y; y < (this->innerPoint.topRight.y + this->height); ++y) {
            ssd1306.drawPixel(this->innerPoint.topRight.x + 1, y, WHITE);
        }

        ssd1306.display();
        this->innerPoint.topRight.x = this->innerPoint.topRight.x + 1;
        this->innerPoint.bottomRight.x = this->innerPoint.bottomRight.x + 1;

        for (uint8_t y = this->innerPoint.topLeft.y; y < (this->innerPoint.topLeft.y + this->height); ++y) {
            ssd1306.drawPixel(this->innerPoint.topLeft.x, y, BLACK);
        }

        ssd1306.display();
        this->innerPoint.topLeft.x = this->innerPoint.topLeft.x + 1;
        this->innerPoint.bottomLeft.x = this->innerPoint.bottomLeft.x + 1;
    }

    void moveDown(Adafruit_SSD1306& ssd1306) {

        if (!this->isDraw)
            return;

        for (uint8_t x = this->innerPoint.bottomLeft.x; x < (this->innerPoint.bottomLeft.x + this->width); ++x) {
            ssd1306.drawPixel(x, this->innerPoint.bottomLeft.y + 1, WHITE);
        }

        ssd1306.display();
        this->innerPoint.bottomLeft.y = this->innerPoint.bottomLeft.y + 1;
        this->innerPoint.bottomRight.y = this->innerPoint.bottomRight.y + 1;

        for (uint8_t x = this->innerPoint.topLeft.x; x < (this->innerPoint.topLeft.x + this->width); ++x) {
            ssd1306.drawPixel(x, this->innerPoint.topLeft.y, BLACK);
        }

        ssd1306.display();
        this->innerPoint.topLeft.y = this->innerPoint.topLeft.y + 1;
        this->innerPoint.topRight.y = this->innerPoint.topRight.y + 1;
    }

    void moveUp(Adafruit_SSD1306& ssd1306) {

        if (!this->isDraw)
            return;

        for (uint8_t x = this->innerPoint.topLeft.x; x < (this->innerPoint.topLeft.x + this->width); ++x) {
            ssd1306.drawPixel(x, this->innerPoint.topLeft.y - 1, WHITE);
        }

        ssd1306.display();
        this->innerPoint.topLeft.y = this->innerPoint.topLeft.y - 1;
        this->innerPoint.topRight.y = this->innerPoint.topRight.y - 1;

        for (uint8_t x = this->innerPoint.bottomLeft.x; x < (this->innerPoint.bottomLeft.x + this->width); ++x) {
            ssd1306.drawPixel(x, this->innerPoint.bottomLeft.y, BLACK);
        }

        ssd1306.display();
        this->innerPoint.bottomLeft.y = this->innerPoint.bottomLeft.y - 1;
        this->innerPoint.bottomRight.y = this->innerPoint.bottomRight.y - 1;
    }

private:
    uint8_t width;
    uint8_t height;
    TwoDRInnerPoint innerPoint;

    bool isDraw;

    TwoDPoint calculateDrawPointTL(const TwoDPoint& point) {
        uint8_t startingX = point.x;
        uint8_t startingY = point.y;
        return {startingX, startingY};
    }

    TwoDPoint calculateDrawPointBL(const TwoDPoint& point) {
        uint8_t startingX = point.x;
        uint8_t startingY = point.y - (this->height - 1);
        return {startingX, startingY};
    }

    TwoDPoint calculateDrawPointTR(const TwoDPoint& point) {
        uint8_t startingX = point.x - (this->width - 1);
        uint8_t startingY = point.y;
        return {startingX, startingY};
    }

    TwoDPoint calculateDrawPointBR(const TwoDPoint& point) {
        uint8_t startingX = point.x - (this->width - 1);
        uint8_t startingY = point.y - (this->height - 1);
        return {startingX, startingY};
    }

    TwoDPoint calculateDrawPointCT(const TwoDPoint& point) {
        uint8_t startingX = point.x - ((this->width - 1) / 2);
        uint8_t startingY = point.y;
        return {startingX, startingY};
    }

    TwoDPoint calculateDrawPointCL(const TwoDPoint& point) {
        uint8_t startingX = point.x;
        uint8_t startingY = point.y - ((this->height - 1) / 2);
        return {startingX, startingY};
    }

    TwoDPoint calculateDrawPointCB(const TwoDPoint& point) {
        uint8_t startingX = point.x - ((this->width - 1) / 2);
        uint8_t startingY = point.y - (this->height - 1);
        return {startingX, startingY};
    }

    TwoDPoint calculateDrawPointCR(const TwoDPoint& point) {
        uint8_t startingX = point.x - (this->width - 1);
        uint8_t startingY = point.y - ((this->height - 1) / 2);
        return {startingX, startingY};
    }

    TwoDPoint calculateDrawPointC(const TwoDPoint& point) {
        uint8_t startingX = point.x - ((this->width - 1) / 2);
        uint8_t startingY = point.y - ((this->height - 1) / 2);
        return {startingX, startingY};
    }

    /**
     * Get where the top left corner of the object is if given part of it is at given position.
     */
    TwoDPoint getTopLeft(const TwoDPoint& point, const uint8_t& relative) {

        switch (relative) {

            case OP_TL:
                return this->calculateDrawPointTL(point);

            case OP_TC:
                return this->calculateDrawPointCT(point);

            case OP_TR:
                return this->calculateDrawPointTR(point);

            case OP_RC:
                return this->calculateDrawPointCR(point);

            case OP_C:
                return this->calculateDrawPointC(point);

            case OP_LC:
                return this->calculateDrawPointCL(point);

            case OP_BL:
                return this->calculateDrawPointBL(point);

            case OP_BC:
                return this->calculateDrawPointCB(point);

            case OP_BR:
                return this->calculateDrawPointBR(point);

            default:
                break;
        }
    }

    void drawLine(Adafruit_SSD1306& ssd1306, const TwoDPoint& from, const uint8_t& direction, const uint8_t& length, const uint16_t& color) {

        switch (direction) {

            case UP:
                this->drawLineUp(ssd1306, from, length, color);
                break;

            case DOWN:
                this->drawLineDown(ssd1306, from, length, color);
                break;

            case LEFT:
                this->drawLineLeft(ssd1306, from, length, color);
                break;

            case RIGHT:
                this->drawLineRight(ssd1306, from, length, color);
                break;

            default:
                break;
        }
    }

    void drawLineUp(Adafruit_SSD1306& ssd1306, const TwoDPoint& from, const uint8_t& length, const uint16_t& color) {
        ssd1306.drawLine(from.x, from.y, from.x, from.y - (length - 1), color);
        ssd1306.display();
    }

    void drawLineDown(Adafruit_SSD1306& ssd1306, const TwoDPoint& from, const uint8_t& length, const uint16_t& color) {
        ssd1306.drawLine(from.x, from.y, from.x, from.y + (length - 1), color);
        ssd1306.display();
    }

    void drawLineLeft(Adafruit_SSD1306& ssd1306, const TwoDPoint& from, const uint8_t& length, const uint16_t& color) {
        ssd1306.drawLine(from.x, from.y, from.x - (length - 1), from.y, color);
        ssd1306.display();
    }

    void drawLineRight(Adafruit_SSD1306& ssd1306, const TwoDPoint& from, const uint8_t& length, const uint16_t& color) {
        ssd1306.drawLine(from.x, from.y, from.x + (length - 1), from.y, color);
        ssd1306.display();
    }
};

TwoDRObject twoDrObject;

/*
 * TODO: Следващото нещо, което трябва да направя е да сложа пример за това рисуване спрямо част от обекта в README-то :)
 * */
void setup() {
    Serial.begin(9600);

    Wire.setPins(OLED_SDA_PIN, OLED_SCL_PIN);

    oledDisplay = Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire);
    oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    delay(2000);
    oledDisplay.clearDisplay();

    twoDrObject = TwoDRObject(10, 10);

    TwoDPoint twoDPoint = TwoDPoint{(OLED_WIDTH / 2) - 1, (OLED_HEIGHT / 2) - 1};

/*    twoDrObject.draw(oledDisplay, twoDPoint, OP_C);
    delay(2000);*/


    /*oledDisplay.writePixel(0,0, WHITE);
    oledDisplay.display();*/
}

void loop() {

    /*   twoDrObject.moveUp(oledDisplay);
       delay(500);*/

}