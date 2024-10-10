
void start_code(void);     //게임이 종료,시작 되었을때 어떤 게임을 실행할지 정하기 위한 함수
void setLed(byte value);   //원하는 led 켜기
void begin_melody1(void);  //터치게임게임 시작음
void begin_melody2(void);  //기억력게임 시작음
void up_melody(void);      //도 미 솔
void down_melody(void);    //솔 미 도
void Read_diff_mode(void); //게임 난이도

void play(unsigned int sound, unsigned int note); //박자수만큼 음을 연주
//void buzzer(unsigned int mel,unsigned char count, unsigned int ontime, unsigned int offtime);//부져음 횟수와 온타임오프타임지정
void increase_melody(int count);                // 부팅멜로디 함수 
void start_play(unsigned int note);         //도도 미미 솔솔 도도
void end_play(unsigned int note);         // 도도 솔솔 미미 도도
void dataread();                            //버튼이 눌렸는지 읽기
void _Gametimer(int offset);                //
void _Waittimer(int offset);              //
unsigned long _Waittimer_read(void);        //기다린 시간(game_speed를 기다린 시간이 넘었는지 확인하기 위한 함수)

#define n32     59.1       
#define n16     118.2
#define n8    236.4

#define do4  262
#define ra4  440
#define do5  262*2
#define re5  294*2
#define mi5  330*2
#define pa5  349*2
#define sol5 392*2
#define ra5  440*2
#define si5  494*2
#define do6  do5*2



int OPIN[]={P0,P1,P2,P3,P4,P5,P6,P7};
int IPIN[]={P8,P9,P10,P11,P12,P13,P14,P15};
int buzz[]={do5,re5,mi5,pa5,sol5,ra5,si5,do6};



//memory
int life=3;
int ranNo[10];
int level=2;
int good_count=0;
int up_count=3;
//


//touch
int gameSpeed = 1500;
int gameSpeedMin=600;
int gameSpeedMax=1500;
int gameRun=30000;
int b_no; //이전 랜덤값
int randNo;
int speed_step=50;
//

unsigned long WaitTime = millis();
unsigned long GameTime = millis();
int GameMode=0;

int CLK = 12;
int DIO = 11;
int buzzerPin = 8;
int buttonPin1 = A1;
int buttonPin2 = A2;
int vrPin = A3;
int difficultyMode=1;


byte data=0;// 버튼 상태정보
int score = 0;

TM1637TinyDisplay6 display(CLK, DIO);
PCF8575 pcf(0x20);



void begin_melody1(void){
    play(ra4,n16);    play(do5,n16);    play(mi5,n16);    play(sol5,n8);  play(mi5,n16);    play(sol5,n8);
}
void begin_melody2(void){
    play(sol5,n16);    play(mi5,n16);    play(do5,n16);    play(ra4,n8);  play(do5,n16);    play(sol5,n8);
}

void up_melody(void){
    play(do5,n32);    play(mi5,n32);    play(sol5,n32);    play(do6,n32);
}

void down_melody(void){
    play(do6,n32);    play(sol5,n32);    play(mi5,n32);    play(do5,n32);
}

void play(unsigned int sound, unsigned int note){    
    tone(buzzerPin, sound, note); 
    delay(note*1.1);    
    noTone(buzzerPin);
    pinMode(buzzerPin,INPUT_PULLUP);
}
void increase_melody(int count){
    for(int j=0;j<count;j++){
        for(int i=1000;i<2500;i=i+50){
            tone(buzzerPin,i,20); delay(10);
        }
    }
    noTone(buzzerPin);
}

void start_play(unsigned int note){
    int melody[]={do5,do5,mi5,mi5,sol5,sol5,do6,do6};
    
    for (int thisNote = 0; thisNote < 9; thisNote++) {
        
        tone(buzzerPin, melody[thisNote], note);
        delay(note);
        noTone(buzzerPin);
    }
   
}

void end_play(unsigned int note){
    int melody[]={do6,do6, sol5, sol5, mi5,mi5,do5, do5};
    
    for (int thisNote = 0; thisNote < 9; thisNote++) {
      
        tone(buzzerPin, melody[thisNote], note);
        delay(note);
        noTone(buzzerPin);
    }
  
}


