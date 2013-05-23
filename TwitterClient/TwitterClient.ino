/*
///////////////////////////////////////////////////////////////////////////////////////////////////////
  Fairly Simple Twitter Client
/////////////////////////////////////////////////////////////////////////////////////////////////////// 
 This sketch connects to Twitter using an Ethernet shield. It parses the XML
 returned, and looks for <text>this is a tweet</text> then it stores the tweet in eeprom
 in small packages so it can easily be read through and displayed on an LCD screen.
 
 Original setup includes an Arduino Duemilinove, Ethernet Shield  and a RGB LCD screen
 with an Adafruit i2c backpack with buttons.
 
 Circuit:
 * Ethernet shield attached to pins 10, 11, 12, 13
 
 created 21 May 2011
 modified 9 Apr 2012
 Patrick Jones
 
This code is in the public domain.
*/
/////////////////////////////////////////////////////////////////////////////////////////////////////// 

#include <SPI.h>
#include <Ethernet.h>
#include <Wire.h>
#include <Adafruit_MCP23017.h>
#include <Adafruit_RGBLCDShield.h>

Adafruit_RGBLCDShield lcd = Adafruit_RGBLCDShield();

#define RED 0x1
#define YELLOW 0x3
#define GREEN 0x2
#define TEAL 0x6
#define BLUE 0x4
#define VIOLET 0x5
#define WHITE 0x7

///////////////////////////////////////////////////////////////////////////////////////////////////////
// Enter a MAC address and IP address for your controller below.
// The IP address will be dependent on your local network:
///////////////////////////////////////////////////////////////////////////////////////////////////////
byte mac[] = { 
  0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x01 };
IPAddress ip(192,168,1,20);
///////////////////////////////////////////////////////////////////////////////////////////////////////
//Lots of Variables
///////////////////////////////////////////////////////////////////////////////////////////////////////
EthernetClient client;
const unsigned long requestInterval = 30000;    // delay between requests
char serverName[] = "api.twitter.com";          // twitter URL
boolean requested;                              // whether you've made a request since connecting
unsigned long lastAttemptTime, timer = 0;              // last time you connected to the server, in milliseconds
String spacer = "                        ";     //A spacer to be added in between strings
String username = "jones_private";
String currentLine, tweet, botTmp, topTmp = "";            // string to hold the text from server
int plh1, plh2, plh3 = 0;
int msgDelay = 3000;
boolean readingTweet = false;       // if you're currently reading the tweet


void setup() {
///////////////////////////////////////////////////////////////////////////////////////////////////////
// reserve space for the strings:
///////////////////////////////////////////////////////////////////////////////////////////////////////
  currentLine.reserve(256);
  tweet.reserve(150);
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Open serial communications and wait for port to open:
///////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.begin(9600);
  lcd.begin(16, 2);
  lcd.setBacklight(GREEN);
  //lcd.autoscroll();

///////////////////////////////////////////////////////////////////////////////////////////////////////  
// attempt a DHCP connection
///////////////////////////////////////////////////////////////////////////////////////////////////////
   if (!Ethernet.begin(mac)) {
    // if DHCP fails, start with a hard-coded address:
     Ethernet.begin(mac, ip);
   }
///////////////////////////////////////////////////////////////////////////////////////////////////////  
// DHCP Troubleshooting
///////////////////////////////////////////////////////////////////////////////////////////////////////
  Serial.print("My address:");
  Serial.println(Ethernet.localIP());
///////////////////////////////////////////////////////////////////////////////////////////////////////
// Display username
///////////////////////////////////////////////////////////////////////////////////////////////////////
//  lcd.clear();
//  delay(1000);
//  lcd.print("Following: " + username);
//  delay(1000);
//  lcd.clear();
///////////////////////////////////////////////////////////////////////////////////////////////////////
// connect to Twitter:
///////////////////////////////////////////////////////////////////////////////////////////////////////
  connectToServer();
}

void loop()
{
  if (client.connected()) {
    if (client.available()) {
///////////////////////////////////////////////////////////////////////////////////////////////////////
      // read incoming bytes:
///////////////////////////////////////////////////////////////////////////////////////////////////////
      char inChar = client.read();
///////////////////////////////////////////////////////////////////////////////////////////////////////
// add incoming byte to end of line:
///////////////////////////////////////////////////////////////////////////////////////////////////////
      currentLine += inChar;
///////////////////////////////////////////////////////////////////////////////////////////////////////
      // if you get a newline, clear the line:
      if (inChar == '\n') {
        currentLine = "";
      } 
///////////////////////////////////////////////////////////////////////////////////////////////////////
      // if the current line ends with <text>, it will
      // be followed by the tweet:
///////////////////////////////////////////////////////////////////////////////////////////////////////
      if ( currentLine.endsWith("<text>")) {
        // tweet is beginning. Clear the tweet string:
        readingTweet = true; 
        tweet = "";
        Serial.print("Reading tweet");
      }
///////////////////////////////////////////////////////////////////////////////////////////////////////
      // if you're currently reading the bytes of a tweet,
      // add them to the tweet String:
///////////////////////////////////////////////////////////////////////////////////////////////////////
      if (readingTweet) {
        if (inChar != '<') {
          tweet += inChar;
        }
        else {
///////////////////////////////////////////////////////////////////////////////////////////////////////
          // if you got a "<" character,
          // you've reached the end of the tweet:
          readingTweet = false;
          // close the connection to the server:
          client.stop();
          lcd.clear();
          tweet.replace(">", "");
///////////////////////////////////////////////////////////////////////////////////////////////////////
          while(timer < 25000
            for (int x = 0;x < 145; x = x + 32){
              plh1 = x;
              plh2 = plh1 + 16;
              plh3 = plh2 + 16;
              Serial.print(plh1);
              Serial.print("\n");
              Serial.print(plh2);
              Serial.print("\n");
              Serial.print(plh3);
              Serial.print("\n");
              topTmp = tweet.substring(plh1,plh2);
              Serial.print(topTmp);
              Serial.print("\n");
              botTmp = tweet.substring(plh2,plh3);
              Serial.print(botTmp);
              Serial.print("\n");
              if(topTmp.substring(0,1) != ""){
                lcd.print(topTmp + spacer + botTmp + spacer);
              }
              else {
                Serial.print("\nNo more characters in tweet\n");
                break;
              }
              delay(msgDelay); 
            }
            lcd.clear();
            delay(500);
            lcd.print("Done");
            Serial.print("Done");
        }
      }
    }   
  }
  else if (millis() - lastAttemptTime > requestInterval) {
    // if you're not connected, and two minutes have passed since
    // your last connection, then attempt to connect again:
    connectToServer();
  }
  }

void connectToServer() {
  // attempt to connect, and wait a millisecond:
  Serial.println("connecting to server...");
  if (client.connect(serverName, 80)) {
    Serial.println("making HTTP request...");
    // make HTTP GET request to twitter:
    client.println("GET /1/statuses/user_timeline.xml?screen_name=" + username + "&count=1 HTTP/1.1");
    client.println("HOST: api.twitter.com");
    client.println();
  }
  // note the time of this connect attempt:
  lastAttemptTime = millis();
}
