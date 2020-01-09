 /*
  *  https://github.com/pixelmatix/SmartMatrix/tree/teensylc
  *  
  */  
 
/* -------------------------- Display Config Initialisation -------------------- */

#define MATRIX_WIDTH 64
#define MATRIX_HEIGHT 32

/* -------------------------- Class Initialisation -------------------------- */

#define  GPIOPINOUT AZSMZ_ESP32Matrix_v12
//#define  GPIOPINOUT AZSMZ_ESP32Matrix_v15

#include "ESP32Matrix.h"
#include <SmartMatrix3.h>

#include <FastLED.h>

#define COLOR_DEPTH 24                  // known working: 24, 48 - If the sketch uses type `rgb24` directly, COLOR_DEPTH must be 24
const uint8_t kMatrixWidth = 64;        // known working: 32, 64, 96, 128
const uint8_t kMatrixHeight = 32;       // known working: 16, 32, 48, 64
const uint8_t kRefreshDepth = 36;       // known working: 24, 36, 48
const uint8_t kDmaBufferRows = 4;       // known working: 2-4, use 2 to save memory, more to keep from dropping frames and automatically lowering refresh rate
const uint8_t kPanelType = SMARTMATRIX_HUB75_32ROW_MOD16SCAN;   // use SMARTMATRIX_HUB75_16ROW_MOD8SCAN for common 16x32 panels
const uint8_t kMatrixOptions = (SMARTMATRIX_OPTIONS_NONE);      // see http://docs.pixelmatix.com/SmartMatrix for options
const uint8_t kBackgroundLayerOptions = (SM_BACKGROUND_OPTIONS_NONE);

SMARTMATRIX_ALLOCATE_BUFFERS(matrix, kMatrixWidth, kMatrixHeight, kRefreshDepth, kDmaBufferRows, kPanelType, kMatrixOptions);
SMARTMATRIX_ALLOCATE_BACKGROUND_LAYER(backgroundLayer, kMatrixWidth, kMatrixHeight, COLOR_DEPTH, kBackgroundLayerOptions);

#include "Effects.h"
Effects effects;
#include "Drawable.h"
#include "Playlist.h"
//#include "Geometry.h"

#include "Patterns.h"
Patterns patterns;

/* -------------------------- Some variables -------------------------- */
unsigned long ms_current  = 0;
unsigned long ms_previous = 0;
unsigned long ms_animation_max_duration = 10000; // 10 seconds
unsigned long next_frame = 0;

void setup()
{
  // Setup serial interface
  Serial.begin(115200);
  delay(250); 
//  matrix.begin(R1_PIN, G1_PIN, B1_PIN, R2_PIN, G2_PIN, B2_PIN, A_PIN, B_PIN, C_PIN, D_PIN, E_PIN, LAT_PIN, OE_PIN, CLK_PIN );  // setup the LED matrix

  // Initialize Matrix
  matrix.addLayer(&backgroundLayer); 
  matrix.begin();

  backgroundLayer.setBrightness(30);

  Serial.println("**************** Starting Aurora Effects Demo ****************");

   // setup the effects generator
  effects.Setup();

  delay(500);
  Serial.println("Effects being loaded: ");
  listPatterns();


  patterns.setPattern(0); //   // simple noise
  patterns.start();     

  Serial.print("Starting with pattern: ");
  Serial.println(patterns.getCurrentPatternName());

}

void loop()
{
    // menu.run(mainMenuItems, mainMenuItemCount);  
    ms_current = millis();

  
    if ( (ms_current - ms_previous) > ms_animation_max_duration ) 
    {
     //  patterns.moveRandom(1);

       patterns.stop();      
       patterns.move(1);
       patterns.start();  
 
       
       Serial.print("Changing pattern to:  ");
       Serial.println(patterns.getCurrentPatternName());
        
       ms_previous = ms_current;

       // Select a random palette as well
       //effects.RandomPalette();
    }
 
    if ( next_frame < ms_current)
      next_frame = patterns.drawFrame() + ms_current; 
       
}


void listPatterns() {
  patterns.listPatterns();
}
