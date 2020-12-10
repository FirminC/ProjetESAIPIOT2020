
/*
  Arduino Webserver using ESP8266
  Displays temperature in a webpage

  Arduino Mega has three Serial communication ports,this code works well with 
  Arduino Mega.For UNO users,use Softserial library to connect ESP8266 with
  Arduino UNO 
  If you're unsure about your Arduino model or its pin cofigurations,please check
  the documentation at http://www.arduino.cc
 
  modified August 2016
  By Joyson Chinta and Gladson Kujur
 */
#include <SoftwareSerial.h>

SoftwareSerial wifiSerial(10, 11);

#define DEBUG true

void setup()
{
  Serial.begin(9600);     ///////For Serial monitor
  wifiSerial.begin(9600); ///////ESP Baud rate
  pinMode(A0,INPUT);      //Pour potentiometre
  pinMode(A1,INPUT);      //Pour photoresistance
  pinMode(6,OUTPUT);
  
  sendData("AT+RST\r\n", 2000, DEBUG);      // reset module
  sendData("AT+CWMODE=1\r\n", 1000, DEBUG); // configure as access point
  sendData("AT+CWJAP=\"OnePlus 3T de Fifi\",\"fifilepatissier\"\r\n", 10000, DEBUG);
  sendData("AT+CIFSR\r\n", 1000, DEBUG);          // get ip address
  sendData("AT+CIPMUX=1\r\n", 1000, DEBUG);       // configure for multiple connections
  sendData("AT+CIPSERVER=1,80\r\n", 1000, DEBUG); // turn on server on port 80
}

int connectionId;
void loop()
{
  if (wifiSerial.available())
  {
    /////////////////////Recieving from web browser to toggle led
    if (wifiSerial.find("+IPD,"))
    {
      delay(300);
      connectionId = wifiSerial.read() - 48;
      if(wifiSerial.find("LEDON"))
      { 
        Serial.println("J'allume la LED");
        digitalWrite(6,HIGH);
      }
      else
      {
        Serial.println("J'éteinds la LED");
        digitalWrite(6,LOW);
      }
      int poto = analogRead(A0);
      int photo = analogRead(A1);
      String webpage = "HTTP/1.1 200 OK\r\nConnection: close\r\n\r\n";
      webpage +
      = (String)"{\"potentiometer\": {\"value\": "+poto+"}, \"photoresistor\": {\"value\": "+photo+"}}";
      espsend(webpage);

      String closeCommand = "AT+CIPCLOSE="; ////////////////close the socket connection////esp command
      closeCommand += connectionId;         // append connection id
      closeCommand += "\r\n";
      sendData(closeCommand, 3000, DEBUG); 
    }
  }
}

//////////////////////////////sends data from ESP to webpage///////////////////////////

void espsend(String d)
{
  String cipSend = " AT+CIPSEND=";
  cipSend += connectionId;
  cipSend += ",";
  cipSend += d.length();
  cipSend += "\r\n";
  sendData(cipSend, 1000, DEBUG);
  sendData(d, 1000, DEBUG);
}

//////////////gets the data from esp and displays in serial monitor///////////////////////
String sendData(String command, const int timeout, boolean debug)
{
  String response = "";
  wifiSerial.print(command);
  long int time = millis();
  while ((time + timeout) > millis())
  {
    while (wifiSerial.available())
    {
      char c = wifiSerial.read(); // read the next character.
      response += c;
    }
  }

  if (debug)
  {
    Serial.print(response); //displays the esp response messages in arduino Serial monitor
  }
  return response;
}

//////////////Music///////////////////////
