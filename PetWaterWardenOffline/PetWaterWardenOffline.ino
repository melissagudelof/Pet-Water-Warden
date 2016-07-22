/************************************************
  * Author: Melissa Agudelo
  * Project: Pet Water Warden Offline
  * Company: Make Magazine // RadioShack
  * Date: 7/22/16
  * Version: 1.0
  * http://makezine.com/projects/petwaterwarden/
  *************************************************/
  
  #include <SPI.h>
   
  // Message to post to twitter in case of a fail
  char msg[] = "Pet Water Warden Error: Please check my water!";
 
  //Set output pin 8 to control switchtail/pump
  const int pump = 8;
 
  //Failsafe off
  long timerA = 0;
  int wardenFailed = 0; // Flag incase something goes wrong
 
void setup() {
  // initialize serial communication at 9600 bits per second:
  Serial.begin(9600);
 
  //Set pump output pin
  pinMode(pump, OUTPUT);
 
  //Start with pump off
  digitalWrite(pump, LOW);
 
  Serial.begin(9600);  // starts serial communications so we can debug easier
  delay(1000);        // a 1 second delay to let everything settle down!
}
 
 
void loop(){
 
  //Read the input on A0-A1
  //High and Low Sensors
  int sensorLow = analogRead(A1);
  int sensorHigh = analogRead(A0);
 
  //Convert to a voltage
  float voltageLow = sensorLow * (5.0 / 1023.0);
  float voltageHigh = sensorHigh * (5.0 / 1023.0);  
 
  //Sensor States
  int lowState = 0;
  int highState = 0;
 
  //Are the sensors on or off?
  //Write states, voltage comparison values may need to be adjusted depending on your transistor 
  //and if you are using extneral or the MakerShields LEDs
  if (voltageLow >= 3.3){lowState = 0;}
  else if (voltageLow < 3.3){lowState = 1;}
 
  if (voltageHigh >= 3.3){highState = 0;}
  else if (voltageHigh < 3.3){highState = 1;}
 
  //Turn on the pump?
  if(highState == 1 && lowState == 1 && wardenFailed == 0){
    digitalWrite(pump, LOW);
    timerA = 0;  
  }else if(highState == 0 && digitalRead(pump) == LOW && wardenFailed == 0){
    //FailSafe Timers
    timerA = 0;
    digitalWrite(pump, HIGH);
    timerA = millis();
    Serial.print("Starting timer: ");    
    Serial.println(timerA);
  }
 
  //My pet bowl fills in about 45 sec, adjust to the size
  // of your pet bowl
  if( (millis() - timerA) >= 45000 && timerA != 0){
    digitalWrite(pump, LOW);  
    Serial.println(timerA);
    Serial.println(millis());
    timerA = 0;
    //Either no water left or the pump didn't turn off, bad sensor?
    wardenFailed = 1; // the Pet Warden Warden has run into trouble and failed
    Serial.print("Something went wrong! The wardenFailed status is: ");
    Serial.println(wardenFailed);
    exit(0);// exit the program until error is fixed
    Serial.println("Either no water left or the pump didn't turn off, bad sensor?");
  }
 
 
  //Debug Prints
  Serial.print("Low Sensor: ");
  Serial.println(lowState);
  Serial.print("High Sensor: ");
  Serial.println(highState);
  Serial.println(voltageLow);
  Serial.println(voltageHigh);
 
  //Check Sensors every 10 sec
  delay(10000);
 
}
 
