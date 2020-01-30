#include <Arduino.h>
#include <HTTPClient.h>
#define double_buffer
#include <PxMatrix.h>
#include <ezTime.h>
#include "weather_icons.h"

#define P_LAT 22
#define P_A 19
#define P_B 23
#define P_C 18
#define P_D 5
#define P_E 15
#define P_OE 2
hw_timer_t * timer = NULL;
portMUX_TYPE timerMux = portMUX_INITIALIZER_UNLOCKED;

Timezone myTZ, othTZ;



// Pins for LED MATRIX

//PxMATRIX display(32,16,P_LAT, P_OE,P_A,P_B,P_C);
PxMATRIX display(64,32,P_LAT, P_OE,P_A,P_B,P_C,P_D);
//PxMATRIX display(64,64,P_LAT, P_OE,P_A,P_B,P_C,P_D,P_E);

// This draws the weather icons and temperature
void draw_weather_icon (PxMATRIX display, uint16_t icon, uint16_t location, int temp,bool ab);

void IRAM_ATTR display_updater(){
  // Increment the counter and set the time of ISR
  portENTER_CRITICAL_ISR(&timerMux);
  display.display(70);
  portEXIT_CRITICAL_ISR(&timerMux);
}

struct Text {
	char *text;
	uint16_t width, height;
	int16_t x, y;
	int16_t dx, dy;
} text = {"HH:MM:SS   ", 0, 0, 0, 0, 1, 1};


uint16_t textColor0 = display.color565(0, 0, 255);
uint16_t textColor1 = display.color565(0, 128, 0);
uint16_t myBLACK = display.color565(0, 0, 0);
uint16_t lineColor = display.color565(255, 0, 0);
uint16_t backgroundColor = display.color565(0, 255, 0);

