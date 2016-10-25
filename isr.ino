ISR(TIMER1_CAPT_vect){
  cnt++;
  tachBuf  = ICR1;
  TCNT1=0x0;
}
