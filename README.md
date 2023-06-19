# cst816t capacitive touch ic

This is an Arduino library for the cst816t capacitive touch ic.  The cst816t capacitive touch ic is able to decode clicks, double clicks, swipes and long press in hardware.

## smartwatch display

As an example a small Arduino sketch for the [P169H002-CTP](https://www.google.com/search?q=P169H002-CTP) smartwatch display is given. The P169H002-CTP is a 1.69 inch 240x280 lcd display with an ST7789 lcd driver and a CST816T touch sensor.

## lvgl

[LVGL](http://www.lvgl.io) (Light and Versatile Graphics Library) is a graphics library for embedded systems. As an example, an Arduino sketch is given which draws a button on a P169H002-CTP display.

This lvgl example uses the Adafruit GFX library. You may wish to check if there exists a library that has been optimized for the processor you use. 

The lvgl sketch decodes clicks and swipes in software. 

## links
Arduino libraries for ST7789 tft driver:

- [Adafruit ST7789](https://github.com/adafruit/Adafruit-ST7735-Library/)
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [ST7789-STM32](https://github.com/Floyd-Fish/ST7789-STM32)
- [ST7789-STM32-uGUI](https://github.com/deividAlfa/ST7789-STM32-uGUI)

The P169H002-CTP display is available from [Aliexpress](https://www.aliexpress.com/item/1005005238299349.html).
