/*
Footswitch 0 to D2
Footswitch 1 to D3
Footswitch 2 to D4
Footswitch 3 to D5
Footswitch 4 to D6
Footswitch 5 to D7

Relay 0 to D15
Relay 1 to D16
Relay 2 to D17
Relay 3 to D18
Relay 4 to D19
Relay 5 to D20
Relay 6 to D21

Relay: Panasonic TQ2-5V
Transistor: 2N3904
Enclosure: Hammond 1590BX2
5V Regulator: JRC 78M05A
*/

#include <EEPROM.h>

//////////////////////////////////////////////////////////
// Global Variables
//////////////////////////////////////////////////////////

//pin numbers
const int SWITCHES[] = {3,4,5,6,7,2};
const int RELAYS[] = {15,16,17,18,19,20,21};

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
*/
const int BANK_SWITCH_COUNT = 5;
const int TOTAL_SWITCH_COUNT = 6;
const int RELAY_COUNT = 7;

//how long someone has to hold down a switch to save a bank
const int BANK_PRESS_TIME = 2000;

/*
Current State Arrays:
  switchStates : current state of the switch pins
  relayStates : current state of the relay pins
*/
int switchStates[] = {0,0,0,0,0,0};
int relayStates[] = {0,0,0,0,0,0,0}; 

//storage of the preset banks (obtained from EEPROM)
bool presetBanks[BANK_SWITCH_COUNT][BANK_SWITCH_COUNT];

//current active bank array
bool activeBanks[] = {false,false,false,false,false};

//current loop mode
int currentLoopMode = 0;

//////////////////////////////////////////////////////////
// SETUP, LOOP
//////////////////////////////////////////////////////////

void setup() {
  for(int i = 0; i < RELAY_COUNT; i++){
    if(i < TOTAL_SWITCH_COUNT){
      pinMode(SWITCHES[i],INPUT_PULLUP);
      digitalWrite(switchStates[i], ON);  
    }
    if(i < RELAY_COUNT){
      pinMode(RELAYS[i],INPUT_PULLUP);
      digitalWrite(RELAYS[i], LOW);
    }
  }
  Serial.begin(9600);
  check_eep();
}

void loop() {
  checkSwitchStates();
  for(int i = 0; i < TOTAL_SWITCH_COUNT; i++){
    if(switchStates[i] == LOW){
      if(i == 5){
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
  for(int i = 0; i < 5; i++){
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
  for(int j = 0; j < 5; j++){
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
  for(int i = 0; i < 5; i++){
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

void switchMode(){
  for(int i = 0; i < 5; i++){
    digitalWrite(RELAYS[i], LOW);
    relayStates[i] = OFF;  
  }
  if(currentLoopMode == MODE_ZERO){
    relayStates[6] = 1;
    digitalWrite(RELAYS[6], HIGH);
    currentLoopMode = MODE_ONE;
    Serial.println("Mode 0 to Mode 1");
  }else if(currentLoopMode == MODE_ONE){
    relayStates[5] = 1;
    digitalWrite(RELAYS[5], HIGH);
    currentLoopMode = MODE_TWO;
    Serial.println("Mode 1 to Mode 2");
  }else if(currentLoopMode == MODE_TWO){
    relayStates[5] = 0;
    relayStates[6] = 0;
    digitalWrite(RELAYS[5], LOW);
    digitalWrite(RELAYS[6], LOW);
    currentLoopMode = MODE_ZERO;
    Serial.println("Mode 2 to Mode 0");
  }
  while(digitalRead(SWITCHES[5]) == LOW){
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
  for(int i = 0; i < 5; i++){
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
        digitalWrite(RELAYS[5], LOW);
        digitalWrite(RELAYS[6], LOW);
        break;
      case 1:
        digitalWrite(RELAYS[6], HIGH);
        break;
      case 2:
        digitalWrite(RELAYS[5], HIGH);
    }
    twirl_count++;  
  }
  digitalWrite(RELAYS[5], LOW);
  digitalWrite(RELAYS[6], HIGH);
}

//////////////////////////////////////////////////////////
// EEPROM METHODS
//////////////////////////////////////////////////////////

void check_eep(){
  if(bitRead(EEPROM.read(25),0)==1){
    int eIndex = 0;
    for(int i = 0; i < BANK_SWITCH_COUNT; i++){
      for(int j = 0; j < BANK_SWITCH_COUNT; j++){
        presetBanks[i][j] = bitRead(EEPROM.read(eIndex),0)==1;
        eIndex++;
      }
    }
    for(int i = 0; i < 26; i++){
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
  for(int i = (5*bankNo); i < (5*bankNo)+5; i++){
    EEPROM.write(i,presetBanks[bankNo][j]);
    j++;
  }
  EEPROM.write(25,1);
  twirlColors();
  Serial.println("EEPROM FUNC: Bank " + String(bankNo) + " Written");
}
