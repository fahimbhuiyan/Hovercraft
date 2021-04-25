#define IMG_SENSOR_YDIM 32
#define SERIAL_DELAY 90 //~2*8*1000000/BAUDRATE

long lastObstacleTime = -1;
unsigned long img[IMG_SENSOR_YDIM]; //the image array

void setup() {
  // put your setup code here, to run once:
  pinMode(LED_BUILTIN, OUTPUT);
  Serial.begin(115200); 
  Serial.setTimeout(5);
}

void serialWait(){
  while(Serial.peek() != 's') {
    if (Serial.peek() == 'i'){ //Read image data coming on the Serial bus
    char t = Serial.read();
    digitalWrite(LED_BUILTIN, HIGH);   // turn the LED on (HIGH is the voltage level)
    delay(1);                      
    digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
    delay(1);
    for (int j = IMG_SENSOR_YDIM-1; j >= 0; j--)
      {
        char longarr[8];
        for  (int i = 0; i < 8; i++){
          longarr[i] = Serial.read();
          if (Serial.available()< 4){
            delayMicroseconds(SERIAL_DELAY);
          }         
        }          
        unsigned long hexval =  strtoul((const char*)longarr, 0, 16);   
        img[j] = hexval;
      }          
      if (Serial.peek() == '@'){    
        char t = Serial.read(); //discard the @
        //Print out the image, to check if it was received properly
        Serial.write(0x23);Serial.write(0x23);Serial.write("\r\n");
        Serial.write("printing image...");      
        Serial.write("\r\n");
        for  (int b = 0; b < IMG_SENSOR_YDIM; b++)
        {
          for (int c = 0; c < 32; c++) //for (int c = 31; c >= 0; c--)          
          {
             Serial.print(bitRead(img[b],c));
             Serial.write(" ");
          }  
          Serial.write("\r\n");
        }         
        Serial.write(0x40);Serial.write(0x40);Serial.write("\r\n");
      }         
  }
      if (Serial.peek() == 's'){    
      //char t = Serial.read(); //discard the s
      digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
      return;// break out of the while loop and continue to the parsing steps.
    }
    if (Serial.available() > 2){
      char t = Serial.read();
    }
  }
  digitalWrite(LED_BUILTIN, LOW);    // turn the LED off by making the voltage LOW
}   

void serialEmpty() {
  while(Serial.available()) {
    char t = Serial.read();
  }
}


  short readImage(short imgX, short imgY){
  return bitRead(img[imgY],imgX);
  }

void loop() {  
  //Read serial port until sensor data is sent
  serialWait();
  
  //Interpret the data String
  //Arduino Section 1***************************************
  //data string received in format (simtime, front sensor result, right sensor result, left sensor result, left sensor 45 result,  servo rotation status)
  float simTime = Serial.parseFloat();
 /* int FrontSensor_R = Serial.parseInt();
  int RightSensor_R = Serial.parseInt();
  int LeftSensor_R = Serial.parseInt();
  int LeftSensor_45_R = Serial.parseInt();
  int RightSensor_45_R = Serial.parseInt(); */
  int LastCommand = Serial.parseInt(); //Left = 0; Right = 1; No value = 2

  float tilt;
  long timeNow = millis();
  float FrontFan_R = 0.75;

  if ((bitRead(img[15],0)) == 0  && (bitRead(img[15],31)) == 0){
    if (simTime < 3){
    tilt = -0.45;
    }
  }
  
  if ((bitRead(img[0],0)) == 0  && (bitRead(img[0],31)) == 0){
    if (simTime < 4){
      if (simTime>3){
      tilt = -1.57;
       }
    }
    /*  else if (simTime > 3.85){
        if (simTime < 6){
          tilt = -1;
          }
      }
      else if (simTime > 9){
         if (simTime < 6){
          tilt = 1;
          }
      }*/
      }
   if ((bitRead(img[0],0)) == 1  && (bitRead(img[0],31)) == 1){
    if (simTime < 15){
      if (simTime>9){
      tilt = 1.57;
       }
     }
   }
   if ((bitRead(img[0],0)) == 1  && (bitRead(img[0],31)) == 1){
    if (simTime < 25){
      if (simTime>15){
      tilt = -1.57;
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
   Serial.write(",");
   Serial.print(bitRead(img[0],0));
   Serial.write(",");
  Serial.print(bitRead(img[15],0));
   Serial.write(",");
  Serial.print(bitRead(img[15],31));
   Serial.write(",");
  Serial.print(bitRead(img[0],31));
  Serial.write("\r\n");

  //Serial.print("Hello CoppeliaSim! \r\n");
}
