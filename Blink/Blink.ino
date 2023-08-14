
//Correct Final Code
#include "bsp_ee_emerge.h"
#include "ws2812.h"

char secondaryArr[36];

int count0 = 0;

int count1 = 6;
int colCount1 = 11;

int count2 = 12;

int count3 = 18;
int colCount3 = 23;

int count4 = 24;

int count5 = 30;
int colCount5 = 35;



int count = 0;
int gameCount = 0;

int atEdge = 0;

uint8_t user[2][3];
char user1Char = 0;
char user2Char = 1;

uint8_t buttonPressed_last[6];
// the setup routine runs once when you press reset:
void setup()
{
    if (CALBC1_16MHZ==0xFF)    // If calibration constant erased
    {
        while(1);              // do not load, trap CPU!!
    }

    // configure clock to 16 MHz
    BCSCTL1 = CALBC1_16MHZ;    // DCO = 16 MHz
    DCOCTL = CALDCO_16MHZ;
    
    BSP_Init();

      user[0][0] = 64;
      user[0][1] = 0;
      user[0][2] = 0;
      user[1][0] = 0;
      user[1][1] = 64;
      user[1][2] = 0;
      buttonPressed_last[0] = 0;
      buttonPressed_last[1] = 0;
      buttonPressed_last[2] = 0;
      buttonPressed_last[3] = 0;
      buttonPressed_last[4] = 0;
      buttonPressed_last[5] = 0;
      setupArray(secondaryArr);
}
void debounce(uint8_t* button_state, uint8_t button_current_value)
{
    // LSB used to store state
    (*button_state) = (0x80 & (*button_state)) | (0x7F & (*button_state))<<1 | button_current_value;
//1000 0000 | 0110 1100 - 1101 1000 | 0000 0001 = 1101 1001
    if( (0x7F & (*button_state)) == 0x7F)
    {
       (*button_state) = 0xFF;
    }
    else if ((0x7F & (*button_state)) == 0x00)
    {
        (*button_state) = 0x00;
    }
}

uint8_t button_pressed(uint8_t *button_state)
{
    if( (*button_state) == 0xFF)
    {
        return 1;
    }
    else if ((*button_state) == 0x00)
    {
        return 0;
    }
    else
    {
        return (0x80 & (*button_state)) ? 1 : 0;
    }
}

uint8_t activeUser = 0;
uint8_t buttonState[6];
uint8_t pressed()
{
    uint8_t buttonPressed_current[6];
    uint8_t result = 0;
    debounce(&buttonState[0], Button1_Get());
    debounce(&buttonState[1], Button2_Get());
    debounce(&buttonState[2], Button3_Get());
    debounce(&buttonState[3], Button4_Get());
    debounce(&buttonState[4], Button5_Get());
    debounce(&buttonState[5], Button6_Get());
    buttonPressed_current[0] = button_pressed(&buttonState[0]);
    buttonPressed_current[1] = button_pressed(&buttonState[1]);
    buttonPressed_current[2] = button_pressed(&buttonState[2]);
    buttonPressed_current[3] = button_pressed(&buttonState[3]);
    buttonPressed_current[4] = button_pressed(&buttonState[4]);
    buttonPressed_current[5] = button_pressed(&buttonState[5]);
    
    if( buttonPressed_current[0] &&  !buttonPressed_last[0])
    {
          result = 1;
    }
    else if(buttonPressed_current[1] &&  !buttonPressed_last[1]){
          result = 2;
    }
    else if(buttonPressed_current[2] &&  !buttonPressed_last[2]){
          result = 3;
    }
    else if(buttonPressed_current[3] &&  !buttonPressed_last[3]){
          result = 4;
    }
    else if(buttonPressed_current[4] &&  !buttonPressed_last[4]){
          result = 5;
    }
    else if(buttonPressed_current[5] &&  !buttonPressed_last[5]){
          result = 6;
    }
    buttonPressed_last[0] = buttonPressed_current[0];
    buttonPressed_last[1] = buttonPressed_current[1];
    buttonPressed_last[2] = buttonPressed_current[2];
    buttonPressed_last[3] = buttonPressed_current[3];
    buttonPressed_last[4] = buttonPressed_current[4];
    buttonPressed_last[5] = buttonPressed_current[5];
    return result;
}
void loop()
{
  uint8_t button = pressed();
  if(button)
  {
        if(userIn(user[activeUser][0],user[activeUser][1],user[activeUser][2], activeUser,button)){
          activeUser = !activeUser;
        }
        showStrip();

  }
}
void setupArray(char secondaryArr[36]){
  for(int i = 0; i < 36; i++){
    secondaryArr[i] = 'n';
  }
}



