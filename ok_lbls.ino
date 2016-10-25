void okm1(){
  if(str[0]==strpin[0]&&str[1]==strpin[1]&&str[2]==strpin[2]&&str[3]==strpin[3]){
    pincode=true;
    lev=2;
    tostrt=true;//goto strt;
  }
  else{
    lev=0; steppin=0;
    display.setCursor(15,35);
    display.print("Incorrect");
    display.fillRect(26, 22, 28, 8, WHITE); // x, y, w, h, color
    //irrecv.resume(); goto strt;
    }
}

void okm2(){
  //Serial.println("ok");
  if(inlev==0){/*Serial.println("d m");*/ lev=3;}
  else {/*Serial.println("s");*/ lev=4;}
  tostrt=true;//irrecv.resume(); goto strt;
}

void numgtl3(){
  unsigned int tods;
  if(tmpcnt==0) tmparr[0]=tmparr[1]=tmparr[2]=tmparr[3]=0;
   if(inlev==2 && tmpcnt<5){//Serial.print("slvl "); Serial.println(numgt);
    tmparr[tmpcnt]=numgt; tmpcnt++; dose=0; tods=1;
    for(byte i=0;i<tmpcnt;i++){
      dose+=tmparr[tmpcnt-i-1]*tods; tods*=10;
    }
  }
  prntl3();
}

void okm4(){
  //Serial.println("ok");
  if(inlev==0) lev=5;
  else if(inlev==1) lev=6;
  else if(inlev==2) lev=7;
  else if(inlev==3) lev=8;
  tostrt=true;//irrecv.resume(); goto strt;
}

void numgtl8(){
  unsigned int tods;
  if(tmpcnt==0) tmparr[0]=tmparr[1]=tmparr[2]=tmparr[3]=0;
  if(tmpcnt<4){//Serial.print("slinvl8_1: "); Serial.println(numgt);
    tmparr[tmpcnt]=numgt; tmpcnt++; tods=1;
    if(inlev==0){
      vton=0;
      for(byte i=0;i<tmpcnt;i++){
        vton+=tmparr[tmpcnt-i-1]*tods; tods*=10;
      }
    }
    else if(inlev==1){
      tgmm=0;
      for(byte i=0;i<tmpcnt;i++){
        tgmm+=tmparr[tmpcnt-i-1]*tods; tods*=10;
      }
    }
    else if(inlev==2){
      tpl=0;
      for(byte i=0;i<tmpcnt;i++){
        tpl+=tmparr[tmpcnt-i-1]*tods; tods*=10;
      }
    }
    prntl8(); eewrite();
  }
  /*else if(inlev==1 && tmpcnt<4){Serial.print("slinvl8_2: "); Serial.println(numgt);
    tmparr[tmpcnt]=numgt; tmpcnt++; vtoff=0; tods=1;
    for(byte i=0;i<tmpcnt;i++){
      vtoff+=tmparr[tmpcnt-i-1]*tods; tods*=10;
    }
    prntl8(); eewrite();
  }*/
}

