#include <Arduino.h>
#include <Wire.h>
#include <SPI.h>
#include <Adafruit_Sensor.h>
#include <Adafruit_BME280.h>
#include "LED.h"

#define BME_CS 10
#define SEALEVELPRESSURE_HPA (1013.25)

Adafruit_BME280 bme(BME_CS);  // SPI

unsigned long delayTime;
const int buttonPin1 = 4;  // the number of the pushbutton pin
const int buttonPin2 = 5;  // the number of the pushbutton pin

short brightness=8;
uint8_t swhumid=0;

void setup() {
  Serial.begin(115200);

  if (!bme.begin(0x77, &Wire)) {
    Serial.println("Could not find a valid BME280 sensor, check wiring!");
    while (1)
      ;
  }

  delayTime = 10;

  LED_setup();
  Serial.println();

  pinMode(buttonPin1, INPUT);
  pinMode(buttonPin2, INPUT);
}

uint16_t idelay=0;

void loop() {
  if(!idelay){
    if(!swhumid){
      float temp = bme.readTemperature();
      Serial.print("Temperature = ");
      Serial.print(temp);
      Serial.println(" °C");
      disp_temp(temp);
    } else {
      float humi = bme.readHumidity();
      Serial.print("Humidity = ");
      Serial.print(humi);
      Serial.println(" %");
      disp_humid(humi);
    }
    idelay=1000;
  } else {
    idelay--;
  }

  uint8_t buttonState = digitalRead(buttonPin1);
  if(buttonState == HIGH){
    delay(5);
    buttonState = digitalRead(buttonPin1);
    if(buttonState == HIGH){
      brightness--;
      Serial.print("brightness=");
      Serial.println(brightness);
      if(brightness<0) brightness=8;
      LED_Driver_Brightness(LDaddrs1, brightness);  // brightness= 0～15
      disp_temp((float)brightness);
      delay(500);
      while(1){
        buttonState = digitalRead(buttonPin1);
        if(!buttonState) break;
      }
      idelay=0;
    }
  }

  uint8_t buttonState2 = digitalRead(buttonPin2);
  if(buttonState2 == HIGH){
    delay(5);
    buttonState2 = digitalRead(buttonPin2);
    if(buttonState2 == HIGH){
      if(swhumid==0) swhumid=1;
      else if(swhumid==1) swhumid=0;
      while(1){
        buttonState2 = digitalRead(buttonPin2);
        if(!buttonState2) break;
      }
      idelay=0;
    }
  }

  delay(delayTime);
}

