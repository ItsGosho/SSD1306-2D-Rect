#include "TwoDRObject.h"

TwoDRObject::TwoDRObject() {
}

TwoDRObject::TwoDRObject(const uint8_t& width, const uint8_t& height, Adafruit_SSD1306& ssd1306) : ssd1306(ssd1306) {
    this->setWidth(width);
    this->setHeight(height);
    this->isDraw = false;
}

void TwoDRObject::draw(const Point& point, const InnerPosition& relative) {

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

void TwoDRObject::redraw() {

    if (!this->isDraw)
        return;

    Point topLeft = this->innerPoint.topLeft;

    for (uint8_t y = topLeft.y; y < (topLeft.y + this->height); ++y) {
        for (uint8_t x = topLeft.x; x < (topLeft.x + this->width); ++x) {
            this->ssd1306.drawPixel(x, y, WHITE);
        }
    }
}

void TwoDRObject::move(const Direction& direction) {

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

void TwoDRObject::moveLeft() {

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

void TwoDRObject::moveRight() {

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

void TwoDRObject::moveDown() {

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

void TwoDRObject::moveUp() {

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

void TwoDRObject::moveLeftUp() {

    if (!this->isDraw)
        return;

    this->moveLeft();
    this->moveUp();
}

void TwoDRObject::moveLeftDown() {

    if (!this->isDraw)
        return;

    this->moveLeft();
    this->moveDown();
}

void TwoDRObject::moveRightUp() {

    if (!this->isDraw)
        return;

    this->moveRight();
    this->moveUp();
}

void TwoDRObject::moveRightDown() {

    if (!this->isDraw)
        return;

    this->moveRight();
    this->moveDown();
}

bool TwoDRObject::checkCollision(const TwoDRObject& secondObject) {
    bool collisionX = this->innerPoint.topLeft.x + this->width > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x;
    bool collisionY = this->innerPoint.topLeft.y + this->height > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y;

    return collisionX && collisionY;
}

bool TwoDRObject::isMoveCollision(const TwoDRObject& secondObject, const Direction& direction) {

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

bool TwoDRObject::isRightMoveCollision(const TwoDRObject& secondObject) const {
    bool collisionX = this->innerPoint.topLeft.x + this->width + 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x + 1;
    bool collisionY = this->innerPoint.topLeft.y + this->height > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y;

    return collisionX && collisionY;
}

bool TwoDRObject::isLeftMoveCollision(const TwoDRObject& secondObject) const {
    bool collisionX = this->innerPoint.topLeft.x + this->width - 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x - 1;
    bool collisionY = this->innerPoint.topLeft.y + this->height > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y;

    return collisionX && collisionY;
}

bool TwoDRObject::isUpMoveCollision(const TwoDRObject& secondObject) const {
    bool collisionX = this->innerPoint.topLeft.x + this->width > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x;
    bool collisionY = this->innerPoint.topLeft.y - 1 + this->height > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y - 1;

    return collisionX && collisionY;
}

bool TwoDRObject::isDownMoveCollision(const TwoDRObject& secondObject) const {
    bool collisionX = this->innerPoint.topLeft.x + this->width > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x;
    bool collisionY = this->innerPoint.topLeft.y + 1 + this->height > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y + 1;

    return collisionX && collisionY;
}

bool TwoDRObject::isLeftUpMoveCollision(const TwoDRObject& secondObject) const {
    bool collisionX = this->innerPoint.topLeft.x + this->width - 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x - 1;
    bool collisionY = this->innerPoint.topLeft.y + this->height - 1 > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y - 1;

    return collisionX && collisionY;
}

bool TwoDRObject::isLeftDownMoveCollision(const TwoDRObject& secondObject) const {
    bool collisionX = this->innerPoint.topLeft.x + this->width - 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x - 1;
    bool collisionY = this->innerPoint.topLeft.y + this->height + 1 > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y + 1;

    return collisionX && collisionY;
}

bool TwoDRObject::isRightUpMoveCollision(const TwoDRObject& secondObject) const {
    bool collisionX = this->innerPoint.topLeft.x + this->width + 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x + 1;
    bool collisionY = this->innerPoint.topLeft.y + this->height - 1 > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y - 1;

    return collisionX && collisionY;
}

bool TwoDRObject::isRightDownMoveCollision(const TwoDRObject& secondObject) const {
    bool collisionX = this->innerPoint.topLeft.x + this->width + 1 > secondObject.innerPoint.topLeft.x && secondObject.innerPoint.topLeft.x + secondObject.width > this->innerPoint.topLeft.x + 1;
    bool collisionY = this->innerPoint.topLeft.y + this->height + 1 > secondObject.innerPoint.topLeft.y && secondObject.innerPoint.topLeft.y + secondObject.height > this->innerPoint.topLeft.y + 1;

    return collisionX && collisionY;
}

bool TwoDRObject::isFront(const TwoDRObject& object) {
    return object.innerPoint.bottomLeft.x > this->innerPoint.topRight.x;
}

bool TwoDRObject::isAbove(const TwoDRObject& object) {
    return object.innerPoint.bottomLeft.y < this->innerPoint.topLeft.y;
}

bool TwoDRObject::isBehind(const TwoDRObject& object) {
    return object.innerPoint.topRight.x < this->innerPoint.topLeft.x;
}

bool TwoDRObject::isBelow(const TwoDRObject& object) {
    return object.innerPoint.topRight.y > this->innerPoint.bottomRight.y;
}

void TwoDRObject::setWidth(const uint8_t& width) {
    if (width % 2 == 0) {
        this->width = width + 1;
    } else {
        this->width = width;
    }
}

void TwoDRObject::setHeight(const uint8_t& height) {
    if (height % 2 == 0) {
        this->height = height + 1;
    } else {
        this->height = height;
    }
}

Point TwoDRObject::calculateDrawPointTL(const Point& point) const {
    uint8_t startingX = point.x;
    uint8_t startingY = point.y;
    return {startingX, startingY};
}

Point TwoDRObject::calculateDrawPointBL(const Point& point) const {
    uint8_t startingX = point.x;
    uint8_t startingY = point.y - (this->height - 1);
    return {startingX, startingY};
}

Point TwoDRObject::calculateDrawPointTR(const Point& point) const {
    uint8_t startingX = point.x - (this->width - 1);
    uint8_t startingY = point.y;
    return {startingX, startingY};
}

Point TwoDRObject::calculateDrawPointBR(const Point& point) const {
    uint8_t startingX = point.x - (this->width - 1);
    uint8_t startingY = point.y - (this->height - 1);
    return {startingX, startingY};
}

Point TwoDRObject::calculateDrawPointCT(const Point& point) const {
    uint8_t startingX = point.x - ((this->width - 1) / 2);
    uint8_t startingY = point.y;
    return {startingX, startingY};
}

Point TwoDRObject::calculateDrawPointCL(const Point& point) const {
    uint8_t startingX = point.x;
    uint8_t startingY = point.y - ((this->height - 1) / 2);
    return {startingX, startingY};
}

Point TwoDRObject::calculateDrawPointCB(const Point& point) const {
    uint8_t startingX = point.x - ((this->width - 1) / 2);
    uint8_t startingY = point.y - (this->height - 1);
    return {startingX, startingY};
}

Point TwoDRObject::calculateDrawPointCR(const Point& point) const {
    uint8_t startingX = point.x - (this->width - 1);
    uint8_t startingY = point.y - ((this->height - 1) / 2);
    return {startingX, startingY};
}

Point TwoDRObject::calculateDrawPointC(const Point& point) const {
    uint8_t startingX = point.x - ((this->width - 1) / 2);
    uint8_t startingY = point.y - ((this->height - 1) / 2);
    return {startingX, startingY};
}

/**
 * Get where the top left corner of the object is if given part of it is at given position.
 */
Point TwoDRObject::getTopLeft(const Point& point, const InnerPosition& relative) const {

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

void TwoDRObject::drawLine(const Point& from, const Direction& direction, const uint8_t& length, const uint16_t& color) {

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

void TwoDRObject::drawLineUp(const Point& from, const uint8_t& length, const uint16_t& color) {
    this->ssd1306.drawLine(from.x, from.y, from.x, from.y - (length - 1), color);
}

void TwoDRObject::drawLineDown(const Point& from, const uint8_t& length, const uint16_t& color) {
    this->ssd1306.drawLine(from.x, from.y, from.x, from.y + (length - 1), color);
}

void TwoDRObject::drawLineLeft(const Point& from, const uint8_t& length, const uint16_t& color) {
    this->ssd1306.drawLine(from.x, from.y, from.x - (length - 1), from.y, color);
}

void TwoDRObject::drawLineRight(const Point& from, const uint8_t& length, const uint16_t& color) {
    this->ssd1306.drawLine(from.x, from.y, from.x + (length - 1), from.y, color);
}