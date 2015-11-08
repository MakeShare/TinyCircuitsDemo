
// Color definitions
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

unsigned char flappyBird[]={
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,BLACK,BLACK,WHITE,WHITE,WHITE,BLACK,WHITE,WHITE,BLACK,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,BLACK,WHITE,WHITE,YELLOW,YELLOW,BLACK,WHITE,WHITE,WHITE,WHITE,BLACK,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,BLACK,WHITE,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,WHITE,WHITE,WHITE,BLACK,WHITE,BLACK,ALPHA,ALPHA,
  ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,WHITE,WHITE,WHITE,BLACK,WHITE,BLACK,ALPHA,ALPHA,
  ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,WHITE,WHITE,WHITE,WHITE,BLACK,ALPHA,ALPHA,
  ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,
  ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,RED,RED,RED,RED,RED,RED,BLACK,
  ALPHA,ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,RED,BLACK,BLACK,BLACK,BLACK,BLACK,BLACK,
  ALPHA,ALPHA,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,RED,RED,RED,RED,RED,BLACK,ALPHA,
  ALPHA,ALPHA,ALPHA,BLACK,BLACK,YELLOW,YELLOW,YELLOW,YELLOW,YELLOW,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA
};
//7x8
unsigned char wingUp[]={
  ALPHA,BLACK,BLACK,BLACK,BLACK,ALPHA,ALPHA,
  BLACK,WHITE,WHITE,WHITE,WHITE,BLACK,ALPHA,
  BLACK,WHITE,WHITE,WHITE,WHITE,WHITE,BLACK,
  BLACK,YELLOW,WHITE,WHITE,WHITE,YELLOW,BLACK,
  ALPHA,BLACK,YELLOW,YELLOW,YELLOW,BLACK,ALPHA,
  ALPHA,ALPHA,BLACK,BLACK,BLACK,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
};
unsigned char wingMid[]={
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,
  BLACK,WHITE,WHITE,WHITE,WHITE,WHITE,BLACK,
  BLACK,YELLOW,WHITE,WHITE,WHITE,YELLOW,BLACK,
  ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
};
unsigned char wingDown[]={
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,ALPHA,
  ALPHA,BLACK,BLACK,BLACK,BLACK,BLACK,ALPHA,
  BLACK,YELLOW,WHITE,WHITE,WHITE,YELLOW,BLACK,
  BLACK,WHITE,WHITE,WHITE,WHITE,BLACK,ALPHA,
  BLACK,WHITE,WHITE,YELLOW,BLACK,ALPHA,ALPHA,
  ALPHA,BLACK,BLACK,BLACK,BLACK,ALPHA,ALPHA,
};
char wingHeight=8;
char wingWidth=7;
uint8_t highScore=0;
char flappyBirdX=25;
char flappyBirdY=30;
char flappyBirdHeight=12;
char flappyBirdWidth=17;
char amtPipes=4;
char PipeHeight[]={10,15,20,25,25,10,10};
char pipeOffsetX=0;
const char pipeWidth=10;
const char pipeSpacingX=26;
const char pipeSpacingY=26;
unsigned char lineBuffer[16+96+(pipeSpacingX+pipeWidth)];
unsigned char BGcolor=BLUE;
unsigned char pipeColor=GREEN;
char wingPos=1;
char cloudY[]={5,15,30,25,5,0};
char cloudOffsetX=0;
const char cloudWidth=15;
const char cloudSpacingX=30;
unsigned char cloudColor=WHITE;
char groundY=52;
char groundOffsetX=0;
char birdYmod=1;
unsigned long framecount=0;
char brightnessChanged=0;
char brightness=5;
char movePipe=4;
char movePipeMod=1;
uint8_t score=0;

void setBuffer(char i, char amt, unsigned char color){
  char endbyte=i+amt;
  while(i<endbyte)
    lineBuffer[i++]=color;
}

