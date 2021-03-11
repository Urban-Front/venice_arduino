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
int pixelsInMessage = (message.length() * 10) + 8;
int x = matrix.width();
int count = 0;

void setup() {
  matrix.begin();
  matrix.clear();
  matrix.setTextWrap(false);
  matrix.setFont(&FreeSansBold9pt7b);
  matrix.setTextColor(matrix.Color(255, 255, 255));
  matrix.setTextSize(1);
  matrix.setBrightness(4);
}

void loop() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 10);
  matrix.print(message);
  if(--x < -pixelsInMessage) {
    count++;
    x = matrix.width();
    if (count % 2 == 0) {
      matrix.setTextColor(matrix.Color(255, 40, 10));
    } else {
      matrix.setTextColor(matrix.Color(255, 255, 255));
    }
  }
  matrix.show();
  delay(50);
}