bool userIn(uint8_t r, uint8_t g, uint8_t b, char userChar, uint8_t button){

    bool used = true;
    if( button==1  && count0<6){
        secondaryArr[count0] = userChar;
        setLEDColor(count0++, r, g, b);
        gameCount++;
        checkWin(count0-1,userChar);
    }
    else if( button==2  && count1<12){
        setLEDColor(colCount1--, r, g, b);
        //insert into win condition array
        secondaryArr[count1] = userChar;
        count1++;
        gameCount++;
        checkWin(count1-1,userChar);
    }
    else if( button==3 && count2<18){
        secondaryArr[count2] = userChar;
        setLEDColor(count2++, r, g, b);
        gameCount++;
        checkWin(count2-1,userChar);
        
    }
 
    else if( button==4  && count3<24){
        setLEDColor(colCount3--, r, g, b);
        //insert into win condition array
        secondaryArr[count3] = userChar;
        count3++;
        gameCount++;
        checkWin(count3-1,userChar);
    }

    else if( button==5  && count4<30){
        secondaryArr[count4] = userChar;
        setLEDColor(count4++, r, g, b);//setLEDColor(0,r,g,b);
        gameCount++;
        checkWin(count4-1,userChar);
    }
    else if( button==6  && count5<36){
        setLEDColor(colCount5--, r, g, b);//setLEDColor(0,r,g,b);
        //insert into win condition array
        secondaryArr[count5] = userChar;
        count5++;
        gameCount++;
        checkWin(count5-1,userChar);
    }
    else{
      used = false;
    }
    return used;
}
int winFlag=0;
void checkWin (const int startIdx, char user){
    if (checkDown(startIdx, user) || checkRDown (startIdx, user) || checkRight(startIdx, user) || checkRUp (startIdx, user)){
      winFlag = 1;
      gameOver();
    }
    if(gameCount>=36){
      showStrip();
      gameOver();
    }
    
}

void gameOver(){
  int r,g,b;
  showStrip();
  r = user[activeUser][0];
  g = user[activeUser][1];
  b = user[activeUser][2];
  if(gameCount>=36&&!winFlag){
    r=0;
    g=0;
    b=64;
  }
  delay(3000);
  for (int i = 0; i < 36; i++) {
    setLEDColor(i, r, g, b);//setLEDColor(0,r,g,b);
  }
  showStrip();
  delay(3000);
    for (int i = 0; i < 36; i++) {
    setLEDColor(i, 0, 0, 0);//setLEDColor(0,r,g,b);
  }
  count0 = 0;

  count1 = 6;
  colCount1 = 11;

  count2 = 12;

  count3 = 18;
  colCount3 = 23;

  count4 = 24;

  count5 = 30;
  colCount5 = 35;
  activeUser = 0;

  count = 0;
  gameCount = 0;
  user1Char = 0;
  user2Char = 1;
  BSP_Init();
  user[0][0] = 64;
  user[0][1] = 0;
  user[0][2] = 0;
  user[1][0] = 0;
  user[1][1] = 64;
  user[1][2] = 0;
  setupArray(secondaryArr);
  winFlag = 0;
}

bool checkDown (const int startIdx, char user){
  for (int i = 1; i < 4; i++){
    if(((startIdx - i) % 6 == 0) && i < 3 || (startIdx- i) % 6 == 5){
      return 0;
    }
    if(secondaryArr[startIdx - i] != user){
      return 0;
    }
  }
  return 1;
}

bool checkRDown (const int startIdx, char user){
  for (int i = 1; i < 4; i++){
    if((startIdx + i*5) >= 36 || (startIdx + i*5) % 6 == 5 ){
        return checkLUp(startIdx + (i-1)*5,user);
    }
    if(secondaryArr[startIdx + i*5] != user){
      return checkLUp(startIdx + (i-1)*5, user);
    }
  }
  return 1;
}

bool checkRight (const int startIdx, char user){
  for (int i = 1; i < 4; i++){
    if((startIdx + i*6) >= 36){
      return checkLeft(startIdx + (i-1)*6, user);
    }
    if(secondaryArr[startIdx + i*6] != user){
      return checkLeft(startIdx + (i-1)*6, user);
    }
  }
  return 1;
}

bool checkRUp (const int startIdx, char user){
  for (int i = 1; i < 4; i++){
    if((startIdx + i*7) % 6 == 0 || (startIdx + i*7) >= 37){
        return checkLDown(startIdx + (i-1)*7, user);
    }
    if(secondaryArr[startIdx + i*7] != user){
      return checkLDown(startIdx + (i-1)*7, user);
    }
  }
  return 1;
}

bool checkLUp (const int startIdx, char user){
   for (int i = 1; i < 4; i++){
    if(((startIdx - i*5) % 6 == 0 || (startIdx - i*5) <= 0)){
      return 0;
    }
    if(secondaryArr[startIdx - i*5] != user){
      return 0;
    }
  }
  return 1;
}

bool checkLeft (const int startIdx, char user){
  for (int i = 1; i < 4; i++){
    if((startIdx - i*6) <= -1){
      return 0;
    }
    if(secondaryArr[startIdx - i*6] != user){
      return 0;
    }
  }
  return 1;
}

bool checkLDown (const int startIdx, char user){
  for (int i = 1; i < 4; i++){
    if((startIdx - i*7) % 6 == 5 || (startIdx - i*7) <= -1){
      return 0;
    }
    if(secondaryArr[startIdx - i*7] != user){
      return 0;
    }
  }
  return 1;
}
