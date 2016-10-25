#ifndef EEPROM_W_H
#define EEPROM_W_H
void eewrite(){
  int adr=0; EEPROM.put(adr, qtyusr);
  adr+=sizeof(qtyusr); EEPROM.put(adr, dose);
  adr+=sizeof(dose); EEPROM.put(adr, dailyusr);
  adr+=sizeof(dailyusr); EEPROM.put(adr, str[0]);
  adr+=sizeof(str[0]); EEPROM.put(adr, str[1]);
  adr+=sizeof(str[1]); EEPROM.put(adr, str[2]);
  adr+=sizeof(str[2]); EEPROM.put(adr, str[3]);
  adr+=sizeof(str[3]); EEPROM.put(adr, all);
  adr+=sizeof(all); EEPROM.put(adr, dailyadm);
  adr+=sizeof(dailyadm); EEPROM.put(adr, qtyadm);
  adr+=sizeof(qtyadm);
  msk=0; msk|=ures?0x80:0;// msk|=strtb?0x40:0; msk|=fwm?0x20:0;
  //Serial.println(msk,HEX); 
  EEPROM.put(adr, msk);
  adr+=sizeof(msk); EEPROM.put(adr, vton);
  adr+=sizeof(vton); EEPROM.put(adr, tgmm);
  adr+=sizeof(tgmm); EEPROM.put(adr, tpl);
}

void eeread(){
  int adr=0; EEPROM.get(adr, qtyusr);
  adr+=sizeof(qtyusr); EEPROM.get(adr, dose);
  adr+=sizeof(dose); EEPROM.get(adr, dailyusr);
  adr+=sizeof(dailyusr); EEPROM.get(adr, str[0]);
  adr+=sizeof(str[0]); EEPROM.get(adr, str[1]);
  adr+=sizeof(str[1]); EEPROM.get(adr, str[2]);
  adr+=sizeof(str[2]); EEPROM.get(adr, str[3]);
  adr+=sizeof(str[3]); EEPROM.get(adr, all);
  adr+=sizeof(all); EEPROM.get(adr, dailyadm);
  adr+=sizeof(dailyadm); EEPROM.get(adr, qtyadm);
  adr+=sizeof(qtyadm); EEPROM.get(adr, msk);
  ures=msk&0x80;// strtb=msk&0x40; fwm=msk&0x20;
  adr+=sizeof(msk); EEPROM.get(adr, vton);
  adr+=sizeof(vton);EEPROM.get(adr, tgmm);
  adr+=sizeof(tgmm);EEPROM.get(adr, tpl);
  //Serial.println(msk,HEX);
}
#endif
