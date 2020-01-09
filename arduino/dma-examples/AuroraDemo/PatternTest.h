
#ifndef PatternTest_H
#define PatternTest_H

uint16_t color565(uint8_t red, uint8_t green, uint8_t blue) {
    return ((red & 0xF8) << 8) | ((green & 0xFC) << 3) | ((blue & 0xF8) >> 3);
}

class PatternTest : public Drawable {
  private:

  public:
    PatternTest() {
      name = (char *)"Test Pattern";
    }

    unsigned int drawFrame() {

    backgroundLayer.fillScreen({128, 0, 0});  
//    backgroundLayer.fillScreen(backgroundLayer.color565(128, 0, 0));      
    }
};

#endif
