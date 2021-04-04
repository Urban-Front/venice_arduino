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
// Parameter 1 = width of EACH NEOPIXEL MATRIX (not total display)
// Parameter 2 = height of each matrix
// Parameter 3 = number of matrices arranged horizontally
// Parameter 4 = number of matrices arranged vertically
// Parameter 5 = pin number (most are valid)
// Parameter 6 = matrix layout flags, add together as needed:
//   NEO_MATRIX_TOP, NEO_MATRIX_BOTTOM, NEO_MATRIX_LEFT, NEO_MATRIX_RIGHT:
//     Position of the FIRST LED in the FIRST MATRIX; pick two, e.g.
//     NEO_MATRIX_TOP + NEO_MATRIX_LEFT for the top-left corner.
//   NEO_MATRIX_ROWS, NEO_MATRIX_COLUMNS: LEDs WITHIN EACH MATRIX are
//     arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_MATRIX_PROGRESSIVE, NEO_MATRIX_ZIGZAG: all rows/columns WITHIN
//     EACH MATRIX proceed in the same order, or alternate lines reverse
//     direction; pick one.
//   NEO_TILE_TOP, NEO_TILE_BOTTOM, NEO_TILE_LEFT, NEO_TILE_RIGHT:
//     Position of the FIRST MATRIX (tile) in the OVERALL DISPLAY; pick
//     two, e.g. NEO_TILE_TOP + NEO_TILE_LEFT for the top-left corner.
//   NEO_TILE_ROWS, NEO_TILE_COLUMNS: the matrices in the OVERALL DISPLAY
//     are arranged in horizontal rows or in vertical columns, respectively;
//     pick one or the other.
//   NEO_TILE_PROGRESSIVE, NEO_TILE_ZIGZAG: the ROWS/COLUMS OF MATRICES
//     (tiles) in the OVERALL DISPLAY proceed in the same order for every
//     line, or alternate lines reverse direction; pick one.  When using
//     zig-zag order, the orientation of the matrices in alternate rows
//     will be rotated 180 degrees (this is normal -- simplifies wiring).
//   See example below for these values in action.
// Parameter 7 = pixel type flags, add together as needed:
//   NEO_RGB     Pixels are wired for RGB bitstream (v1 pixels)
//   NEO_GRB     Pixels are wired for GRB bitstream (v2 pixels)
//   NEO_KHZ400  400 KHz bitstream (e.g. FLORA v1 pixels)
//   NEO_KHZ800  800 KHz bitstream (e.g. High Density LED strip)
//   For Spark Core developement it should probably also be WS2812B if you're
//   using adafruit neopixels.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(60, 7, 2, 1, PIN,
  NEO_TILE_TOP   + NEO_TILE_LEFT   + NEO_TILE_COLUMNS   + NEO_TILE_ZIGZAG  +
  NEO_MATRIX_TOP + NEO_MATRIX_LEFT + NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB + NEO_KHZ800);

String message = "PATRIARCHY";
int percent = 23;
String subMessage = "%";
bool isRed = false;
uint16_t red = matrix.Color(255, 20, 60);
uint16_t green = matrix.Color(20, 255, 60);
uint16_t yellow = matrix.Color(240, 240, 40);
uint16_t white = matrix.Color(255, 255, 255);
int pixelsInMessage = (message.length() * 10) + 22;
int x = matrix.width();
int count = 0;

void setup() {
  matrix.begin();
  matrix.clear();
  matrix.setTextWrap(false);
  matrix.setFont(&FreeSansBold9pt7b);
  matrix.setTextColor(matrix.Color(255, 255, 255));
  matrix.setTextSize(1);
  matrix.setBrightness(8);
  Serial.begin(9600);
}

void loop() {
  matrix.fillScreen(0);
  matrix.setCursor(x, 6);
  matrix.setTextColor(white);
  matrix.setFont(&FreeSansBold9pt7b);
  matrix.print(message);
  matrix.setCursor(x + pixelsInMessage + 12, 6);
  Serial.println(x);
  if (isRed) { // Going down
    for (int i = 1; i < 5; i++) {
      matrix.drawFastHLine(x + pixelsInMessage - i + 6, 5 - i + 1, i, red);
    }
    for (int j = 1; j < 5; j++) {
      matrix.drawFastHLine(x + pixelsInMessage + 6, 5 - j + 1, j, red);
    }
    matrix.setTextColor(yellow);
  } else { // Going up
    for (int i = 1; i < 5; i++) {
      matrix.drawFastHLine(x + pixelsInMessage - i + 6, i + 1, i, green);
    }
    for (int j = 1; j < 5; j++) {
      matrix.drawFastHLine(x + pixelsInMessage + 6, j + 1, j, green);
    }
    matrix.setTextColor(yellow);
  }
  matrix.setFont();
  matrix.print(String(percent + subMessage));
  if(--x < -pixelsInMessage - 24) {
    count++;
    int ran = int(random(-8, 10));
    if (ran < 0) {
      isRed = true;
    } else {
      isRed = false;
    }
    percent = constrain(percent + ran, -99, 99);
    x = matrix.width();
  }
  matrix.show();
  delay(10);
}
