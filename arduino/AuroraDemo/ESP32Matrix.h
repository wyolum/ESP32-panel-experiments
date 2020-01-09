/*
 * SmartMatrix Library - Hardware-Specific Header File (for SmartMatrix Shield V4)
 *
 * Copyright (c) 2015 Louis Beaudoin (Pixelmatix)
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

 // Note: only one MatrixHardware_*.h file should be included per project

#ifndef MATRIX_HARDWARE_H
#define MATRIX_HARDWARE_H
// formula used is 80000000L/(cfg->clkspeed_hz + 1), must result in >=2.  Acceptable values 26.67MHz, 20MHz, 16MHz, 13.34MHz...
#define ESP32_I2S_CLOCK_SPEED (20000000UL)

#define AZSMZ_ESP32Matrix_v12       0
#define AZSMZ_ESP32Matrix_v15       1

#ifndef GPIOPINOUT
#define GPIOPINOUT AZSMZ_ESP32Matrix_v15
#endif
//#define GPIOPINOUT ESP32_FORUM_PINOUT_WITH_LATCH // note this mode is untested as of 2018-05-17 - not being used anymore now that SmartMatrix Shield is available
//#define GPIOPINOUT SMARTLED_SHIELD_V0_PINOUT

//Upper half RGB
#define BIT_R1  (1<<0)   
#define BIT_G1  (1<<1)   
#define BIT_B1  (1<<2)   
//Lower half RGB
#define BIT_R2  (1<<3)   
#define BIT_G2  (1<<4)   
#define BIT_B2  (1<<5)   

// Control Signals
#define BIT_LAT (1<<6) 
#define BIT_OE  (1<<7)  

#define BIT_A (1<<8)    
#define BIT_B (1<<9)    
#define BIT_C (1<<10)   
#define BIT_D (1<<11)   
#define BIT_E (1<<12)   

#if (GPIOPINOUT == AZSMZ_ESP32Matrix_v12)

    #pragma message "ESP32 forum wiring"

    // ADDX is output directly using GPIO
    #define CLKS_DURING_LATCH   0 
    #define MATRIX_I2S_MODE I2S_PARALLEL_BITS_16
    #define MATRIX_DATA_STORAGE_TYPE uint16_t

    /*
    HUB 75
    01 02 B0
    03 04 Gnd
    05 06 G1
    07 08 E

    09 10 B
    11 12 D
    13 14 STB/Latch
    15 16 Gnd
                        ESP32 pin / comment
    1 R0  2 Red Data (columns 1-16)
    2 G0  15  Green Data (columns 1-16)

    3 B0  4 Blue Data (columns 1-16)
    4 GND GND Ground
    
    5 R1  16/RX2  Red Data (columns 17-32)
    6 G1  27  Green Data (columns 17-32)
    
    7 B1  17/TX2  Blue Data (columns 17-32)
    8 E 12  Demux Input E for 64x64 panels
   
    9 A 5 Demux Input A0
    10  B 18  Demux Input A1

    11  C 19  Demux Input A2
    12  D 21  Demux Input E1, E3 (32x32 panels only)
    
    13  CLK 22  LED Drivers' Clock
    14  STB 26  LED Drivers' Latch
    
    15  OE  25  LED Drivers' Output Enable
    16  GND GND Ground
    */ 
    #define R1_PIN  17
    #define G1_PIN  2
    #define B1_PIN  16
    #define R2_PIN  4
    #define G2_PIN  15
    #define B2_PIN  13 // 12

    #define A_PIN   26
    #define B_PIN   27  //13
    #define C_PIN   14
    #define D_PIN   12  // 27
    #define E_PIN   -1  // 22
    
    #define LAT_PIN 33
    #define OE_PIN  32
    #define CLK_PIN 25

//    #define GPIO_PWM0A_OUT GPIO_NUM_35
//    #define GPIO_SYNC0_IN  GPIO_NUM_34

#elif (GPIOPINOUT == AZSMZ_ESP32Matrix_v15)

    #pragma message "ESP32 forum wiring"

    // ADDX is output directly using GPIO
    #define CLKS_DURING_LATCH   0 
    #define MATRIX_I2S_MODE I2S_PARALLEL_BITS_16
    #define MATRIX_DATA_STORAGE_TYPE uint16_t

    /*
    HUB 75
    01 02 B0
    03 04 Gnd
    05 06 G1
    07 08 E

    09 10 B
    11 12 D
    13 14 STB/Latch
    15 16 Gnd
                        ESP32 pin / comment
    1 R0  2 Red Data (columns 1-16)
    2 G0  15  Green Data (columns 1-16)

    3 B0  4 Blue Data (columns 1-16)
    4 GND GND Ground
    
    5 R1  16/RX2  Red Data (columns 17-32)
    6 G1  27  Green Data (columns 17-32)
    
    7 B1  17/TX2  Blue Data (columns 17-32)
    8 E 12  Demux Input E for 64x64 panels
   
    9 A 5 Demux Input A0
    10  B 18  Demux Input A1

    11  C 19  Demux Input A2
    12  D 21  Demux Input E1, E3 (32x32 panels only)
    
    13  CLK 22  LED Drivers' Clock
    14  STB 26  LED Drivers' Latch
    
    15  OE  25  LED Drivers' Output Enable
    16  GND GND Ground
    */ 
    #define R1_PIN  17
    #define G1_PIN  2
    #define B1_PIN  16
    #define R2_PIN  4
    #define G2_PIN  15
    #define B2_PIN  12  // 13 // 12

    #define A_PIN   26
    #define B_PIN   13  // 27  //13
    #define C_PIN   14
    #define D_PIN   27  //12  // 27
    #define E_PIN   22  //-1  // 22
    
    #define LAT_PIN 33
    #define OE_PIN  32
    #define CLK_PIN 25

//    #define GPIO_PWM0A_OUT GPIO_NUM_35
//    #define GPIO_SYNC0_IN  GPIO_NUM_34

#endif

//#define DEBUG_PINS_ENABLED
#define DEBUG_1_GPIO    GPIO_NUM_13
//#define DEBUG_2_GPIO    GPIO_NUM_12

#endif
