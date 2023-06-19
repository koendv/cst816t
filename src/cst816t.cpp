/* CS816T capacitive touch-screen driver.
 Koen De Vleeschauwer, 2023
 Literature:
 DS-CST816T-V2.2.pdf datasheet
 AN-CST816T-v1.pdf register description
 CST78XX-V1.0.pdf firmware upgrade how-to
*/

#include "cst816t.h"
#include <stdint.h>

#define CST816T_ADDRESS 0x15

#define REG_GESTURE_ID 0x01
#define REG_FINGER_NUM 0x02
#define REG_XPOS_H 0x03
#define REG_XPOS_L 0x04
#define REG_YPOS_H 0x05
#define REG_YPOS_L 0x06
#define REG_CHIP_ID 0xA7
#define REG_PROJ_ID 0xA8
#define REG_FW_VERSION 0xA9
#define REG_FACTORY_ID 0xAA
#define REG_SLEEP_MODE 0xE5
#define REG_IRQ_CTL 0xFA
#define REG_LONG_PRESS_TICK 0xEB
#define REG_MOTION_MASK 0xEC

#define MOTION_MASK_CONTINUOUS_LEFT_RIGHT 0b100
#define MOTION_MASK_CONTINUOUS_UP_DOWN 0b010
#define MOTION_MASK_DOUBLE_CLICK 0b001

#define IRQ_EN_TOUCH 0x40
#define IRQ_EN_CHANGE 0x20
#define IRQ_EN_MOTION 0x10
#define IRQ_EN_LONGPRESS 0x01

static bool tp_event = false;
static void tp_isr() {
  tp_event = true;
}

cst816t::cst816t(TwoWire &_wire, int8_t _rst, int8_t _irq)
  : wire(_wire) {
  rst = _rst;
  irq = _irq;
  chip_id = 0;
  firmware_version = 0;
  tp_event = false;
  gesture_id = 0;
  finger_num = 0;
  x = 0;
  y = 0;
}

void cst816t::begin() {
  uint8_t dta[4];
  pinMode(irq, INPUT);
  if (rst >= 0) {
    pinMode(rst, OUTPUT);
    digitalWrite(rst, HIGH);
    delay(100);
    digitalWrite(rst, LOW);
    delay(10);
    digitalWrite(rst, HIGH);
    delay(100);
  }
  wire.begin();
  if (i2c_read(CST816T_ADDRESS, REG_CHIP_ID, dta, sizeof(dta)))
    Serial.println("i2c error");
  chip_id = dta[0];
  firmware_version = dta[3];

  // set number of ticks for long press
  uint8_t long_press_tick = 100;
  i2c_write(CST816T_ADDRESS, REG_LONG_PRESS_TICK, &long_press_tick, 1);

#if 0
  // enable double click - slows down overall response to single click
  uint8_t motion_mask = MOTION_MASK_DOUBLE_CLICK;
  i2c_write(CST816T_ADDRESS, REG_MOTION_MASK, &motion_mask, 1);
#endif

  // interrupt on gesture and long press
  uint8_t irq_en = IRQ_EN_MOTION | IRQ_EN_LONGPRESS;
  i2c_write(CST816T_ADDRESS, REG_IRQ_CTL, &irq_en, 1);
  attachInterrupt(digitalPinToInterrupt(irq), tp_isr, FALLING);
}

void cst816t::enable(bool enable_touch, bool enable_change, bool enable_motion, bool enable_long_press) {
  uint8_t irq_en = 0;
  if (enable_touch) irq_en |= IRQ_EN_TOUCH;
  if (enable_change) irq_en |= IRQ_EN_CHANGE;
  if (enable_motion) irq_en |= IRQ_EN_MOTION;
  if (enable_long_press) irq_en |= IRQ_EN_LONGPRESS;
  i2c_write(CST816T_ADDRESS, REG_IRQ_CTL, &irq_en, 1);
}

bool cst816t::available() {
  uint8_t dta[6];
  if (tp_event && !i2c_read(CST816T_ADDRESS, REG_GESTURE_ID, dta, sizeof(dta))) {
    tp_event = false;
    gesture_id = dta[0];
    finger_num = dta[1];
    x = (((uint16_t)dta[2] & 0x0f) << 8) | (uint16_t)dta[3];
    y = (((uint16_t)dta[4] & 0x0f) << 8) | (uint16_t)dta[5];
    return true;
  }
  return false;
}

uint8_t cst816t::i2c_read(uint16_t addr, uint8_t reg_addr, uint8_t *reg_data, uint32_t length) {
  wire.beginTransmission(addr);
  wire.write(reg_addr);
  if (wire.endTransmission(true)) return -1;
  wire.requestFrom(addr, length, true);
  for (int i = 0; i < length; i++) {
    *reg_data++ = wire.read();
  }
  return 0;
}

uint8_t cst816t::i2c_write(uint8_t addr, uint8_t reg_addr, const uint8_t *reg_data, uint32_t length) {
  wire.beginTransmission(addr);
  wire.write(reg_addr);
  for (int i = 0; i < length; i++) {
    wire.write(*reg_data++);
  }
  if (wire.endTransmission(true)) return -1;
  return 0;
}

String cst816t::version() {
  String tp_version;
  switch (chip_id) {
    case CHIPID_CST716: tp_version = "CST716"; break;
    case CHIPID_CST816S: tp_version = "CST816S"; break;
    case CHIPID_CST816T: tp_version = "CST816T"; break;
    case CHIPID_CST816D: tp_version = "CST816D"; break;
    default:
      tp_version = "? 0x" + String(chip_id, HEX);
      break;
  }
  tp_version += " firmware " + String(firmware_version);
  return tp_version;
}

String cst816t::state() {
  String s;
  switch (gesture_id) {
    case GESTURE_NONE:
      s = "NONE";
      break;
    case GESTURE_SWIPE_DOWN:
      s = "SWIPE DOWN";
      break;
    case GESTURE_SWIPE_UP:
      s = "SWIPE UP";
      break;
    case GESTURE_SWIPE_LEFT:
      s = "SWIPE LEFT";
      break;
    case GESTURE_SWIPE_RIGHT:
      s = "SWIPE RIGHT";
      break;
    case GESTURE_SINGLE_CLICK:
      s = "SINGLE CLICK";
      break;
    case GESTURE_DOUBLE_CLICK:
      s = "DOUBLE CLICK";
      break;
    case GESTURE_LONG_PRESS:
      s = "LONG PRESS";
      break;
    default:
      s = "UNKNOWN 0x";
      s += String(gesture_id, HEX);
      break;
  }
  s += '\t' + String(x) + '\t' + String(y);
  return s;
}
