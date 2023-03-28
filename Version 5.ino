#include <Servo.h> 
#include <RTClib.h>
#include <DFPlayerMini_Fast.h>

String command;                             //Start/pause on app
int stuck = 0;

DFPlayerMini_Fast myMP3;                    //MP3
void printDetail(uint8_t type, int value);

const byte numChars = 32;                   //Bluetooth
char receivedChars[numChars]; 
boolean newData = false;      

int servoPinL = 7;                          //Servo
int servoPinR = 6;
int ypos;
Servo Servo1; 
Servo Servo2; 

int pir1 = 2;                              //PIR Motion Right
int pir2 = 4;                              //PIR Motion Back
int pir3 = 13;                              //PIR Motion Left
int pirState = LOW;
int val1, val2, val3 = 0;

uint8_t DAILY_EVENT_START_HH = 6;           //Event start time @ hour
uint8_t DAILY_EVENT_END_HH   = 19;          //Event end time @ hour
RTC_DS3231 rtc;
char daysOfTheWeek[7][12] = {
  "Sunday",
  "Monday",
  "Tuesday",
  "Wednesday",
  "Thursday",
  "Friday",
  "Saturday"
};
int notifState = 0;
int ooff;

void setup() {
  Servo1.attach(servoPinL);                       //Servo
  Servo2.attach(servoPinR); 

  pinMode(pir1, INPUT);                           //PIR Motion
  pinMode(pir2, INPUT);  
  pinMode(pir3, INPUT);  

  Serial1.begin(9600);                            //Smartphone
  Serial2.begin(9600);                            //MP3 Player

  if (! rtc.begin()) {
    Serial1.println("Couldn't find RTC");
    while (1);
  }
  
  rtc.adjust(DateTime(F(__DATE__), F(__TIME__))); //Set to the date & time on PC this sketch was compiled
  //rtc.adjust(DateTime(2023, 2, 11, 10, 0, 0));     //January 21, 2021 @ 3 A.M.

  if (!myMP3.begin(Serial2)) {                    //HardwareSerial to communicate w/ MP3
    Serial1.println(F("Unable to begin:"));
    Serial1.println(F("1.Please recheck the connection!"));
    Serial1.println(F("2.Please insert the SD card!"));
    while(true);
  }
  myMP3.volume(30);
  Servo1.write(0);                 
  Servo2.write(120);
  Serial1.println("Initializing device. Please wait 1 minute.");
  Serial1.println("");
  delay(60000);
  Serial1.println("Initializing Done.");
  Serial1.println("");
}

void loop() {
  DateTime now = rtc.now();
  val1 = digitalRead(pir1);
  val2 = digitalRead(pir2);
  val3 = digitalRead(pir3);

  if (now.hour() >= DAILY_EVENT_START_HH && now.hour() <= DAILY_EVENT_END_HH) {
    ooff = 1;
  }
  else {
    ooff = 2;
  }

  switch (ooff) {
    case 1:
      if (notifState == 0) {
        notifState = 1;
        Serial1.println("Device turned on!");
        Serial1.println("");
      }

      receiveInfo();
      if (val1 == HIGH || val2 == HIGH || val3 == HIGH) {
        if (pirState == LOW && stuck == 0) {
          pirState = HIGH;
          myMP3.randomAll();
          delay(500);
          myMP3.pause();
          playArms();
          sendInfo(now);
        }
      }
      else {
        if (pirState == HIGH) {
          pirState = LOW;
          myMP3.stop();
          staticArms();
          Serial1.println("No bird detected in the surrounding area!");
          Serial1.println("");
        }
      }
      break;
    case 2:
      if (notifState == 1) {
        notifState = 0;
        Serial1.println("Device turned off!");
        Serial1.println("");
      }
    break;
  }
}

void receiveInfo() {
  while (Serial1.available() == 1) {}
    command = Serial1.readString();
    command.toLowerCase();

    if (command == "pause") {
      stuck = 1;
      Serial1.println("Device is paused!");
      Serial1.println("");
    }

    if (command == "start") {
      stuck = 0;
      Serial1.println("Device is starting!");   
      Serial1.println("");
    }
}

void sendInfo(DateTime time) {
  Serial1.println("System detected bird!");
  Serial1.println("Deploying countermeasures!");
  Serial1.println("Time Detected: ");
  Serial1.print(time.hour(), DEC);
  Serial1.print(':');
  Serial1.print(time.minute(), DEC);   
  Serial1.println("");
}

void playArms(){
  ypos = random(270);                 //Generate Random Value For Servo
  Servo1.write(ypos);                 
  Servo2.write(ypos);

  int delayAmount = random(2000,3000);  //M.s.
  delay(delayAmount);
}

void staticArms(){
  Servo1.write(0);                 
  Servo2.write(120);
}