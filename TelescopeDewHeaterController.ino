#include "Globals.h"   // include the global variables, constants etc

void setup(){
  // start serial port, set up as might use output
  Serial.begin(9600);
  
  // Start/reset the sensor libraries - AM2320 & DS18B20s
  resetAmbientSensor();                               // ambient sensors = AM2320, DHT22, BME280
  resetDS18B20();
  
  // set up the LCD/OLED display
  //displaySetUp();

  // setup mode pin
  pinMode(controlSwitchPin, INPUT_PULLUP);            // internal pullup resistor, so LOW = active

  // setup heater pins and initialise power to 0%
  for (int i=0; i < numChannels; i++) {
    pinMode(heaterPin[i], OUTPUT);                    // set the heater output pin as output
    heaterDutyCycle[i] = 0;                           // initially 0%
    analogWrite(heaterPin[i] , heaterDutyCycle[i]);   // write 0% power to outputs
  }
  heaterManualPower[0] = defaultHeaterManualPower;                              // initialise manual power to 0%
  heaterManualPower[1] = defaultHeaterManualPower;                              // initialise manual power to 0%
}

void loop() { 
  // Read ambient temperature & humidity from AM2320 sensor. And display data

  getAmbientData();
  //displayAmbientData();
  #ifdef SERIALCOMM_ON 
  //printAmbientData();
  getCommand();
  #endif
  // check for mode change, if modebutton pressed, change mode
  //if (checkForModeChange()) changeTheMode();

  // go through the DS18B20/heater channels
  for (int currentChannel=0; currentChannel < numChannels; currentChannel++) {
    // read DS18B20 temperature
    getDS18B20data(currentChannel);

    // set the heater output & display power
    setHeaterData(currentChannel);
    outputHeaterData(currentChannel);
    if (heaterMode != 0) {
      // if not OFF, display the DS18B20 and heater data
      //displayDS18B20data(currentChannel);
      //displayHeaterOutput(currentChannel);
      #ifdef SERIALCOMM_ON 
      //printHeaterDS18B20data(currentChannel);
      #endif
    }
    // check for mode change, if modebutton pressed, change mode
    //if (checkForModeChange()) changeTheMode();
  }
  #ifdef SERIALCOMM_ON
  sendData();
  #endif
  delay(theDelay);

}
