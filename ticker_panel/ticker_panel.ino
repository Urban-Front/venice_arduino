// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
// Our text
#include <Fonts/FreeSansBold9pt7b.h>
// Our matrix code
#include <Adafruit_NeoMatrix.h>
// Our neopixel code
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

// Our pin to send data from
#define PIN 6

// In this application we'd like to use it as a 60x13 matrix,
// with the first pixel at the top left. 
// Lines are arranged in zig zag order.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(60, 13, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

// Our message – can be changed
String message = "ENVIRONMENTAL DEGRADATION";
// Our submessage – always empty to begin with
String subMessage = "";
// Whether our arrow is red or not
bool isRed = false;
// Our colors
uint16_t red = matrix.Color(255, 20, 60);
uint16_t green = matrix.Color(20, 255, 60);
uint16_t yellow = matrix.Color(240, 240, 40);
uint16_t white = matrix.Color(255, 255, 255);
// This is always going to be 60
int w = matrix.width();
// Price – which we use to determine up/down arrow as well
float price = 0.0;
// This might need to be adjusted manually,
// because our font isn't monospaced,
// but it's how many pixels long our message is
int pixelsInMessage = (message.length() * 12) + 11;
// This is the total pixels in our text, including submessage
int pixelsPlusWidth = pixelsInMessage + w;
// Offset: how many panels we are from the far right
int offset = w * 0;

void setup() {
  matrix.begin();
  matrix.clear();
  matrix.setTextWrap(false);
  matrix.setFont(&FreeSansBold9pt7b);
  matrix.setTextColor(matrix.Color(255, 255, 255));
  matrix.setTextSize(1);
  matrix.setBrightness(8);
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(230400);
}

void loop() {
  if (Serial.available() > 0) {
    String data = Serial.readStringUntil('\n');
    int counter = data.substring(0, data.indexOf(':')).toInt();
    float newPrice = data.substring(data.indexOf(':') + 1).toFloat();
    if (newPrice != price) {
      if (newPrice < price) {
        isRed = true;
      } else {
        isRed = false;
      }
    }
    price = newPrice;
    Serial.print("Data: ");
    Serial.println(data);
    // x = counter % totalPixelsInText;
    int x = -counter + offset;
    matrix.fillScreen(0);
    matrix.setCursor(x + w, 6);
    matrix.setTextColor(white);
    matrix.setFont(&FreeSansBold9pt7b);
    matrix.print(message);
    matrix.setCursor(x + pixelsPlusWidth + 12, 6);
    if (isRed) { // Going down
      for (int i = 1; i < 5; i++) {
        matrix.drawFastHLine(x + pixelsPlusWidth - i + 6, 5 - i + 1, i, red);
      }
      for (int j = 1; j < 5; j++) {
        matrix.drawFastHLine(x + pixelsPlusWidth + 6, 5 - j + 1, j, red);
      }
      matrix.setTextColor(yellow);
    } else { // Going up
      for (int i = 1; i < 5; i++) {
        matrix.drawFastHLine(x + pixelsPlusWidth - i + 6, i + 1, i, green);
      }
      for (int j = 1; j < 5; j++) {
        matrix.drawFastHLine(x + pixelsPlusWidth + 6, j + 1, j, green);
      }
      matrix.setTextColor(yellow);
    }
    matrix.setFont();
    matrix.print(String(price + subMessage));
    matrix.show();
  }
}
