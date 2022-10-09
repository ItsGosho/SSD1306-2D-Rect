#ifndef SSD1306_2D_RECT_TWODROBJECT_H
#define SSD1306_2D_RECT_TWODROBJECT_H

#include "Arduino.h"
#include <Adafruit_SSD1306.h>

struct Point {
    uint8_t x;
    uint8_t y;
};

struct InnerPoint {
    Point topLeft;
    Point topRight;
    Point bottomLeft;
    Point bottomRight;
};

enum Direction {
    UP = 0, DOWN = 1, LEFT = 2, RIGHT = 3, LEFT_UP = 4, LEFT_DOWN = 5, RIGHT_UP = 6, RIGHT_DOWN = 7
};

enum InnerPosition {
    TL = 0, TC = 1, TR = 2, RC = 3, C = 4, LC = 5, BL = 6, BC = 7, OP_BR = 8
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

    void draw(const Point& point, const InnerPosition& relative) {

        if (this->isDraw)
            return;

        Point topLeft = this->getTopLeft(point, relative);

        for (uint8_t y = topLeft.y; y < (topLeft.y + this->height); ++y) {
            for (uint8_t x = topLeft.x; x < (topLeft.x + this->width); ++x) {
                this->ssd1306.drawPixel(x, y, WHITE);
            }
        }

        uint8_t bottomRightX = topLeft.x + (width - 1);
        uint8_t bottomRightY = topLeft.y + (height - 1);
        Point bottomRight{bottomRightX, bottomRightY};

        uint8_t bottomLeftX = topLeft.x;
        uint8_t bottomLeftY = topLeft.y + (height - 1);
        Point bottomLeft{bottomLeftX, bottomLeftY};

        uint8_t topRightX = topLeft.x + (width - 1);
        uint8_t topRightY = topLeft.y;
        Point topRight{topRightX, topRightY};

        this->innerPoint = InnerPoint{topLeft, topRight, bottomLeft, bottomRight};
        this->isDraw = true;
    }

    //In case parts of the given object were deleted due to collision with another object
    void redraw() {

        if (!this->isDraw)
            return;

        Point topLeft = this->innerPoint.topLeft;

        for (uint8_t y = topLeft.y; y < (topLeft.y + this->height); ++y) {
            for (uint8_t x = topLeft.x; x < (topLeft.x + this->width); ++x) {
                this->ssd1306.drawPixel(x, y, WHITE);
            }
        }
    }

    void move(const Direction& direction) {

        if (!this->isDraw)
            return;

        switch (direction) {
            case Direction::UP:
                this->moveUp();
                break;

            case Direction::DOWN:
                this->moveDown();
                break;

            case Direction::LEFT:
                this->moveLeft();
                break;

            case Direction::RIGHT:
                this->moveRight();
                break;

            case Direction::LEFT_UP:
                this->moveLeftUp();
                break;

            case Direction::LEFT_DOWN:
                this->moveLeftDown();
                break;

            case Direction::RIGHT_UP:
                this->moveRightUp();
                break;

            case Direction::RIGHT_DOWN:
                this->moveRightDown();
                break;

            default:
                break;
        }
    }

    void moveLeft() {

        if (!this->isDraw)
            return;

        Point createLinePoint{(uint8_t) (this->innerPoint.topLeft.x - 1), this->innerPoint.topLeft.y};
        this->drawLineDown(createLinePoint, this->height, WHITE);

        Point clearLinePoint{(uint8_t) (this->innerPoint.topRight.x), this->innerPoint.topRight.y};
        this->drawLineDown(clearLinePoint, this->height, BLACK);

        this->innerPoint.topRight.x = this->innerPoint.topRight.x - 1;
        this->innerPoint.bottomRight.x = this->innerPoint.bottomRight.x - 1;
        this->innerPoint.topLeft.x = this->innerPoint.topLeft.x - 1;
        this->innerPoint.bottomLeft.x = this->innerPoint.bottomLeft.x - 1;
    }

    void moveRight() {

        if (!this->isDraw)
            return;

        Point createLinePoint{(uint8_t) (this->innerPoint.topRight.x + 1), this->innerPoint.topRight.y};
        this->drawLineDown(createLinePoint, this->height, WHITE);

        Point clearLinePoint{(uint8_t) (this->innerPoint.topLeft.x), this->innerPoint.topLeft.y};
        this->drawLineDown(clearLinePoint, this->height, BLACK);

        this->innerPoint.topLeft.x = this->innerPoint.topLeft.x + 1;
        this->innerPoint.bottomLeft.x = this->innerPoint.bottomLeft.x + 1;
        this->innerPoint.topRight.x = this->innerPoint.topRight.x + 1;
        this->innerPoint.bottomRight.x = this->innerPoint.bottomRight.x + 1;
    }

    void moveDown() {

        if (!this->isDraw)
            return;

        Point createLinePoint{this->innerPoint.bottomLeft.x, (uint8_t) (this->innerPoint.bottomLeft.y + 1)};
        this->drawLineRight(createLinePoint, this->width, WHITE);

        Point clearLinePoint{this->innerPoint.topLeft.x, (uint8_t) (this->innerPoint.topLeft.y)};
        this->drawLineRight(clearLinePoint, this->width, BLACK);

        this->innerPoint.bottomLeft.y = this->innerPoint.bottomLeft.y + 1;
        this->innerPoint.bottomRight.y = this->innerPoint.bottomRight.y + 1;
        this->innerPoint.topLeft.y = this->innerPoint.topLeft.y + 1;
        this->innerPoint.topRight.y = this->innerPoint.topRight.y + 1;
    }

