#ifndef MV_P_CNUMS_H
#define MV_P_CNUMS_H
void slctmv(unsigned int to, unsigned int amnt){
  if(to>amnt) return;
  display.fillRect(slctw, 3, 5, LCDHEIGHT-7, WHITE); // x, y, w, h, color
  display.display();
  switch(amnt){
    case 0: display.setCursor(slctw,20); break;
    case 1:
      switch(to){
        case 0: display.setCursor(slctw,3); break;
        case 1: display.setCursor(slctw,25); break;
      }
    break;
    case 2:
    switch(to){
      case 0: display.setCursor(slctw,3); break;
      case 1: display.setCursor(slctw,20); break;
      case 2: display.setCursor(slctw,35); break;
    }
    break;
    case 3:
      switch(to){
        case 0: display.setCursor(slctw,3); break;
        case 1: display.setCursor(slctw,14); break;
        case 2: display.setCursor(slctw,25); break;
        case 3: display.setCursor(slctw,37); break;
      }
    break;
  }
  display.print("<");
  display.display();
}

void cnum(unsigned int num){
  //Serial.println(num);
  if(steppin==0) display.setCursor(29,22);// to * symbols;
  if(steppin<4){
    strpin[steppin]=(byte)num;
    steppin++;
    display.print("*");
  }
  //Serial.print(steppin); Serial.println("q"); for(int i=0;i<4;i++) Serial.print (strpin[i]); Serial.println("qq");
}

void cnum1(unsigned int num, unsigned int inlev){
  //Serial.println(num);
  switch(inlev){
    case 0:
      if(steppin==0) display.setCursor(chpl,3);// to * symbols;
      if(steppin<4){
        strpin[steppin]=(byte)num;
        steppin++;
        display.print("*");
      }
    break;
    case 1:
      if(steppinn==0) display.setCursor(chpl,20);// to * symbols;
      if(steppinn<4){
        strpinn[steppinn]=(byte)num;
        steppinn++;
        display.print("*");
      }
    break;
    case 2:
      if(steppinc==0) display.setCursor(chpl,35);// to * symbols;
      if(steppinc<4){
        strpinc[steppinc]=(byte)num;
        steppinc++;
        display.print("*");
      }
    break;
  }
}
#endif
