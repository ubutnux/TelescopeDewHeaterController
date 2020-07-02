/*
 * here's all the definitions, variables, constants etc
 * first define which devices are to be used
*/

/*
 * DEFINE WHICH DEVICES ARE USED WITH _ON & _OFF
*/

// ambient temp/humidity sensor = AM2320 AND DHT22
#define AM2320_OFF
#define DHT22_ON          
// serial output data to PC
#define SERIALCOMM_ON                     // send sensor & heater data to PC

// Max of 4x possible channels for heater output/sensor with DS18B20
// Currently set for 2x channels
const int maxChannels = 2;                  // max number of channels = 4
const int numChannels = 2;                  // number of used channels = 2

// these are all the DIGITAL PINS for sensor input, heater output & control. Doesn't include sensors on I2C
const int controlSwitchPin = 3;             // Mode control switch PIN, uses internal pull-up resistor  
const int heaterPin[]  = {9, 10};     // MOSFET heater output channels on PWM pins. 2 channels = pins 9, 10
const int DS18B20pin[] = {7, 8};     // heater temp sensor pins. 2 channels = 7, 8

/* 
 *  Define libraries & parameters for the devices
*/


/* 
 *  THE AMBIENT TEMP/HUMIDITY SENSORS
*/

// AM2320 for ambient temperature & humidity sensing on I2C - ADAFRUIT libraries
#ifdef AM2320_ON
#include "Adafruit_Sensor.h"
#include "Adafruit_AM2320.h"
Adafruit_AM2320 am2320 = Adafruit_AM2320();
#endif

// DHT22 for ambient temperature & humidity sensing on one-wire, digital pin 2
// DHT22 (AM2302). There's also DHT11 & DHT21 (AM2301) - but I've not included those
#ifdef DHT22_ON
#include "DHT.h"
#define DHTTYPE DHT22   // DHT 22 (AM2302)
#define DHTPIN 2    // the DHT22 pin
DHT dht(DHTPIN, DHTTYPE);
#endif

// and variables for ambient sensor
float       dewPointAmbient;
float       temperatureAmbient;
float       humidityAmbient;
boolean     errorAmbientSensor;

/* 
 *  THE SCOPE TEMP SENSORS
*/

// DS18B20s: setup oneWire instances and pass references to Dallas Temperature
#include <OneWire.h>
#include <DallasTemperature.h>
// include definitions for all 4 channels, even if using fewer. Can't figure out how to make this to suit numChannels
OneWire oneWire0(DS18B20pin[0]);
OneWire oneWire1(DS18B20pin[1]);
DallasTemperature sensor0(&oneWire0);
DallasTemperature sensor1(&oneWire1);

// and the DS18B20 variables
float     tempSensor[numChannels];          // DS18B20 sensor temperatures from sensors1/2.
boolean   errorDS18B20[numChannels];        // error if tempSensor == -127C (DEVICE_DISCONNECTED_C)
const int resDS18B20 = 9;                   // resolution of DS18B20 read. I think 9-12 = 0.5 - 0.125 degC

// Heater output definitions
int       heaterManualPower[numChannels];                // power level if in Manual mode OR Auto & no sensor connected: 0-10 = 0-100%
const int defaultHeaterManualPower = 0;     // and its default 
int       heaterDutyCycle[numChannels];     // drive to heater

// global constants & variables
const float tempCutOff = 32;                // heater cut-off if too hot
const float dewPointThreshold = 6;          // temp threshold above dew point to start heater
const int   theDelay = 1000;                // delay for DS18B20 read in parasitic mode, displays, switches etc
boolean     changeMode = false;             // change to MODE
int         heaterMode[] = {2, 2};                 // mode: 0=Off 1=Manual 2=Auto 3=Reset
const int   defaultHeaterMode = 2;          // and its deault

String inputString = "";         // a string to hold incoming data
boolean stringComplete = false;  // whether the string is complete
String commandString = "";
