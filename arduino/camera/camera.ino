#include <Arduino_OV767X.h>

const int width = 176;
const int height = 144;

unsigned short pixels[width * height];

void setup() {
  Serial.begin(9600);
  while (!Serial);

  Serial.println("ov7670 test");

  if (!Camera.begin(QCIF, RGB565, 1)) {
    Serial.println("Failed to init cam");
  }

  Serial.println("init cam done");

}

void loop() {
  if (Serial.read() == 'c') {
    Camera.readFrame(pixels);
    for (int i = 0; i < width * height; i++) {
      unsigned short p = pixels[i];

      if (p < 0x1000) Serial.print('0');
      if (p < 0x0100) Serial.print('0');
      if (p < 0x0010) Serial.print('0');
      Serial.print(p, HEX);
    }
    Serial.println(" ");
  }
}