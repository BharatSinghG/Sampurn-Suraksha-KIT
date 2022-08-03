#include <Adafruit_Fingerprint.h>
#include <SoftwareSerial.h>
SoftwareSerial sim(11, 10);
int _timeout;
String _buffer;
String number = "+919720292792";
#define eyeSensor 8
#define digitalalcSensor 9
#define buzzer 4
#define motorSpeed 5
#define echoPin 6 // attach pin D2 Arduino to pin Echo of HC-SR04
#define trigPin 7 //attach pin D3 Arduino to pin Trig of HC-SR04
int match=0;

#if (defined(__AVR__) || defined(ESP8266)) && !defined(__AVR_ATmega2560__)
// For UNO and others without hardware serial, we must use software serial...
// pin #2 is IN from sensor (GREEN wire)
// pin #3 is OUT from arduino  (WHITE wire)
// Set up the serial port to use softwareserial..
SoftwareSerial mySerial(2, 3);

#else
// On Leonardo/M0/etc, others with hardware serial, use hardware serial!
// #0 is green wire, #1 is white
#define mySerial Serial1

#endif


Adafruit_Fingerprint finger = Adafruit_Fingerprint(&mySerial);

// defines variables
long duration; // variable for the duration of sound wave travel
int distance; // variable for the distance measurement
void setup() {
    delay(10000);
    Serial.begin(9600);
    _buffer.reserve(50);
  Serial.println("Sistem Started...");
  sim.begin(9600);
  delay(1000);
    pinMode(eyeSensor,INPUT);
    pinMode(digitalalcSensor,INPUT);
    pinMode(buzzer,OUTPUT);
    pinMode(motorSpeed, OUTPUT);
    pinMode(trigPin, OUTPUT); // Sets the trigPin as an OUTPUT
    pinMode(echoPin, INPUT); // Sets the echoPin as an INPUT


  //while (!Serial);  // For Yun/Leo/Micro/Zero/...
  //delay(100);
  Serial.println("\n\nAdafruit finger detect test");

  // set the data rate for the sensor serial port
  finger.begin(57600);
  delay(5);
  if (finger.verifyPassword()) {
    Serial.println("Found fingerprint sensor!");
  } else {
    Serial.println("Did not find fingerprint sensor :(");
    while (1) { delay(1); }
  }

  Serial.println(F("Reading sensor parameters"));
  finger.getParameters();
  Serial.print(F("Status: 0x")); Serial.println(finger.status_reg, HEX);
  Serial.print(F("Sys ID: 0x")); Serial.println(finger.system_id, HEX);
  Serial.print(F("Capacity: ")); Serial.println(finger.capacity);
  Serial.print(F("Security level: ")); Serial.println(finger.security_level);
  Serial.print(F("Device address: ")); Serial.println(finger.device_addr, HEX);
  Serial.print(F("Packet len: ")); Serial.println(finger.packet_len);
  Serial.print(F("Baud rate: ")); Serial.println(finger.baud_rate);

  finger.getTemplateCount();

  if (finger.templateCount == 0) {
    Serial.print("Sensor doesn't contain any fingerprint data. Please run the 'enroll' example.");
  }
  else {
    Serial.println("Waiting for valid finger...");
      Serial.print("Sensor contains "); Serial.print(finger.templateCount); Serial.println(" templates");
  }
while(match==2){
getFingerprintID();
  delay(50);     
}
}   








void eyeSENSE() {
  int arr[5];
for(int i=0; i<3; i++) {
    arr[i]=digitalRead(eyeSensor);
    if(arr[i] == 1) {
    digitalWrite(buzzer,LOW);
    }
    delay(100);

}

if( arr[0]==0 && arr[1]==0 && arr[2]==0){
           carMotor(false);
     digitalWrite(buzzer,HIGH);
     delay(1000);
     
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(200);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "sleepy drowsiness, take sleep";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(1000);
  _buffer = sim.readString();


      carMotor(true);
    
    //exit(0);
}
}
void alcSENSE() {
 // int alc[10];
    // SendMessage();
  int sum=0;
 // for (int i = 0; i < 10; i++)
 // {
 //   alc[i]=digitalRead(digitalalcSensor);
 //   sum = sum+alc[i];
 //   delay(100);
 // }
  
 // if(sum<3){
  //alcohol detected with accuracy 70 percent or more
  sum=digitalRead(digitalalcSensor);
  if(sum==0){
  soundBuzzer(true);
  //delay(100);
  carMotor(false);//Serial.println ("Sending Message");
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(100);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(100);
  String SMS = "Drink and drive attempt";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(100);
  _buffer = sim.readString();

exit(0);
  }
  else{
    digitalWrite(buzzer,LOW);
  }
}


