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

const int SWITCH_ZERO = 2;
const int SWITCH_ONE = 3;
const int SWITCH_TWO = 4;
const int SWITCH_THREE = 5;

const int LED_R = 14;
const int LED_G = 15;
const int LED_B = 16;

const int RELAY_ZERO = 17;
const int RELAY_ONE = 18;
const int RELAY_TWO = 19;

int on = 1;
int off = 0;

const int switchcount = 3;
const int ledcount = 3;
const int bankcount = 3;

int numSwitches = 4;
int numRelays = 3;

int bankTime = 2000;

int switchPins[] = {SWITCH_ZERO,SWITCH_ONE,SWITCH_TWO,SWITCH_THREE};
int ledPins[] = {LED_R,LED_G,LED_B};
int relayPins[] = {RELAY_ZERO,RELAY_ONE,RELAY_TWO};

int switchStates[] = {0,0,0,0};
int relayStates[] = {0,0,0}; 

int loop_mode = 0;
const int mode0 = 0;
const int mode1 = 1;
const int mode2 = 2;
 
const int change_switch = 4;

int indicator = switchcount * switchcount;

bool presetBanks[switchcount][switchcount];

bool activeBanks[] = {false,false,false};

//////////////////////////////////////////////////////////
// SETUP, LOOP
//////////////////////////////////////////////////////////

void setup() {
  for(int i = 0; i < numSwitches; i++){
    if(i < numSwitches){
      pinMode(switchPins[i],INPUT_PULLUP);
      digitalWrite(switchStates[i], HIGH);  
    }
    if(i < ledcount){
      pinMode(ledPins[i],OUTPUT);
      digitalWrite(ledPins[i],LOW);
    }
    if(i < numRelays){
      pinMode(relayPins[i],INPUT_PULLUP);
      digitalWrite(relayPins[i], LOW);
    }
  }
  digitalWrite(ledPins[0],HIGH);
  Serial.begin(9600);
  check_eep();
}

void loop() {
  checkSwitchStates();
  for(int i = 0; i < numSwitches; i++){
    if(switchStates[i] == LOW){
      if(i == change_switch){
        switchMode();
      }else if(loop_mode == mode1){ 
        mode_one_change(i);
      }else if(loop_mode == mode2){
        mode_two_change(i);
      }else if(relayStates[i] == 0){
        mode_zero_change(i);
      }else{
        mode_zero_off(i);
      }
      i = numSwitches;
    }
  }
  makeSwitchHigh();
  if(loop_mode == mode1){
    delay(200);
  }
}

//////////////////////////////////////////////////////////
// MODE ZERO
//////////////////////////////////////////////////////////

void mode_zero_change(int relay){
  for(int i = 0; i < switchcount; i++){
    if(i == relay){
      relayStates[i] = on;
      digitalWrite(relayPins[i], HIGH);
    }else{
      relayStates[i] = off;
      digitalWrite(relayPins[i], LOW);
    }
  }
  Serial.println("M0: R" + String(relay) + " On");
  while(digitalRead(switchPins[relay]) == LOW){
    delay(1);
  }
  delay(250);
}

