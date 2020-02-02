#include <Wire.h>
#include <I2CNavKey.h>
#include <i2cEncoderLibV2.h>
#include <BleKeyboard.h>


BleKeyboard bleKeyboard;

void remote_setup() {
  bleKeyboard.begin();
}


const int IntPin = 16; /* Definition of the interrupt pin*/
//Class initialization with the I2C addresses
i2cNavKey navkey(0b0010000); /* Default address when no jumper are soldered */


uint8_t pwm, fade = 0;

void UP_Button_Pressed(i2cNavKey* p) {
  Serial.println("Button UP Pressed!");
  media_vol_up();
}

void DOWN_Button_Pressed(i2cNavKey* p) {
  Serial.println("Button DOWN Pressed!");
  media_vol_down();
}

void LEFT_Button_Pressed(i2cNavKey* p) {
  Serial.println("Button LEFT Pressed!");
  media_prev_track();
}

void RIGHT_Button_Pressed(i2cNavKey* p) {
  Serial.println("Button RIGHT Pressed!");
  media_next_track();
}

void CENTRAL_Button_Pressed(i2cNavKey* p) {
  Serial.println("Button Central Pressed!");
  media_playpause();
}

void CENTRAL_Button_Double(i2cNavKey* p) {
  Serial.println("Button Central Double push!");
}

void Encoder_Rotate(i2cNavKey* p) {
  Serial.println(p->readCounterInt());

}
void navkey_setup(void)
{
  pinMode(IntPin, INPUT);
  Wire.begin();
  Serial.begin(115200);
  Serial.println("**** I2C navkey V2 basic example ****");
  /*
      INT_DATA= The register are considered integer.
      WRAP_ENABLE= The WRAP option is enabled
      DIRE_RIGHT= navkey right direction increase the value
      IPUP_ENABLE= INT pin have the pull-up enabled.
  */

  navkey.reset();
  navkey.begin(i2cNavKey::INT_DATA | i2cNavKey::WRAP_ENABLE | i2cNavKey::DIRE_RIGHT | i2cNavKey::IPUP_ENABLE);

  navkey.writeCounter((int32_t)0); /* Reset the counter value */
  navkey.writeMax((int32_t)10); /* Set the maximum threshold*/
  navkey.writeMin((int32_t)-10); /* Set the minimum threshold */
  navkey.writeStep((int32_t)1); /* Set the step to 1*/

  navkey.writeDoublePushPeriod(30);  /*Set a period for the double push of 300ms */

  navkey.onUpPush = UP_Button_Pressed;
  navkey.onDownPush = DOWN_Button_Pressed;
  navkey.onRightPush = RIGHT_Button_Pressed;
  navkey.onLeftPush = LEFT_Button_Pressed;
  navkey.onCentralPush = CENTRAL_Button_Pressed;
  navkey.onCentralDoublePush = CENTRAL_Button_Double;
  navkey.onChange = Encoder_Rotate;

  navkey.autoconfigInterrupt(); /* Enable the interrupt with the attached callback */

  Serial.print("ID CODE: 0x");
  Serial.println(navkey.readIDCode(), HEX);
  Serial.print("Board Version: 0x");
  Serial.println(navkey.readVersion(), HEX);

}

void setup(){
  Serial.begin(115200);
  Serial.println("Starting BLE work!");

  navkey_setup();
  remote_setup();
}
const byte _add  = 0b0010000;
void writeEncoder(uint8_t reg, uint8_t data) {
  Wire.beginTransmission(_add);
  Wire.write(reg);
  Wire.write(data);
  Wire.endTransmission();
}

void media_playpause(){
  if(bleKeyboard.isConnected()) {
    Serial.println("Sending Play/Pause media key...");
    bleKeyboard.write(KEY_MEDIA_PLAY_PAUSE);
    bleKeyboard.releaseAll();
 }
}

void media_vol_up(){
  if(bleKeyboard.isConnected()) {
    Serial.println("Sending Vol up media key...");
    bleKeyboard.write(KEY_MEDIA_VOLUME_UP);
  }
}
void media_vol_down(){
  if(bleKeyboard.isConnected()) {
    Serial.println("Sending Vol down media key...");
    bleKeyboard.write(KEY_MEDIA_VOLUME_DOWN);
  }
}
void media_next_track(){
  if(bleKeyboard.isConnected()) {
    Serial.println("Sending next track media key...");
    bleKeyboard.write(KEY_MEDIA_NEXT_TRACK);
  }
}
void media_prev_track(){
  if(bleKeyboard.isConnected()) {
    Serial.println("Sending prev track media key...");
    bleKeyboard.write(KEY_MEDIA_PREVIOUS_TRACK);
  }
}

void loop() {
  uint8_t enc_cnt;
  if (digitalRead(IntPin) == LOW) {
    navkey.updateStatus();
  }
}
