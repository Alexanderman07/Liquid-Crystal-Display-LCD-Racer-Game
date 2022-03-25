/* Your Name & E-mail: Alexander Manjarrez & amanj003@ucr.edu
* Lab Section: Section 22
* Assignment: Lab #7 Exercise #2
* Exercise Description: In this lab exercise, I built an LCD Racer game.
*
* I acknowledge all content contained herein, excluding
* template or example code, is my own original work.
*
* Demo Link: https://www.youtube.com/watch?v=TIgDO70zH9s 
*/
// C++ code

#include <LiquidCrystal.h>

const int rs = 12, en = 11, d4 = 5, d5 = 4, d6 = 3, d7 = 2;
LiquidCrystal lcd(rs, en, d4, d5, d6, d7);
int randVar = 0;
int randRow = 0;
int row = 2;
int column = 15;
int cnt = 0;
bool place = false;
bool dontmove = false;
unsigned char obs_cnt;
char lcdShiftT[16] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
char lcdShiftB[16] = {' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' ', ' '};
unsigned char temp1 = 0;
unsigned char temp2 = 0;
unsigned char i = 15;
unsigned char j = 15;
unsigned char flag= 0;
bool resetStop = false;

typedef struct task {
  int state;
  unsigned long period;
  unsigned long elapsedTime;
  int (*TickFct)(int);
}task;

const unsigned short tasksNum = 2;
task tasks[tasksNum];

enum Car_States {Start, Release};
int TickFct_playerCar(int state) {
  switch(state) {
    case Start: 
    obs_cnt = 0;
    lcd.setCursor(0,0);
    lcd.print('>'); //Player's car
    state = Release;
    place = false;
    break;
    
    case Release: 
    
     if(flag == 1 || flag == 0) {
      lcd.setCursor(0,0);
       if(dontmove == true) {
         lcd.print('!');
       }
       else {
      lcd.print('>');
       }
    }
    
    if(flag == 2) {
      lcd.setCursor(0,1);
      if(dontmove == true) {
         lcd.print('!');
       }
      else {
      lcd.print('>');
      }
    }
    
    if(digitalRead(A0) == HIGH && digitalRead(A1) == LOW && digitalRead(A2) == LOW) {
      lcd.clear();
      lcd.setCursor(0,0);
      lcd.print('>');
      flag = 1;
      place = false;
      state = Release;
    }
    else if(digitalRead(A0) == LOW && digitalRead(A1) == HIGH && digitalRead(A2) == LOW) {
      lcd.clear();
      lcd.setCursor(0,1);
      lcd.print('>');
      flag = 2;
      place = true;
      state = Release;
    }
    else if(digitalRead(A0) == LOW && digitalRead(A1) == LOW && digitalRead(A2) == HIGH) {
      lcd.clear();
      
       for(int g = 0; g < 16; g++){
      lcdShiftB[g] = ' ';
      lcdShiftT[g] = ' ';
    }

      lcd.setCursor(0,0);
      lcd.print('>');
      resetStop = true;
      i = 15;
      flag= 0;
      tasks[1].period = 300;
      state = Start;
    }
    break; 
    
    default:
    break;
  }

    switch(state) {
      case Start:
      break;
      
      case Release:
      break;
      
      default:
      break;            
  }
  return state;
}

enum Obstacle_States{Start_obs, Move, EndGame, Display};
int TickFct_Obstacles(int state) {
  switch(state) {
    case Start_obs:
    obs_cnt = 0;
    cnt = 0;
    state = Move;
    break;
    
    case Move:
    obs_cnt++;
    
    if((obs_cnt % 10) == 0) {
      if(tasks[1].period > 100) {
        tasks[1].period -= 10;
      }
    }
    
    if((place == false && lcdShiftT[0] == 1) || (place == true && lcdShiftB[0] == 1)) {
        dontmove = true;
      if(flag == 1 || flag == 0) {
      lcd.setCursor(0,0);
      lcd.print('!');
     }
      else if(flag == 2) {
      lcd.setCursor(0,1);
      lcd.print('!');
     }
        state = EndGame;
      }
    
    randVar = random(0,8);
    if(randVar == 0){
      randRow = random(0,2);
      if(randRow == 1){
        lcdShiftT[15] = 1;
      }
      else{
        lcdShiftB[15] = 1;
      }
    }
    else{
      lcdShiftT[15] = 0;
      lcdShiftB[15] = 0;
    }
    
    
    if(dontmove != true) {
    
    for(int j = 0; j < 16; j++) {
      if(lcdShiftT[j] == 1){
        lcd.setCursor(j, 0);
        lcd.print('*');
      }
      else{
        lcd.setCursor(j, 0);
        lcd.print(' ');
      }
    }
    for(int j = 0; j < 16; j++){
      if(lcdShiftB[j] == 1){
        lcd.setCursor(j, 1);
        lcd.print('*');
      }
      else{
        lcd.setCursor(j, 1);
        lcd.print(' ');
      }
    }
    
    for(int k = 0; k < 15; k++) {
      lcdShiftT[k] = lcdShiftT[k+1];
      lcdShiftB[k] = lcdShiftB[k+1]; 
    }
      state = Move;
    }
    
    break;
    
    case EndGame: 
  cnt++;
    Serial.println(cnt);
    if(cnt == 10){
    state = Display;
    }
    break;
    
    case Display:
    lcd.clear();
    lcd.setCursor(2,0);
    lcd.print("Game Over");
    lcd.setCursor(2,1);
    lcd.print("Score:");
    lcd.setCursor(8,1);
    lcd.print(obs_cnt);
    if(digitalRead(A0) == LOW && digitalRead(A1) == LOW && digitalRead(A2) == HIGH) {
      lcd.clear();
       for(int g = 0; g < 16; g++){
      lcdShiftB[g] = ' ';
      lcdShiftT[g] = ' ';
    }

      lcd.setCursor(0,0);
      lcd.print('>'); //Player's car
      resetStop = true;
      i = 15;
      tasks[1].period = 300;
      flag= 0;
      dontmove = false;
      state = Start;
    }
    
    
    break;
    
    default:
    break;
  }

  switch(state) {
    case Start_obs:
    break;
    
    case Move:
    break;
    
    case Display:
    break;
    
    default:
    break;
  }
  return state;
}

void setup()
{
  lcd.begin(16, 2);
  unsigned char i = 0;
  tasks[i].state = Start;
  tasks[i].period = 200;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_playerCar;
  i++;
  tasks[i].state = Start_obs;
  tasks[i].period = 300;
  tasks[i].elapsedTime = 0;
  tasks[i].TickFct = &TickFct_Obstacles;
  i++;
  Serial.begin(9600);
}

void loop()
{
  lcd.setCursor(0, 1);
  unsigned char i;
  for (i = 0; i < tasksNum; ++i) {
    if ( (millis() - tasks[i].elapsedTime) >= tasks[i].period) {
        tasks[i].state = tasks[i].TickFct(tasks[i].state);
        tasks[i].elapsedTime = millis(); // Last time this task was ran
    }
  }
  delay(100);
}
