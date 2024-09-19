
void messegePrint(void);        //기억력게임 serial.print
void keyRead(int level);        //맞추었는지 틀렸는지 확인하는 함수
void dataread_me(void);         //버튼이 툴리면 그에 해당하는 led켜기
void random_led_on_me(int levelcnt);        //랜덤하게 led 켜기
void level_up(void);                    //굿카운트가 4 이상이면 level을 올리고 굿카운트를 0으로 초기화, 굿 카운트가 0보다 작으면 level,score을 1씩 깎고 굿카운트 초기화
void check_life(void);                  //생명이 0이되면 메모리 게임을 종료하기 start_code실행

void messegePrint(void){
    Serial.print("score= ");
    Serial.print(score);
    Serial.print("    life= ");
    Serial.print(life);
    Serial.print("    level= ");
    Serial.println(level);
    Serial.print("    good_count= ");
    Serial.println(good_count);
    
}

void keyRead(int level){
   
    bool runState=true;
    for(int i=0;i<level && runState ;i++){ 
        _Waittimer(0);
        while(runState){
            dataread_me();
            if     (ranNo[i]==0 && data&0x01) {play(do5 ,n32); good_count++;score++; break;}
            else if(ranNo[i]==1 && data&0x02) {play(re5 ,n32); good_count++;score++; break;}
            else if(ranNo[i]==2 && data&0x04) {play(mi5 ,n32); good_count++;score++; break;}
            else if(ranNo[i]==3 && data&0x08) {play(pa5 ,n32); good_count++;score++; break;}   
            else if(ranNo[i]==4 && data&0x10) {play(sol5,n32); good_count++;score++; break;}
            else if(ranNo[i]==5 && data&0x20) {play(ra5 ,n32); good_count++;score++; break;}
            else if(ranNo[i]==6 && data&0x40) {play(si5 ,n32); good_count++;score++; break;}   
            else if(ranNo[i]==7 && data&0x80) {play(do6 ,n32); good_count++;score++; break;}    
            else{
                    if(data!=0)   {play(do4,n8);play(do4,n8); play(do4,n8);life--; good_count-=7;runState=false; }
            }   
            if(_Waittimer_read() > 3000){ play(do4,n8);play(do4,n8);life--; good_count-=7; runState=false; }
                 
       }
     
       score=max(score,0);
       display.showNumberDec(life, 0, true,2,0);
       display.showNumberDec(score, 0, true,3,3);
    }
    
}  
    
void dataread_me(void){
   dataread();
    if(data==0x01){
        setLed(0x01);
        delay(200);
        setLed(0x00);
    }else if(data==0x02){
        setLed(0x02);
        delay(200);
        setLed(0x00);
    }else if(data==0x04){
        setLed(0x04);
        delay(200);
        setLed(0x00);
    }else if(data==0x08){
        setLed(0x08);
        delay(200);
        setLed(0x00);
    }else if(data==0x10){
        setLed(0x10);
        delay(200);
        setLed(0x00);
    }else if(data==0x20){
        setLed(0x20);
        delay(200);
        setLed(0x00);
    }else if(data==0x40){
        setLed(0x40);
        delay(200);
        setLed(0x00);
    }else if(data==0x80){
        setLed(0x80);
        delay(200);
        setLed(0x00);
    }
   
}


void random_led_on_me(int levelcnt){
    for(int i=0;i<levelcnt;i++){
        setLed(0x00);
        switch(ranNo[i]){
            case 0: play(do5,n8);   setLed(0x01);   break;
            case 1: play(re5,n8);   setLed(0x02);   break;
            case 2: play(mi5,n8);   setLed(0x04);   break;
            case 3: play(pa5,n8);   setLed(0x08);   break;
            case 4: play(sol5,n8);  setLed(0x10);   break;
            case 5: play(ra5,n8);   setLed(0x20);   break;
            case 6: play(si5,n8);   setLed(0x40);   break;
            case 7: play(do6,n8);   setLed(0x80);   break;
        }
        delay(100);
        setLed(0x00);
        delay(150);
    }
}


void level_up(void){
    for(int i=0;i<10;i++){
        ranNo[i] = random(0,8);
    }
    display.showNumberDec(life, 0, true,2,0);
    display.showNumberDec(score, 0, true,3,3);
    
    if(good_count>=up_count) {
        up_melody();
        level++;
        good_count=0;
        display.showNumberDec(life, 0, true,2,0);
        display.showNumberDec(score, 0, true,3,3);
    }else if(good_count<0) {
        
        level--;
        down_melody();
        good_count=0;
        display.showNumberDec(life, 0, true,2,0);
        display.showNumberDec(score, 0, true,3,3);
    }else{
        play(do6,n32);play(do6,n16);play(do6,n8);delay(200);
    }
}


void check_life(void){
    if(life<=0){
        display.showNumberDec(0, 0, true,2,0);
        
         setLed(0x00);
        end_play(n8);
        for(int i=0;i<5;i++){
            setLed(0xFF);delay(200);
            setLed(0x00);delay(200);
        }
        
        score=0;
        level=2;
        good_count=0;
        life=3;
        start_code();
       
    }
}