void mode_zero_off(int i){
  for(int j = 0; j < switchcount; j++){
    digitalWrite(relayPins[j],LOW);
    relayStates[j] = off;
  }
  Serial.println("M0: Relays Off");
  while(digitalRead(switchPins[i]) == LOW){
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
  while(digitalRead(switchPins[index]) == LOW && keepLoop){
    delay(1);
    counter++;
    if(counter >= bankTime){
      setPresetBank(index);
      keepLoop = false;
      bankPress = true;
    }
  }
  keepLoop = false;
  if(!bankPress){
    if(relayStates[index] == 0){
      digitalWrite(relayPins[index], HIGH);
      relayStates[index] = on;
      Serial.println("M1: R" + String(index) + " On");
    }else{
      digitalWrite(relayPins[index], LOW);
      relayStates[index] = off;
      Serial.println("M1: R" + String(index) + " Off");
    }
  }
  while(digitalRead(switchPins[index]) == LOW){
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
  while(digitalRead(switchPins[i]) == LOW){
    delay(1);
  }
  delay(250);
}

void updateBanks(int bankNo){
  for(int i = 0; i < bankcount; i++){
    if(i == bankNo){
      activeBanks[i] = true;  
    }else{
      activeBanks[i] = false;  
    }  
  }
}

void turnBankOn(int bankNo){
  for(int i = 0; i < bankcount; i++){
    if(presetBanks[bankNo][i]){
      digitalWrite(relayPins[i], HIGH);  
    }else{
      digitalWrite(relayPins[i], LOW);   
    }
  }
}

//////////////////////////////////////////////////////////
// HELPER METHODS
//////////////////////////////////////////////////////////

void checkSwitchStates(){
  for(int i = 0; i < numSwitches; i++){
    switchStates[i] = digitalRead(switchPins[i]);
  }
}

//redo
void switchMode(){
  for(int i = 0; i < switchcount; i++){
    digitalWrite(relayPins[i], LOW);
    relayStates[i] = off;  
  }
  if(loop_mode == mode0){
    digitalWrite(ledPins[0],LOW);
    digitalWrite(ledPins[1], HIGH);
    loop_mode = mode1;
    Serial.println("Mode 0 to Mode 1");
  }else if(loop_mode == mode1){
    digitalWrite(ledPins[1],LOW);
    digitalWrite(ledPins[2], HIGH);
    loop_mode = mode2;
    Serial.println("Mode 1 to Mode 2");
  }else if(loop_mode == mode2){
    digitalWrite(ledPins[2], LOW);
    digitalWrite(ledPins[0], HIGH);
    loop_mode = mode0;
    Serial.println("Mode 2 to Mode 0");
  }
  while(digitalRead(switchPins[change_switch]) == LOW){
    delay(1);  
  }
  delay(250);
}

void makeSwitchHigh(){
  for(int i = 0; i < numSwitches; i++){
    digitalWrite(switchStates[i], HIGH);
  }
}

void deactivateRelays(){
  for(int i = 0; i < numRelays; i++){
    digitalWrite(relayPins[i], LOW);
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
        digitalWrite(ledPins[0],LOW);
        digitalWrite(ledPins[1], HIGH);
        break;
      case 1:
        digitalWrite(ledPins[1],LOW);
        digitalWrite(ledPins[2], HIGH);
        break;
      case 2:
        digitalWrite(ledPins[2],LOW);
        digitalWrite(ledPins[0], HIGH);
    }
    twirl_count++;  
  }
  digitalWrite(ledPins[0],LOW);
  digitalWrite(ledPins[1], HIGH);
  digitalWrite(ledPins[2],LOW);
}

//////////////////////////////////////////////////////////
// EEPROM METHODS
//////////////////////////////////////////////////////////

void check_eep(){
  
  if(bitRead(EEPROM.read(indicator),0)==1){
    int eIndex = 0;
    for(int i = 0; i < switchcount; i++){
      for(int j = 0; j < switchcount; j++){
        presetBanks[i][j] = bitRead(EEPROM.read(eIndex),0)==1;
        eIndex++;
      }
    }
    for(int i = 0; i < indicator+1; i++){
      Serial.print(bitRead(EEPROM.read(i),0));  
    }
    Serial.println("\nRetreived EEPROM Storage");
  }else{
    Serial.println("No EEPROM Memory"); 
  }
}

void setPresetBank(int bankNo){
  for(int i = 0; i < switchcount; i++){
    presetBanks[bankNo][i] = relayStates[i] == on;
  }
  int j = 0;
  for(int i = (switchcount*bankNo); i < (switchcount*bankNo)+switchcount; i++){
    EEPROM.write(i,presetBanks[bankNo][j]);
    j++;
  }
  EEPROM.write(indicator,1);
  twirlColors();
  Serial.println("EEPROM FUNC: Bank " + String(bankNo) + " Written");
}
