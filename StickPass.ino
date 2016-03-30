extern "C"{
#include "pkcs5_pbkdf2.h"
#include "sha.h"
}

#include <SPI.h>  // Include SPI if you're using SPI
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library

const int resetPin = 9;  // Connect RST to pin 9 (req. for SPI and I2C)
const int dataCommandPin = 8;  // Connect DC to pin 8 (required for SPI)
const int chipSelectPin = 10; // Connect CS to pin 10 (required for SPI)

const int leftPin = 5;
const int rightPin = 3;
const int upPin = 7;
const int downPin = 4;
const int centrePin = 6;

bool leftPressed = false;
bool rightPressed = false;
bool upPressed = false;
bool downPressed = false;
bool centrePressed = false;


const int clickTimeout = 200;

MicroOLED oled(resetPin, dataCommandPin, chipSelectPin);

const int charsStart = 32;
const int charsLen = 127-charsStart;

void setup() {
  oled.begin();

  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(centrePin, INPUT);
  
  
  Serial.begin(9600);
  //delay(5000);
  Serial.println("starting");
  const char *password = "password";
  const char *salt = "saltieness";
  uint8_t tk[SHA1_HASH_BYTES];
  int i;

  /*pkcs5_pbkdf2(password, strlen(password), salt, strlen(salt),
                        (const char*)tk, SHA1_HASH_BYTES, 100);
  
  for(i=0; i<SHA1_HASH_BYTES; i++) {
    Serial.print(tk[i], HEX);
  }*/
  oled.setFontType(1);
  drawLetterSelect();
}

int curchar = 32;

int scrollInterval = 0;
unsigned long lastScroll = 0;
bool centreWasPressed = false;

void loop() {
  readButtons();

  if(leftPressed) {
    if(millis()-lastScroll>scrollInterval) {
      curchar = wrapChar(curchar+1);
      lastScroll = millis();

      if(scrollInterval == 0) {
        scrollInterval = 250;
      }else if(scrollInterval > 75){
        scrollInterval *= 0.9f;
      }
      drawLetterSelect();
    }
  }else if(rightPressed) {
    if(millis()-lastScroll>scrollInterval) {
      curchar = wrapChar(curchar-1);
      lastScroll = millis();

      if(scrollInterval == 0) {
        scrollInterval = 250;
      }else if(scrollInterval > 75){
        scrollInterval *= 0.9f;
      }
      drawLetterSelect();
    }
  }else{
    scrollInterval = 0;
    
  }

  if(centrePressed && !centreWasPressed) {
    centreWasPressed = true;
    Serial.write(curchar);
  }else if(!centrePressed) {
    centreWasPressed = false;
  }
  
  
}

unsigned long lastClickChange = 0;

void drawLetterSelect() {
  oled.clear(PAGE);
  oled.drawChar((64-8)/2-20, 32, wrapChar(curchar-2));
  oled.drawChar((64-8)/2-10, 24, wrapChar(curchar-1));
  oled.drawChar((64-8)/2, 16, curchar);
  oled.drawChar((64-8)/2+10, 24, wrapChar(curchar+1));
  oled.drawChar((64-8)/2+20, 32, wrapChar(curchar+2));
  oled.rect((64-8)/2-2, 14, 11, 19);
  
  oled.display();
}

void readButtons() {
  unsigned long timeSinceChange = millis()-lastClickChange;
  // Don't allow any changes until after debounce time
  if(timeSinceChange < clickTimeout) return;
  bool clickChanged = false;

  if(!digitalRead(leftPin) != leftPressed) {
    leftPressed = !leftPressed;
    clickChanged = true;
  }

  if(!digitalRead(rightPin) != rightPressed) {
    rightPressed = !rightPressed;
    clickChanged = true;
  }

  if(!digitalRead(upPin) != upPressed) {
    upPressed = !upPressed;
    clickChanged = true;
  }

  if(!digitalRead(downPin) != downPressed) {
    downPressed = !downPressed;
    clickChanged = true;
  }

  if(!digitalRead(centrePin) != centrePressed) {
    centrePressed = !centrePressed;
    clickChanged = true;
  }

  if(clickChanged) timeSinceChange = millis();
  
  
}

uint8_t wrapChar(uint8_t c) {
  while(c<charsStart) c+=charsLen;
  while(c>=charsStart+charsLen) c-=charsLen;
  return c;
}

