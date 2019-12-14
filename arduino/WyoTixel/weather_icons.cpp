#include "weather_icons.h"

union single_double{
  uint8_t two[2];
  uint16_t one;
} this_single_double;

// This draws the weather icons and temperature
int draw_weather_icon (PxMATRIX display, uint16_t icon, uint16_t location, int temp,bool ab){
  display.setFont(&TomThumb);

  if (location>2)
  location=2;

  if (icon>10)
  icon=10;
  for (int yy=0; yy<10;yy++)
  {
    for (int xx=0; xx<10;xx++)
    {
      uint16_t byte_pos=(xx+icon*10)*2+yy*220;
      if (ab){
        this_single_double.two[1]=weather_icons_a[byte_pos];
        this_single_double.two[0]=weather_icons_a[byte_pos+1];
      }
      else
      {
        this_single_double.two[1]=weather_icons_b[byte_pos];
        this_single_double.two[0]=weather_icons_b[byte_pos+1];
      }
      display.drawPixel(1+xx+location*12,yy,this_single_double.one);
    }

  }

  int pixel_shift=0;
  if ((temp>-10)&&(temp<10))
  pixel_shift=2;

  if (location==0)
  display.setCursor(2+pixel_shift,16);
  else
  display.setCursor(14+pixel_shift,16);

  if (temp<0)
  {
    temp=temp*-1;
    if (location==0)
    display.drawPixel(pixel_shift,13,myWHITE);
    else
    display.drawPixel(12+pixel_shift,13,myWHITE);
  }
  display.println(temp);
}
