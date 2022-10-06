# SSD1306-2D-R
Have you ever worked with a library for the **SSD1306** like the **Adafruit one**? (https://github.com/adafruit/Adafruit_SSD1306)

Then you know the pain, when trying to create a simple game using only it. It is understandable, that isn't her job.



- The library is inspired from the **Pong-Pong** game, written to **test the functionalities** of **SSD1306**
- The library works only with **rectangular objects**, **that are filled** and **have perfect center
<img src=".\pics\Screenshot_1.png" alt="Screenshot_1" style="zoom: 33%;" />**

## Game Ideas:

##### Pixel Pong:

- Two moveable tiles. Hit the bouncing pixel and send it to the another tile. Try to not miss it, because you lose.

##### Flappy Pixel:

- A pixel, that goes up or down, trying to pass as most obstacles it can.

##### Pixel Maze:

- A pixel is going after your pixel. Try to be faster than it and not hit any walls - otherwise you lose.

## Functionalities:

- **Object positioning** - Position your object at given point depending on position in the object.
- **Object tracking** - Create your rectangular object and draw it onto the display. Keep track of its position.
- **Object collision detection** - Check if your rectangular object doesn't collide with another one.
- **Object moving** - Move your object into basic direction, including diagonal ones by given position in your object
- **Object distance** - Measure the distance from your object to another by given position in your object**?**
- **Object swapping** - Swap two objects onto the display**?**
