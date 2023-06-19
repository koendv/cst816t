/* demo for the CST816T capacitive touch ic */

#include <Wire.h>
#include "cst816t.h"

#define TP_SDA PB11
#define TP_SCL PB10
#define TP_RST PA15
#define TP_IRQ PB3

TwoWire Wire2(TP_SDA, TP_SCL);
cst816t touchpad(Wire2, TP_RST, TP_IRQ);

void setup() {
  touchpad.begin();
  Serial.println(touchpad.version());
}

void loop() {
  if (touchpad.available())
    Serial.println(touchpad.state());
}
