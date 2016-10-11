#ifndef EEPRM_H
#define EEPRM_H
void eewrite(){
  int adr=0; byte msk=0;
  msk|=lmp1.stt?0x80:0; msk|=lmp2.stt?0x40:0; msk|=lmp3.stt?0x20:0; msk|=lmp4.stt?0x10:0;
  EEPROM.put(adr, msk);
  Serial.print("write: "); Serial.println(msk,BIN);
}

void eeread(){
  int adr=0; byte msk=0;
  EEPROM.get(adr, msk);
  lmp1.stt=msk&0x80; lmp2.stt=msk&0x40; lmp3.stt=msk&0x20; lmp4.stt=msk&0x10;
  Serial.print("read: "); Serial.println(msk,BIN);
}
#endif