#include <WiFiManager.h>
WiFiManager wifiManager;
void wifi_setup(){
  Serial.print("Connecting Wifi: ");
  wifiManager.autoConnect("WyoTixel");
  Serial.println("Yay connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}

String jsonLookup(String s, String name){
  name = String("\"") + name + String("\"");
  int start = s.indexOf(name) + name.length() + 2;
  int stop = s.indexOf('"', start);
  Serial.println(s.substring(start, stop));
  return s.substring(start, stop);
}

String get_timezone_from_ip(){

  HTTPClient http;
  String timezone_str;
  
  Serial.print("[HTTP] begin...\n");
  //http.begin("https://timezoneapi.io/api/ip");// no longer works!
  //http.begin("https://ipapi.co/json");
  String url = String("https://www.wyolum.com/utc_offset/utc_offset.py") +
    String("?refresh=") + String(millis()) +
    String("&localip=") +
    String(WiFi.localIP()[0]) + String('.') + 
    String(WiFi.localIP()[1]) + String('.') + 
    String(WiFi.localIP()[2]) + String('.') + 
    String(WiFi.localIP()[3]) + String('&') +
    String("macaddress=") + WiFi.macAddress() + String('&') + 
    String("dev_type=WyoTixel");
  Serial.println(url);
  http.begin(url);
  
  Serial.print("[HTTP] GET...\n");
  // start connection and send HTTP header
  int httpCode = http.GET();
  
  // httpCode will be negative on error
  if(httpCode > 0) {
    // HTTP header has been send and Server response header has been handled
    Serial.printf("[HTTP] GET... code: %d\n", httpCode);
    
    // file found at server
    //String findme = String("offset_seconds");
    if(httpCode == HTTP_CODE_OK) {
      String payload = http.getString();
      Serial.print("payload:");
      Serial.println(payload);
      payload.replace(" ", "");
      String offset_str = jsonLookup(payload, String("utc_offset"));
      int hours = offset_str.substring(0, 3).toInt();
      int minutes = offset_str.substring(3, 5).toInt();
      if(hours < 0){
	minutes *= -1;
      }
      int offset = hours * 3600 + minutes * 60;

      Serial.print("timezone_offset String:");
      Serial.println(offset_str);
      Serial.print("timezone_offset:");
      Serial.println(offset);
      timezone_str = jsonLookup(payload, String("timezone"));
      Serial.print("timezone:");
      Serial.println(timezone_str);
    }
    else{
      Serial.println("No timezone found");
    }
  }
  return timezone_str;
}

void display_setup(){
  display.begin(16);
  display.flushDisplay();
  display.setTextWrap(false);
  timer = timerBegin(0, 80, true);
  timerAttachInterrupt(timer, &display_updater, true);
  timerAlarmWrite(timer, 2000, true);
  timerAlarmEnable(timer);
  
  int16_t x1 = 0, y1 = 0;
  display.getTextBounds(text.text, 0, 0, &x1, &y1, &text.width, &text.height);
}

void clock_setup(){
  String my_tz = get_timezone_from_ip();

  // Setup EZ Time
  setDebug(INFO);
  waitForSync();
  while(!myTZ.setLocation(my_tz)){
    delay(1000);
  }
  while(!othTZ.setLocation("Asia/Kolkata")){
    delay(1000);
  }
}

void clock_loop(){
  events();
}

void setup() {
  // put your setup code here, to run once:
  Serial.begin(115200);
  wifi_setup(); // prior to display setup or else crash
  display_setup();
  clock_setup();
  
  Serial.print(F("Time in your set timezone:         "));
  Serial.println(myTZ.dateTime());
  Serial.println(othTZ.dateTime());

  text.width-=2;
  text.height-=2;
}

int16_t x=0, dx=1;

String Olson2City(String Olson){
  int slash = Olson.indexOf("/");
  return Olson.substring(slash + 1);
}
void loop() {
  clock_loop();

//  display.clearDisplay();
  display.fillScreen(myBLACK);

  if(x+dx>=display.width() || x+dx<0)
  	dx=-dx;
  x+=dx;
  //display.drawLine(x,0, display.width()-x-1, display.height()-1, lineColor);

  String timeString;
  display.setTextColor(textColor0);
  display.setCursor(0, 8 * 0);
  display.print(Olson2City(myTZ.getOlson()));

  timeString = myTZ.dateTime("g:i:s");
  display.setCursor(0, 8 * 1);
  display.print(timeString);

  display.setTextColor(textColor1);
  display.setCursor(0, 8 * 2);
  //display.print(Olson2City(othTZ.getOlson()));

  timeString = othTZ.dateTime("g:i:s");
  display.setCursor(0, 8 * 3);
  //display.print(timeString);

  //################################################################################
  //draw_weather_icon (display, 0, 0, 0,0);
  uint16_t icon = 0;
  uint16_t x, y;
  int temp;
  bool ab = myTZ.second()%2;
  display.setFont(&TomThumb);
  for(x = 0; x < 64; x += 11){
    for(y = 0; y < 32; y += 11){
      for (int yy=0; yy<10;yy++){
	  for (int xx=0; xx<10;xx++){
	      uint16_t byte_pos=(xx+icon*10)*2+yy*220;
	      if (ab){
		wi_single_double.two[1]=weather_icons_a[byte_pos];
		wi_single_double.two[0]=weather_icons_a[byte_pos+1];
	      }
	      else{
		wi_single_double.two[1]=weather_icons_b[byte_pos];
		wi_single_double.two[0]=weather_icons_b[byte_pos+1];
	      }
	      display.drawPixel(1+xx+x,yy+y,wi_single_double.one);
	  }
      }
      icon++;
      icon%=11;
    }
  }
  //################################################################################
  
  display.showBuffer();
  delay(20);
}

// This draws the weather icons and temperature
void draw_weather_icon (PxMATRIX display, uint16_t icon, uint16_t location, int temp,bool ab){
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
    display.drawPixel(pixel_shift,13, rgb565(255, 255, 255));
    else
    display.drawPixel(12+pixel_shift,13,rgb565(255, 255, 255));
  }
  display.println(temp);
}
