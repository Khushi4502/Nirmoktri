/*
हल code 

हल is the advanced version of the traditional scarecrow which uses several sensors, controllers, and different IoT devices 
that runs on renewable energy produces less pollution, and don't hurt nearby living things.
It detected if any unnecessary stray animals or birds entering the field and tries to stop them out of the field area using the 
alarm system which generates the high frequency sound waves and also sends the alert to the farmer using the “GPRS Module” at the 
same time.

In addition to these we added some basic sensors like Moisture Sensor and Rain sensor which provides the reading of the moisture
level in the soil time to time and also alert the farmer if it is raining or not.
Which help us to take proper care of the farm and also all the data will send to the farmer using “SMS”.

In this code we are having basic softwareserial library and in code PIR sensor, Rain sensor, Moisture sensor.And also we are 
using sms send system and we are also detecting PIR sensor reading and make diffrent types of functions and define every single
things in code. 
This is a SoftwareSerial library :- https://github.com/PaulStoffregen/SoftwareSerial Here you can download library.

*/
#include <SoftwareSerial.h>

SoftwareSerial mySerial(2, 3);  //SIM800L Tx & Rx is connected to Arduino #3 & #2

int PIR_led = 12;     // the pin that the PIR INDICATION LED is atteched to
int PIR_SENSOR = A2;  // the pin that the sensor is atteched to
int PIR_STATE = LOW;  // by default, no motion detected
int PIR_VAL = 0;      // variable to store the sensor status (value)
int P = 0;      
int R = 0;
int S = 0;
int MA;

String PHONENUMBER = "";

const unsigned long EYELED_DELAY = 500;         //3600000; //in ms
const unsigned long eventTime_1_BUTTON = 3000;  //3600000; //in ms

unsigned long previousTime_mode1 = 0;
unsigned long previousTime_mode2 = 0;

const int RAIN_SENSOR_D = A0;          //Rain sensor constant
float RAIN_SENSOR_VAL;
float MOISTURE_SENSOR_A = A1;
float MOISTURE_SENSOR_value;  // variable to store the sensor status (value)

void setup() {  // put your setup code here, to run once:

  Serial.begin(9600);  // initialize serial

  Initializ_PIN();

  Serial.println("PLEASE ENTER YOUR PHONE NUMBER WITN COUNTRY CODE:");

  while (Serial.available() == 0) {
  }

  PHONENUMBER = Serial.readString();
  Serial.println(PHONENUMBER);

  Initializ_GSM();
}

void loop() {             

  unsigned long currentTime = millis();

  updateSerial();

  PIRSENSOR();

  MOISTURE_UPDATE();
}

