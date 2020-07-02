// send data to serial output

#ifdef SERIALCOMM_ON

void serialEvent() {
  while (Serial.available()) {
    // get the new byte:
    char inChar = (char)Serial.read();
    // add it to the inputString:
    inputString += inChar;
    // if the incoming character is a newline, set a flag
    // so the main loop can do something about it:
    if (inChar == '\n') {
      stringComplete = true;
    }
  }
}

void sendData(){
  Serial.print(temperatureAmbient);
  Serial.print("|");
  Serial.print(humidityAmbient);
  Serial.print("|");
  Serial.print(dewPointAmbient);
  Serial.print("|");
  Serial.print(heaterMode[0]);
  Serial.print("|");
  Serial.print(tempSensor[0]);
  Serial.print("|");
  Serial.print(100 * heaterDutyCycle[0] / 255);
  Serial.print("|");
  Serial.print(heaterMode[1]);
  Serial.print("|");
  Serial.print(tempSensor[1]);
  Serial.print("|");
  Serial.println(100 * heaterDutyCycle[1] / 255);
}
void printAmbientData() {
  Serial.println("");
  Serial.print("Ambient: ");
  if (errorAmbientSensor) {
    Serial.println("no sensor. ");
  }
  else {
    Serial.print("Temp = ");
    Serial.print(temperatureAmbient,1);
    Serial.print("C. ");
    Serial.print("Humidity = ");
    Serial.print(humidityAmbient,0);
    Serial.print("%. ");
    Serial.print("DewPoint = ");
    Serial.print(dewPointAmbient,1);
    Serial.println("C. ");
  }
}

void printHeaterDS18B20data(int theChannel) {
  Serial.print("Heater");
  Serial.print(theChannel+1); 
  Serial.print(": "); 
  // check if error reading
  if (errorDS18B20[theChannel]) {
    Serial.print("no DS18B20, "); 
  }
  else {
    Serial.print("DS18B20 "); 
    Serial.print("temp = ");
    Serial.print(tempSensor[theChannel], 1);
    Serial.print("C. ");
  }
  if (heaterMode==1) Serial.print("Manual ");
  if (heaterMode==2) Serial.print("Automatic ");
  Serial.print("heater power = ");
  Serial.print(int(100 * heaterDutyCycle[ theChannel ] / 255));
  Serial.print("%. ");
  Serial.println();
}

void getCommand() {
  if(stringComplete){
    if(inputString.length()>0){
      //Serial.println(inputString);
      stringComplete = false;
      commandString = inputString.substring(1,5);
      //Serial.println(commandString);
      if(commandString.equals("POW1")){
        int mode = inputString.substring(5,6).toInt();
        if (mode == 1){
          heaterMode[0] = mode;
          String plusmin = inputString.substring(6,7);
          
          if (plusmin.equals("+")){
            if (heaterManualPower[0] < 10) heaterManualPower[0] += 1;
          }
          else if (plusmin.equals("-")){
            if (heaterManualPower[0] > 0) heaterManualPower[0] -= 1;
          }
        }
        else{
          heaterMode[0] = mode;
        }
      }
      else if(commandString.equals("POW2")){
        int mode = inputString.substring(5,6).toInt();
        if (mode == 1){
          heaterMode[1] = mode;
          String plusmin = inputString.substring(6,7);
          
          if (plusmin.equals("+")){
            if (heaterManualPower[1] < 10) heaterManualPower[1] += 1;
          }
          else if (plusmin.equals("-")){
            if (heaterManualPower[1] > 0) heaterManualPower[1] -= 1;
          }
        }
        else{
          heaterMode[1] = mode;
        }
      }
      inputString = "";
    }
  }
}
#endif
