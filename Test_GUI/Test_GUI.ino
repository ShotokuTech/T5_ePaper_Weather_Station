
#define LILYGO_T5_V213
#include <boards.h>
#include <GxEPD.h>
#include <GxDEPG0213BN/GxDEPG0213BN.h>       // 2.13" b/w form DKE GROUP
#include <U8g2_for_Adafruit_GFX.h>
#include <GxIO/GxIO_SPI/GxIO_SPI.h>
#include <GxIO/GxIO.h>
#include <WiFi.h>

GxIO_Class io(SPI,  EPD_CS, EPD_DC,  EPD_RSET);
GxEPD_Class display(io, EPD_RSET, EPD_BUSY);
U8G2_FOR_ADAFRUIT_GFX u8g2Fonts;


void setup(void)
{
    Serial.begin(115200);
    Serial.println();
    Serial.println("setup");
    SPI.begin(EPD_SCLK, EPD_MISO, EPD_MOSI);
    display.init(); // enable diagnostic output on Serial
    u8g2Fonts.begin(display);

    u8g2Fonts.setFontMode(0);                           // use u8g2 transparent mode (this is default)
    u8g2Fonts.setFontDirection(1);                      // left to right (this is default)
    u8g2Fonts.setForegroundColor(GxEPD_BLACK);          // apply Adafruit GFX color
    u8g2Fonts.setBackgroundColor(GxEPD_WHITE);          // apply Adafruit GFX color
    u8g2Fonts.setFont(u8g2_font_courB10_tr);            // select u8g2 font from here: https://github.com/olikraus/u8g2/wiki/fntlistall

    display.fillScreen(GxEPD_WHITE);   
    u8g2Fonts.setCursor(107, 10);                          // start writing at this position
    u8g2Fonts.print("T:100");
    u8g2Fonts.setCursor(107, 70);    
    u8g2Fonts.print("H:100");
    u8g2Fonts.setCursor(107, 140);    
    u8g2Fonts.print("P:1000");
    u8g2Fonts.setCursor(92, 10);                          // start writing at this position
    u8g2Fonts.print("W:100");
    u8g2Fonts.setCursor(92, 70);    
    u8g2Fonts.print("Partly Cloudy");
    u8g2Fonts.setCursor(77, 10);                          // start writing at this position
    u8g2Fonts.print("Today L:100");
    u8g2Fonts.setCursor(77, 130);    
    u8g2Fonts.print("H:100");
    u8g2Fonts.setCursor(62, 10);                          // start writing at this position
    u8g2Fonts.print("W:100");
    u8g2Fonts.setCursor(62, 70);    
    u8g2Fonts.print("Partly Cloudy");
    u8g2Fonts.setCursor(47, 10);                          // start writing at this position
    u8g2Fonts.print("Tomorrow L:100");
    u8g2Fonts.setCursor(47, 160);    
    u8g2Fonts.print("H:100");
    u8g2Fonts.setCursor(32, 10);                          // start writing at this position
    u8g2Fonts.print("W:100"); 
    u8g2Fonts.setCursor(32, 70);     
    u8g2Fonts.print("Partly Cloudy");            
    u8g2Fonts.setCursor(17, 10);                          // start writing at this position
    u8g2Fonts.print("Alert Event");
    u8g2Fonts.setCursor(2, 10);                          // start writing at this position
    u8g2Fonts.print("Alert Summary");

//    display.updateWindow(0, 0, 250, 122, true);
    display.update();    
}
void loop()
{
  ;    
}