void Initializ_PIN() {       // in this code we are getting initialize all the finctions and in this we have rain,PIR, 
                                                                //  Moisture sensor are used.
  pinMode(PIR_led, OUTPUT);
  pinMode(PIR_SENSOR, INPUT);  // initialize sensor as an input
  pinMode(RAIN_SENSOR_D, INPUT);
  pinMode(MOISTURE_SENSOR_A, INPUT);
  Serial.println("RESPECTED SIR/MAM");   //from this all the functions getting print in output
  delay(100);
  Serial.println("Team Name :- NIRMOKTRI");
  delay(100);
  Serial.print("THIS IS OUR PROJECT :- ");
  Serial.println("HAL");
  delay(200);
  Serial.println();
  Serial.println("MY TEAM MEMBERS ARE");
  Serial.println("1.URVISH SHAH");
  Serial.println("2.KHUSHI GAJJAR");
  Serial.println("3.JAY PATIL");
  Serial.println("4.KRISHNA SAHO");
  delay(300);
  Serial.println("OUR PROJECT IS AS FOLLOWS");
  delay(100);
}
void Initializ_GSM() { // this is a basic functions that are used for GSM module and in this function used for GSM Initializing
  mySerial.begin(9600);  //Begin serial communication with Arduino and SIM800L
  Serial.println("Initializing...");
  delay(500);
  mySerial.println("AT");  //Once the handshake test is successful, it will back to OK
  updateSerial();
}
void updateSerial() {     // This function are generally used for Serial library reading.
  delay(100);
  while (Serial.available()) {
    mySerial.write(Serial.read());  //Forward what Serial received to Software Serial Port
  }
  while (mySerial.available()) {
    Serial.write(mySerial.read());  //Forward what Software Serial received to Serial Port
  }
}
void TEXT_MODE() { //in this function we are generated text mode and configuring that text mode
  mySerial.println("AT+CMGF=1");  // Configuring TEXT mode
  updateSerial();
  mySerial.println("AT+CMGS=\"" + PHONENUMBER + "\"");  //change ZZ with country code and xxxxxxxxxxx with phone number to sms
  updateSerial();
}
void SENDSMS() {  // this is used for sendsms in serial mode
  updateSerial();
  mySerial.write(26);
}
void RAIN_SENSOR() {  //This is a rain sensor data and this data are generally for rain sensor if rain is happning in field and 
                                                 //also write serial data in file and output 
  if (digitalRead(RAIN_SENSOR_D) == HIGH) {
    if (R <= 1) {
      Serial.println("ITS RAINING IN THE FIELD");
      TEXT_MODE();
      mySerial.print("ITS RAINING IN THE FIELD");  //text content
      SENDSMS();
      updateSerial();
      R++;
    }
  } else {
    R = 0;
  }
}
void MOISTURE_SENSOR() {  // This sensor block we are having moisture value and what kind of moisture needed to our crop and in 
                                                         //this we are having sms and serial both library.
  MA = analogRead(MOISTURE_SENSOR_A);
  MOISTURE_SENSOR_value = (100 - ((MA / 1023.00) * 100));
  if (MOISTURE_SENSOR_value <= 5) {
    if (S <= 1) {
      TEXT_MODE();
      Serial.println("MOISTURE IS LESS THAN 5%;PLEASE WATERED YOUR CROP");  //text content
      mySerial.print("MOISTURE IS LESS THAN 5%;PLEASE WATERED YOUR CROP");  //text content
      SENDSMS();   // uses for send sms
      updateSerial();   //update serial ports
      S++;
    }
  } else {
    S = 0;
  }
}
void PIRSENSOR() {   //In this we are having PIR Sensor and also having diffrent PIR state values and rain & moisture sensor 
                              //and also detect if someone enter in perticular field and send sms in user database and user sms.
  PIR_VAL = digitalRead(PIR_SENSOR);  // read sensor value
  if (PIR_VAL == HIGH) {
    RAIN_SENSOR();    // print rain sensor value 
    MOISTURE_SENSOR();    //Moisture sensor value 
    if (PIR_STATE == LOW) {
      if (P <= 1) {
        TEXT_MODE();
        Serial.println("SOMEONE ENTERS IN YOUR FIELD");  //text content
        mySerial.print("SOMEONE ENTERS IN YOUR FIELD");  //text content
        SENDSMS();
        P++;
        RAIN_SENSOR();
        MOISTURE_SENSOR();
      }
      PIR_STATE = HIGH;  // update variable state to HIGH
    }

} else {
    RAIN_SENSOR();
    MOISTURE_SENSOR();
    if (PIR_STATE == HIGH) {
      PIR_STATE = LOW;  // update variable state to LOW
      P = 0;
      RAIN_SENSOR();
      MOISTURE_SENSOR();
    }
  }
}

void MOISTURE_UPDATE() {
  unsigned long currentTime = millis();
  while (currentTime - previousTime_mode1 >= eventTime_1_BUTTON) {
    TEXT_MODE();
    Serial.print("MOISTURE IN SOIL IS ");
    Serial.print(MOISTURE_SENSOR_value);
    Serial.println("%");
    mySerial.print(MOISTURE_SENSOR_value);  //text content
    SENDSMS();
    previousTime_mode1 = currentTime;
  }
}
