
#include <SPI.h>  // Include SPI if you're using SPI
#include <SFE_MicroOLED.h>  // Include the SFE_MicroOLED library
#include <Wire.h>

const int resetPin = 9;  // Connect RST to pin 9 (req. for SPI and I2C)
const int dataCommandPin = 8;  // Connect DC to pin 8 (required for SPI)
const int chipSelectPin = 10; // Connect CS to pin 10 (required for SPI)

const int leftPin = 5;
const int rightPin = A0;
const int upPin = 7;
const int downPin = 4;
const int centrePin = 6;

bool leftPressed = false;
bool rightPressed = false;
bool upPressed = false;
bool downPressed = false;
bool centrePressed = false;

const int clickTimeout = 200;

const uint8_t secureCode[] = {0xE5, 0x47, 0x47};


MicroOLED oled(resetPin, dataCommandPin, chipSelectPin);

char *pass = "000000";

void setup() {
  oled.begin();
  Wire.begin();

  pinMode(leftPin, INPUT);
  pinMode(rightPin, INPUT);
  pinMode(upPin, INPUT);
  pinMode(downPin, INPUT);
  pinMode(centrePin, INPUT);
  
  Serial.begin(9600);
  delay(5000);
  Serial.println("starting");

  Wire.beginTransmission(0x00);
  Wire.endTransmission();
  delay(10);

  uint8_t data[1] = {0x00};

  // Verify write 7 password
  cryptoVerifyPassword(0x07, 0xE5, 0x47, 0x47);
  delay(10);
  
  // Dummy write to read PAC for write 7 password
  cryptoWriteSystem(0x00, 0xE8, 0, NULL);
  cryptoReadCmd(1, data);
  for(uint8_t n=0;n<1;n++) {
    Serial.println(data[n], HEX);
  }

  oled.setFontType(1);
  drawLetterSelect();
  
  
}

int scrollInterval = 0;
unsigned long lastScroll = 0;
bool leftWasPressed = false;
bool rightWasPressed = false;
byte charid = 0;

void loop() {
  readButtons();

  if(upPressed) {
    if(millis()-lastScroll>scrollInterval) {
      pass[charid] = wrapChar(pass[charid]+1);
      lastScroll = millis();

      if(scrollInterval == 0) {
        scrollInterval = 500;
      }else {
        scrollInterval = 150;
      }
      drawLetterSelect();
    }
  }else if(downPressed) {
    if(millis()-lastScroll>scrollInterval) {
      pass[charid] = wrapChar(pass[charid]-1);
      lastScroll = millis();

      if(scrollInterval == 0) {
        scrollInterval = 500;
      }else {
        scrollInterval = 150;
      }
      drawLetterSelect();
    }
  }else{
    scrollInterval = 0;
    
  }

  if(leftPressed && !leftWasPressed) {
    leftWasPressed = true;
    if(charid>0) charid--;
    drawLetterSelect();
  }else if(!leftPressed) {
    leftWasPressed = false;
  }

  if(rightPressed && !rightWasPressed) {
    rightWasPressed = true;
    if(charid<5) charid++;
    drawLetterSelect();
  }else if(!rightPressed) {
    rightWasPressed = false;
  }
  
  
}

void cryptoWriteCmd(uint8_t cmd, uint8_t addr1, uint8_t addr2, uint8_t n, uint8_t* data) {
  Wire.beginTransmission(cmd>>1);
  Wire.write(addr1);
  Wire.write(addr2);
  Wire.write(n);
  for(uint8_t i=0;i<n;i++) {
    Wire.write(data[i]);
  }
  uint8_t s = Wire.endTransmission();
  if(s!=0) {
    Serial.print("Write error:");
    Serial.println(s);
  }
  delay(5);
}

void cryptoWriteUser(uint8_t addr1, uint8_t addr2, uint8_t n, uint8_t *data) {
  cryptoWriteCmd(0xB0, addr1, addr2, n, data);
}

void cryptoWriteSystem(uint8_t addr1, uint8_t addr2, uint8_t n, uint8_t *data) {
  cryptoWriteCmd(0xB4, addr1, addr2, n, data);
}

void cryptoVerifyPassword(uint8_t idx, uint8_t p0, uint8_t p1, uint8_t p2) {
  uint8_t data[3];
  data[0] = p0; data[1] = p1; data[2] = p2;
  cryptoWriteCmd(0xBA, idx, 0x00, 3, data);
  // Dummy poll command
  cryptoWriteCmd(0xB6, 0x00, 0x00, 0, NULL);
}

void cryptoReadCmd(uint8_t n, uint8_t *data) {
  // Read the data out
  Wire.requestFrom(0xB0>>1, n);
  while(Wire.available()) {
    *(data++) = Wire.read();
  }
}

void drawLetterSelect() {
  char tmp[] = "******";
  tmp[charid] = pass[charid];
  oled.clear(PAGE);
  oled.setCursor(0,16);
  oled.print(tmp);
  //oled.line(charid*(oled.getFontWidth()+1),15+oled.getFontHeight(), charid*(oled.getFontWidth()+1)+oled.getFontWidth()-2, 15+oled.getFontHeight());
  
  oled.display();
}


unsigned long lastClickChange = 0;

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

char wrapChar(char c) {
  while(c<'0') c+=10;
  while(c>'9') c-=10;
  return c;
}

