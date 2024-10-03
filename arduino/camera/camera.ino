#include "ov7670.h"

const int imageWidth = 160;  // Width of the image in pixels
const int imageHeight = 120; // Height of the image in pixels

uint8_t imageBuffer[imageWidth * imageHeight] = { 0 }; // Buffer to hold the image data

void captureImage() {
  int pixelIndex = 0;

  // Wait for VSYNC to start (frame sync)
  Serial.println("VSYNC HIGH BEFORE");
  while (digitalRead(CAM_PIN_VSNC) == HIGH);  // Wait until VSYNC goes LOW (frame start)
  Serial.println("VSYNC HIGH => LOW");
  while (digitalRead(CAM_PIN_VSNC) == LOW);   // Wait until VSYNC goes HIGH (frame end)
  Serial.println("VSYNC LOW AFTER");

  // Now we are in a frame, start reading each line
  for (int row = 0; row < imageHeight; row++) {
    // Wait for HREF to indicate the start of a row
    while (digitalRead(CAM_PIN_HREF) == LOW);  // Wait for HREF to go HIGH (row start)

    for (int col = 0; col < imageWidth; col++) {
      // Wait for the rising edge of PCLK (pixel clock)
      while (digitalRead(CAM_PIN_PCLK) == LOW);  // Wait for PCLK to go HIGH

      // Read the data pins (D0-D7) to get the pixel data
      uint8_t pixelValue = (digitalRead(CAM_PIN_D0) << 0) |
                           (digitalRead(CAM_PIN_D1) << 1) |
                           (digitalRead(CAM_PIN_D2) << 2) |
                           (digitalRead(CAM_PIN_D3) << 3) |
                           (digitalRead(CAM_PIN_D4) << 4) |
                           (digitalRead(CAM_PIN_D5) << 5) |
                           (digitalRead(CAM_PIN_D6) << 6) |
                           (digitalRead(CAM_PIN_D7) << 7);

      // Store the pixel value in the image buffer
      imageBuffer[pixelIndex++] = pixelValue;

      // Wait for the falling edge of PCLK (pixel clock)
      while (digitalRead(CAM_PIN_PCLK) == HIGH);  // Wait for PCLK to go LOW
    }

    // Wait for HREF to go LOW (end of row)
    while (digitalRead(CAM_PIN_HREF) == HIGH);
  }
}

void setup() {
  Serial.begin(9600);

  // Set pin modes for camera input signals
  pinMode(CAM_PIN_PCLK, INPUT);
  pinMode(CAM_PIN_HREF, INPUT);
  pinMode(CAM_PIN_VSNC, INPUT);

  // Set pin modes for camera data pins (D0-D7)
  pinMode(CAM_PIN_D0, INPUT);
  pinMode(CAM_PIN_D1, INPUT);
  pinMode(CAM_PIN_D2, INPUT);
  pinMode(CAM_PIN_D3, INPUT);
  pinMode(CAM_PIN_D4, INPUT);
  pinMode(CAM_PIN_D5, INPUT);
  pinMode(CAM_PIN_D6, INPUT);
  pinMode(CAM_PIN_D7, INPUT);
}

void loop() {
  Serial.println("CaptureStart");
  delay(500);
  captureImage();
  Serial.println("CaptureDone");
  // Process or send the imageBuffer data over Serial, BLE, etc.

  
  for (int i = 0; i < imageWidth * imageHeight; i++) {
    Serial.print(imageBuffer[i]);
  }
  // delay(1000);
  
}
