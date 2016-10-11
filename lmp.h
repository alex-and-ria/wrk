class lmp{
  private:
    byte pin;
  public:
    bool stt=false;
    lmp(byte pn): pin(pn){
      pinMode(pin,OUTPUT);
    }
    void lght(){
      if(!stt) digitalWrite(pin,HIGH);//relay is activated by ground;
      else digitalWrite(pin,LOW);
    }
};

