/*
   Open Weather One Call Library
   v3.0.4
   Copyright 2020 - Jessica Hershey
   www.github.com/JHershey69

   One Call API key at www.openweathermap.org
   Google Developer Key no longer required.

   Simple_Latitude_Longitude_Weather_Example.ino

   Returns ALL information and uses Latitude/Longitude, CITY ID, or IP Address
   If using a hotspot to connect your ESP32 to the WWW your results for IP
   search maybe be radically out of range. Please consult the documentation
   for use and variables available in the returned weather message

*/

// ===== Required libraries (Other required libraries are installed in header file)
#include <OpenWeatherOneCall.h>
#include <WiFi.h> //<-- Might be native to ESP32
#include "arduino_secrets.h"
// ===============================================================================


// ========= Required only for GxEPD Display ==============
#define LILYGO_T5_V213
#include <boards.h>
#include <GxEPD.h>
#include <GxDEPG0213BN/GxDEPG0213BN.h>       // 2.13" b/w form DKE GROUP
#include <U8g2_for_Adafruit_GFX.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>

GxIO_Class io(SPI,  EPD_CS, EPD_DC,  EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;
//========= End GxEPD Library =============================


char HOMESSID[] = SECRET_SSID;
char HOMEPW[] =  SECRET_PASS;
char ONECALLKEY[] = SECRET_APIKEY;

//  OpenWeatherOneCall variables

//For Latitude and Longitude Location setting if used
float myLATITUDE = SECRET_LAT;    //<-----Some location on theGulf Coast of Florida
float myLONGITUDE = SECRET_LON;

int myUNITS = IMPERIAL;          //<-----METRIC, IMPERIAL, KELVIN (IMPERIAL is default)

//Can't get current and historical at the same time
int myHISTORY = NULL;            //<-----Only required for historical data up to 5 days

//See manual for excludes, only CURRENT Data allows 1,000,000 calls a month
//int myEXCLUDES = EXCL_D+EXCL_H+EXCL_M+EXCL_A;              //<-----0 Excludes is default
int myEXCLUDES = EXCL_H+EXCL_M;             //<-----0 Excludes is default

//for debugging loop counting
int nextCall = 0;
//int callAttempt = 1;

OpenWeatherOneCall OWOC;              // <------ Invoke Library like this

// WiFi Connect function **********************************
void connectWifi() {
  WiFi.begin(HOMESSID, HOMEPW);
  printf("Connecting.");
  int TryNum = 0;
  while (WiFi.status() != WL_CONNECTED) {
    printf(".");
    delay(200);
    TryNum++;
    if (TryNum > 20) {
      printf("\nUnable to connect to WiFi. Please check your parameters.\n");
      for (;;);
    }
  }
  printf("Connected to: % s\n\n Wifi");
} //================== END WIFI CONNECT =======================



void setup() {
  // In this sketch for display of values
  Serial.begin(115200);
  while (!Serial) {
    ;
  }
  Serial.println("Serial Monitor Initialized");

    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init(); // enable diagnostic output on Serial
    u8g2Fonts.begin(display);

    u8g2Fonts.setFontMode(0);                           // use u8g2 transparent mode (this is default)
    u8g2Fonts.setFontDirection(1);                      // left to right (this is default)
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);          // apply Adafruit GFX color
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);          // apply Adafruit GFX color
    u8g2Fonts.setFont(u8g2_font_courB10_tr); 


  // WiFi Connection required *********************
  while (WiFi.status() != WL_CONNECTED) {
    connectWifi();
  } //<----------------End WiFi Connection Check


  /*
  Getting the Weather
  If you desire WiFi Triangulation to find your
  Latitude and Longitude, please install the
  WiFiTriangulation Library
  Otherwise Latitude and Longitude can be from any
  source such as a GPS or entered manually.
  If you DO NOT set the Latitude and Longitude
  the program will attempt to find your location
  by IP Address (which could be grossly innacurate
  if you are using a HOTSPOT on your phone)
  */

  // First set your Key value
  OWOC.setOpenWeatherKey(ONECALLKEY);

  // Here we use the Lat and Lon for Pensacola, Florida (Using the predefined values)
  OWOC.setLatLon(myLATITUDE, myLONGITUDE);

  // Third set any EXCLUDES if required (Here we are not using any
  OWOC.setExcl(myEXCLUDES);

  // Set History if you want historical weather other wise NULL
  OWOC.setHistory(myHISTORY);
   
    // Set UNITS of MEASURE otherwise default is IMPERIAL
  OWOC.setUnits(myUNITS);

}