void updateDisplay(){
  display.goTo(0,0);
  display.startData();
  for(unsigned char y=0;y<groundY;y++){
    
    for(unsigned char i=16;i<112;i++)
      lineBuffer[i]=BGcolor;
      
    int x=cloudOffsetX;
    char cloud=0;
    while(x<16+96){
      if(y>cloudY[cloud] && y<cloudY[cloud]+8){
        if(y<cloudY[cloud]+2 || y>cloudY[cloud]+6)
          setBuffer(x+1, cloudWidth-2, cloudColor);
        else
          setBuffer(x, cloudWidth, cloudColor);
      }
      cloud++;
      x+=(cloudSpacingX+cloudWidth);
    }
    
    x=pipeOffsetX;
    char pipe=0;
    while(x<16+96){
      if(y<PipeHeight[pipe] || y>PipeHeight[pipe]+pipeSpacingY){
        if(y<PipeHeight[pipe]-4 || y>PipeHeight[pipe]+pipeSpacingY+4)
          setBuffer(x, pipeWidth, pipeColor);
        else
          setBuffer(x-1, pipeWidth+2, pipeColor);
      }
      pipe++;
      x+=(pipeSpacingX+pipeWidth);
    }
    
    if(y>=flappyBirdY && y<flappyBirdY+flappyBirdHeight){
      int offset=(y-flappyBirdY)*flappyBirdWidth;
      for(int i=0;i<flappyBirdWidth;i++){
        unsigned char color=flappyBird[offset+i];
        if(color!=ALPHA)
          lineBuffer[16+flappyBirdX+i]=color;
      }
    }
    char wingY=flappyBirdY+3;
    if(y>=wingY && y<wingY+wingHeight){
      int offset=(y-wingY)*wingWidth;
      for(int i=0;i<wingWidth;i++){
        unsigned char color;
        if(wingPos==0)
          color=wingUp[offset+i];
        if(wingPos==1)
          color=wingMid[offset+i];
        if(wingPos==2)
          color=wingDown[offset+i];
        if(color!=ALPHA)
          lineBuffer[16+flappyBirdX+i]=color;
      }
    }
    
    
    char highScoreString[3];
    highScoreString[0]=(highScore/10)+'0';
    highScoreString[1]=(highScore%10)+'0';
    highScoreString[2]='\0';
    
    putString(y,80,1,highScoreString,liberationSans_10ptFontInfo);
    
    
    char scoreString[3];
    scoreString[0]=(score/10)+'0';
    scoreString[1]=(score%10)+'0';
    scoreString[2]='\0';
    
    
    putString(y,60,15,scoreString,liberationSans_16ptFontInfo);
    
    
    display.writeBuffer(lineBuffer+16,96);
  }
  for(int z=0;z<21;z++){
    lineBuffer[16+(z*5)]=GREEN;
    lineBuffer[17+(z*5)]=GREEN;
    lineBuffer[18+(z*5)]=GREEN;
    lineBuffer[19+(z*5)]=DGREEN;
    lineBuffer[20+(z*5)]=DGREEN;
  }
  display.writeBuffer(lineBuffer+16+groundOffsetX,96);
  display.writeBuffer(lineBuffer+17+groundOffsetX,96);
  display.writeBuffer(lineBuffer+18+groundOffsetX,96);
  display.writeBuffer(lineBuffer+19+groundOffsetX,96);
  display.writeBuffer(lineBuffer+20+groundOffsetX,96);
  
  
  for(unsigned char i=16;i<112;i++)
    lineBuffer[i]=BROWN;
  for(unsigned char i=0;i<7;i++)
    display.writeBuffer(lineBuffer+16,96);
  
  display.endTransfer();
}


