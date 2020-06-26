/* Test code to read pressure and temp from
Sensirion SD810-125Pa pressure sensor
Zosuls BU 2020.

This uses the polling method where the sensor is woken up
to sample then goes back to sleep. It will work faster in the 
continuous mode but this may be fast enough.
*/
#include <Wire.h>

char packet[9];
int i = 0;
uint16_t twoCompPress;
int binaryPress;
int binaryTemp;
float scaleFactorInH20 = 59780;
float scaleFactorTempC = 200.0;
float pressInch;
float tempC;
void setup() {
  // put your setup code here, to run once:
  Wire.begin();
  Serial.begin(9600);
}

void loop() {
  //I2c stuff
  Wire.beginTransmission(0x25);  //adress of the sensor
  Wire.write(0x36);
  Wire.write(0x2F);
  Wire.endTransmission();
  delay(46); //needs to be at least 46 ms for the sensor data acq.
  Wire.requestFrom(0x25,9);
  //revceive all 9 bytes into the packet char array
  i = 0;
   while(Wire.available())    // slave may send less than requested
  {
    packet[i] = Wire.read();    // receive a byte as character
    
    //Serial.print(packet[i]);         // print the character
    i++;
  }
  //assemble the press and temp bytes
  twoCompPress = packet[1] + 256* packet[0];
  //decode the 2s compliment
  if( twoCompPress > 32767){
    binaryPress = twoCompPress - 65535;
  }
   else{
    binaryPress = twoCompPress;
  }

  //conversions
  pressInch = (float)binaryPress / scaleFactorInH20;
  binaryTemp = packet[4] + 256* packet[3];
  tempC = (float)binaryTemp / scaleFactorTempC;
  //talking
  Serial.print("binary ");
  Serial.print(binaryPress);
  Serial.print("  inches H20  ");
  Serial.print(pressInch, 4);
  Serial.print(" Deg C  ");
  Serial.println(tempC);
  //delay(1);
}
