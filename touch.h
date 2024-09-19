
void State_Print(void);                     //터치게임 게임 serial.print
void Read_Button(void);                     //터치게임 게임 LED를 잡으면 점수++
void Set_RandNo_LED(void);                  //랜덤으로 led 켜기
void check_gameTime(void);                  //진행시간이 제한시간을 넘었다면 터치게임 게임 종료
unsigned long _Gametimer_read(void);        //게임 진행시간

void State_Print(void){
    Serial.print("score=");
    Serial.println(score); 
    Serial.print("gameSpeed=");
    Serial.println(gameSpeed);
    Serial.print("gameTime=");
    Serial.print(_Gametimer_read());
}

void Read_Button(void){
    _Waittimer(0);
    while(1){
        int tempTime = gameRun-_Gametimer_read();
        tempTime = max(tempTime,0);
        
        display.showNumberDec(tempTime/100, 0b010000<<2, true,3,0);
        display.showNumberDec(score, 0, true,2,4);
     
        dataread();
        if     (randNo==0 && data&0x01) {play(do5 ,n32); score++; gameSpeed-=speed_step; break;}
        else if(randNo==1 && data&0x02) {play(re5 ,n32); score++; gameSpeed-=speed_step; break;}
        else if(randNo==2 && data&0x04) {play(mi5 ,n32); score++; gameSpeed-=speed_step; break;}
        else if(randNo==3 && data&0x08) {play(pa5 ,n32); score++; gameSpeed-=speed_step; break;}   
        else if(randNo==4 && data&0x10) {play(sol5,n32); score++; gameSpeed-=speed_step; break;}
        else if(randNo==5 && data&0x20) {play(ra5 ,n32); score++; gameSpeed-=speed_step; break;}
        else if(randNo==6 && data&0x40) {play(si5 ,n32); score++; gameSpeed-=speed_step; break;}   
        else if(randNo==7 && data&0x80) {play(do6 ,n32); score++; gameSpeed-=speed_step; break;}        
        else {
            if(data!=0) {play(do4,n8);play(do4,n8); gameSpeed+=speed_step; break;}
        }
        if(_Waittimer_read() > gameSpeed) {play(do4,n8);play(do4,n8); gameSpeed+=speed_step; break;}
       // display.showNumberDec((gameRun-_Gametimer_read())/100, 0b010000<<2, true,3,0);
        //display.showNumberDec(score, 0, true,2,4);
        
    }
    
    gameSpeed=max(gameSpeed,gameSpeedMin);
    gameSpeed=min(gameSpeed,gameSpeedMax);
    score=max(score,0);
    
    
}    

void Set_RandNo_LED(void){
    while(1){
        randNo = random(0,8);
        if(randNo != b_no){
            b_no=randNo;
            //levelup();
             setLed(0x00);
            //play(buzz[randNo],n8);
            delay(100);
            pcf.digitalWrite(OPIN[randNo], HIGH);
            break;
        }
        
    }
}

void check_gameTime(void){
    if(_Gametimer_read()>gameRun){
        
        display.showNumberDec(0, 0, true,3,0);
        setLed(0x00);
        end_play(n8);
        for(int i=0;i<5;i++){
            setLed(0xFF);delay(200);
            setLed(0x00);delay(200);
        }
         score=0; // 공용
        gameSpeed=1500; //터치게임    
        increase_melody(GameMode+1);
        //_Gametimer(0); //터치게임 
        start_code();
        _Gametimer(0);
       
    }
}

unsigned long _Gametimer_read(void){
    return millis()-GameTime;
}


