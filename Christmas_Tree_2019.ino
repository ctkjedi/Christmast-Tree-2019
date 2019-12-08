#include <FastLED.h>

#define BRIGHTNESS  128
#define LED_TYPE    WS2812
#define COLOR_ORDER GRB
#define NUM_LEDS_PER_STRIP  60
#define ARRAY_SIZE(A) (sizeof(A) / sizeof((A)[0]))

extern const TProgmemRGBGradientPalettePtr gGradientPalettes[];
const uint8_t kMatrixWidth  = 5;
const uint8_t kMatrixHeight = 60;
const bool    kMatrixSerpentineLayout = true;

int stripCount = 5;
float currSpeed = .05;
int totalDrips = 10;
int leds_done = 0;
int offset = 0;

static const uint32_t strip1Array[] PROGMEM = { 0,   1,   2,   3,   4,   5,   6,   7,   8,   9,  10,  11,  12,  13,  14,  15,  16,  17,  18,  19,  20,  21,  22,  23,  24,  25,  26,  27,  28,  29,  30,  31,  32,  33,  34,  35,  36,  37,  38,  39,  40,  41,  42,  43,  44,  45,  46,  47,  48,  49,  50,  51,  52,  53,  54,  55,  56,  57,  58,  59 };
static const uint32_t strip2Array[] PROGMEM = { 60,  61,  62,  63,  64,  65,  66,  67,  68,  69,  70,  71,  72,  73,  74,  75,  76,  77,  78,  79,  80,  81,  82,  83,  84,  85,  86,  87,  88,  89,  90,  91,  92,  93,  94,  95,  96,  97,  98,  99, 100, 101, 102, 103, 104, 105, 106, 107, 108, 109, 110, 111, 112, 113, 114, 115, 116, 117, 118, 119 };
static const uint32_t strip3Array[] PROGMEM = { 120, 121, 122, 123, 124, 125, 126, 127, 128, 129, 130, 131, 132, 133, 134, 135, 136, 137, 138, 139, 140, 141, 142, 143, 144, 145, 146, 147, 148, 149, 150, 151, 152, 153, 154, 155, 156, 157, 158, 159, 160, 161, 162, 163, 164, 165, 166, 167, 168, 169, 170, 171, 172, 173, 174, 175, 176, 177, 178, 179 };
static const uint32_t strip4Array[] PROGMEM = { 180, 181, 182, 183, 184, 185, 186, 187, 188, 189, 190, 191, 192, 193, 194, 195, 196, 197, 198, 199, 200, 201, 202, 203, 204, 205, 206, 207, 208, 209, 210, 211, 212, 213, 214, 215, 216, 217, 218, 219, 220, 221, 222, 223, 224, 225, 226, 227, 228, 229, 230, 231, 232, 233, 234, 235, 236, 237, 238, 239 };
static const uint32_t strip5Array[] PROGMEM = { 240, 241, 242, 243, 244, 245, 246, 247, 248, 249, 250, 251, 252, 253, 254, 255, 256, 257, 258, 259, 260, 261, 262, 263, 264, 265, 266, 267, 268, 269, 270, 271, 272, 273, 274, 275, 276, 277, 278, 279, 280, 281, 282, 283, 284, 285, 286, 287, 288, 289, 290, 291, 292, 293, 294, 295, 296, 297, 298, 299 };


