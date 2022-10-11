#ifndef _SSD1306_2D_RECT_TWODROBJECT_H_
#define _SSD1306_2D_RECT_TWODROBJECT_H_

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

namespace itsgosho {

    class TwoDRObject {

    public:

        TwoDRObject();

        /*
         * @param width can be only odd number. 1, 3, 5, 7 ...
         * @param height can be only odd number. 1, 3, 5, 7 ...
         * If @param width or @param height are incorrectly passed such as 2, 4, 5, 8 ..., then
         * @param width will be +1 and @param height will be +1 to comply with the rule above.
         * */
        TwoDRObject(const uint8_t& width, const uint8_t& height, Adafruit_SSD1306* ssd1306);

        void draw(const Point& point, const InnerPosition& relative);

        /**
         * By default, the position of the object relative to the point will be C (Center)
         */
        void draw(const Point& pont);

        void redraw();

        void move(const Direction& direction);

        void moveLeft();

        void moveRight();

        void moveDown();

        void moveUp();

        void moveLeftUp();

        void moveLeftDown();

        void moveRightUp();

        void moveRightDown();

        bool checkCollision(const TwoDRObject& secondObject);

        bool isMoveCollision(const TwoDRObject& secondObject, const Direction& direction);

        bool isRightMoveCollision(const TwoDRObject& secondObject) const;

        bool isLeftMoveCollision(const TwoDRObject& secondObject) const;

        bool isUpMoveCollision(const TwoDRObject& secondObject) const;

        bool isDownMoveCollision(const TwoDRObject& secondObject) const;

        bool isLeftUpMoveCollision(const TwoDRObject& secondObject) const;

        bool isLeftDownMoveCollision(const TwoDRObject& secondObject) const;

        bool isRightUpMoveCollision(const TwoDRObject& secondObject) const;

        bool isRightDownMoveCollision(const TwoDRObject& secondObject) const;

        bool isFront(const TwoDRObject& object);

        bool isAbove(const TwoDRObject& object);

        bool isBehind(const TwoDRObject& object);

        bool isBelow(const TwoDRObject& object);

        void setWidth(const uint8_t& width);

        void setHeight(const uint8_t& height);

    private:
        uint8_t width;
        uint8_t height;
        Adafruit_SSD1306* ssd1306;
        InnerPoint innerPoint;

        bool isDraw;

        Point calculateDrawPointTL(const Point& point) const;

        Point calculateDrawPointBL(const Point& point) const;

        Point calculateDrawPointTR(const Point& point) const;

        Point calculateDrawPointBR(const Point& point) const;

        Point calculateDrawPointCT(const Point& point) const;

        Point calculateDrawPointCL(const Point& point) const;

        Point calculateDrawPointCB(const Point& point) const;

        Point calculateDrawPointCR(const Point& point) const;

        Point calculateDrawPointC(const Point& point) const;

        /**
         * Get where the top left corner of the object is if given part of it is at given position.
         */
        Point getTopLeft(const Point& point, const InnerPosition& relative) const;

        void drawLine(const Point& from, const Direction& direction, const uint8_t& length, const uint16_t& color);

        void drawLineUp(const Point& from, const uint8_t& length, const uint16_t& color);

        void drawLineDown(const Point& from, const uint8_t& length, const uint16_t& color);

        void drawLineLeft(const Point& from, const uint8_t& length, const uint16_t& color);

        void drawLineRight(const Point& from, const uint8_t& length, const uint16_t& color);
    };
}

#endif
