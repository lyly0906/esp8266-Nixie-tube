/*
 Displaying a string that contains a period on a seven-segment display.

 Contributed by Matthew Nielsen (github.com/xunker) for the SevSeg Arduino
 Library (github.com/DeanIsMe/SevSeg) by Dean Reading.

 Licensed under the Apache License, Version 2.0 (the "License");
 you may not use this file except in compliance with the License.
 You may obtain a copy of the License at
 http://www.apache.org/licenses/LICENSE-2.0

 Unless required by applicable law or agreed to in writing, software
 distributed under the License is distributed on an "AS IS" BASIS,
 WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 See the License for the specific language governing permissions and
 limitations under the License.

 */

#include "SevSeg.h"
#include <Arduino.h> 
#include <ESP8266WiFi.h>

#define WIFI_AP "*****"  //SSID
#define WIFI_PASSWORD "******" //password
SevSeg sevseg; //Instantiate a seven segment controller object
#define MAX_NUMBER_STRINGS 12
#define MAX_STRING_SIZE 8
char testStrings[MAX_NUMBER_STRINGS][MAX_STRING_SIZE];

#define PATTERN_CHANGE_TIME 600
unsigned long timer = millis();
byte testStringsPos = 0;
unsigned char  sjh;
unsigned char sjm;
unsigned char sjs;
WiFiClient client;
int status = WL_IDLE_STATUS;

void setup() {
  Serial.begin(9600);
  byte numDigits = 4;
  byte digitPins[] = {1, 3, 0, 2}; //数码管公共极连接Arduino的引脚 4,3,2,1
  byte segmentPins[] = {13, 12, 14, 16, 9, 10, 4, 5}; //数码管a,b,c,d,e,f,g,dp对应引脚
  bool resistorsOnSegments = false; // 'false' means resistors are on digit pins
  byte hardwareConfig = COMMON_ANODE; // See README.md for options
  bool updateWithDelays = false; // Default. Recommended
  bool leadingZeros = false; // Use 'true' if you'd like to keep the leading zeros
  bool disableDecPoint = false; // Use 'true' if your decimal point doesn't exist or isn't connected
  sevseg.begin(hardwareConfig, numDigits, digitPins, segmentPins, resistorsOnSegments, updateWithDelays, leadingZeros, disableDecPoint);
  sevseg.setBrightness(90);
  InitWiFi();
  getWeatherCode();
}

void loop() {

  // Cycle to the next string every one second
  static int deciSeconds = 0;
  if (millis() - timer >= 100) {
    timer += 100;
    deciSeconds++; // 100 milliSeconds is equal to 1 deciSecond
    
    if (deciSeconds == 600) { // Reset to 0 after counting for 1000 seconds.
      deciSeconds=0;
      getWeatherCode();
    }
    int xx = sjh * 100 + sjm;
    sevseg.setNumber(xx, 2);
    //getWeatherCode();
  }
  sevseg.refreshDisplay(); // Must run repeatedly
}

void InitWiFi() {
  
  Serial.println("Connecting to AP ...");
  // attempt to connect to WiFi network

  WiFi.begin(WIFI_AP, WIFI_PASSWORD);
  while (WiFi.status() != WL_CONNECTED) {
    delay(500);
    Serial.print(".");
  }
  Serial.println("Connected to AP");
}

void reconnect() {
  // Loop until we're reconnected
  while (!client.connected()) {
    status = WiFi.status();
    if ( status != WL_CONNECTED) {
      WiFi.begin(WIFI_AP, WIFI_PASSWORD);
      while (WiFi.status() != WL_CONNECTED) {
        delay(500);
        Serial.print(".");
      }
      Serial.println("Connected to AP");
    }
  }
}

 int   getWeatherCode(){
    
     
         const char* host1 = "time.beijing-time.org";        // 获取 HTTP网站,的IP地址用

    String url1 = "/time.asp";        //  获取 HTTP网站主页下的连接
      
         
    WiFiClient client;

   
      const int httpPort = 80;
      
      
      
      
      
      //sevseg.setNumber(client.connect(host1, httpPort), 2);
      if (!client.connect(host1, httpPort))
      {
        
           return  -1;
       
      }

      //sevseg.setNumber(111, 2);
      client.print(String("GET ") + url1 + " HTTP/1.1\r\n" +
                   "Host: " + host1 + "\r\n" +
                   "User-Agent: BuildFailureDetectorESP8266\r\n" +
                   "Connection: close\r\n\r\n");
        delay(150);
      
String str = "";
while (client.connected()) {
      String line = client.readStringUntil('\r');
        str += line;
  }

     //sevseg.setNumber(str.length(), 2);
    
    //     Serial.println(str);  OK
     
        String sjall_data ="";
    
      String new_data ="";
        String       STSJS ="";
       String STSJM ="";
        String STSJH ="";
    
     //    new_data =   str.substring(   str.indexOf("nyear")+1 ,    str.length()     ) ;

       // ========================分割字符, "nyear"开始-到结束,只需要这段字符
       
       
       sjall_data =   str.substring(   str.indexOf("getTime")+10 ,    str.length()     ) ;
       
            if( sjall_data.length() <5 )  return -1;  // 

      new_data =   str.substring(   str.indexOf("nyear")+6 ,    str.length()     ) ;   // "nyear" 0 -- end,

  
       new_data =   new_data.substring(  new_data.indexOf("nhrs")+5 ,    new_data.length()     ) ;
       
   
       
       
  
     STSJH =   new_data.substring(  0, new_data.indexOf(":")  ) ;   // hhhhhhhhhhhhhhhhhhhhhhhhhhh
  
  
  
   new_data =   new_data.substring(  new_data.indexOf("nmin")+5 ,    new_data.length()     ) ;
  
    STSJM =   new_data.substring(  0, new_data.indexOf(":")  ) ;        //mmmmmmmmmmmmmmmmmmmmmmmmm
     
    
       new_data =   new_data.substring(  new_data.indexOf("nsec")+5 ,    new_data.length()     ) ;
       
      STSJS =   new_data.substring(  0, new_data.indexOf(":")  ) ;        // sssssmmmmmmmmmmmmsssssss
     
  
           
 // ===================字符转INT,如果字符是纯数字,转换后的INT值就可以赋值给数码管使用,    sjm sjh
 
      sjm  =  STSJM.toInt();
      sjh  =  STSJH.toInt();
      sjs =  STSJS.toInt()+6;
 
 Serial.println("====  SJ=;      "); 
 Serial.println(sjm); 
           
      return  99;



    }

/// END ///
