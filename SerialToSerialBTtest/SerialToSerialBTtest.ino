#include <BluetoothSerial.h>
#include "esp_camera.h"

BluetoothSerial SerialBT;

camera_config_t config;
camera_fb_t *fb = NULL;

void sendImage(uint8_t *imgBuf, size_t imgSize) {
  // Send the image size to the connected device
  SerialBT.write((const uint8_t *)&imgSize, sizeof(imgSize));

  // Send the image data to the connected device
  SerialBT.write(imgBuf, imgSize);
}

void setup() {
  Serial.begin(115200);
  SerialBT.begin("tester"); // Set the Bluetooth device name

  config.ledc_channel = LEDC_CHANNEL_0;
  config.ledc_timer = LEDC_TIMER_0;
  config.pin_d0 = 5; // Y2
  config.pin_d1 = 18; // Y3
  config.pin_d2 = 19; // Y4
  config.pin_d3 = 21; // Y5
  config.pin_d4 = 36; // Y6
  config.pin_d5 = 39; // Y7
  config.pin_d6 = 34; // Y8
  config.pin_d7 = 35; // Y9
  config.pin_xclk = 0; // XCLK
  config.pin_pclk = 22; // PCLK
  config.pin_vsync = 25; // VSYNC
  config.pin_href = 23; // HREF
  config.pin_sscb_sda = 26; // SIOD
  config.pin_sscb_scl = 27; // SIOC
  config.pin_pwdn = 32; // PWDN
  config.pin_reset = -1; // RESET (not connected)
  config.xclk_freq_hz = 20000000;
  config.pixel_format = PIXFORMAT_JPEG;
  config.frame_size = FRAMESIZE_UXGA;
  config.jpeg_quality = 10;
  config.fb_count = 2;

  esp_err_t err = esp_camera_init(&config);
  if (err != ESP_OK) {
    Serial.printf("Camera initialization failed with error 0x%x", err);
    return;
  }

  Serial.println("Camera Initialized");
  SerialBT.println("Bluetooth Initialized");
}

void loop() {
  // Check if data is available to read from Bluetooth Serial
  if (SerialBT.available()) {
    // Read any incoming data (if required)

    // Capture an image
    fb = esp_camera_fb_get();
    if (!fb) {
      Serial.println("Camera capture failed");
      return;
    }

    // Send the captured image
    sendImage(fb->buf, fb->len);

    // Release the frame
    esp_camera_fb_return(fb);
  }

  // Other code or delay if needed
  delay(1000); // Capture and send an image every 1 second
}
