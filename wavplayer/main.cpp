
#include "mbed.h"
#include "SDFileSystem.h"
#include "wave_player.h"
#include "uLCD_4DGL.h"
#include "PinDetect.h"
#include "Speaker.h"
#include <vector>
 #include <string>
 using namespace std;
vector<string> filenames; //filenames are stored in a vector string
// use class to setup the LCD
uLCD_4DGL lcd(p28,p27,p29); 

// use class to setup pushbuttons pins
PinDetect pb1(p22);
PinDetect pb2(p23);
PinDetect pb3(p24);
PinDetect pb4(p25);
SDFileSystem sd(p5, p6, p7, p8, "sd"); //SD card
// use class to setup Mbed's four on-board LEDs
DigitalIn myinput(p14);
DigitalOut myLED1(LED1);
DigitalOut myLED2(LED2);
DigitalOut myLED3(LED3);
DigitalOut myLED4(LED4);

AnalogOut DACout(p18);

wave_player waver(&DACout);

void read_file_names(char *dir)
{
    DIR *dp;
    struct dirent *dirp;
    dp = opendir(dir);
  //read all directory and file names in current directory into filename vector
    while((dirp = readdir(dp)) != NULL) 
    {
        filenames.push_back(string(dirp->d_name));
    }
}
int s=0;
int mode=0;
void pb1_hit_callback (void)
{
    lcd.cls();
s=(s+1)%10;
}
// Callback routine is interrupt activated by a debounced pb2 hit
void pb2_hit_callback (void)
{
    lcd.cls();
    a:
    s=(s-1)%10;
    if (s < 0)
    {
        lcd.cls();    
    s = 10;
    goto a;
    }
}
// Callback routine is interrupt activated by a debounced pb3 hit
void pb3_hit_callback (void)
{
 myLED3 = !myLED3;
mode=(mode+1)%2;

}
extern int volume=0;
void pb4_hit_callback (void)
{
//myLED2 = !myLED2;
volume=(volume+1)%16;
}
int main()
{
  myinput.mode(PullUp);
pb1.mode(PullUp);
    pb2.mode(PullUp);
    pb3.mode(PullUp);
     pb4.mode(PullUp);
    wait(.01);
      pb1.attach_deasserted(&pb1_hit_callback);
    pb2.attach_deasserted(&pb2_hit_callback);
    pb3.attach_deasserted(&pb3_hit_callback);
        pb4.attach_deasserted(&pb4_hit_callback);
    // Start sampling pb inputs using interrupts
    pb1.setSampleFrequency();
    pb2.setSampleFrequency();
    pb3.setSampleFrequency();
       pb4.setSampleFrequency();
         if(sd.check(myinput)==false)
     {
      lcd.printf("Insert SD card");    
      wait(5); 
     } 
     while(1)
    {   
     if(sd.check(myinput)==false)
     {
      lcd.printf("Insert SD card");     
     } 
      
// read file names into vector of strings
read_file_names("/sd/myMusic");
// print filename strings from vector using an iterator
vector<string>::iterator it=filenames.begin();

while(it < filenames.end())
{
/*
if(s==0)
{
   lcd.cls();
it=filenames.begin();
}
else if(s==1)
{
   lcd.cls();
++it;
}
else if(s==2)
{
   lcd.cls();
++it;
++it;
}
else if(s==3)
{
   lcd.cls();
++it;
++it;
++it;
}
string a=(*it).c_str();
int l=a.length();
a=a.substr(0,l-4);

lcd.printf("%s\r",a);*/
if(mode==1)
{
if(s==0)
{
   lcd.cls();
it=filenames.begin();
}
else if(s==1)
{
   lcd.cls();
++it;
}
else if(s==2)
{
   lcd.cls();
++it;
++it;
}
else if(s==3)
{
   lcd.cls();
++it;
++it;
++it;
}
string a=(*it).c_str();
int l=a.length();
a=a.substr(0,l-4);

lcd.printf("%s\r",a); wait(0.2);
FILE *wave_file;
lcd.locate(0,1);
lcd.printf("Song playing");
 string SongName;
SongName.append("/sd/myMusic/"); // ======== Change if not using base sd card location =====
SongName.append(a); // Add current song name
SongName.append(".wav");  // Add .wav extension
wave_file=fopen(SongName.c_str(),"r"); 
waver.play(wave_file,mode);
    fclose(wave_file);
}
if(mode==0)
{
it=filenames.begin();
if(s==0)
{
   lcd.cls();
it=filenames.begin();
}
else if(s==1)
{
   lcd.cls();
++it;
}
else if(s==2)
{
   lcd.cls();
++it;
++it;
}
else if(s==3)
{
   lcd.cls();
++it;
++it;
++it;
}
string a=(*it).c_str();
int l=a.length();
a=a.substr(0,l-4);
lcd.printf("%s\r",a); wait(0.2);
lcd.locate(0,1);
lcd.printf("             ");

}
it=filenames.begin();
}
    /*FILE *wave_file;
    printf("\n\n\nHello, wave world!\n");
    wave_file=fopen("/sd/sample.wav","r");
    waver.play(wave_file);
    fclose(wave_file);*/
    }
}
