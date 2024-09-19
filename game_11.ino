#include <PCF8575.h>              // https://github.com/xreef/PCF8575_library
#include <TM1637TinyDisplay6.h> // https://github.com/jasonacox/TM1637TinyDisplay

#include "main.h"
#include "mole.h"
#include "memory.h"


void setup(){
    Serial.begin(57600);
    display.begin();
    display.setBrightness(0x02);
    display.clear();
    for(int i=0;i<8;i++){
        pcf.pinMode(OPIN[i],OUTPUT);
        pcf.pinMode(IPIN[i],INPUT);
    }
    pcf.begin();
    randomSeed(analogRead(A1));
    score=0;
    gameSpeed=1500;
    data=0;
    score=0;
    level=2;
    good_count=0;
    life=3;
    start_code();
    _Gametimer(0);
}

void loop(){
    if(GameMode==1){
        moleGame();
    }else if(GameMode==2){
        
        memoryGame();
    }
    
   
}

//터치게임 게임
void moleGame(void){  
          
   
    Set_RandNo_LED();
    Read_Button();
    State_Print();
    check_gameTime(); //게임진행시간 체크
    delay(100);
}

//기억력 게임
void memoryGame(void){
    //Read_diff_mode();
    level_up();
    random_led_on_me(level);
    keyRead(level);
    messegePrint();
    level=max(level,1);
    
    delay(1000);
    check_life();
}
