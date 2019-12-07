#include <ezTime.h>
//#include <WiFi.h>
#include <WiFiManager.h>
WiFiManager wifiManager;
void wifi_setup(){
  wifiManager.autoConnect("WyoTixel");
  Serial.println("Yay connected!");
  Serial.println("IP address: ");
  Serial.println(WiFi.localIP());
}
void setup() {

	Serial.begin(115200);
	while (!Serial) { ; }		// wait for Serial port to connect. Needed for native USB port only
	//WiFi.begin("your-ssid", "your-password");
	wifi_setup();

	// Uncomment the line below to see what it does behind the scenes
	// setDebug(INFO);
	
	waitForSync();

	Serial.println();
	Serial.println("UTC:             " + UTC.dateTime());

	Timezone myTZ;

	// Provide official timezone names
	// https://en.wikipedia.org/wiki/List_of_tz_database_time_zones
	myTZ.setLocation(F("Pacific/Auckland"));
	Serial.print(F("New Zealand:     "));
	Serial.println(myTZ.dateTime());

	// Wait a little bit to not trigger DDoS protection on server
	// See https://github.com/ropg/ezTime#timezonedropnl
	delay(5000);

	// Or country codes for countries that do not span multiple timezones
	myTZ.setLocation(F("de"));
	Serial.print(F("Germany:         "));
	Serial.println(myTZ.dateTime());

	// Same as above
	delay(5000);

	// See if local time can be obtained (does not work in countries that span multiple timezones)
	Serial.print(F("Local (GeoIP):   "));
	if (myTZ.setLocation()) {
		Serial.println(myTZ.dateTime());
	} else {
		Serial.println(errorString());
	}

}

void loop() {
	events();
}