    void moveUp() {

        if (!this->isDraw)
            return;

        Point createLinePoint{this->innerPoint.topLeft.x, (uint8_t) (this->innerPoint.topLeft.y - 1)};
        this->drawLineRight(createLinePoint, this->width, WHITE);

        Point clearLinePoint{this->innerPoint.bottomLeft.x, (uint8_t) (this->innerPoint.bottomLeft.y)};
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

    bool isMoveCollision(const TwoDRObject& secondObject, const Direction& direction) {

        if (!this->isDraw)
            return false;

        switch (direction) {
            case Direction::UP:
                return this->isUpMoveCollision(secondObject);

            case Direction::DOWN:
                return this->isDownMoveCollision(secondObject);

            case Direction::LEFT:
                return this->isLeftMoveCollision(secondObject);

            case Direction::RIGHT:
                return this->isRightMoveCollision(secondObject);

            case Direction::LEFT_UP:
                return this->isLeftUpMoveCollision(secondObject);

            case Direction::LEFT_DOWN:
                return this->isLeftDownMoveCollision(secondObject);

            case Direction::RIGHT_UP:
                return this->isRightUpMoveCollision(secondObject);

            case Direction::RIGHT_DOWN:
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
    InnerPoint innerPoint;

    bool isDraw;

    Point calculateDrawPointTL(const Point& point) {
        uint8_t startingX = point.x;
        uint8_t startingY = point.y;
        return {startingX, startingY};
    }

    Point calculateDrawPointBL(const Point& point) {
        uint8_t startingX = point.x;
        uint8_t startingY = point.y - (this->height - 1);
        return {startingX, startingY};
    }

    Point calculateDrawPointTR(const Point& point) {
        uint8_t startingX = point.x - (this->width - 1);
        uint8_t startingY = point.y;
        return {startingX, startingY};
    }

    Point calculateDrawPointBR(const Point& point) {
        uint8_t startingX = point.x - (this->width - 1);
        uint8_t startingY = point.y - (this->height - 1);
        return {startingX, startingY};
    }

    Point calculateDrawPointCT(const Point& point) {
        uint8_t startingX = point.x - ((this->width - 1) / 2);
        uint8_t startingY = point.y;
        return {startingX, startingY};
    }

    Point calculateDrawPointCL(const Point& point) {
        uint8_t startingX = point.x;
        uint8_t startingY = point.y - ((this->height - 1) / 2);
        return {startingX, startingY};
    }

    Point calculateDrawPointCB(const Point& point) {
        uint8_t startingX = point.x - ((this->width - 1) / 2);
        uint8_t startingY = point.y - (this->height - 1);
        return {startingX, startingY};
    }

    Point calculateDrawPointCR(const Point& point) {
        uint8_t startingX = point.x - (this->width - 1);
        uint8_t startingY = point.y - ((this->height - 1) / 2);
        return {startingX, startingY};
    }

    Point calculateDrawPointC(const Point& point) {
        uint8_t startingX = point.x - ((this->width - 1) / 2);
        uint8_t startingY = point.y - ((this->height - 1) / 2);
        return {startingX, startingY};
    }

    /**
     * Get where the top left corner of the object is if given part of it is at given position.
     */
    Point getTopLeft(const Point& point, const InnerPosition& relative) {

        switch (relative) {

            case InnerPosition::TL:
                return this->calculateDrawPointTL(point);

            case InnerPosition::TC:
                return this->calculateDrawPointCT(point);

            case InnerPosition::TR:
                return this->calculateDrawPointTR(point);

            case InnerPosition::RC:
                return this->calculateDrawPointCR(point);

            case InnerPosition::C:
                return this->calculateDrawPointC(point);

            case InnerPosition::LC:
                return this->calculateDrawPointCL(point);

            case InnerPosition::BL:
                return this->calculateDrawPointBL(point);

            case InnerPosition::BC:
                return this->calculateDrawPointCB(point);

            case InnerPosition::OP_BR:
                return this->calculateDrawPointBR(point);

            default:
                break;
        }
    }

    void drawLine(const Point& from, const Direction& direction, const uint8_t& length, const uint16_t& color) {

        switch (direction) {

            case Direction::UP:
                this->drawLineUp(from, length, color);
                break;

            case Direction::DOWN:
                this->drawLineDown(from, length, color);
                break;

            case Direction::LEFT:
                this->drawLineLeft(from, length, color);
                break;

            case Direction::RIGHT:
                this->drawLineRight(from, length, color);
                break;

            default:
                break;
        }
    }

    void drawLineUp(const Point& from, const uint8_t& length, const uint16_t& color) {
        this->ssd1306.drawLine(from.x, from.y, from.x, from.y - (length - 1), color);
    }

    void drawLineDown(const Point& from, const uint8_t& length, const uint16_t& color) {
        this->ssd1306.drawLine(from.x, from.y, from.x, from.y + (length - 1), color);
    }

    void drawLineLeft(const Point& from, const uint8_t& length, const uint16_t& color) {
        this->ssd1306.drawLine(from.x, from.y, from.x - (length - 1), from.y, color);
    }

    void drawLineRight(const Point& from, const uint8_t& length, const uint16_t& color) {
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


#endif //SSD1306_2D_RECT_TWODROBJECT_H