void putString(uint8_t y, uint8_t fontX, uint8_t fontY, char * string, const FONT_INFO& fontInfo){
  const FONT_CHAR_INFO* fontDescriptor=fontInfo.charDesc;
  uint8_t fontHeight=fontInfo.height;
  if(y>=fontY && y<fontY+fontHeight){
      const unsigned char* fontBitmap=fontInfo.bitmap;
      uint8_t fontFirstCh=fontInfo.startCh;
      uint8_t fontLastCh=fontInfo.endCh;
      //if(!_fontFirstCh)return 1;
      //if(ch<_fontFirstCh || ch>_fontLastCh)return 1;
      //if(_cursorX>xMax || _cursorY>yMax)return 1;
      uint8_t stringChar=0;
      uint8_t ch=string[stringChar++];
      while(ch){
        uint8_t chWidth=pgm_read_byte(&fontDescriptor[ch-fontFirstCh].width);
        uint8_t bytesPerRow=chWidth/8;
        if(chWidth>bytesPerRow*8)
          bytesPerRow++;
        uint16_t offset=pgm_read_word(&fontDescriptor[ch-fontFirstCh].offset)+(bytesPerRow*fontHeight)-1;
      
        for(uint8_t byte=0; byte<bytesPerRow; byte++){
          uint8_t data=pgm_read_byte(fontBitmap+offset-(y-fontY)-((bytesPerRow-byte-1)*fontHeight));
          uint8_t bits=byte*8;
          for(uint8_t i=0; i<8 && (bits+i)<chWidth; i++){
            if(data&(0x80>>i)){
              lineBuffer[16+fontX]=0;
             }else{
              //SPDR=_fontBGcolor;
             }
             fontX++;
           }
         }
        fontX+=2;
        ch=string[stringChar++];
      }
  }
}

void setup(void) {
  Wire.begin();
  Serial.begin(9600);
  display.begin();
  display.setFlip(1);
  display.setBrightness(brightness);
  uint16_t time = millis();
}

byte flip=0;
void loop() {
  unsigned long timer=micros();
  updateDisplay();
  timer=micros()-timer;
  Serial.println(timer);
  
  if(pipeOffsetX<18 && pipeOffsetX>6){
    if(flappyBirdY<(PipeHeight[1]) || flappyBirdY>(PipeHeight[1]+pipeSpacingY-13)){
      pipeColor=RED;
      if(score>highScore)
        highScore=score;
      score=0;
    }else{
      pipeColor=GREEN;
      if(pipeOffsetX==7)
        score++;
    }
  }else{
    pipeColor=GREEN;
  }
  
  
  framecount++;
  
  if(display.getButtons()){
    wingPos=(framecount)%3;
    flappyBirdY-=(birdYmod*2);
    if(flappyBirdY<0)
      flappyBirdY=0;
  }else{
    wingPos=(framecount>>2)%3;
    flappyBirdY+=birdYmod;
    if(flappyBirdY>40)
      flappyBirdY=40;
  }
    
  pipeOffsetX-=1;
  
  if(movePipe && movePipe<5){
    PipeHeight[movePipe-1]+=movePipeMod;
    if(PipeHeight[movePipe-1]<1)
      movePipeMod=abs(movePipeMod);
    if(PipeHeight[movePipe-1]>groundY-3-pipeSpacingY)
      movePipeMod=-1*abs(movePipeMod);
  }
  if(pipeOffsetX<=0){
    pipeOffsetX=(pipeSpacingX+pipeWidth);
    PipeHeight[0]=PipeHeight[1];
    PipeHeight[1]=PipeHeight[2];
    PipeHeight[2]=PipeHeight[3];
    PipeHeight[3]=PipeHeight[4];
    PipeHeight[4]=3+micros()%(groundY-8-pipeSpacingY);
    if(movePipe)
      movePipe-=1;
    else
      movePipe=3+micros()%5;
  }
  
  groundOffsetX+=1;
  if(groundOffsetX>=5)
    groundOffsetX=0;
  
  if(!(framecount%2))
    cloudOffsetX--;
  if(cloudOffsetX<=0){
    cloudOffsetX=cloudSpacingX+cloudWidth;
    cloudY[0]=cloudY[1];
    cloudY[1]=cloudY[2];
    cloudY[2]=cloudY[3];
    cloudY[3]=cloudY[4];
    cloudY[4]=cloudY[5];
    cloudY[5]=micros()%(30);
  }

}