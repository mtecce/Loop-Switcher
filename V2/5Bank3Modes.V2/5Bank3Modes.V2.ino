/*
Footswitch 0 to D2
Footswitch 1 to D3
Footswitch 2 to D4
Footswitch 3 to D5

LED R to D14
LED G to D15
LED B to D16

Relay 0 to D17
Relay 1 to D18
Relay 2 to D19

Relay: Panasonic TQ2-5V
Transistor: 2N3904
Enclosure: Hammond 1590BX2
5V Regulator: JRC 78M05A
*/

#include <EEPROM.h>

//pin numbers
const int SWITCHES[] = {2,3,4,5,6,7};
const int LEDS[] = {14,15,16};
const int RELAYS[] = {17,18,19,20,21};

//states of relays
const int ON = 1;
const int OFF = 0;

//loop modes
const int MODE_ZERO = 0;
const int MODE_ONE = 1;
const int MODE_TWO = 2;

/*
Pin quantity values:
  BANK_SWITCH_COUNT : used for cycling through preset banks
  TOTAL_SWITCH_COUNT : used for getting and setting switch states
  RELAY_COUNT : used in setup of relay pins
  LED_COUNT : used in LED pin state changes
*/
const int BANK_SWITCH_COUNT = 5;
const int TOTAL_SWITCH_COUNT = 6;
const int RELAY_COUNT = 5;
const int LED_COUNT = 3;

//how long someone has to hold down a switch to save a bank
const int BANK_PRESS_TIME = 2000;

//index of mode change switch
const int CHANGE_SWITCH = 5;

//EEPROM memory indicator index
const int INDICATOR = BANK_SWITCH_COUNT * BANK_SWITCH_COUNT;

/*
Current State Arrays:
  switchStates : current state of the switch pins
  relayStates : current state of the relay pins
*/
int switchStates[] = {0,0,0,0,0,0};
int relayStates[] = {0,0,0,0,0}; 

//current loop mode
int currentLoopMode = 0;

//storage of the preset banks (obtained from EEPROM)
bool presetBanks[BANK_SWITCH_COUNT][BANK_SWITCH_COUNT];

//current active bank array
bool activeBanks[] = {false,false,false,false,false};

//////////////////////////////////////////////////////////
// SETUP, LOOP
//////////////////////////////////////////////////////////

void setup() {
  for(int i = 0; i < TOTAL_SWITCH_COUNT; i++){
    if(i < TOTAL_SWITCH_COUNT){
      pinMode(SWITCHES[i],INPUT_PULLUP);
      digitalWrite(switchStates[i], HIGH);  
    }
    if(i < LED_COUNT){
      pinMode(LEDS[i],OUTPUT);
      digitalWrite(LEDS[i],LOW);
    }
    if(i < RELAY_COUNT){
      pinMode(RELAYS[i],INPUT_PULLUP);
      digitalWrite(RELAYS[i], LOW);
    }
  }
  digitalWrite(LEDS[0],HIGH);
  Serial.begin(9600);
  check_eep();
}

void loop() {
  checkSwitchStates();
  for(int i = 0; i < TOTAL_SWITCH_COUNT; i++){
    if(switchStates[i] == LOW){
      if(i == CHANGE_SWITCH){
        switchMode();
      }else if(currentLoopMode == MODE_ONE){ 
        mode_one_change(i);
      }else if(currentLoopMode == MODE_TWO){
        mode_two_change(i);
      }else if(relayStates[i] == 0){
        mode_zero_change(i);
      }else{
        mode_zero_off(i);
      }
      i = TOTAL_SWITCH_COUNT;
    }
  }
  makeSwitchHigh();
  if(currentLoopMode == MODE_ONE){
    delay(200);
  }
}

//////////////////////////////////////////////////////////
// MODE ZERO
//////////////////////////////////////////////////////////

void mode_zero_change(int relay){
  for(int i = 0; i < BANK_SWITCH_COUNT; i++){
    if(i == relay){
      relayStates[i] = ON;
      digitalWrite(RELAYS[i], HIGH);
    }else{
      relayStates[i] = OFF;
      digitalWrite(RELAYS[i], LOW);
    }
  }
  Serial.println("M0: R" + String(relay) + " On");
  while(digitalRead(SWITCHES[relay]) == LOW){
    delay(1);
  }
  delay(250);
}

void mode_zero_off(int i){
  for(int j = 0; j < BANK_SWITCH_COUNT; j++){
    digitalWrite(RELAYS[j],LOW);
    relayStates[j] = OFF;
  }
  Serial.println("M0: Relays Off");
  while(digitalRead(SWITCHES[i]) == LOW){
      delay(1);
  }
  delay(250);
}
//////////////////////////////////////////////////////////
// MODE ONE
//////////////////////////////////////////////////////////

void mode_one_change(int index){
  Serial.println(relayStates[index]);
  int counter = 0;
  bool keepLoop = true;
  bool bankPress = false;
  while(digitalRead(SWITCHES[index]) == LOW && keepLoop){
    delay(1);
    counter++;
    if(counter >= BANK_PRESS_TIME){
      setPresetBank(index);
      keepLoop = false;
      bankPress = true;
    }
  }
  keepLoop = false;
  if(!bankPress){
    if(relayStates[index] == 0){
      digitalWrite(RELAYS[index], HIGH);
      relayStates[index] = ON;
      Serial.println("M1: R" + String(index) + " On");
    }else{
      digitalWrite(RELAYS[index], LOW);
      relayStates[index] = OFF;
      Serial.println("M1: R" + String(index) + " Off");
    }
  }
  while(digitalRead(SWITCHES[index]) == LOW){
    delay(1);
  }
  bankPress = false;
  delay(250);
}

