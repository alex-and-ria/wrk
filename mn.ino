#include <my_ir.h>
#include "lmp.h"
#include <EEPROM.h>
#define irpin 11// ir reciver's pin;
IRrecv irr(irpin);
decode_results results;
byte ircd;
lmp lmp1(4);
lmp lmp2(5);
lmp lmp3(6);
lmp lmp4(7);

void setup(){
  Serial.begin(9600);
  irr.enableIRIn();
  //eewrite();
  //eeread(); 
  lmp1.lght(); lmp2.lght(); lmp3.lght(); lmp4.lght();
}

void loop() {
  ircd=getcd(&irr,&results);
  if(ircd!=0){
    switch(ircd){
      case 1:
        Serial.println("1");
        lmp1.stt=!lmp1.stt;
        lmp1.lght();
        eewrite();
      break;
      case 2:
        Serial.println("2");
        lmp2.stt=!lmp2.stt;
        lmp2.lght();
        eewrite();
      break;
      case 3:
        Serial.println("3");
        lmp3.stt=!lmp3.stt;
        lmp3.lght();
        eewrite();
      break;
      case 4:
        Serial.println("4");
        lmp4.stt=!lmp4.stt;
        lmp4.lght();
        eewrite();
      break;
      case 16:
        Serial.println("16");
        eeread();
        lmp1.lght(); lmp2.lght(); lmp3.lght(); lmp4.lght();
      break;
    }
  }
}