uint16_t serpentineArray[] = {
  0,  60, 120, 180, 240,
  1,  61, 121, 181, 241,
  2,  62, 122, 182, 242,
  3,  63, 123, 183, 243,
  4,  64, 124, 184, 244,
  5,  65, 125, 185, 245,
  6,  66, 126, 186, 246,
  7,  67, 127, 187, 247,
  8,  68, 128, 188, 248,
  9,  69, 129, 189, 249,
  10,  70, 130, 190, 250,
  11,  71, 131, 191, 251,
  12,  72, 132, 192, 252,
  13,  73, 133, 193, 253,
  14,  74, 134, 194, 254,
  15,  75, 135, 195, 255,
  16,  76, 136, 196, 256,
  17,  77, 137, 197, 257,
  18,  78, 138, 198, 258,
  19,  79, 139, 199, 259,
  20,  80, 140, 200, 260,
  21,  81, 141, 201, 261,
  22,  82, 142, 202, 262,
  23,  83, 143, 203, 263,
  24,  84, 144, 204, 264,
  25,  85, 145, 205, 265,
  26,  86, 146, 206, 266,
  27,  87, 147, 207, 267,
  28,  88, 148, 208, 268,
  29,  89, 149, 209, 269,
  30,  90, 150, 210, 270,
  31,  91, 151, 211, 271,
  32,  92, 152, 212, 272,
  33,  93, 153, 213, 273,
  34,  94, 154, 214, 274,
  35,  95, 155, 215, 275,
  36,  96, 156, 216, 276,
  37,  97, 157, 217, 277,
  38,  98, 158, 218, 278,
  39,  99, 159, 219, 279,
  40, 100, 160, 220, 280,
  41, 101, 161, 221, 281,
  42, 102, 162, 222, 282,
  43, 103, 163, 223, 283,
  44, 104, 164, 224, 284,
  45, 105, 165, 225, 285,
  46, 106, 166, 226, 286,
  47, 107, 167, 227, 287,
  48, 108, 168, 228, 288,
  49, 109, 169, 229, 289,
  50, 110, 170, 230, 290,
  51, 111, 171, 231, 291,
  52, 112, 172, 232, 292,
  53, 113, 173, 233, 293,
  54, 114, 174, 234, 294,
  55, 115, 175, 235, 295,
  56, 116, 176, 236, 296,
  57, 117, 177, 237, 297,
  58, 118, 178, 238, 298,
  59, 119, 179, 239, 299
};


#define NUM_LEDS (kMatrixWidth * kMatrixHeight)
#define MAX_DIMENSION ((kMatrixWidth>kMatrixHeight) ? kMatrixWidth : kMatrixHeight)

// The leds
CRGB leds[kMatrixWidth * kMatrixHeight];

// The 16 bit version of our coordinates
static uint16_t x;
static uint16_t y;
static uint16_t z;
uint8_t gHue = 0; // rotating "base color" used by many of the patterns

//  x/y dimensions to map to the x/y pixels on the matrix.
// z-axis for "time".  speed determines how fast time moves forward.
// 1 for a very slow moving effect, or 60 for something that ends up looking like water.
uint16_t speed = 1; // speed is set dynamically once we've started up

// Scale determines how far apart the pixels in our noise matrix are.  Try
// changing these values around to see how it affects the motion of the display.  The
// higher the value of scale, the more "zoomed out" the noise. A value of 1 will be so zoomed in, you'll mostly see solid colors.
uint16_t scale = 30; // scale is set dynamically once we've started up

// This is the array that we keep our computed noise values in
uint8_t noise[MAX_DIMENSION][MAX_DIMENSION];

CRGBPalette16 currentPalette( PartyColors_p );
CRGBPalette16 targetPalette( PartyColors_p );

uint8_t       colorLoop = 1;

//twinkle vars
unsigned long previousMillis = 0;
unsigned long currentMillis;
unsigned long twinkleMillis = 0;

//Custom class creates Drips object and stores position, age and which strip is resides on
class Drips {
    float stringPos; //positon on the strip
    uint16_t age;        //how old drip is to speed up
    uint8_t stripNumber;  //which strip the drip is on



  public:
    Drips(float yPos, uint16_t howOld, uint8_t whichStrip) {
      stringPos = yPos;
      age = howOld;
      stripNumber = whichStrip;
    }

    void updateDrip() {
      //if the current position is less than the last pixel on the strip, increase the position by
      //base speed multiplied by how old it is. Set that pixel's new color and increase it's age
      if (round(stringPos) < (NUM_LEDS_PER_STRIP * (stripNumber + 1)) && round(stringPos) < NUM_LEDS - 1) {
        float newPos = stringPos + (currSpeed * age);
        stringPos = newPos;
        leds[round(stringPos)] = CHSV(255, 0, random(128, 255));
        Serial.println((String)"stringPos " + newPos);
        if (age < 30) age++;
      } else {
        //otherwise run function to choose a new strip and reset it back to 0 position
        resetDrip();
      }
    }