//////////////////////////////////////////////////////////
// MODE TWO
//////////////////////////////////////////////////////////

void mode_two_change(int i){
  if(!activeBanks[i]){
    updateBanks(i);
    turnBankOn(i);
    Serial.println("M2: Bank " + String(i) + " On");
  }else{
    deactivateRelays();
    activeBanks[i] = false;
    Serial.println("M2: Banks Off");
  }
  while(digitalRead(SWITCHES[i]) == LOW){
    delay(1);
  }
  delay(250);
}

void updateBanks(int bankNo){
  for(int i = 0; i < BANK_SWITCH_COUNT; i++){
    if(i == bankNo){
      activeBanks[i] = true;  
    }else{
      activeBanks[i] = false;  
    }  
  }
}

void turnBankOn(int bankNo){
  for(int i = 0; i < BANK_SWITCH_COUNT; i++){
    if(presetBanks[bankNo][i]){
      digitalWrite(RELAYS[i], HIGH);  
    }else{
      digitalWrite(RELAYS[i], LOW);   
    }
  }
}

//////////////////////////////////////////////////////////
// HELPER METHODS
//////////////////////////////////////////////////////////

void checkSwitchStates(){
  for(int i = 0; i < TOTAL_SWITCH_COUNT; i++){
    switchStates[i] = digitalRead(SWITCHES[i]);
  }
}

//redo
void switchMode(){
  for(int i = 0; i < BANK_SWITCH_COUNT; i++){
    digitalWrite(RELAYS[i], LOW);
    relayStates[i] = OFF;  
  }
  if(currentLoopMode == MODE_ZERO){
    digitalWrite(LEDS[0],LOW);
    digitalWrite(LEDS[1], HIGH);
    currentLoopMode = MODE_ONE;
    Serial.println("Mode 0 to Mode 1");
  }else if(currentLoopMode == MODE_ONE){
    digitalWrite(LEDS[1],LOW);
    digitalWrite(LEDS[2], HIGH);
    currentLoopMode = MODE_TWO;
    Serial.println("Mode 1 to Mode 2");
  }else if(currentLoopMode == MODE_TWO){
    digitalWrite(LEDS[2], LOW);
    digitalWrite(LEDS[0], HIGH);
    currentLoopMode = MODE_ZERO;
    Serial.println("Mode 2 to Mode 0");
  }
  while(digitalRead(SWITCHES[CHANGE_SWITCH]) == LOW){
    delay(1);  
  }
  delay(250);
}

void makeSwitchHigh(){
  for(int i = 0; i < TOTAL_SWITCH_COUNT; i++){
    digitalWrite(switchStates[i], HIGH);
  }
}

void deactivateRelays(){
  for(int i = 0; i < RELAY_COUNT; i++){
    digitalWrite(RELAYS[i], LOW);
  }
}


/*
  Switch when board change to pin powered led is implemented
*/
void twirlColors(){
  int twirl_count = 1;
  int notecount = 0;
  int delayarr[] = {500,500,100,100,100,100,100,100,100};
  while(twirl_count < 8){
    int mod = twirl_count%3;
    delay(delayarr[notecount]);
    notecount++;
    switch(mod){
      case 0:
        digitalWrite(LEDS[0],LOW);
        digitalWrite(LEDS[1], HIGH);
        break;
      case 1:
        digitalWrite(LEDS[1],LOW);
        digitalWrite(LEDS[2], HIGH);
        break;
      case 2:
        digitalWrite(LEDS[2],LOW);
        digitalWrite(LEDS[0], HIGH);
    }
    twirl_count++;  
  }
  digitalWrite(LEDS[0],LOW);
  digitalWrite(LEDS[1], HIGH);
  digitalWrite(LEDS[2],LOW);
}

//////////////////////////////////////////////////////////
// EEPROM METHODS
//////////////////////////////////////////////////////////

void check_eep(){
  
  if(bitRead(EEPROM.read(INDICATOR),0)==1){
    int eIndex = 0;
    for(int i = 0; i < BANK_SWITCH_COUNT; i++){
      for(int j = 0; j < BANK_SWITCH_COUNT; j++){
        presetBanks[i][j] = bitRead(EEPROM.read(eIndex),0)==1;
        eIndex++;
      }
    }
    for(int i = 0; i < INDICATOR+1; i++){
      Serial.print(bitRead(EEPROM.read(i),0));  
    }
    Serial.println("\nRetreived EEPROM Storage");
  }else{
    Serial.println("No EEPROM Memory"); 
  }
}

void setPresetBank(int bankNo){
  for(int i = 0; i < BANK_SWITCH_COUNT; i++){
    presetBanks[bankNo][i] = relayStates[i] == ON;
  }
  int j = 0;
  for(int i = (BANK_SWITCH_COUNT*bankNo); i < (BANK_SWITCH_COUNT*bankNo)+BANK_SWITCH_COUNT; i++){
    EEPROM.write(i,presetBanks[bankNo][j]);
    j++;
  }
  EEPROM.write(INDICATOR,1);
  twirlColors();
  Serial.println("EEPROM FUNC: Bank " + String(bankNo) + " Written");
}
