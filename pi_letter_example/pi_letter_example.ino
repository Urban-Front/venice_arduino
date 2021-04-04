// Adafruit_NeoMatrix example for single NeoPixel Shield.
// Scrolls 'Howdy' across the matrix in a portrait (vertical) orientation.

#include <Adafruit_GFX.h>    // Core graphics library
#include <Adafruit_TFTLCD.h> // Hardware-specific library
// 13 lines tall for text size 1
#include <Fonts/FreeSansBold9pt7b.h>
// 11 lines tall for text size 1
// #include <Fonts/FreeMonoBold9pt7b.h>
#include <Adafruit_NeoMatrix.h>
#include <Adafruit_NeoPixel.h>
#ifndef PSTR
 #define PSTR // Make Arduino Due happy
#endif

#define PIN 6

// MATRIX DECLARATION:
// Parameter 1 = width of NeoPixel matrix
// Parameter 2 = height of matrix
// Parameter 3 = pin number (most are valid)
// Parameter 4 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the matrix; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs are arranged in horizontal
//     rows or in vertical columns, respectively; pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns proceed
//     in the same order, or alternate lines reverse direction; pick one.
//   See example below for these values in action.
// Parameter 5 = pixel type flags, add together as needed:
//   NEO_KHZ800  800 KHz bitstream (most NeoPixel products w/WS2812 LEDs)
//   NEO_KHZ400  400 KHz (classic 'v1' (not v2) FLORA pixels, WS2811 drivers)
//   NEO_GRB     Pixels are wired for GRB bitstream (most NeoPixel products)
//   NEO_GRBW    Pixels are wired for GRBW bitstream (RGB+W NeoPixel products)
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 FLORA pixels, not v2)


// In this application we'd like to use it as a 60x13 matrix,
// with the first pixel at the top left. 
// Lines are arranged in zig zag order.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(60, 13, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

String message = "PATRIARCHY";
int percent = 23;
String subMessage = "";
bool isRed = false;
uint16_t red = matrix.Color(255, 20, 60);
uint16_t green = matrix.Color(20, 255, 60);
uint16_t yellow = matrix.Color(240, 240, 40);
uint16_t white = matrix.Color(255, 255, 255);
int pixelsInMessage = (message.length() * 10) + 22;
int w = matrix.width();
int x = 0;
int counter = 0;
float price = 0.0;

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
    counter = data.substring(0, data.indexOf(':')).toInt();
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
    x = counter % (pixelsInMessage + 24 + w);
    matrix.fillScreen(0);
    matrix.setCursor(-x + w, 6);
    matrix.setTextColor(white);
    matrix.setFont(&FreeSansBold9pt7b);
    matrix.print(message);
    matrix.setCursor(-x + w + pixelsInMessage + 12, 6);
    if (isRed) { // Going down
      for (int i = 1; i < 5; i++) {
        matrix.drawFastHLine(-x + w + pixelsInMessage - i + 6, 5 - i + 1, i, red);
      }
      for (int j = 1; j < 5; j++) {
        matrix.drawFastHLine(-x + w + pixelsInMessage + 6, 5 - j + 1, j, red);
      }
      matrix.setTextColor(yellow);
    } else { // Going up
      for (int i = 1; i < 5; i++) {
        matrix.drawFastHLine(-x + w + pixelsInMessage - i + 6, i + 1, i, green);
      }
      for (int j = 1; j < 5; j++) {
        matrix.drawFastHLine(-x + w + pixelsInMessage + 6, j + 1, j, green);
      }
      matrix.setTextColor(yellow);
    }
    matrix.setFont();
    matrix.print(String(price + subMessage));
    matrix.show();
  }
}
