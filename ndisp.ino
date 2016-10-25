//typedef unsigned short int usi;
#include <Adafruit_GFX.h>
#include <Adafruit_PCD8544.h>
#include <gfxfont.h>
#include <IRremote.h>
#include <EEPROM.h>
#include <avr/io.h>
#include <avr/interrupt.h>
#include "Wire.h"
#include "defs.h"

void setup(){
  Serial.begin(9600);
  pinMode(btnpn,INPUT_PULLUP);
  pinMode(trin,INPUT_PULLUP);
  pinMode(trout,OUTPUT);
  digitalWrite(trout,LOW);
//while(1){Serial.println(digitalRead(btnpn));}// tst;
  display.begin();            // Инициализация дисплея
  display.setContrast(50);    // Устанавливаем контраст
  display.setTextColor(BLACK);// Устанавливаем цвет текста
  display.setTextSize(1/2);   // Устанавливаем размер текста
  display.clearDisplay();     // Очищаем дисплей
  display.display();
  irrecv.enableIRIn();// Начало прослушки с пульта
//eewrite();
  cli();
  TCCR1A=0x00;//clear TCCR1Al
//TCCR1B=0x00;//clear TCCR1Bl
  TIMSK1=0x00;//clear TIMSK1
  TCNT1=0x00;//clear TCNT1
  ICR1=0x00;//clear ICR1

  TCCR1B|=(1<<ICES1)|(1<<CS12)|/*(1<<CS11)|*/(1<<CS10)|(1<<ICNC1);//rising edge,T/1024,Inp capt noise Canceler;
  TCCR1B&=~(/*(1<<ICES1)|*//*(1<<CS12)|*/(1<<CS11)/*|(1<<CS10)*/);
  TIMSK1|=(1<<ICIE1);//interrupt catch TC1;
  sei();
  display.clearDisplay(); display.display();  eeread();
  display.drawRect(0,0,LCDWIDTH-1,LCDHEIGHT-1,BLACK);// x,y,w,h,color
//prntlev();
}