void loop() {


   //Now call the weather. Please notice no arguments are required in this call
  OWOC.parseWeather();

  //Now display some information, note the pointer requirement for current and alert, this is NEW for v3.0.0

  // Location info is available for ALL modes (History/Current)
  printf("\nLocation: % s, % s % s\n", OWOC.location.CITY, OWOC.location.STATE, OWOC.location.COUNTRY);

  //Verify all other values exist before using
  if (myHISTORY) //Remember you can't get historical and current weather at the same time
  {
    if (OWOC.history)
    {
      printf("Mean Temp for % s : % .0f\n", OWOC.history[0].weekDayName, OWOC.history[0].temperature);
    }
  } else
  {      
    if (OWOC.alert) {
      Serial.printf("ALERT *** ALERT *** ALERT\n");
      Serial.printf("Sender : % s\n", OWOC.alert->senderName);
      Serial.printf("Event : % s\n", OWOC.alert->event);
      Serial.printf("ALERT : % s\n", OWOC.alert->summary);      
    u8g2Fonts.setCursor(17, 10);                          // start writing at this position
    u8g2Fonts.print(OWOC.alert->event);
    u8g2Fonts.setCursor(2, 10);                          // start writing at this position
    u8g2Fonts.print(OWOC.alert->summary);
          
    }
    
    if (OWOC.current)
    {
      Serial.printf("Last Update: % s\n", OWOC.current->readableDateTime);
      Serial.printf("Current Temp : % .0f\n", OWOC.current->temperature);      
      Serial.printf("Current Humidity : % .0f\n", OWOC.current->humidity);
      Serial.printf("Current Pressure : % .0f\n", OWOC.current->pressure);             
      Serial.printf("Current Wind : % .0f\n", OWOC.current->windSpeed);      
      Serial.printf("Current Condition : % s\n", OWOC.current->summary);
    display.fillScreen(GxEPD_WHITE);   
    u8g2Fonts.setCursor(107, 10);                          // start writing at this position
    u8g2Fonts.printf("T:%.0f", OWOC.current->temperature);
    u8g2Fonts.setCursor(107, 70);    
    u8g2Fonts.printf("H:%.0f", OWOC.current->humidity);
    u8g2Fonts.setCursor(107, 140);    
    u8g2Fonts.printf("P:%.0f", OWOC.current->pressure);
    u8g2Fonts.setCursor(92, 10);                          // start writing at this position
    u8g2Fonts.printf("W:%.0f", OWOC.current->windSpeed);
    u8g2Fonts.setCursor(92, 70);    
    u8g2Fonts.printf("%s", OWOC.current->summary);
                             
    }

    if (OWOC.forecast)
    {
      Serial.printf("Forecast Temp Today : % .0f\n", OWOC.forecast[0].temperatureHigh);
      Serial.printf("Forecast Wind Today : % .0f\n", OWOC.forecast[0].windSpeed);      
      Serial.printf("Forecast Condition Today : % s\n", OWOC.forecast[0].summary);                
      Serial.printf("Forecast Temp Tomorrow : % .0f\n", OWOC.forecast[1].temperatureHigh);
      Serial.printf("Forecast Wind Tomorrow : % .0f\n", OWOC.forecast[1].windSpeed);      
      Serial.printf("Forecast Condition Tomorrow : % s\n", OWOC.forecast[1].summary);
    u8g2Fonts.setCursor(77, 10);                          // start writing at this position
    u8g2Fonts.printf("Today Lo:%.0f", OWOC.forecast[0].temperatureLow);
    u8g2Fonts.setCursor(77, 140);    
    u8g2Fonts.printf("Hi:%.0f", OWOC.forecast[0].temperatureHigh);
    u8g2Fonts.setCursor(62, 10);                          // start writing at this position
    u8g2Fonts.printf("W:%.0f", OWOC.forecast[0].windSpeed);
    u8g2Fonts.setCursor(62, 70);    
    u8g2Fonts.printf("%s", OWOC.forecast[0].summary);
     
    u8g2Fonts.setCursor(47, 10);                          // start writing at this position
    u8g2Fonts.printf("Tomorrow Lo:%.0f", OWOC.forecast[1].temperatureLow);
    u8g2Fonts.setCursor(47, 170);    
    u8g2Fonts.printf("Hi:%.0f", OWOC.forecast[1].temperatureHigh);
    u8g2Fonts.setCursor(32, 10);                          // start writing at this position
    u8g2Fonts.printf("W:%.0f", OWOC.forecast[1].windSpeed); 
    u8g2Fonts.setCursor(32, 70);     
    u8g2Fonts.printf("%s", OWOC.forecast[1].summary);            
    u8g2Fonts.setCursor(17, 10);                          // start writing at this position

    display.update();  
                     
    }
}
delay(600000);
}
