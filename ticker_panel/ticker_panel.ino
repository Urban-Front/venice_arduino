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

// CONSTANTS FOR WORDS
#define NUM_ITEMS 2
#define WORD_GAP 120

// In this application we'd like to use it as a 60x13 matrix,
// with the first pixel at the top left. 
// Lines are arranged in zig zag order.
Adafruit_NeoMatrix matrix = Adafruit_NeoMatrix(60, 13, PIN,
  NEO_MATRIX_TOP     + NEO_MATRIX_LEFT +
  NEO_MATRIX_ROWS + NEO_MATRIX_ZIGZAG,
  NEO_GRB            + NEO_KHZ800);

// Our message – can be changed
String messages[2] = {"ENVIRONMENTAL DESTRUCTION", "PATRIARCHY"};
// Our submessage – always empty to begin with
String subMessages[2] = {"", ""};
// Whether our arrow is red or not
bool isReds[2] = {false, false};
// Our colors
uint16_t red = matrix.Color(255, 20, 60);
uint16_t green = matrix.Color(20, 255, 60);
uint16_t yellow = matrix.Color(240, 240, 40);
uint16_t white = matrix.Color(255, 255, 255);
// This is always going to be 60
int w = matrix.width();
// Price – which we use to determine up/down arrow as well
float prices[2] = {0.0, 0.0};
float newPrices[2] = {0.0, 0.0};
// This might need to be adjusted manually,
// because our font isn't monospaced,
// but it's how many pixels long our message is
int pixelsInMessages[2] = {messages[0].length() * 12, messages[1].length() * 12};
// This is our pixels plus the width of the panel
int pixelsPlusWidths[2] = {pixelsInMessages[0] + w, pixelsInMessages[1] + w};
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
    Serial.print("Data: ");
    Serial.println(data);
    long int counter = data.substring(0, data.indexOf(':')).toInt();
    for (int i = 0; i < NUM_ITEMS; i++) {
      newPrices[i] = data.substring(data.indexOf(':') + 1).toFloat();
      if (newPrices[i] != prices[i]) {
        if (newPrices[i] < prices[i]) {
          isReds[i] = true;
        } else {
          isReds[i] = false;
        }
      }
      prices[i] = newPrices[i];
    }
    int modulo = offset;
    for (int i = 0; i < NUM_ITEMS; i++) {
      modulo += pixelsInMessages[i] + 24 + WORD_GAP;
    }
    int x = (-counter + offset) % -modulo;
    // Blank out the screen
    matrix.fillScreen(0);
    // Draw the first word
    drawWord(x, 0);
    // Draw the second word
    drawWord(x, 1);
    // SHOW THE MATRIX
    matrix.show();
  }
}

void drawWord(int x, int wordNum) {
  // WORD DRAWING
  // Set the cursor at the start
  int cursorStart = x + w;
  for (int i = 0; i < wordNum; i++) {
    cursorStart += pixelsInMessages[i] + WORD_GAP;
  }
  matrix.setCursor(cursorStart, 6);
  matrix.setTextColor(white);
  matrix.setFont(&FreeSansBold9pt7b);
  matrix.print(messages[wordNum]);
  // Draw the arrow up/down
  if (isReds[wordNum]) { // Going down
    for (int i = 1; i < 5; i++) {
      matrix.drawFastHLine(cursorStart + pixelsInMessages[wordNum] - i + 6, 5 - i + 1, i, red);
    }
    for (int j = 1; j < 5; j++) {
      matrix.drawFastHLine(cursorStart + pixelsInMessages[wordNum] + 6, 5 - j + 1, j, red);
    }
    matrix.setTextColor(yellow);
  } else { // Going up
    for (int i = 1; i < 5; i++) {
      matrix.drawFastHLine(cursorStart + pixelsInMessages[wordNum] - i + 6, i + 1, i, green);
    }
    for (int j = 1; j < 5; j++) {
      matrix.drawFastHLine(cursorStart + pixelsInMessages[wordNum] + 6, j + 1, j, green);
    }
    matrix.setTextColor(yellow);
  }
  // Set the cursor to the right of the message
  matrix.setCursor(cursorStart + pixelsInMessages[wordNum] + 12, 6);
  matrix.setTextColor(white);
  matrix.setFont();
  matrix.print(String(prices[wordNum] + subMessages[wordNum]));
}
