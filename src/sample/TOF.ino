/*
*******************************************************************************
* Copyright (c) 2023 by M5Stack
*                  Equipped with M5StickC sample source code
*                          �z��  M5StickC ���ጹ��?
* Visit for more information: https://docs.m5stack.com/en/core/m5stickc
* ?��X��?��???: https://docs.m5stack.com/zh_CN/core/m5stickc
*
* Product:  ToF.  ����?��
* Date: 2022/6/3
*******************************************************************************
  Please connect to hat,Use ToF Hat to detect distance and display distance
  data on the screen in real time.
  ??��HAT,�g�pToF HAT??��?�C��ݛ�����???����?�����B

  Please install vl53l0x lib first(https://github.com/pololu/vl53l0x-arduino)
  lib in Sketch->Includ Library->Library Manager, search for vl53l0x
  Author:pololu
*/

#include <VL53L0X.h>
#include <Wire.h>

#include "M5StickC.h"

VL53L0X sensor;
TFT_eSprite img = TFT_eSprite(&M5.Lcd);

void setup() {
    Wire.begin(0, 26, 100000UL);  //(0,26, 100000UL) for I2C of HAT connection

    M5.begin();

    img.createSprite(160, 80);
    img.fillSprite(BLACK);
    img.setTextColor(WHITE);
    img.setTextSize(2);

    sensor.setTimeout(500);
    if (!sensor.init()) {
        img.setCursor(10, 10);
        img.print("Failed");
        img.pushSprite(0, 0);
        Serial.println("Failed to detect and initialize sensor!");
        while (1) {
        }
    }
    // Start continuous back-to-back mode (take readings as
    // fast as possible).  To use continuous timed mode
    // instead, provide a desired inter-measurement period in
    // ms (e.g. sensor.startContinuous(100)).
    sensor.startContinuous();
}

void loop() {
    uint16_t distance = sensor.readRangeContinuousMillimeters();
    Serial.print(distance);
    if (sensor.timeoutOccurred()) {
        Serial.print(" TIMEOUT");
    }
    Serial.println();
    img.fillSprite(BLACK);
    img.setCursor(10, 10);
    img.print(distance);
    img.pushSprite(0, 0);
}