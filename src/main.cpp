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
#define LEFT_UP 4
#define LEFT_DOWN 5
#define RIGHT_UP 6
#define RIGHT_DOWN 7

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

    TwoDRObject(Adafruit_SSD1306& ssd1306) : ssd1306(ssd1306) {
    }

    /*
     * @param width can be only odd number. 1, 3, 5, 7 ...
     * @param height can be only odd number. 1, 3, 5, 7 ...
     * If @param width or @param height are incorrectly passed such as 2, 4, 5, 8 ..., then
     * @param width will be +1 and @param height will be +1 to comply with the rule above.
     * */
    TwoDRObject(const uint8_t& width, const uint8_t& height, Adafruit_SSD1306& ssd1306) : ssd1306(ssd1306) {
        this->setWidth(width);
        this->setHeight(height);
        this->isDraw = false;
    }

    void draw(const TwoDPoint& point, const uint8_t& relative) {

        if (this->isDraw)
            return;

        TwoDPoint topLeft = this->getTopLeft(point, relative);

        for (uint8_t y = topLeft.y; y < (topLeft.y + this->height); ++y) {
            for (uint8_t x = topLeft.x; x < (topLeft.x + this->width); ++x) {
                this->ssd1306.drawPixel(x, y, WHITE);
            }
        }

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

    //In case parts of the given object were deleted due to collision with another object
    void redraw() {

        if (!this->isDraw)
            return;

        TwoDPoint topLeft = this->innerPoint.topLeft;

        for (uint8_t y = topLeft.y; y < (topLeft.y + this->height); ++y) {
            for (uint8_t x = topLeft.x; x < (topLeft.x + this->width); ++x) {
                this->ssd1306.drawPixel(x, y, WHITE);
            }
        }
    }

    void move(const uint8_t& direction) {

        if (!this->isDraw)
            return;

        switch (direction) {
            case UP:
                this->moveUp();
                break;

            case DOWN:
                this->moveDown();
                break;

            case LEFT:
                this->moveLeft();
                break;

            case RIGHT:
                this->moveRight();
                break;

            case LEFT_UP:
                this->moveLeftUp();
                break;

            case LEFT_DOWN:
                this->moveLeftDown();
                break;

            case RIGHT_UP:
                this->moveRightUp();
                break;

            case RIGHT_DOWN:
                this->moveRightDown();
                break;

            default:
                break;
        }
    }

    void moveLeft() {

        if (!this->isDraw)
            return;

        TwoDPoint createLinePoint{(uint8_t) (this->innerPoint.topLeft.x - 1), this->innerPoint.topLeft.y};
        this->drawLineDown(createLinePoint, this->height, WHITE);

        TwoDPoint clearLinePoint{(uint8_t) (this->innerPoint.topRight.x), this->innerPoint.topRight.y};
        this->drawLineDown(clearLinePoint, this->height, BLACK);

        this->innerPoint.topRight.x = this->innerPoint.topRight.x - 1;
        this->innerPoint.bottomRight.x = this->innerPoint.bottomRight.x - 1;
        this->innerPoint.topLeft.x = this->innerPoint.topLeft.x - 1;
        this->innerPoint.bottomLeft.x = this->innerPoint.bottomLeft.x - 1;
    }

    void moveRight() {

        if (!this->isDraw)
            return;

        TwoDPoint createLinePoint{(uint8_t) (this->innerPoint.topRight.x + 1), this->innerPoint.topRight.y};
        this->drawLineDown(createLinePoint, this->height, WHITE);

        TwoDPoint clearLinePoint{(uint8_t) (this->innerPoint.topLeft.x), this->innerPoint.topLeft.y};
        this->drawLineDown(clearLinePoint, this->height, BLACK);

        this->innerPoint.topLeft.x = this->innerPoint.topLeft.x + 1;
        this->innerPoint.bottomLeft.x = this->innerPoint.bottomLeft.x + 1;
        this->innerPoint.topRight.x = this->innerPoint.topRight.x + 1;
        this->innerPoint.bottomRight.x = this->innerPoint.bottomRight.x + 1;
    }

    void moveDown() {

        if (!this->isDraw)
            return;

        TwoDPoint createLinePoint{this->innerPoint.bottomLeft.x, (uint8_t) (this->innerPoint.bottomLeft.y + 1)};
        this->drawLineRight(createLinePoint, this->width, WHITE);

        TwoDPoint clearLinePoint{this->innerPoint.topLeft.x, (uint8_t) (this->innerPoint.topLeft.y)};
        this->drawLineRight(clearLinePoint, this->width, BLACK);

        this->innerPoint.bottomLeft.y = this->innerPoint.bottomLeft.y + 1;
        this->innerPoint.bottomRight.y = this->innerPoint.bottomRight.y + 1;
        this->innerPoint.topLeft.y = this->innerPoint.topLeft.y + 1;
        this->innerPoint.topRight.y = this->innerPoint.topRight.y + 1;
    }

    void moveUp() {

        if (!this->isDraw)
            return;

        TwoDPoint createLinePoint{this->innerPoint.topLeft.x, (uint8_t) (this->innerPoint.topLeft.y - 1)};
        this->drawLineRight(createLinePoint, this->width, WHITE);

        TwoDPoint clearLinePoint{this->innerPoint.bottomLeft.x, (uint8_t) (this->innerPoint.bottomLeft.y)};
        this->drawLineRight(clearLinePoint, this->width, BLACK);

        this->innerPoint.bottomLeft.y = this->innerPoint.bottomLeft.y - 1;
        this->innerPoint.bottomRight.y = this->innerPoint.bottomRight.y - 1;
        this->innerPoint.topLeft.y = this->innerPoint.topLeft.y - 1;
        this->innerPoint.topRight.y = this->innerPoint.topRight.y - 1;
    }

    void moveLeftUp() {

        if (!this->isDraw)
            return;

        this->moveLeft();
        this->moveUp();
    }

    void moveLeftDown() {

        if (!this->isDraw)
            return;

        this->moveLeft();
        this->moveDown();
    }

    void moveRightUp() {

        if (!this->isDraw)
            return;

        this->moveRight();
        this->moveUp();
    }

    void moveRightDown() {

        if (!this->isDraw)
            return;

        this->moveRight();
        this->moveDown();
    }

    bool checkCollision(const TwoDRObject& secondObject) {
        bool collisionX = this->innerPoint.topLeft.x + this->width > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x;
        bool collisionY = this->innerPoint.topLeft.y + this->height > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y;

        return collisionX && collisionY;
    }

    bool isMoveCollision(const TwoDRObject& secondObject, const uint8_t& direction) {

        if (!this->isDraw)
            return false;

        switch (direction) {
            case UP:
                return this->isUpMoveCollision(secondObject);

            case DOWN:
                return this->isDownMoveCollision(secondObject);

            case LEFT:
                return this->isLeftMoveCollision(secondObject);

            case RIGHT:
                return this->isRightMoveCollision(secondObject);

            case LEFT_UP:
                return this->isLeftUpMoveCollision(secondObject);

            case LEFT_DOWN:
                return this->isLeftDownMoveCollision(secondObject);

            case RIGHT_UP:
                return this->isRightUpMoveCollision(secondObject);

            case RIGHT_DOWN:
                return this->isRightDownMoveCollision(secondObject);

            default:
                break;
        }
    }

    bool isRightMoveCollision(const TwoDRObject& secondObject) {
        bool collisionX = this->innerPoint.topLeft.x + this->width + 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x + 1;
        bool collisionY = this->innerPoint.topLeft.y + this->height > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y;

        return collisionX && collisionY;
    }

    bool isLeftMoveCollision(const TwoDRObject& secondObject) {
        bool collisionX = this->innerPoint.topLeft.x + this->width - 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x - 1;
        bool collisionY = this->innerPoint.topLeft.y + this->height > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y;

        return collisionX && collisionY;
    }

    bool isUpMoveCollision(const TwoDRObject& secondObject) {
        bool collisionX = this->innerPoint.topLeft.x + this->width > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x;
        bool collisionY = this->innerPoint.topLeft.y - 1 + this->height > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y - 1;

        return collisionX && collisionY;
    }

    bool isDownMoveCollision(const TwoDRObject& secondObject) {
        bool collisionX = this->innerPoint.topLeft.x + this->width > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x;
        bool collisionY = this->innerPoint.topLeft.y + 1 + this->height > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y + 1;

        return collisionX && collisionY;
    }

    bool isLeftUpMoveCollision(const TwoDRObject& secondObject) {
        bool collisionX = this->innerPoint.topLeft.x + this->width - 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x - 1;
        bool collisionY = this->innerPoint.topLeft.y + this->height - 1 > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y - 1;

        return collisionX && collisionY;
    }

    bool isLeftDownMoveCollision(const TwoDRObject& secondObject) {
        bool collisionX = this->innerPoint.topLeft.x + this->width - 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x - 1;
        bool collisionY = this->innerPoint.topLeft.y + this->height + 1 > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y + 1;

        return collisionX && collisionY;
    }

    bool isRightUpMoveCollision(const TwoDRObject& secondObject) {
        bool collisionX = this->innerPoint.topLeft.x + this->width + 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x + 1;
        bool collisionY = this->innerPoint.topLeft.y + this->height - 1 > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y - 1;

        return collisionX && collisionY;
    }

    bool isRightDownMoveCollision(const TwoDRObject& secondObject) {
        bool collisionX = this->innerPoint.topLeft.x + this->width + 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x + 1;
        bool collisionY = this->innerPoint.topLeft.y + this->height + 1 > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y + 1;

        return collisionX && collisionY;
    }

    bool isFront(const TwoDRObject& object) {
        return object.innerPoint.bottomLeft.x > this->innerPoint.topRight.x;
    }

    bool isAbove(const TwoDRObject& object) {
        return object.innerPoint.bottomLeft.y < this->innerPoint.topLeft.y;
    }

    bool isBehind(const TwoDRObject& object) {
        return object.innerPoint.topRight.x < this->innerPoint.topLeft.x;
    }

    bool isBelow(const TwoDRObject& object) {
        return object.innerPoint.topRight.y > this->innerPoint.bottomRight.y;
    }

private:
    uint8_t width;
    uint8_t height;
    Adafruit_SSD1306& ssd1306;
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

    void drawLine(const TwoDPoint& from, const uint8_t& direction, const uint8_t& length, const uint16_t& color) {

        switch (direction) {

            case UP:
                this->drawLineUp(from, length, color);
                break;

            case DOWN:
                this->drawLineDown(from, length, color);
                break;

            case LEFT:
                this->drawLineLeft(from, length, color);
                break;

            case RIGHT:
                this->drawLineRight(from, length, color);
                break;

            default:
                break;
        }
    }

    void drawLineUp(const TwoDPoint& from, const uint8_t& length, const uint16_t& color) {
        this->ssd1306.drawLine(from.x, from.y, from.x, from.y - (length - 1), color);
    }

    void drawLineDown(const TwoDPoint& from, const uint8_t& length, const uint16_t& color) {
        this->ssd1306.drawLine(from.x, from.y, from.x, from.y + (length - 1), color);
    }

    void drawLineLeft(const TwoDPoint& from, const uint8_t& length, const uint16_t& color) {
        this->ssd1306.drawLine(from.x, from.y, from.x - (length - 1), from.y, color);
    }

    void drawLineRight(const TwoDPoint& from, const uint8_t& length, const uint16_t& color) {
        this->ssd1306.drawLine(from.x, from.y, from.x + (length - 1), from.y, color);
    }

public:
    void setWidth(const uint8_t& width) {
        if (width % 2 == 0) {
            this->width = width + 1;
        } else {
            this->width = width;
        }
    }

    void setHeight(const uint8_t& height) {
        if (height % 2 == 0) {
            this->height = height + 1;
        } else {
            this->height = height;
        }
    }
};

