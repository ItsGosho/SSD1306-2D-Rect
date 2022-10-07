#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>

#define OLED_SDA_PIN 27
#define OLED_SCL_PIN 26

#define OLED_WIDTH 128
#define OLED_HEIGHT 32

Adafruit_SSD1306 oledDisplay;

#define OP_TL 0
#define OP_CT 1
#define OP_TR 2
#define OP_CR 3
#define OP_C 4
#define OP_CL 5
#define OP_BL 6
#define OP_CB 7
#define OP_BR 8

struct TwoDPoint {
    uint8_t x;
    uint8_t y;
};

struct TwoDRInnerPoint {
    TwoDPoint topLeftCorner;
    TwoDPoint centerTop;
    TwoDPoint topRightCorner;
    TwoDPoint centerRight;
    TwoDPoint center;
    TwoDPoint centerLeft;
    TwoDPoint bottomLeft;
    TwoDPoint centerBottom;
    TwoDPoint bottomRight;
};

class TwoDRObject {

public:
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

        TwoDPoint drawPoint = this->calculateDrawPoint(point, relative);

        Serial.println(drawPoint.x);
        Serial.println(drawPoint.y);
        Serial.println("ZZZZZZZZZZZZZ>");

        for (uint8_t y = drawPoint.y; y < (drawPoint.y + this->height); ++y) {
            for (uint8_t x = drawPoint.x; x < (drawPoint.x + this->width); ++x) {
                Serial.println(x);
                Serial.println(y);
                Serial.println("------>");
                ssd1306.drawPixel(x, y, WHITE);
            }
        }

        ssd1306.display();

        /*TODO: Don't forget to update the twoDPoint's inner points after drawing.*/
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

    TwoDPoint calculateDrawPoint(const TwoDPoint& point, const uint8_t& relative) {

        switch (relative) {

            case OP_TL:
                return this->calculateDrawPointTL(point);

            case OP_CT:
                return this->calculateDrawPointCT(point);

            case OP_TR:
                return this->calculateDrawPointTR(point);

            case OP_CR:
                return this->calculateDrawPointCR(point);

            case OP_C:
                return this->calculateDrawPointC(point);

            case OP_CL:
                return this->calculateDrawPointCL(point);

            case OP_BL:
                return this->calculateDrawPointBL(point);

            case OP_CB:
                return this->calculateDrawPointCB(point);

            case OP_BR:
                return this->calculateDrawPointBR(point);

            default:
                break;
        }
    }
};

void setup() {
    Serial.begin(9600);

    Wire.setPins(OLED_SDA_PIN, OLED_SCL_PIN);

    oledDisplay = Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire);
    oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    delay(2000);
    oledDisplay.clearDisplay();

    TwoDRObject twoDrObject = TwoDRObject(25, 25);

    TwoDPoint twoDPoint = TwoDPoint{(OLED_WIDTH / 2) - 1, (OLED_HEIGHT / 2) - 1};

    twoDrObject.draw(oledDisplay, twoDPoint, OP_C);

    /*oledDisplay.writePixel(0,0, WHITE);
    oledDisplay.display();*/
}

void loop() {

}