int disSENSE(){
   // Clears the trigPin condition
  digitalWrite(trigPin, LOW);
  delayMicroseconds(2);
  // Sets the trigPin HIGH (ACTIVE) for 10 microseconds
  digitalWrite(trigPin, HIGH);
  delayMicroseconds(10);
  digitalWrite(trigPin, LOW);
  // Reads the echoPin, returns the sound wave travel time in microseconds
  duration = pulseIn(echoPin, HIGH);
  // Calculating the distance
  distance = duration * 0.034 / 2; // Speed of sound wave divided by 2 (go and back)
  // Displays the distance on the Serial Monitor
  Serial.print("Distance: ");
  Serial.print(distance);
  Serial.println(" cm");
  if(distance<10&& distance>1){
      
    
    
    sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(100);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(100);
  String SMS = "crashed";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(100);
  _buffer = sim.readString();
  }
  return distance;
}

void carMotor(bool choice){
    
  if (choice)
      digitalWrite(motorSpeed,HIGH);
    
    else
      digitalWrite(motorSpeed,LOW);
    
}

void soundBuzzer (bool cond) {
    if(cond)
    digitalWrite(buzzer,HIGH);
    else
    digitalWrite(buzzer,LOW);
}

uint8_t getFingerprintID() {
  uint8_t p = finger.getImage();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image taken");
      break;
    case FINGERPRINT_NOFINGER:
      Serial.println("No finger detected");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_IMAGEFAIL:
      Serial.println("Imaging error");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK success!

  p = finger.image2Tz();
  switch (p) {
    case FINGERPRINT_OK:
      Serial.println("Image converted");
      break;
    case FINGERPRINT_IMAGEMESS:
      Serial.println("Image too messy");
      return p;
    case FINGERPRINT_PACKETRECIEVEERR:
      Serial.println("Communication error");
      return p;
    case FINGERPRINT_FEATUREFAIL:
      Serial.println("Could not find fingerprint features");
      return p;
    case FINGERPRINT_INVALIDIMAGE:
      Serial.println("Could not find fingerprint features");
      return p;
    default:
      Serial.println("Unknown error");
      return p;
  }

  // OK converted!
  p = finger.fingerSearch();
  if (p == FINGERPRINT_OK) {
    Serial.println("Found a print match!");
    match=1;
  } else if (p == FINGERPRINT_PACKETRECIEVEERR) {
    Serial.println("Communication error");
    return p;
  } else if (p == FINGERPRINT_NOTFOUND) {
    Serial.println("Did not find a match");
    //match = 0;
    return p;
  } else {
    Serial.println("Unknown error");
    return p;
  }

  // found a match!
  Serial.println("*********************************************************************");
  match =1;
    Serial.println(match);

    Serial.println("*********************************************************************");

  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);

  return finger.fingerID;
}

// returns -1 if failed, otherwise returns ID #
int getFingerprintIDez() {
  uint8_t p = finger.getImage();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.image2Tz();
  if (p != FINGERPRINT_OK)  return -1;

  p = finger.fingerFastSearch();
  if (p != FINGERPRINT_OK)  return -1;

  // found a match!
  match =1;
  
    
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(100);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Authorised user";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = sim.readString();


  Serial.print("Found ID #"); Serial.print(finger.fingerID);
  Serial.print(" with confidence of "); Serial.println(finger.confidence);
      



  return finger.fingerID;
}
int i=0;
void loop() {
 // getFingerprintID();
 // delay(50);     
  Serial.println("*********************************************************************");

    Serial.println(match);
    i=i+1;
if(i==1)
{

    
  sim.println("AT+CMGF=1");    //Sets the GSM Module in Text Mode
  delay(100);
  //Serial.println ("Set SMS Number");
  sim.println("AT+CMGS=\"" + number + "\"\r"); //Mobile phone number to send message
  delay(200);
  String SMS = "Authorised user";
  sim.println(SMS);
  delay(100);
  sim.println((char)26);// ASCII code of CTRL+Z
  delay(200);
  _buffer = sim.readString();


  
}
    Serial.println("*********************************************************************");
  if(match ==1) {
    
  alcSENSE();
  eyeSENSE();  
  //if(match ==1) {




  int dist = disSENSE();
      if (dist>30)
      {
        carMotor(true);
      }
      else if (dist>8 && dist<20){
        carMotor(false);
        delay(500);
        carMotor(true);
        delay(500);
        carMotor(false);
        delay(500);
        carMotor(true);
        delay(500);
      }
      else if(dist<8 && dist>1){
        carMotor(false);
          soundBuzzer(true);
       
      }

      }
/*  if(match!=2 || match!=1){
    match =2;
    soundBuzzer(true);
    delay(500);
    soundBuzzer(false);
  }
*/
}
