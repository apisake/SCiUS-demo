// ipst microbox se
#include <ipst.h>

// define pins
const int SPEAKER = 16;
const int MUTESWITCH = 18;
const int SW1 = 22;
const int LED = 19;
int tempFreq;
boolean speakerStatus = false;

// logo background SCiUS
char* scius= "SCiUS";


const int BLINKRATE = 100; // text blinking rate (in ms)

// logo position
int rowOrigin = 2;
int colOrigin = 3;

void setup()
{
  glcdClear();
  
  glcdSetColorWordBGR();
  setTextColor(GLCD_WHITE);
  glcdMode(0);
  
  pinLED8(17);
  
  showSCiUSlogo();
  
  pinMode(SW1, INPUT);
  demoScreen();
}


void showSCiUSlogo(void) {
  // text size = 1 for small text
  setTextSize(1);
  setTextColor(GLCD_YELLOW);
  glcd(14,8,"OK     DEMO");
  setTextColor(GLCD_WHITE);
  glcd(14,2,"Press");
  glcd(14,11,"for");
  

  // text size = 2 
  setTextSize(2);
  for(int i=0; i<4; i++) {
    glcd(rowOrigin+i,colOrigin,scius);
  }
  
 logoBlink();
  
  while(!sw_OK()) {
    logoBlink();  
  }
  // clear screen if SW1 is pressed
  glcdClear();
  beep(16);
  
  
}

void logoBlink(void) {
   // blank out for PSC and SU text
  glcd(rowOrigin+1,colOrigin+1,"   "); // for PSC
  glcd(rowOrigin+2,colOrigin+2,"  ");  // for SU
  delay(BLINKRATE/2);  // no text duration
  
  // PSC and SU first blinking
  // PSC is magenta
  setTextColor(GLCD_MAGENTA);
  glcd(rowOrigin+1,colOrigin+1,"PSC"); // for PSC
  
  // SU is green
  setTextColor(GLCD_GREEN);
  glcd(rowOrigin+2,colOrigin+2,"SU");
  
  delay(BLINKRATE);
}

// placeholder for demo screen
void demoScreen(void) {
  
  setTextSize(2);
  setTextColor(GLCD_WHITE);
  glcd(1,3,"FREQ:");
  setTextSize(1);
  setTextBackgroundColor(GLCD_RED);
  setTextColor(GLCD_YELLOW);
  glcd(15,0,"      sound: OFF          ");

}

void loop()
{
  int freq;
  
  // check sound switch
  if (!digitalRead(MUTESWITCH)) {
    if (speakerStatus) {
      setTextSize(1);
      setTextBackgroundColor(GLCD_RED);
      setTextColor(GLCD_YELLOW);
      glcd(15,0,"      sound: OFF          ");
      speakerStatus = false;
      delay(100);
    }
    else {
      setTextSize(1);
      setTextBackgroundColor(GLCD_RED);
      setTextColor(GLCD_YELLOW);
      glcd(15,0,"      sound:  ON          ");
      speakerStatus = true;
      delay(100);
    }
  }
    
  
  // check knob
  freq = knob();
  
  setTextSize(3);
  setTextBackgroundColor(GLCD_BLACK);
  setTextColor(GLCD_YELLOW);
  if ( digitCount(freq) != digitCount(tempFreq) ) {
    glcd(2,2,"    ");
  }
  
  glcd(2,2,"%d",freq);
  if (speakerStatus) {
    sound(SPEAKER,freq,50); 
  }
  
  tempFreq = freq;
  
  // LED dimmer
  analogWrite(LED,map((constrain(freq,85,1023)),85,1023,0,255));
  
  LED8(0x01<<(map((constrain(freq,85,1023)),85,1020,0,7)));
}

int digitCount(int digit) {
  if (digit < 10) {
    return 1;
  } else if (digit < 100) {
    return 2;
  } else if (digit < 1000) {
    return 3;
  } else if (digit < 10000) {
    return 4;
  }
}
