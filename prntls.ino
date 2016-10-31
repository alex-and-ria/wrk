#ifndef PRNTLS_H
#define PRNTLS_H

void prntl0(){
  display.fillRect(slctw, 3, 5, LCDHEIGHT-7, WHITE); // x, y, w, h, color
  display.fillRect(1, 1, slctw-1, LCDHEIGHT-3, WHITE); // x, y, w, h, color
  display.setTextSize(2);
  display.setCursor(prenw+20,3);//  display.setTextSize(3/2);
  //display.print("QTY: ");
  display.print(qtyusr);
  display.display();

  display.setTextSize(1/2);
  display.setCursor(prenw,25);//  display.setTextSize(1/2);
  display.print(" dose:  ");
  display.print(dose);
  display.display();

  display.setTextSize(1);
  display.setCursor(prenw,35);//  display.setTextSize(1);
  display.print("daily: ");
  display.print(dailyusr/(ml2lout+0.0));
  display.display();
}

void prntl1(){
  display.fillRect(1, 1, slctw-1, LCDHEIGHT-3, WHITE); // x, y, w, h, color
  display.setCursor(15,10);
  display.print("Enter pin");  
  //display.setCursor(29,22);  display.print("****");
  display.drawRect(25, 21, 30, 10, BLACK); // x, y, w, h, color
  display.display();
}

void prntl2(){
  display.fillRect(1, 1, slctw-1, LCDHEIGHT-3, WHITE); // x, y, w, h, color
  display.setCursor(prenw,3);
  display.print("Device menu");
  display.setCursor(prenw,25);
  display.print("Settings");
  display.display();
}

void prntl3(){
  display.fillRect(1, 1, slctw-1, LCDHEIGHT-3, WHITE); // x, y, w, h, color
  display.setCursor(prenw,3); display.print("All: "); display.print(all/(ml2lout+0.0));
  display.setCursor(prenw,14); display.print("QTY: "); display.print(qtyadm);
  display.setCursor(prenw,25); display.print("dose: "); display.print(dose);
  display.setCursor(prenw,37); display.print("daily: "); display.print(dailyadm/(ml2lout+0.0));
  display.display();
}

void prntl4(){
  display.fillRect(1, 1, slctw-1, LCDHEIGHT-3, WHITE); // x, y, w, h, color
  display.setCursor(prenw,3); display.print("Pin menu");
  display.setCursor(prenw,14); display.print("Button menu");
  display.setCursor(prenw,25); display.print("Freeweel");
  display.setCursor(prenw,37); display.print("Times");
  display.display();
}

void prntl5(){
  display.fillRect(slctw, 3, 5, LCDHEIGHT-7, WHITE); // x, y, w, h, color
  display.fillRect(1, 1, slctw-1, LCDHEIGHT-3, WHITE); // x, y, w, h, color
  display.setCursor(prenw,3); display.print("old pin: ");
  display.setCursor(prenw,20); display.print("new pin: ");  
  display.setCursor(prenw,35); display.print("confirm: ");
  display.display();
  for(unsigned short i=0;i<4;i++) strpin[i]=strpinn[i]=strpinc[i]='\0';
  strpinn[0]=5; strpinc[0]=7; steppin = 0; steppinn = 0; steppinc = 0;
}

void prntl5l(unsigned int inlev){
  switch(inlev){
    case 0:
      display.fillRect(chpl, 3, 25, 7, WHITE); // x, y, w, h, color
      steppin=0; strpin[0]=strpin[1]=strpin[2]=strpin[3]='\0';
    break;
    case 1:
      display.fillRect(chpl, 20, 25, 7, WHITE); // x, y, w, h, color
      steppinn=0; strpinn[0]=5; strpinn[1]=strpinn[2]=strpinn[3]='\0';
    break;
    case 2:
      display.fillRect(chpl, 35, 25, 7, WHITE); // x, y, w, h, color
      steppinc=0; strpinc[0]=7; strpinc[1]=strpinc[2]=strpinc[3]='\0';
    break;
  }
}

void prntl6(){
  display.fillRect(1, 1, slctw-1, LCDHEIGHT-3, WHITE); // x, y, w, h, color
  display.setCursor(prenw,3);
  if(ures) display.print("user reset   on");
  else display.print("user reset   off");
  display.setCursor(prenw,25);
  if(strtb) display.print("start button flow");
  else display.print("start button stop");
  display.display();
}

void prntl7(){
  display.fillRect(1, 1, slctw-1, LCDHEIGHT-3, WHITE); // x, y, w, h, color
  display.setCursor(prenw,20);
  display.print("mlitres: "); display.print(ltrs);
  display.display();
}

void prntl8(){
  display.fillRect(1, 1, slctw-1, LCDHEIGHT-3, WHITE); // x, y, w, h, color
  display.setCursor(prenw,3); display.print("t_on: "); display.print(vton);
  display.setCursor(prenw,20); display.print("t_off: "); display.print(tgmm);
  display.setCursor(prenw,35); display.print("tpl: "); display.print(tpl);
  //if(fwm) display.print("30"); else display.print("60");
  display.display();
}
#endif
