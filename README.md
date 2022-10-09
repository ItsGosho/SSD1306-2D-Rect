# SSD1306-2D-R
Have you ever worked with a library for the **SSD1306** like the **[Adafruit one](https://github.com/adafruit/Adafruit_SSD1306)**?

Then you know the pain, when trying to create a simple game using only it. It is understandable, that isn't her job.



- The library was written alongside with the **Pixel-Pong** game, written to **test the** of **SSD1306**
- The library works **only** with **rectangular objects**, **that are filled** and **have perfect center
<img src=".\pics\Screenshot_1.png" alt="Screenshot_1" style="zoom: 33%;" />**

## Functionalities:

- **Object positioning** - Position a object at a point relative to part of it.
- **Object collision detection** - Check if a object doesn't collide with another one
- **Object moving** - Move your object into different direction
- **Check object position relative to another** - Check if your object is at given side of another object



## Example:

- Creating a borders around the display and placing a ball at the center. Moving it in random direction and if a border is hit moving it in another random direction forever.

```c++
#include <Arduino.h>
#include <Wire.h>
#include <Adafruit_SSD1306.h>
#include "TwoDRObject.h"

#define OLED_SDA_PIN 27
#define OLED_SCL_PIN 26

#define OLED_WIDTH 128
#define OLED_HEIGHT 32

Adafruit_SSD1306 oledDisplay;

void setup() {
    Wire.setPins(OLED_SDA_PIN, OLED_SCL_PIN);

    oledDisplay = Adafruit_SSD1306(OLED_WIDTH, OLED_HEIGHT, &Wire);
    oledDisplay.begin(SSD1306_SWITCHCAPVCC, 0x3C);

    oledDisplay.clearDisplay();

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

    oledDisplay.display();

    bool isDiagonalRandomTime = false;

    Direction currentDirection;

    while (true) {
        if (isDiagonalRandomTime) {
            currentDirection = static_cast<Direction>(random(4, 8));
        } else {
            currentDirection = static_cast<Direction>(random(0, 4));
        }

        while (!pixelBall.isMoveCollision(topBorder, currentDirection) &&
               !pixelBall.isMoveCollision(bottomBorder, currentDirection) &&
               !pixelBall.isMoveCollision(leftBorder, currentDirection) &&
               !pixelBall.isMoveCollision(rightBorder, currentDirection)) {
            pixelBall.move(currentDirection);
            oledDisplay.display();
            delayMicroseconds(500);
        }

        isDiagonalRandomTime = !isDiagonalRandomTime;
    }
}

void loop() {

}
```

## Object Positioning:

- When you create a object `TwoDRObject object = TwoDRObject(3, 3, oledDisplay);` and place it on the screen at a point, you can specify which position of the object will be on the given point `object.draw({2,2}, InnerPosition::TL);` , `object.draw({2,2}, InnerPosition::BL);` and so on.

<img src=".\pics\image-20221010010820515.png" alt="image-20221010010820515" style="zoom: 67%;" />

- The above picture demonstrates positioning a object with size 3x3 at position X: 2 Y:2
- It is super useful in situations, where you must place a object representing a ball at the center of the screen and create different borders

<img src=".\pics\image-20221010012249955.png" alt="image-20221010012249955" style="zoom:67%;" />

- **Note that positioning a object doesn't check for collisions!**

*The Pixel-Pong game uses that. 4 borders are drawn at each side of the screen, ball is placed at the center of the screen and two tiles from both sides are placed too.*

## Object Collision Detection:

- The collision detection is straightforward. You check if a object collides with another one, but also check if a movement in given direction will cause collision.

<img src=".\pics\image-20221010013350182.png" alt="image-20221010013350182" style="zoom:50%;" />

- `redObject.checkCollision(green);`
- `object.isMoveCollision(anotherObject, Direction::LEFT);` 
- `object.isLeftMoveCollision(anotherObject);` 
- ...

## Object Moving:

- The moving is straightforward. You move a object in a given direction:

<img src=".\pics\image-20221010013927582.png" alt="image-20221010013927582" style="zoom:50%;" />

- `object.move(Direction::LEFT);`
- `object.moveLeft();`
- ...

- **Note that moving a object doesn't check for collisions!**

## Object Position relative to another:

- Checking if a object is behind, front, above or below from a given object:

<img src=".\pics\image-20221010014343541.png" alt="image-20221010014343541" style="zoom: 67%;" />

- Note that a object can be above and behind at the same time relative to another object:

<img src=".\pics\image-20221010014524256.png" alt="image-20221010014524256" style="zoom:50%;" />

- Note that all of the checking are absolute, meaning that a object must be fully behind, front, above or below relative to another object.

<img src=".\pics\image-20221010014949557.png" alt="image-20221010014949557" style="zoom:67%;" />

## Game Ideas:

##### Pixel Pong:

- Two moveable tiles. Hit the bouncing pixel and send it to the another tile. Try to not miss it, because you lose.

##### Flappy Pixel:

- A pixel, that goes up or down, trying to pass as most obstacles it can.

##### Pixel Maze:

- A pixel is going after your pixel. Try to be faster than it and not hit any walls - otherwise you lose.

## Additional Information:

- The library was written with the idea to be used in [another project](https://github.com/ItsGosho/Pong-Pong).

- While writing the library, most of the ideas and concepts were drawn in [Excalidraw](https://excalidraw.com/) ,which is a sketching tool. You can find the latest sketches in the [excali](https://github.com/ItsGosho/SSD1306-2D-Rect/tree/dev/excali) folder.

  <img src=".\pics\image-20221010015222978.png" alt="image-20221010015222978" style="zoom:50%;" />