void dataread(){
    //8개의 버튼정보를 8비트에 넣기 위해 사용되는 함수
    PCF8575::DigitalInput di = pcf.digitalReadAll();
    //char btn_data[] = {di.p8,di.p9,di.p10,di.p11,di.p12,di.p13,di.p14,di.p15}; 

    // b7   b6   b5   b4  b3   b2   b1   b0
    //  1   1     1    1   1   1    1    (0) di.p8
    //                              (0)di.p9
    //
    //                         (0)do.10
    // (0) do.p15

    
    data = ~(di.p8 | di.p9<<1 | di.p10<<2 | di.p11<<3 | di.p12<<4 | di.p13<<5 | di.p14<<6 | di.p15<<7);//0~255    
}

void Read_diff_mode(void){
    int analog_value=analogRead(vrPin);
    
    int temp_difficultyMode =analog_value/350;
    
    if(difficultyMode!= temp_difficultyMode){
        difficultyMode= temp_difficultyMode;
        display.clear();
        //display.showNumberDec(temp_difficultyMode, 0, true,2,4);
        switch (difficultyMode){

            case 0 : display.showString("EASY"); speed_step=30; life=5; up_count=7; break;
            case 1 : display.showString("NORMAL"); speed_step=50; life=30; up_count=5; break;
            case 2 : display.showString("HARD"); speed_step=70; life=2; up_count=3; break;
        }
        play(do5,n8); play(sol5,n8); play(do5,n8);
        
    }
     
}
void start_code(void){
    pinMode(buzzerPin,OUTPUT);
    pinMode(pinSTART,INPUT_PULLUP);
    pinMode(buttonPin1,INPUT_PULLUP);
    pinMode(buttonPin2,INPUT_PULLUP);
    setLed(0x00);
    display.showString("ready");
    start_play(n8);
    increase_melody(1);            //시작음을 1회 소리내기
    
    
    while(1){
        
            
        Read_diff_mode();    
        dataread();
        Serial.println(data,HEX);
        if(digitalRead(buttonPin1)==LOW){
            GameMode=2; break;
        }else if(digitalRead(buttonPin2)==LOW){
            GameMode=1; break;
        }
        display.showString("ready");
        setLed(0xA5);delay(100);
        display.clear();
        setLed(0x5A);delay(100);
    }
    if(GameMode==1){
        display.showString("start1");
        begin_melody1();
    }else if(GameMode==2){
        display.showString("start2");
        begin_melody2();
    }
    
     
     
     delay(1000);
     display.clear();
    _Gametimer(0); //터치게임
  /*  score=0; // 공용
    gameSpeed=1500; //터치게임    
    increase_melody(GameMode+1);
    */ 
    
}

void setLed(byte value){
   
    
    for(int i=0;i<8;i++){
        (value&1<<i) ? pcf.digitalWrite(OPIN[i], HIGH) : pcf.digitalWrite( OPIN[i], LOW);
    }
    
    /*
    (value&1<<0) ? pcf.digitalWrite(OPIN[0], HIGH) : pcf.digitalWrite( OPIN[0], LOW);  //1
    (value&1<<1) ? pcf.digitalWrite(OPIN[1], HIGH) : pcf.digitalWrite( OPIN[1], LOW);  //2
    (value&1<<2) ? pcf.digitalWrite(OPIN[2], HIGH) : pcf.digitalWrite( OPIN[2], LOW);  //4
    (value&1<<3) ? pcf.digitalWrite(OPIN[3], HIGH) : pcf.digitalWrite( OPIN[3], LOW);  //8
    (value&1<<4) ? pcf.digitalWrite(OPIN[4], HIGH) : pcf.digitalWrite( OPIN[4], LOW);  //16
    (value&1<<5) ? pcf.digitalWrite(OPIN[5], HIGH) : pcf.digitalWrite( OPIN[5], LOW);  //32
    (value&1<<6) ? pcf.digitalWrite(OPIN[6], HIGH) : pcf.digitalWrite( OPIN[6], LOW);  //64
    (value&1<<7) ? pcf.digitalWrite(OPIN[7], HIGH) : pcf.digitalWrite( OPIN[7], LOW);  //128
    */
}

void _Gametimer(int offset){
    GameTime = millis()+offset;
}


void _Waittimer(int offset){
    WaitTime = millis()+offset;
}
unsigned long _Waittimer_read(void){
    return millis()-WaitTime;
}




