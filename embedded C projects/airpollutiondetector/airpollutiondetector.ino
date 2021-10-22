//Libraires
#include <stdlib.h>
#include <DHT.h>
#include <LiquidCrystal.h>


/*----------------------------------------------------------*/
int sensorPin = A0;
int sensorPin1 = A1;
int sensorPin2 = A2;
const int ledPin =  13;      // the number of the LED pin
const int ledPin1 =  12;
/*-----------------ESP8266 Serial WiFi Module---------------*/
#define SSID "abcd"     // "SSID-WiFiname" 
#define PASS ""       // "password"
#define IP "184.106.153.149"// thingspeak.com ip
LiquidCrystal lcd(10, 11, 9,8, 7, 6);
String msg = "GET /update?key=RMCPHBHT24UREN2U"; //change it with your key...
/*-----------------------------------------------------------*/

//Variables
float temp;
float temp1;
float temp2;
int hum;
String tempC;
int error;
void setup()
{
  Serial.begin(9600); 
   lcd.begin(16, 2);
  Serial.println("AT");
  delay(5000);
  if(Serial.find("OK")){
    connectWiFi();
  }
}

void loop(){
  //Read temperature and humidity values from DHT sensor:
  start: //label 
  error=0;
   temp1 = analogRead(sensorPin1);
   temp2 = analogRead(sensorPin2);
 /* temp1=temp1-150;
 if((temp1>150)&&(temp1<250))
   {temp1=temp1-150;}
   if((temp1>200)&&(temp1<350))
   {temp1=temp1-250;}
   if((temp1>350))
   {temp1=70;}
   if(temp1<0)
   {temp1=75;}
    if((temp2>240)&&(temp2<350))
   {temp2=temp2-250;}
    */
  temp = analogRead(sensorPin);
  temp=temp-30;
    temp1=temp1-100;
  hum = dht.readHumidity();
  char buffer[10];
  // there is a useful c function called dtostrf() which will convert a float to a char array 
  //so it can then be printed easily.  The format is: dtostrf(floatvar, StringLengthIncDecimalPoint, numVarsAfterDecimal, charbuf);
  tempC = dtostrf(temp, 4, 1, buffer); 

  updateTemp();
  //Resend if transmission is not completed 
  if (error==1){
    goto start; //go to label "start"
  }
  
  delay(360); //Update every 1 hour
}

void updateTemp(){
  String cmd = "AT+CIPSTART=\"TCP\",\"";
  cmd += IP;
  cmd += "\",80";
  Serial.println(cmd);
  delay(2000);
  if(Serial.find("Error")){
    return;
  }
  lcd.clear();
    lcd.setCursor(0, 1);
    lcd.print("Temp ");
     lcd.print(temp);
   //  lcd.clear();
    lcd.setCursor(0, 0);
      lcd.print("Alco");
     lcd.print(temp1);
        lcd.setCursor(8, 0);
        lcd.print(" Co2 ");
     temp1=temp1-170;
     temp2=temp2-100;
     lcd.print(temp2);
     if( temp>34)
  {
digitalWrite(ledPin,HIGH);
delay(500);
}
  if( temp<34)
  {
digitalWrite(ledPin,LOW);
delay(500);
}
if( temp1>60)
  {
digitalWrite(ledPin1,HIGH);
delay(1500);
}
if( temp2>380)
  {
digitalWrite(ledPin1,HIGH);
delay(500);
}
else
{digitalWrite(ledPin,LOW);
digitalWrite(ledPin1,LOW);
  
  }
  cmd = msg ;
  cmd += "&field1=";    //field 1 for temperature
  cmd += tempC;
  cmd += "&field2=";  //field 2 for humidity
  cmd += String(temp1);
  cmd += "&field3=";  //field 2 for humidity
  cmd += String(temp2);
  cmd += "\r\n";

  }
  else{

    error=1;
  }
}

 
boolean connectWiFi(){
  Serial.println("AT+CWMODE=1");
  delay(2000);
  String cmd="AT+CWJAP=\"";
  cmd+=SSID;
  cmd+="\",\"";
  cmd+=PASS;
  cmd+="\"";
  Serial.println(cmd);
  delay(5000);
  if(Serial.find("OK")){
    return true;
  }else{
    return false;
  }
}