    //Choose a new strip, set its position to the top of said strip and reset its age
    void resetDrip() {
      uint8_t sN = random(0, stripCount);
      age = 0;
      stripNumber = sN;
      stringPos = (sN * NUM_LEDS_PER_STRIP);
    }

    //A function to randomize drips across all strips
    void randomDrip() {
      uint8_t sN = random(0, stripCount);
      stripNumber = sN;
      stringPos = random(sN * NUM_LEDS_PER_STRIP, ( (sN + 1) * (NUM_LEDS_PER_STRIP) ) - 1);
      age = 0;
    }

};

//Array of 10 drips - would love to do this dynamically,
//looping through to create X amount based on totalDrips var
Drips drip[] = {
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0),
  Drips(0, 0, 0)
};

void setup() {
  delay(3000);
  //randomize all them drips
  for (int i = 0; i < totalDrips; i++) {
    drip[i].randomDrip();
  }
  Serial.begin(9600);
  FastLED.addLeds<WS2812, 21, GRB>(leds, 0, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, 19, GRB>(leds, NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, 17, GRB>(leds, 2 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, 14, GRB>(leds, 3 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.addLeds<WS2812, 13, GRB>(leds, 4 * NUM_LEDS_PER_STRIP, NUM_LEDS_PER_STRIP);
  FastLED.setBrightness(BRIGHTNESS);

  // Initialize our coordinates to some random values
  x = random16();
  y = random16();
  z = random16();
}



uint8_t gCurrentPatternNumber = 0; // Index number of which pattern is current

// Fill the x/y array of 8-bit noise values using the inoise8 function.
void fillnoise8() {
  // If we're runing at a low "speed", some 8-bit artifacts become visible
  // from frame-to-frame.  In order to reduce this, we can do some fast data-smoothing.
  // The amount of data smoothing we're doing depends on "speed".
  uint8_t dataSmoothing = 0;
  if ( speed < 50) {
    dataSmoothing = 200 - (speed * 4);
  }

  for (int i = 0; i < MAX_DIMENSION; i++) {
    int ioffset = scale * i;
    for (int j = 0; j < MAX_DIMENSION; j++) {
      int joffset = scale * j;

      uint8_t data = inoise8(x + ioffset, y + joffset, z);

      // The range of the inoise8 function is roughly 16-238.
      // These two operations expand those values out to roughly 0..255
      // You can comment them out if you want the raw noise data.
      data = qsub8(data, 16);
      data = qadd8(data, scale8(data, 39));

      if ( dataSmoothing ) {
        uint8_t olddata = noise[i][j];
        uint8_t newdata = scale8( olddata, dataSmoothing) + scale8( data, 256 - dataSmoothing);
        data = newdata;
      }

      noise[i][j] = data;
    }
  }

  z += speed;

  // apply slow drift to X and Y, just for visual variation.
  x += speed / 8;
  y -= speed / 16;
}

void mapNoiseToLEDsUsingPalette() {
  static uint8_t ihue = 0;

  for (int i = 0; i < kMatrixWidth; i++) {
    for (int j = 0; j < kMatrixHeight; j++) {
      // We use the value at the (i,j) coordinate in the noise
      // array for our brightness, and the flipped value from (j,i)
      // for our pixel's index into the color palette.

      uint8_t index = noise[j][i];
      uint8_t bri =   noise[i][j];

      // if this palette is a 'loop', add a slowly-changing base value
      if ( colorLoop) {
        index += ihue;
      }

      // brighten up, as the color palette itself often contains the
      // light/dark dynamic range desired
      if ( bri > 127 ) {
        bri = 255;
      } else {
        bri = dim8_raw( bri * 2);
      }

      CRGB color = ColorFromPalette( currentPalette, index, bri);
      leds[XY(i, j)] = color;
    }
  }

  ihue += 1;
}


typedef void (*SimplePatternList[])();
SimplePatternList gPatterns = {
  &dripPattern,
  &showCandyCane,
  &runNoise,
  &rainbowFalls
};


void loop() {
  currentMillis = millis();
  /*EVERY_N_MILLISECONDS( 20 ) {
    gHue++;
    }*/

  gPatterns[gCurrentPatternNumber]();
  //dripPattern();

  EVERY_N_SECONDS( 12 ) {
    nextPattern();  // change patterns periodically
  }

}

void nextPattern() {
  // add one to the current pattern number, and wrap around at the end
  gCurrentPatternNumber = (gCurrentPatternNumber + 1) % ARRAY_SIZE( gPatterns);
}

void runNoise() {
  // Periodically choose a new palette, speed, and scale
  ChangePaletteAndSettingsPeriodically();

  uint8_t maxChanges = 16;
  nblendPaletteTowardPalette( currentPalette, targetPalette, maxChanges);

  // generate noise data
  fillnoise8();

  // convert the noise data to colors in the LED array using the current palette
  mapNoiseToLEDsUsingPalette();

  twinkle();
  FastLED.show();
}



// There are several different palettes of colors demonstrated here.
//
// FastLED provides several 'preset' palettes: RainbowColors_p, RainbowStripeColors_p,
// OceanColors_p, CloudColors_p, LavaColors_p, ForestColors_p, and PartyColors_p.
//
// Additionally, you can manually define your own color palettes, or you can write
// code that creates color palettes on the fly.

// 1 = 5 sec per palette
// 2 = 10 sec per palette
// etc
#define HOLD_PALETTES_X_TIMES_AS_LONG 3
#define PALETTE_SPEED 3
#define PALETTE_SCALE 100

void ChangePaletteAndSettingsPeriodically() {
  uint8_t secondHand = ((millis() / 1000) / HOLD_PALETTES_X_TIMES_AS_LONG) % 60;
  static uint8_t lastSecond = 99;

  if ( lastSecond != secondHand) {
    lastSecond = secondHand;
    if ( secondHand ==  0)  {
      targetPalette  = gGradientPalettes[2];
      speed = PALETTE_SPEED;
      scale = PALETTE_SCALE;
      colorLoop = 1;
    }
    if ( secondHand ==  7)  {
      targetPalette  = gGradientPalettes[0];
      speed = PALETTE_SPEED;
      scale = PALETTE_SCALE;
      colorLoop = 1;
    }
    if ( secondHand == 14)  {
      targetPalette  = gGradientPalettes[5];
      speed = PALETTE_SPEED;
      scale = PALETTE_SCALE;
      colorLoop = 1;
    }
    if ( secondHand == 21)  {
      targetPalette  = gGradientPalettes[1];
      speed = PALETTE_SPEED;
      scale = PALETTE_SCALE;
      colorLoop = 0;
    }
    if ( secondHand == 28)  {
      targetPalette  = CloudColors_p;
      speed = PALETTE_SPEED;
      scale = PALETTE_SCALE;
      colorLoop = 0;
    }
    if ( secondHand == 35)  {
      targetPalette  = LavaColors_p;
      speed = PALETTE_SPEED;
      scale = PALETTE_SCALE;
      colorLoop = 0;
    }
    if ( secondHand == 42)  {
      targetPalette  = gGradientPalettes[3];
      speed = PALETTE_SPEED;
      scale = PALETTE_SCALE;
      colorLoop = 0;
    }
    if ( secondHand == 49)  {
      targetPalette  = PartyColors_p;
      speed = PALETTE_SPEED;
      scale = PALETTE_SCALE;
      colorLoop = 1;
    }
    if ( secondHand == 56)  {
      targetPalette  = gGradientPalettes[2];
      speed = PALETTE_SPEED;
      scale = PALETTE_SCALE;
      colorLoop = 1;
    }
  }
}


void twinkle() {
  //create random twinkle
  int rp = random(500, 2000);
  if (currentMillis - twinkleMillis >= rp) {
    twinkleMillis = currentMillis;
    leds[random(NUM_LEDS)] = CRGB::White;
    FastLED.show();
  }
}


//
// Mark's xy coordinate mapping code.  See the XYMatrix for more information on it.
//
uint16_t XY( uint8_t x, uint8_t y) {
  uint16_t i;
  if ( kMatrixSerpentineLayout == false) {
    i = (y * kMatrixWidth) + x;
  }
  if ( kMatrixSerpentineLayout == true) {
    if ( y & 0x01) {
      // Odd rows run backwards
      uint8_t reverseX = (kMatrixWidth - 1) - x;
      i = (y * kMatrixWidth) + reverseX;
    } else {
      // Even rows run forwards
      i = (y * kMatrixWidth) + x;
    }
  }
  return i;
}

void showDrips() {
  fadeToBlackBy(leds, NUM_LEDS, 64);
  for (int i = 0; i < totalDrips; i++) {
    drip[i].updateDrip();
  }
  FastLED.show();
}

void dripPattern() {
  EVERY_N_MILLISECONDS(60) {
    showDrips();
  }
}

void rainbowFalls() {

  uint32_t pos;
  for (uint8_t j = 0; j < NUM_LEDS_PER_STRIP; j++) {
    for (uint8_t i = 0; i <= stripCount; i++) {
      //pos = j*i;

      switch (i) {
        case 0:
          pos = strip1Array[j];
          break;
        case 1:
          pos = strip2Array[j];
          break;
        case 2:
          pos = strip3Array[j];
          break;
        case 3:
          pos = strip4Array[j];
          break;
        case 4:
          pos = strip5Array[j];
          break;
        default:
          break;
      }

      leds[pos] = CHSV(random8(255), 255, 255);
      twinkle();
      FastLED.show();
      delay (10);
      fadeToBlackBy( leds, NUM_LEDS, 1);
    }
  }

}

void showCandyCane() {
  candyStripes(10, CRGB::Red, CRGB::White, 85);
}

void candyStripes(uint8_t width, CRGB color1, CRGB color2, uint8_t speed ) {
 
  EVERY_N_MILLISECONDS(speed) {
    for (int i = 0; i < NUM_LEDS; i++) {
      if ( (i + offset) % (2 * width) < width + 1 ) {
        leds[i] = color2;
      } else {
        leds[i] = color1;
      }
    }
    twinkle();
    FastLED.show();
    offset--;
    if (offset <= 0) offset = NUM_LEDS;
  }
}


DEFINE_GRADIENT_PALETTE( blinds_gp ) {
  0,   0,  0,  0,
  2,   1,  1,  1,
  4,   1,  1,  1,
  7,   1,  3,  1,
  9,   5,  9,  6,
  12,   8, 14, 10,
  14,  11, 18, 12,
  17,  14, 22, 16,
  19,  18, 27, 19,
  22,  21, 31, 23,
  24,  25, 36, 27,
  27,  29, 40, 31,
  29,  32, 45, 35,
  32,  35, 48, 38,
  35,  38, 51, 41,
  37,  40, 54, 43,
  40,  41, 55, 44,
  42,  42, 55, 45,
  44,  56, 71, 60,
  47, 106, 121, 109,
  49, 140, 154, 144,
  51, 167, 178, 170,
  54, 194, 203, 197,
  56, 217, 223, 219,
  59, 237, 239, 237,
  61, 249, 250, 250,
  63, 255, 255, 255,
  66,  27, 38, 29,
  68,  16, 24, 17,
  71,   8, 14, 10,
  73,   4,  8,  5,
  76,   1,  3,  2,
  78,   1,  1,  1,
  81,   1,  1,  1,
  83,   1,  1,  1,
  85,   1,  1,  1,
  88,   0,  0,  0,
  136, 110, 125, 114,
  138, 130, 144, 133,
  141, 171, 182, 174,
  143, 213, 219, 214,
  146, 244, 246, 245,
  148, 255, 255, 255,
  151,  25, 36, 27,
  153,  14, 22, 15,
  156,   7, 12,  8,
  158,   2,  5,  3,
  161,   1,  2,  1,
  163,   1,  1,  1,
  166,   1,  1,  1,
  168,   1,  1,  1,
  171,   0,  0,  0,
  173,   1,  1,  1,
  176,   1,  1,  1,
  178,   1,  1,  1,
  180,   1,  1,  1,
  183,   1,  1,  1,
  185,   1,  1,  1,
  188,   1,  2,  1,
  190,   2,  4,  2,
  193,   3,  7,  4,
  195,   5, 10,  6,
  198,   9, 15, 10,
  200,  12, 20, 14,
  203,  16, 25, 18,
  205,  20, 29, 22,
  208,  23, 33, 25,
  210,  25, 36, 27,
  212,  26, 36, 28,
  215,  28, 40, 31,
  217,  35, 47, 37,
  219,  47, 61, 51,
  222,  66, 81, 69,
  224,  88, 104, 92,
  226, 113, 128, 117,
  229, 137, 151, 140,
  231, 152, 164, 155,
  233, 157, 169, 160,
  236,  16, 24, 17,
  238,   8, 14, 10,
  240,   4,  8,  5,
  243,   1,  3,  2,
  245,   1,  1,  1,
  247,   1,  1,  1,
  250,   1,  1,  1,
  252,   1,  1,  1,
  255,   0,  0,  0
};

DEFINE_GRADIENT_PALETTE( holly_gp) {
  0,    0,  255,  0,  //green
  48,   0,  255,  0,  //green
  49,   255,  0,  0,  //red
  64,   255,  0,  0,  //red
  65,   0,  255,  0,  //green
  114,   0,  255,  0,  //green
  115,   255,  0,  0,  //red
  118,   255,  0,  0,  //red
  119,   0,  255,  0,  //green
  168,  0,  255,  0,  //green
  169,  255,  0,  0,  //red
  184,  255,  0,  0,  //red
  185,  0,  255,  0,  //green
  234,  0,  255,  0,  //green
  235,  255,  0,  0,  //red
  255,  255,  0,  0   //red
};

DEFINE_GRADIENT_PALETTE( candycane_gp) {
  0 , 128, 128, 128,  //white
  32 , 128, 128, 128,  //white
  33 , 255, 0, 0,  //red
  66 , 255, 0, 0,  //red
  67 , 128, 128, 128,  //white
  100 , 128, 128, 128,  //white
  101 , 255, 0, 0,  //red
  134 , 255, 0, 0,  //red
  135 , 128, 128, 128,  //white
  168 , 128, 128, 128,  //white
  169 , 255, 0, 0,  //red
  202 , 255, 0, 0,  //red
  203 , 128, 128, 128,  //white
  236 , 128, 128, 128,  //white
  237 , 255, 0, 0,  //red
  255 , 255, 0, 0  //red
};

DEFINE_GRADIENT_PALETTE( silvergold_gp) {
  46, 191, 201, 224,
  127, 255, 236, 133,
  204, 191, 201, 224
};

DEFINE_GRADIENT_PALETTE( pit ) {
  0,     3,   3,   3,
  64,   13,   13, 255,  //blue
  128,   3,   3,   3,
  192, 255, 130,   3 ,  //orange
  255,   3,   3,   3
};

DEFINE_GRADIENT_PALETTE( rainbow_gp ) {
  0,  88,  0,  0,
  28, 255,  0,  0,
  56, 255, 22,  0,
  85, 255, 104,  0,
  113, 255, 255,  0,
  141, 255, 255,  0,
  169,  17, 255,  1,
  198,   0, 223, 31,
  226,   0, 19, 255,
  255,   88,  0,  0
};

const TProgmemRGBGradientPalettePtr gGradientPalettes[] = {
  holly_gp,
  candycane_gp,
  rainbow_gp,
  pit,
  silvergold_gp,
  blinds_gp
};
