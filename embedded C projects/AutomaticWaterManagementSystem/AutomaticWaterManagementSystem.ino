#include <NewPing.h>
#include <SoftwareSerial.h>
String apiKey = "BF9HMHD1WEGME4JD";    // 2737PEGIY2F9413T Edit this CHANNEL API key according to your Account
String Host_Name = "sree";            // Edit Host_Name
String Password = "sree@123";          // Edit Password

SoftwareSerial ser(3,4);    // RX, TX
#define TRIGGER_PIN  12  
#define ECHO_PIN     11 
#define MAX_DISTANCE 200
int val=0;
NewPing sonar(TRIGGER_PIN, ECHO_PIN, MAX_DISTANCE); 

volatile int flow_frequency; // Measures flow sensor pulses
unsigned int l_hour; // Calculated litres/hour
unsigned char flowsensor = 2; // Sensor Input
unsigned long currentTime;
unsigned long cloopTime;

void flow () // Interrupt function
{
   flow_frequency++;
}

void setup() {  
           
Serial.begin(115200);                  // enable software serial
ser.begin(115200);                     // reset ESP8266
ser.println("AT+RST");               // Resetting ESP8266
char inv ='"';
String cmd = "AT+CWJAP";
       cmd+= "=";
       cmd+= inv;
       cmd+= Host_Name;
       cmd+= inv;
       cmd+= ",";
       cmd+= inv;
       cmd+= Password;
       cmd+= inv;
ser.println(cmd);                    // Connecting ESP8266 to your WiFi Router

  pinMode(flowsensor, INPUT);
   digitalWrite(flowsensor, HIGH); // Optional Internal Pull-Up
  attachInterrupt(0, flow, RISING); // Setup Interrupt
   sei(); // Enable interrupts
   currentTime = millis();
   cloopTime = currentTime;   
  }
void loop() {
     currentTime = millis();
   // Every second, calculate and print litres/hour
   if(currentTime >= (cloopTime + 1000))
   {
    cloopTime = currentTime; // Updates cloopTime
      // Pulse frequency (Hz) = 7.5Q, Q is flow rate in L/min.
      l_hour = (flow_frequency  / 7.5); // (Pulse frequency x 60 min) / 7.5Q = flowrate in L/hour
      flow_frequency = 0; // Reset Counter
     Serial.print(l_hour, DEC); // Print litres/hour
      Serial.println(" L/min");
      Serial.print("\n \n");
   }
  int y=l_hour;
    delay(50);  
   int x=sonar.ping_cm();
   Serial.print("ultra sonic \n");
   Serial.print(x);
   Serial.print("\n \n");
   String cmd = "AT+CIPSTART=\"TCP\",\"";          // Establishing TCP connection
  cmd += "184.106.153.149";                       // api.thingspeak.com
  cmd += "\",80";                                 // port 80
  ser.println(cmd);
  Serial.println(cmd);
 if(ser.find("Error")){
    Serial.println("AT+CIPSTART error");
    return;
  }
String getStr = "GET /update?api_key=";         // prepare GET string
  getStr += apiKey;
  getStr +="&field1=";
  getStr +=String (x);
   getStr +="&field2=";
   getStr +=String (y);
  getStr += "\r\n\r\n";
  cmd = "AT+CIPSEND=";
  cmd += String(getStr.length());                // Total Length of data
  ser.println(cmd);
  Serial.println(cmd);
if(ser.find(">")){
    ser.print(getStr);
    Serial.print(getStr);
  }
  else{
    ser.println("AT+CIPCLOSE");                  // closing connection
    Serial.println("AT+CIPCLOSE");
  }
 delay(1000);                                  // Update after every 1 seconds
}

