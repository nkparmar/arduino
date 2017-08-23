#include <SoftwareSerial.h>

SoftwareSerial GPRS(7, 8); // TX, RX
boolean state;
int SensorDetectionCount=0, phoneCount=0;
int count=0;
int sensorPin = 2;
int buzzerPin = 5;
int led_1 = 10;
int led_2 = 11;

unsigned char buffer[64];
void setup()
{
  GPRS.begin(9600);
  Serial.begin(9600);
  GPRS.println("AT+CMGF=1");
  SensorDetectionCount=0;
   
  delay(60000);
  pinMode(sensorPin, INPUT);
  pinMode(buzzerPin, OUTPUT);
  pinMode(led_1, OUTPUT);
  pinMode(led_2, OUTPUT);
  state = digitalRead(sensorPin);
  Serial.print("First time :" );

  Serial.print("SensorDetectionCount value :");
  Serial.println(SensorDetectionCount);   
}

void loop()
{

//delay(10000);
state = digitalRead(sensorPin);
boolean sendSMS = false;

if (state == HIGH) {
  sendSMS = true;
  SensorDetectionCount++;
}


if (SensorDetectionCount == 1) {
  digitalWrite(led_1, HIGH);
}

if (SensorDetectionCount == 2) {
  digitalWrite(led_2, HIGH);
}

Serial.println("=============================BEGIN==========================");
Serial.print("Movement:  ");
Serial.println(state ? "on" : "off");

  Serial.print("SensorDetectionCount value :");
  Serial.println(SensorDetectionCount);

  Serial.print("PhoneCount::");
  Serial.println(phoneCount);
    

if(sendSMS) { // 
     
     Serial.println("Sending SMS: ");
     Serial.print(sendSMS ? "yes": "no");
     
  // CHANGE THE MOBILE NUMBER!!!
        GPRS.println("AT+CMGS=\"+91XXXXXXXXXX\"\n"); 
        GPRS.println("Bro, looks like I have an uninvited guest. Please check.");  
//        GPRS.println("Yes");  
        GPRS.write( 0x1a ); // ctrl+Z character
  
        if (Serial.available()) {
            byte b = Serial.read();
            if (b == '*')
            GPRS.write(0x1a);
          else
            GPRS.write(b);
      }
  
  Serial.println("Sent SMS");
  delay(20000);
      
      if (SensorDetectionCount >= 2) {
//   If SensorDetectionCount is > 2, place a phone call

                Serial.print("SensorDetectionCount value :");
                Serial.println(SensorDetectionCount);
              Serial.println("Placing call..");
              GPRS.println("ATD+919945060790;");
              delay(20000);
              SensorDetectionCount = 0;
              digitalWrite(led_1, LOW);
              digitalWrite(led_2, LOW);
              
              Serial.println("SensorDetectioncount has been reset");
              Serial.println("SensorDetectionCount value :");
              Serial.print(SensorDetectionCount);
              
              phoneCount++;
              
        if (phoneCount > 2) {
        Serial.println("Raising Alarm...");
        digitalWrite(buzzerPin, HIGH);
        phoneCount =0;
        SensorDetectionCount=0;
      }
    }
   
   }
   Serial.println("=============================END=========================="); 
}

