// Written by Nick Gammon
// February 2011


#include <SPI.h>

byte mymsg[28]={0x7E, 0x00 , 0x18 , 0x10 , 0x01 , 0x00 , 0x13 , 0xA2 , 0x00 , 0x40 , 0xC8 , 0x4F , 0xC7 , 0xFF , 0xFE , 0x00 , 0x00 , 0x69 , 0x20 , 0x68 , 0x61 , 0x74 , 0x65 , 0x20 , 0x79 , 0x6F , 0x75 , 0x76};

void setup (void)
{

  digitalWrite(SS, HIGH);  // ensure SS stays high for now

  // Put SCK, MOSI, SS pins into output mode
  // also put SCK, MOSI into LOW state, and SS into HIGH state.
  // Then put SPI hardware into Master mode and turn SPI on
  SPI.begin ();

  // Slow down the master a bit
  SPI.setClockDivider(SPI_CLOCK_DIV32);
  
}  // end of setup


void loop (void)
{

  char c;

  // enable Slave Select
  digitalWrite(SS, LOW);    // SS is pin 10

  // send test string
//  for (const char * p = "Hello, world!\n" ; c = *p; p++)
  for (int i=0;i<28;i++){
    SPI.transfer (mymsg[i]);
  }

  // disable Slave Select
  digitalWrite(SS, HIGH);

  delay (1000);  // 1 seconds delay 
}  // end of loop