/*
 * TODO: Следващото нещо, което трябва да направя е да сложа пример за това рисуване спрямо част от обекта в README-то :)
 * move с предварително зададени брой движения
 * директно при създаването на object-a да сложа кой е дисплея вместо всеки път да го подавам
 * */
void setup() {
    Serial.begin(9600);

    Wire.setPins(OLED_SDA_PIN, OLED_SCL_PIN);

    oledDisplay = Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire);
    oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    oledDisplay.clearDisplay();

    //firstObject = TwoDRObject(10, 10);
    //secondObject = TwoDRObject(10, 10);

    /*  TwoDPoint centerLeftPoint = TwoDPoint{0, (OLED_HEIGHT / 2) - 1};
      TwoDPoint centerRightPoint = TwoDPoint{OLED_WIDTH - 1, (OLED_HEIGHT / 2) - 1};

      firstObject.draw(oledDisplay, centerLeftPoint, OP_LC);
      secondObject.draw(oledDisplay, centerRightPoint, OP_RC);
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

    tileLeft.draw({15, OLED_HEIGHT / 2 - 1}, OP_C);
    tileRight.draw({OLED_WIDTH - 15 - 1, OLED_HEIGHT / 2 - 1}, OP_C);

    TwoDRObject topBorder = TwoDRObject(OLED_WIDTH - 1, 1, oledDisplay);
    TwoDRObject bottomBorder = TwoDRObject(OLED_WIDTH - 1, 1, oledDisplay);
    TwoDRObject leftBorder = TwoDRObject(1, OLED_HEIGHT - 2 - 1, oledDisplay);
    TwoDRObject rightBorder = TwoDRObject(1, OLED_HEIGHT - 2 - 1, oledDisplay);

    topBorder.draw({0, 0}, OP_TL);
    bottomBorder.draw({0, OLED_HEIGHT - 1}, OP_TL);
    leftBorder.draw({0, 1}, OP_TL);
    rightBorder.draw({OLED_WIDTH - 1, 1}, OP_TL);

    TwoDRObject pixelBall = TwoDRObject(5, 5, oledDisplay);
    pixelBall.draw({OLED_WIDTH / 2, OLED_HEIGHT / 2}, OP_C);

    oledDisplay.display();

    bool isDiagonalRandomTime = false;

    uint8_t currentDirection;

    while (true) {
        if (isDiagonalRandomTime) {
            currentDirection = random(4, 8);
        } else {
            currentDirection = random(0, 4);
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