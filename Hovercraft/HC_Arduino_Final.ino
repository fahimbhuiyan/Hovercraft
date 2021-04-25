long lastObstacleTime = -1;

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); 
  Serial.setTimeout(5);
}

void serialWait(){
  while(Serial.peek() != 's') {
    char t = Serial.read();
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1);                      
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1);
  }
  char t = Serial.read(); //discard the s
}   

void loop() {  
  //Read serial port until sensor data is sent
  serialWait();
  
  //Interpret the data String
  //Arduino Section 1***************************************
  //data string received in format (simtime, front sensor result, right sensor result, left sensor result, left sensor 45 result,  servo rotation status)
  float simTime = Serial.parseFloat();
  int FrontSensor_R = Serial.parseInt();
  int RightSensor_R = Serial.parseInt();
  int LeftSensor_R = Serial.parseInt();
  int LeftSensor_45_R = Serial.parseInt();
  int RightSensor_45_R = Serial.parseInt();
  int LastCommand = Serial.parseInt(); //Left = 0; Right = 1; No value = 2
  
  float tilt;
  long timeNow = millis();
  float FrontFan_R = 0.50;

 if (FrontSensor_R == 0 && (RightSensor_45_R == 1 && LeftSensor_45_R == 1)||(RightSensor_R == 1 && LeftSensor_R == 1)){

      if (simTime < 6.3){
        tilt = -0.45;
      }
 }
 if (FrontSensor_R == 1 && (RightSensor_45_R == 1 && LeftSensor_45_R == 1)||(RightSensor_R == 0 && LeftSensor_R == 1)){

      if (simTime > 4){
         if (simTime < 7.5){
         tilt = -1.57;
        }
      }
 }
 if (FrontSensor_R == 0 && (RightSensor_45_R == 1 && LeftSensor_45_R == 1)||(RightSensor_R == 1 && LeftSensor_R == 1)){

      if (simTime > 6.3){
        if(simTime < 15){
        tilt = 0;
        FrontFan_R = 0.75;
      }
      else if(simTime > 15){
        tilt = -0.65;
      }
 }
 }
 if (FrontSensor_R == 1 && (RightSensor_45_R == 0 && LeftSensor_45_R == 1)&&(RightSensor_R == 1 && LeftSensor_R == 1)){
  if (simTime > 5){
      if (simTime < 9){
        tilt = -1;
      }
  }
 }
if (FrontSensor_R == 1 && (RightSensor_45_R == 1 && LeftSensor_45_R == 1)&&(RightSensor_R == 1 && LeftSensor_R == 0)){

      if (simTime > 11){
        if(simTime < 20){
         tilt = .9;
        FrontFan_R = 0.75;
        } 
      }
 }
 if (FrontSensor_R == 1 && (RightSensor_45_R == 1 && LeftSensor_45_R == 1)&&(RightSensor_R == 0 && LeftSensor_R == 1)){
  if (simTime > 17){
    //  if (simTime < 30){
        tilt = -1.57;
    //  }
  }
 }
 //////////////////////Tests////////////////
 if (FrontSensor_R == 1 && (RightSensor_45_R == 1 && LeftSensor_45_R == 1)&&(RightSensor_R == 1 && LeftSensor_R == 1)){
  if (simTime > 20){
      if (simTime < 30){
        tilt = -1.57;
      }
  }
 }
 if (FrontSensor_R == 1 && (RightSensor_45_R == 1 && RightSensor_R == 1)&&(LeftSensor_45_R == 0 || LeftSensor_R == 0)){
  if (simTime > 12 ){
        tilt = 1.57;
      }
 }
 if (FrontSensor_R == 0 && (RightSensor_45_R == 0 && RightSensor_R == 1)&&(LeftSensor_45_R == 1 && LeftSensor_R == 1)){
  if (simTime > 13 ){
     if(simTime <17){
        tilt = -0.7;
         }
      }
 }
 
  //***************************************
  //data string sent in format (simtime, 
  Serial.print(simTime);
  Serial.write(",");
  Serial.print(timeNow);
  Serial.write(",");  
  Serial.print(FrontFan_R);
  Serial.write(",");
  Serial.print(tilt);
  Serial.write(",");
  Serial.print(LastCommand);
  Serial.write("\r\n");

  //Serial.print("Hello CoppeliaSim! \r\n");
}
