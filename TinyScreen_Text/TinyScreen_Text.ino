/************TinyScreen Demo************
* Created by Poopeye
* November, 2015
* Based on TinyScreen_Nametag provided by TinyCircuits.com
***************************************/
 


#define	BLACK           0x00
#define	BLUE            0xE0
#define	RED             0x03
#define	GREEN           0x1C
#define	DGREEN           0x0C
#define YELLOW          0x1F
#define WHITE           0xFF
#define ALPHA           0xFE
#define	BROWN           0x32

  
#include <TinyScreen.h>
#include <SPI.h>
#include <Wire.h>

TinyScreen display = TinyScreen(0);


void setup(void) {
  Wire.begin();
  display.begin();
}

void loop() {
  display.setFont(liberationSans_12ptFontInfo);
  display.fontColor(BLACK,BLACK);
  for(int i=0;i<5; i++)
  {
    display.setCursor(0,i*12);
    display.print("                    ");
  }
  display.setFont(liberationSans_14ptFontInfo);
  display.setCursor(0,20);
  display.fontColor(RED,BLACK);
  display.print("Make");
  display.fontColor(YELLOW,BLACK);
  display.print("Share");
  while(1);
  
}
