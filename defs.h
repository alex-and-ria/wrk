#ifndef DEFS_H
#define DEFS_H
Adafruit_PCD8544 display = Adafruit_PCD8544(3, 4, 5, 6, 7); /*-----( Declare Constants )-----*/
#define receiver 11 // pin 1 of IR receiver to Arduino digital pin 11
/*-----( Declare objects )-----*/
IRrecv irrecv(receiver);           // create instance of 'irrecv'
decode_results results;            // create instance of 'decode_results'
#define down 0xFFA857
#define up 0xFF629D
#define left 0xFF22DD
#define right 0xFFC23D
#define ok 0xFF02FD
#define one 0xFF6897
#define two 0xFF9867
#define three 0xFFB04F
#define four 0xFF30CF
#define five 0xFF18E7
#define six 0xFF7A85
#define seven 0xFF10EF
#define eight 0xFF38C7
#define nine 0xFF5AA5
#define ast 0xFF42BD
#define zero 0xFF4AB5
#define ltc 0xFF52AD
#define slctw 75// x coord of '<' symbol;
#define prenw 2// pre num width;
#define chpl 50// left coord for change pin opt;
#define btnpn 19// button pin (clear in 0 lev);
#define trin 2// input btn to open valve;
#define trout 9// output pin to open valve via relay;
#define fwminp 8// flowmeter input pin;
#define dvsr 1024// 101 - CLK/1024;
#define tclk 16000000// 16MHz;
#define minml 350// at least 350 milliliters;
#define maxml 20000// maximum of 20 liters;
#define rnd 20// round;
//#define t2ml 0.516
#define ml2lout 1000
byte lev=0;// display level;
byte inlev=0;// display selected item in level;
byte maxinlev=0;// display selected item in level;

bool pincode = false;         // Верность пина
bool match=false;//old pin matches pin;
bool cnfrm=false;//new pin confirmed;
bool ures=true, strtb=false;// user reset, start button;
bool tostrt=false;//goto start;
//bool fwm=true;//flowmetter 30/60;
bool clrq=false,clrd=false;//clear uqty; clear udaily;
byte str[ ] = {1,1,1,1};         // Наш верный пин
byte strpin [4];               //Массив  пин-
byte strpinn [4];
byte strpinc [4];
byte tmparr[5];//for entering numbers from ir
byte numgt;
unsigned short int vton=3000;//valve time on;
unsigned int tgmm=600;// valve t_off;
unsigned int tpl=516;
unsigned long mvton;
unsigned long nt1;
//unsigned int vtoff=120;//valve time off (ms);
byte steppin = 0;               // Шаг в пине
byte steppinn = 0;
byte steppinc = 0;
byte tmpcnt = 0;
byte msk=0;
unsigned long all = 0;      // Поле  со всем количеством
unsigned long dailyadm = 0;    //Дневная выпущенная доза
unsigned long dailyusr = 0;    //Дневная выпущенная доза 
unsigned short int dose = 0;     // Доза
unsigned short int qtyadm = 0;      // Количество
unsigned short int qtyusr = 0;      // Количество
unsigned long ltrs=0;//millilitres;

volatile unsigned long int cnt=0,tachBuf;
unsigned long int vn=0,cntout=dose*(tpl/(1000+0.0));
unsigned int tclkd=tclk/dvsr;
double fk=0.0;
volatile bool inint=false;
bool vviso=false,allwvo=true,fixom=false;//,isfst=true;
#endif
