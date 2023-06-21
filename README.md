# cst816t capacitive touch ic

This is an Arduino library for the cst816t capacitive touch ic.  The cst816t capacitive touch ic is able to decode clicks, double clicks, swipes and long press in hardware.

## Use

The cst816t touch sensor needs an I2C bus, a reset pin  and a interrupt pin:
```
TwoWire Wire2(TP_SDA, TP_SCL);
cst816t touchpad(Wire2, TP_RST, TP_IRQ);
```
The ic can run in four modes: touch, change, fast, motion.

- mode touch
```
touchpad.begin(mode_touch);
```
In mode touch, if a finger touches the display the ic sends an interrupt every 10ms. All processing is done on the mcu.

- mode change
```
touchpad.begin(mode_change);
```
In mode change the ic sends an interrupt when the finger changes position. Compared to mode touch, the number of interrupts is more moderate.

- mode fast

```
touchpad.begin(mode_fast);
```

In mode _fast_ the ic sends an interrupt when the finger has completed the following movements: single click, swipe up, swipe down, swipe left, swipe right.

- mode motion

```
touchpad.begin(mode_motion);
```

Mode _motion_ adds two more movements: double click and long press.
In mode _motion_ the ic sends an interrupt when the finger has completed the following movements: single click, double click, swipe up, swipe down, swipe left, swipe right, long press. All processing is done on the cst816 touch ic.


The program can test if the display has been touched:

```
if (touchpad.available()) {
	if (touchpad.gesture == GESTURE_SINGLE_CLICK) Serial.print("click at ");
	Serial.print(touchpad.x);
	Serial.print(' ');
	Serial.print(touchpad.y);
	Serial.print(' ');
	Serial.println(touchpad.finger_num);
}
```

## Examples

The library comes with three example Arduino sketches: touchme, smartwatch, and lvgl.

### touchme

A simple sketch that uses the CST816T touch sensor. When the display is touched, the (x, y) coordinates and the action - click or swipe - are printed on the serial port.

### smartwatch

A small Arduino sketch for the [P169H002-CTP](https://www.google.com/search?q=P169H002-CTP) smartwatch display. The P169H002-CTP is a 1.69 inch 240x280 lcd display with an ST7789 lcd driver and a CST816T touch sensor. The sketch prints a text - "click" or "swipe" - on the spot where the display is touched.

### lvgl

[LVGL](http://www.lvgl.io) (Light and Versatile Graphics Library) is a graphics library for embedded systems. An Arduino sketch is given which draws a button on a P169H002-CTP display.

This lvgl example uses the Adafruit GFX library. For improved speed, you may wish to check if there is a library that is more optimized for the processor you use.

The lvgl sketch decodes the touchpad in software. If your user interface consists of only clicks, double clicks, long press and swipes, you might consider using CST816T hardware support instead.

## Links
Arduino libraries for ST7789 tft driver:

- [Adafruit ST7789](https://github.com/adafruit/Adafruit-ST7735-Library/)
- [TFT_eSPI](https://github.com/Bodmer/TFT_eSPI)
- [ST7789-STM32](https://github.com/Floyd-Fish/ST7789-STM32)
- [ST7789-STM32-uGUI](https://github.com/deividAlfa/ST7789-STM32-uGUI)


[Schematic](extras/Schematic_drawing_2023-06-21.pdf
) for the P169H002-CTP display. VCC = 3.3V.

The P169H002-CTP display is available from [Aliexpress](https://www.aliexpress.com/item/1005005238299349.html).
G
