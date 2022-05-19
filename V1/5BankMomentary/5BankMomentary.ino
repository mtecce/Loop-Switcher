const int switch0 = 2;
const int switch1 = 3;
const int switch2 = 4;
const int switch3 = 5;
const int switch4 = 6;
const int relay0 = 7;
const int relay1 = 8;
const int relay2 = 9;
const int relay3 = 10;
const int relay4 = 11;

int on = 1;
int off = 0;

int switchstate0 = 0;
int switchstate1 = 0;
int switchstate2 = 0;
int switchstate3 = 0;
int switchstate4 = 0;
int relaystate0 = 0;
int relaystate1 = 0;
int relaystate2 = 0;
int relaystate3 = 0;
int relaystate4 = 0;

void setup() {
  pinMode(switch0, INPUT_PULLUP);
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(switch3, INPUT_PULLUP);
  pinMode(switch4, INPUT_PULLUP);
  pinMode(relay0, INPUT_PULLUP);
  pinMode(relay1, INPUT_PULLUP);
  pinMode(relay2, INPUT_PULLUP);
  pinMode(relay3, INPUT_PULLUP);
  pinMode(relay4, INPUT_PULLUP);

  digitalWrite(relay0, LOW);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  digitalWrite(switchstate0, HIGH);
  digitalWrite(switchstate1, HIGH);
  digitalWrite(switchstate2, HIGH);
  digitalWrite(switchstate3, HIGH);
  digitalWrite(switchstate4, HIGH);

  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  switchstate0 = digitalRead(switch0);
  switchstate1 = digitalRead(switch1);
  switchstate2 = digitalRead(switch2);
  switchstate3 = digitalRead(switch3);
  switchstate4 = digitalRead(switch4);

  if(switchstate0 == LOW && relaystate0 == 0){
    activateRelay(relay0, relay1, relay2, relay3, relay4);
    updateRelayState(on, off, off, off, off);
    Serial.println("Relay 0 On");
    while(digitalRead(switch0) == LOW){
      delay(1);
      }
    delay(250);
  }else if(switchstate0 == LOW && relaystate0 == 1){
    deactivateRelays();
    updateRelayState(off, off, off, off, off);
    Serial.println("Relays Off");
    while(digitalRead(switch0) == LOW){
      delay(1);
      }
    delay(250);
  }else if(switchstate1 == LOW && relaystate1 == 0){
    activateRelay(relay1, relay0, relay2, relay3, relay4);
    updateRelayState(off, on, off, off, off);
    Serial.println("Relay 1 On");
    while(digitalRead(switch1) == LOW){
      delay(1);
      }
    delay(250); 
  }else if(switchstate1 == LOW && relaystate1 == 1){
    deactivateRelays();
    updateRelayState(off, off, off, off, off);
    Serial.println("Relays Off");
    while(digitalRead(switch1) == LOW){
      delay(1);
      }
    delay(250);
  }else if(switchstate2 == LOW && relaystate2 == 0){
    activateRelay(relay2, relay0, relay1, relay3, relay4);
    updateRelayState(off, off, on, off, off);
    Serial.println("Relay 2 On");
    while(digitalRead(switch2) == LOW){
      delay(1);
      }
    delay(250); 
  }else if(switchstate2 == LOW && relaystate2 == 1){
    deactivateRelays();
    updateRelayState(off, off, off, off, off);
    Serial.println("Relays Off");
    while(digitalRead(switch2) == LOW){
      delay(1);
      }
    delay(250);
  }else if(switchstate3 == LOW && relaystate3 == 0){
    activateRelay(relay3, relay0, relay1, relay2, relay4);
    updateRelayState(off, off, off, on, off);
    Serial.println("Relay 3 On");
    while(digitalRead(switch3) == LOW){
      delay(1);
      }
    delay(250); 
  }else if(switchstate3 == LOW && relaystate3 == 1){
    deactivateRelays();
    updateRelayState(off, off, off, off, off);
    Serial.println("Relays Off");
    while(digitalRead(switch3) == LOW){
      delay(1);
      }
    delay(250);
  }else if(switchstate4 == LOW && relaystate4 == 0){
    activateRelay(relay4, relay0, relay1, relay2, relay3);
    updateRelayState(off, off, off, off, on);
    Serial.println("Relay 4 On");
    while(digitalRead(switch4) == LOW){
      delay(1);
      }
    delay(250); 
  }else if(switchstate4 == LOW && relaystate4 == 1){
    deactivateRelays();
    updateRelayState(off, off, off, off, off);
    Serial.println("Relays Off");
    while(digitalRead(switch4) == LOW){
      delay(1);
      }
    delay(250);
  }
  makeSwitchHigh();
}

void makeSwitchHigh(){
  digitalWrite(switchstate0, HIGH);
  digitalWrite(switchstate1, HIGH);
  digitalWrite(switchstate2, HIGH);
  digitalWrite(switchstate3, HIGH);
  digitalWrite(switchstate4, HIGH);
  Serial.println("Switches High");
  }

void activateRelay(int onRelay, int offRelay0, int offRelay1, int offRelay2, int offRelay3){
  digitalWrite(offRelay0, LOW);
  digitalWrite(offRelay1, LOW);
  digitalWrite(offRelay2, LOW);
  digitalWrite(offRelay3, LOW);
  digitalWrite(onRelay, HIGH);
  } 

void updateRelayState(int rS0, int rS1, int rS2, int rS3, int rS4){
  relaystate0 = rS0;
  relaystate1 = rS1;
  relaystate2 = rS2;
  relaystate3 = rS3;
  relaystate4 = rS4;
  }

void deactivateRelays(){
  digitalWrite(relay0, LOW);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  }


















  
