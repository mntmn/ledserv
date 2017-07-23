#include <Adafruit_NeoPixel.h>
#include <avr/power.h>

#define PIN 6
Adafruit_NeoPixel strip = Adafruit_NeoPixel(60, PIN, NEO_GRBW + NEO_KHZ800);

void setup() {
  strip.begin();
  strip.show();
  Serial.begin(57600);
}

int mode = 0;
int cmd_i = 0;
int fxi = 0;
int fxj = 0;
char cmd[66];

void loop() {
  delay(10);
  fxi++;
  fxi&=0xff;
  
  switch (mode) {
    case 0:
      colorWipe(strip.Color(0, 0, 0));
      break;
    case 1:
      rainbow();
      break;
    case 2:
      colorWipe(strip.Color(255, 255, 255));
      break;
    case 3:
      colorWipe(strip.Color(255, 0, 0));
      break;
    case 4:
      colorWipe(strip.Color(0, 255, 0));
      break;
    case 5:
      colorWipe(strip.Color(0, 0, 255));
      break;
    case 6:
      colorWipe(strip.Color(255, 255, 0));
      break;
    case 7:
      colorWipe(strip.Color(255, 0, 255));
      break;
    case 8:
      colorWipe(strip.Color(0, 255, 255));
      break;
  }
  
  if (cmd_i>64) cmd_i = 0;

  if (Serial.available()>0) {
    char chr = Serial.read();
    cmd[cmd_i++] = chr;
    cmd[cmd_i] = 0;
    Serial.print(chr);
    if (chr == '\r') {
      cmd[cmd_i-1] = 0;
      cmd_i = 0;
      fxj = 0;
      
      Serial.println("[");
      Serial.print(cmd);
      Serial.println("]");
      if (!strcmp(cmd, "off")) {
        mode = 0;
      }
      else if (!strcmp(cmd, "rainbow")) {
        mode = 1;
      }
      else if (!strcmp(cmd, "white")) {
        mode = 2;
      }
      else if (!strcmp(cmd, "red")) {
        mode = 3;
      }
      else if (!strcmp(cmd, "green")) {
        mode = 4;
      }
      else if (!strcmp(cmd, "blue")) {
        mode = 5;
      }
      else if (!strcmp(cmd, "yellow")) {
        mode = 6;
      }
      else if (!strcmp(cmd, "purple")) {
        mode = 7;
      }
      else if (!strcmp(cmd, "cyan")) {
        mode = 8;
      }
    }
  }
}

void colorWipe(uint32_t c) {
  if (fxj<strip.numPixels()) {
    strip.setPixelColor(fxj, c);
    strip.show();
    fxj++;
  }
}

void rainbow() {
  for(uint16_t i=0; i<strip.numPixels(); i++) {
    strip.setPixelColor(i, Wheel((i+fxi) & 255));
  }
  strip.show();
}

// Input a value 0 to 255 to get a color value.
// The colours are a transition r - g - b - back to r.
uint32_t Wheel(byte WheelPos) {
  WheelPos = 255 - WheelPos;
  if(WheelPos < 85) {
   return strip.Color(255 - WheelPos * 3, 0, WheelPos * 3);
  } else if(WheelPos < 170) {
    WheelPos -= 85;
   return strip.Color(0, WheelPos * 3, 255 - WheelPos * 3);
  } else {
   WheelPos -= 170;
   return strip.Color(WheelPos * 3, 255 - WheelPos * 3, 0);
  }
}

