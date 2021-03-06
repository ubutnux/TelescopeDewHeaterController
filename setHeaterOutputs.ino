/* 
 *  These 2x functions:
 * - setHeaterData    = sets the power levels
 * - outputHeaterData = writes to output pins
 */

void setHeaterData(int theChannel) {
  // set output duty cycle for heaters
  float aboveDewPoint;
  float aboveDewPointConstrain;
  int theDutyCycle;

  // OFF mode: set heater duty cycle to 0
  if (heaterMode[theChannel] == 0)
    theDutyCycle = 0;

  // MANUAL mode: set heater duty cycle to manual level
  if (heaterMode[theChannel] == 1) {
    theDutyCycle = 25.5 * heaterManualPower[theChannel]; // output range 0 - 255
  }
  
  // AUTO mode: set heater duty cycle to auto set level if no read errors
  if (heaterMode[theChannel] == 2) {
    if (errorDS18B20[theChannel] || errorAmbientSensor) { 
      // if error reading either AM2320 or DS18B20, set heater duty cycle to manual level
      theDutyCycle = 25.5 * heaterManualPower[theChannel]; // as above for manual
    }
    else { 

      //##
      // if no read errors for AM2320 or DS18B20
      aboveDewPoint = tempSensor[0] - dewPointAmbient;
      // restrict between 0 & threshold
      aboveDewPointConstrain = constrain( aboveDewPoint , 0 , dewPointThreshold );
      // PWM 0 - 100% duty cycle EQUIV TO analog 0 - 255
      //theDutyCycle = 255 * (( dewPointThreshold - aboveDewPointConstrain ) / dewPointThreshold );
      //##
      set_temperature = dewPointThreshold + dewPointAmbient;
      Serial.println(set_temperature);
      PID_error = set_temperature - tempSensor[0];
      //Calculate the P value
      PID_p = kp * PID_error;
      //Calculate the I value in a range on +-3
      if(-3 < PID_error <3)
      {
        PID_i = PID_i + (ki * PID_error);
      }
      
      //For derivative we need real time to calculate speed change rate
      timePrev = Time;                            // the previous time is stored before the actual time read
      Time = millis();                            // actual time read
      elapsedTime = (Time - timePrev) / 1000; 
      //Now we can calculate the D calue
      PID_d = kd*((PID_error - previous_error)/elapsedTime);
      //Final total PID value is the sum of P + I + D
      PID_value = PID_p + PID_i + PID_d;
      Serial.println(PID_value);
      //We define PWM range between 0 and 255
      if(PID_value < 0)
      {    PID_value = 0;    }
      if(PID_value > 255)  
      {    PID_value = 255;  }
      //Now we can write the PWM signal to the mosfet on digital pin D3
      theDutyCycle = PID_value;
      previous_error = PID_error;     //Remember to store the previous error for next loop.
      

      
    }
  }
  // set the heater's duty cycle
  heaterDutyCycle[theChannel] = theDutyCycle;
}

void outputHeaterData(int theChannel) {
  int thePWMlevel;

  thePWMlevel = heaterDutyCycle[theChannel];
  // write the heater's duty cycle to the heater
  analogWrite(heaterPin[theChannel] , thePWMlevel);
}