void loop(){
  prntlev();
}
////////////////////////////////////////
void prntlev(){
//strt:
  display.drawRect(0,0,LCDWIDTH-1,LCDHEIGHT-1,BLACK);// x,y,w,h,color
  switch (lev){
    case 0:
      prntl0(); cntout=dose*(tpl/(1000+0.0));
      inlev=0; maxinlev=2;
    //slctmv(inlev,maxinlev);
    //lsn0:
      while(!tostrt){
        /*if(strtb){//the user must hold the button to pour water;
          if(digitalRead(trin)==HIGH&&(millis()-mvton)>=vton) allwvo=true;
          if(digitalRead(trin)==LOW&&allwvo){
            digitalWrite(trout,HIGH);
            if(!fixom){TCNT1=0x0; fk=0.0,vn=0.0; cnt=0; fixom=true;}
            //while(1){
              if(cnt>=cntout/2&&cnt<=cntout/2+cntout/4){
                fk=(1.0*tclkd)/tachBuf;//frequency;
                vn=fk*tgmm/1000;// N valve out;
              }
              if(cnt>=cntout-vn){
                digitalWrite(trout,LOW);
                mvton=millis();
                allwvo=false; fixom=false;
                qtyadm++; qtyusr++;
                dailyadm+=dose; dailyusr+=dose; all+=dose;
                prntl0(); eewrite(); cnt=0;
              }
              if(digitalRead(trin)==HIGH){
                digitalWrite(trout,LOW);
                //break;
              }
            //}
          }
        }*/
        /*else {*///no need to hold the button;
          if(cnt>=cntout/2&&cnt<=cntout/2+cntout/4){
            fk=(1.0*tclkd)/tachBuf;//frequency;
            vn=fk*tgmm/1000;// N valve out;
          }
          if(cnt>=cntout-vn){
            digitalWrite(trout,LOW);
          //Serial.println("params:"); Serial.println(cnt); Serial.println(fk); Serial.println(vn);
            cnt=0;
            qtyadm++; qtyusr++;
            dailyadm+=dose; dailyusr+=dose; all+=dose;
            prntl0(); eewrite(); vviso=false;
          }
          if(digitalRead(trin)==LOW&&vviso==false){
            TCNT1=0x0; fk=0.0,vn=0.0; cnt=0;
            digitalWrite(trout,HIGH);
            vviso=true;
          }
        /*}*/
        if(digitalRead(btnpn)==LOW){
          unsigned long t1=millis();
          while((millis()-t1)<3000){
            clrq=true;
            if(digitalRead(btnpn)==HIGH){
              clrq=false; break;
            }
          }
        //Serial.println(clrq);
          if(clrq){
            t1=millis();
            while((millis()-t1)<7000){
              clrd=true;
              if(digitalRead(btnpn)==HIGH){
                clrd=false; break;
              }
            }
          //Serial.println(clrd);
          }
        }
        if(clrq&&ures){qtyusr=0; clrq=false; prntl0(); eewrite();
        } else clrq=false;
        if(clrd&&ures){dailyusr=0; qtyusr=0; clrd=false; prntl0(); eewrite();
        } else clrd=false;
        if(irrecv.decode(&results)){
          switch (results.value){
            /*case down:
              Serial.println("down");
              if(inlev<maxinlev){inlev++; slctmv(inlev,maxinlev);}
              break;
              case up:
              Serial.println("up");
              if(inlev>0){inlev--; slctmv(inlev,maxinlev);}
              break;*/
            /*case ltc:
              Serial.println("#");
              if(inlev==0){qtyusr=0; prntl0();}
              else if(inlev==2){dailyusr=0; prntl0();}
              break;*/
            case right:
            Serial.println("right");
              lev=1;
              irrecv.resume();
              tostrt=true;//goto strt;
              digitalWrite(trout,LOW); vviso=false;
              break;
          }
          display.display();
          irrecv.resume();
        }
      //if(tostrt) break;
      }
      break;
    case 1:
      prntl1();
      inlev=0; maxinlev=0;
      slctmv(inlev,maxinlev);
    //lsn1:
      nt1=millis();
      while(!tostrt){
        if((millis()-nt1)>=10000){
          Serial.print("millis="); Serial.println(millis());
          Serial.print("nt1="); Serial.println(nt1);
          nt1=millis();
          tostrt=true;
          lev=0;
          steppin=0;
        //Serial.println("timeout");
          break;
        }
        if(irrecv.decode(&results)){
          //nt1=millis();
          switch (results.value){
            case one: cnum(1); break;
            case two: cnum(2); break;
            case three: cnum(3); break;
            case four: cnum(4); break;
            case five: cnum(5); break;
            case six: cnum(6); break;
            case seven: cnum(7); break;
            case eight: cnum(8); break;
            case nine: cnum(9); break;
            case zero: cnum(0); break;
            case ltc: steppin=0; prntl1(); break;
            case right: okm1(); break;//goto okm1;
            case ok: okm1();//okm1:
              break;
            case left:
              lev=0; steppin=0;
              steppin=0; strpin[0]=strpin[1]=strpin[2]=strpin[3]='\0';
              tostrt=true;//irrecv.resume(); goto strt;
              break;
          }
          display.display();
          irrecv.resume();
        }
      //if(!tostrt) goto lsn1;
      }
      break;
    case 2:
      prntl2();
      inlev=0; maxinlev=1;
      slctmv(inlev,maxinlev);
    //lsn2:
      while(!tostrt){
        if(irrecv.decode(&results)){
          switch (results.value){
            case down:
            //Serial.println("down");
              if(inlev<maxinlev){
                inlev++;
                slctmv(inlev,maxinlev);
              }
              break;
            case up:
            //Serial.println("up");
              if(inlev > 0){
                inlev--;
                slctmv(inlev,maxinlev);
              }
              break;
            case left:
              lev=1; steppin=0;
              steppin=0; strpin[0]=strpin[1]=strpin[2]=strpin[3]='\0';
              tostrt=true;//irrecv.resume(); goto strt;
              break;
            case right: okm2(); break;// goto okm2;
            case ok: okm2();//okm2:
              break;
          }
          display.display();
          irrecv.resume();
        }
      //if(!tostrt) goto lsn2;
      }
      break;
    case 3:
      prntl3(); inlev=0; maxinlev=3;
      slctmv(inlev,maxinlev);
    //lsn3:
      while(!tostrt){
        if(irrecv.decode(&results)){
          switch (results.value){
            case down:
            //Serial.println("down");
              if(inlev<maxinlev){
                inlev++;
                slctmv(inlev,maxinlev);
              }
              break;
            case up:
            //Serial.println("up");
              if(inlev > 0){
                inlev--;
                slctmv(inlev,maxinlev);
              }
              break;
            case ltc:
            //Serial.println("#");
              if(inlev==1){
                qtyadm=0;//qty
                prntl3();
                eewrite();
              }
              else if(inlev==2){
                tmpcnt=0;
                dose=0;
                prntl3();
                eewrite();
              }
              else if(inlev==3){
                dailyadm=0;//daily
                qtyadm=0;
                prntl3();
                eewrite();
              }
              break;
            case left:
              dose=dose-(dose % rnd);
              if(dose<minml) dose=minml;
              else if(dose>maxml) dose=maxml;
                prntl3(); eewrite();
                lev=2; tmpcnt=0; tostrt=true;//irrecv.resume(); goto strt;
              break;
            case one: numgt=1; numgtl3(); break;
            case two: numgt=2; numgtl3(); break;
            case three: numgt=3; numgtl3(); break;
            case four: numgt=4; numgtl3(); break;
            case five: numgt=5; numgtl3(); break;
            case six: numgt=6; numgtl3(); break;
            case seven: numgt=7; numgtl3(); break;
            case eight: numgt=8; numgtl3(); break;
            case nine: numgt=9; numgtl3(); break;
            case zero: numgt=0; numgtl3(); break;
          }
          display.display();
          irrecv.resume();
        }
      //if(!tostrt) goto lsn3;
      }
      break;
    case 4:
      prntl4(); inlev=0; maxinlev=3;
      slctmv(inlev,maxinlev);
    //lsn4:
      while(!tostrt){
        if(irrecv.decode(&results)){
          switch (results.value){
            case down:
            //Serial.println("down");
              if(inlev<maxinlev){
                inlev++;
                slctmv(inlev,maxinlev);
              }
              break;
            case up:
            //Serial.println("up");
              if(inlev > 0){
                inlev--;
                slctmv(inlev,maxinlev);
              }
              break;
            case right: okm4(); break;// goto okm4;
            case ok: okm4(); break;
            case left: lev=2; tostrt=true;//irrecv.resume(); goto strt;
              break;
          }
          display.display();
          irrecv.resume();
        }
      //if(!tostrt) goto lsn4;
      }
      break;
    case 5:
      prntl5(); inlev=0; maxinlev=2;
      slctmv(inlev,maxinlev);
    //lsn5:
      while(!tostrt){
        if(irrecv.decode(&results)){
          switch (results.value){
            case one: cnum1(1,inlev); break;
            case two: cnum1(2,inlev); break;
            case three: cnum1(3,inlev); break;
            case four: cnum1(4,inlev); break;
            case five: cnum1(5,inlev); break;
            case six: cnum1(6,inlev); break;
            case seven: cnum1(7,inlev); break;
            case eight: cnum1(8,inlev); break;
            case nine: cnum1(9,inlev); break;
            case zero: cnum1(0,inlev); break;
            case ltc: prntl5l(inlev); break;
            case down:
            //Serial.println("down");
              if(inlev<maxinlev){
                inlev++;
                slctmv(inlev,maxinlev);
              }
              break;
            case up:
            //Serial.println("up");
              if(inlev > 0){
                inlev--;
                slctmv(inlev,maxinlev);
              }
              break;
            case ok:
            //Serial.println("ok");
              if(str[0]==strpin[0]&&str[1]==strpin[1]&&str[2]==strpin[2]&&str[3]==strpin[3])
                match=true;
              if(strpinn[0]==strpinc[0]&&strpinn[1]==strpinc[1]&&strpinn[2]==strpinc[2]&&strpinn[3]==strpinc[3])
                cnfrm=true;
              if(!(match&&cnfrm)){
                display.fillRect(1,1,slctw+5,LCDHEIGHT-3,WHITE);//clear screen (except for the  border);
                /*if(!match){
                  display.setCursor(prenw,3); display.print("err: pin and old pin");
                  }
                  if(!cnfrm){
                  display.setCursor(prenw,17); display.print("err: new pin and confirm pin");
                  }*/display.setTextSize(2);
                display.setCursor(prenw,3); display.print("not    match");
                display.display(); display.setTextSize(1/2);
                match=false; cnfrm=false;
                for (unsigned short i=0; i<4; i++) strpin[i]=strpinn[i]=strpinc[i]='\0';
                strpinn[0]=5; strpinc[0]=7; steppin=0; steppinn=0; steppinc=0;
                while(1){
                  if(irrecv.decode(&results)){
                    if(results.value==left){
                      lev=4;
                      tostrt=true;//irrecv.resume(); goto strt;
                      break;
                    }
                    else irrecv.resume();
                  }
                }
              }
              else {
                display.fillRect(1,1,slctw+5,LCDHEIGHT-3,WHITE);//clear screen (except for the  border);
                display.setCursor(prenw,3);/* display.setTextSize(2);*/ display.print("pin   changed");
                display.display();  display.setTextSize(1/2); match=false; cnfrm=false;
                for (unsigned short int i=0; i<4; i++) str[i]=strpinn[i];
                eewrite();
                while(1){
                  if(irrecv.decode(&results)){
                    if(results.value==left){
                      lev=4;
                      tostrt=true;//irrecv.resume(); goto strt;
                      break;
                    }
                    else irrecv.resume();
                  }
                }
              }
              break;
            case left: lev=4; tostrt=true;//irrecv.resume(); goto strt;
              break;
          }
          display.display();
          irrecv.resume();
        }
      //if(!tostrt) goto lsn5;
      }
      break;
    case 6:
      prntl6(); inlev=0; maxinlev=1;
      slctmv(inlev,maxinlev);
    //lsn6:
      while(!tostrt){
        if(irrecv.decode(&results)){
          switch (results.value){
            case down:
            //Serial.println("down");
              if(inlev<maxinlev){
                inlev++;
                slctmv(inlev,maxinlev);
              }
              break;
            case up:
            //Serial.println("up");
              if(inlev > 0){
                inlev--;
                slctmv(inlev,maxinlev);
              }
              break;
            case ok:
            //Serial.println("ok");
              if(inlev==0) ures=!ures;
              //else strtb=!strtb;
              prntl6(); eewrite();
              break;
            case left: lev=4; tostrt=true;//irrecv.resume(); goto strt;
              break;
          }
          display.display();
          irrecv.resume();
        }
      //if(!tostrt) goto lsn6;
      }
      break;
    case 7:
      prntl7(); inlev=0; maxinlev=0; ltrs=0;
      slctmv(inlev,maxinlev); fixom=false; 
    //lsn7:
      while(!tostrt){
        if(digitalRead(trin)==LOW){
            digitalWrite(trout,HIGH);
        if(!fixom){TCNT1=0x0; cnt=0; fixom=true;}
        }
        if(fixom){
          ltrs=cnt/(tpl/(1000+0.0)); prntl7();
        }
        if(digitalRead(trin)==HIGH) {
          digitalWrite(trout,LOW); fixom=false;
          ltrs=cnt/(tpl/(1000+0.0)); prntl7();
        }
        if(irrecv.decode(&results)){
          switch (results.value){
            case ltc: ltrs=0; prntl7();
              break;
            case left: lev=4; tostrt=true;//irrecv.resume(); goto strt;
              break;
          }
          display.display();
          irrecv.resume();
        }
      //if(!tostrt) goto lsn7;
      }
      break;
    case 8:
      prntl8(); inlev=0; maxinlev=2;
      slctmv(inlev,maxinlev);
    //lsn8:
      while(!tostrt){
        if(irrecv.decode(&results)){
          switch (results.value){
            case down:
            //Serial.println("down");
              if(inlev<maxinlev){
                inlev++;
                slctmv(inlev,maxinlev);
              }
              break;
            case up:
            //Serial.println("up");
              if(inlev > 0){
                inlev--;
                slctmv(inlev,maxinlev);
              }
              break;
            case left: lev=4; tmpcnt=0; tostrt=true;//irrecv.resume(); goto strt;
              break;
            case ok:
              if(inlev==2){
                //fwm=!fwm;
                prntl8();
                eewrite();
              }
              break;
            case ltc:
              if(inlev==0){
                vton=0; tmpcnt=0;prntl8();eewrite();
              }
              else if(inlev==1){tgmm=0; tmpcnt=0; prntl8(); eewrite();}
              else if(inlev==2){tpl=0; tmpcnt=0; prntl8(); eewrite();}
              break;
            case one: numgt=1; numgtl8(); break;
            case two: numgt=2; numgtl8(); break;
            case three: numgt=3; numgtl8(); break;
            case four: numgt=4; numgtl8(); break;
            case five: numgt=5; numgtl8(); break;
            case six: numgt=6; numgtl8(); break;
            case seven: numgt=7; numgtl8(); break;
            case eight: numgt=8; numgtl8(); break;
            case nine: numgt=9; numgtl8(); break;
            case zero: numgt=0; numgtl8(); break;
          }
          display.display();
          irrecv.resume();
        }
      //if(!tostrt) goto lsn8;
      }
      break;
  }
  tostrt=false;//if(tostrt){tostrt=false; goto strt;}
}
