#include "U8glib.h"
#include <SoftwareSerial.h>

SoftwareSerial BLEserial(2, 3);
U8GLIB_SH1106_128X64 u8g(U8G_I2C_OPT_NONE);

int h=0;
int m=0;
int s=0;
int timeinit=0;

unsigned long previoustimetimer = millis();
unsigned long previoustimeble = millis();
unsigned long previoustimescreen = millis();

long timerdelay = 1000;
long bledelay = 2000;
long screendelay = 1000;

String text="NeXTeP";
String oldtext="NeXTeP";

void setup() {
  Serial.begin(115200);
  BLEserial.begin(9600);
  text="Initializing...";
}


void draw() {
  u8g.setFont(u8g_font_helvR14);
  u8g.setPrintPos(0, 20);
  u8g.print(h);u8g.print(":");u8g.print(m);u8g.print(":");u8g.print(s);
  u8g.setPrintPos(0, 64);
  u8g.print(String(text));
}

void screen(){
    u8g.firstPage();
  do {draw();} while (u8g.nextPage() );
}

void timer(){
  if (timeinit>=3){
    s+=1;
      if (s == 60){
        s = 0;
        m += 1;
      }
      if (m == 60){
        m = 0;
        h += 1;
      }
      if (h == 24){
        h = 0;
      }
    } 
  }

void bletext(){
   if (text == "Initializing..."){text="Heure?";}
   if(BLEserial.available()>0){
        text = BLEserial.readString();
          if (oldtext != text){
            if (timeinit==0){
              h = text.toInt();
              text = "Minutes?";
            }
            if (timeinit==1){
              m = text.toInt();
              text = "Secondes?";
            }
            if (timeinit==2){
              s = text.toInt();
              text = "NeXTeP Watch!";
            }
          timeinit += 1; 
          }
        s += 1;
    }
  }

void loop() {
  unsigned long currentTime = millis();
  oldtext = text;
  if (currentTime - previoustimescreen > screendelay){
    previoustimescreen = currentTime;
    screen();
   }
  if (currentTime - previoustimetimer > timerdelay){
    previoustimetimer = currentTime;
    timer();
   }
  if (currentTime - previoustimeble > bledelay){
    previoustimeble = currentTime;
    bletext();
   }
}
