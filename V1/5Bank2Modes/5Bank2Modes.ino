const int switch0 = 3;
const int switch1 = 4;
const int switch2 = 5;
const int switch3 = 6;
const int switch4 = 7;
const int switch5 = 2;

const int relay0 = 15;
const int relay1 = 16;
const int relay2 = 17;
const int relay3 = 18;
const int relay4 = 19;
const int relay5 = 20;
const int relay6 = 21;

int on = 1;
int off = 0;

int switchstate0 = 0;
int switchstate1 = 0;
int switchstate2 = 0;
int switchstate3 = 0;
int switchstate4 = 0;
int switchstate5 = 0;
int relaystate0 = 0;
int relaystate1 = 0;
int relaystate2 = 0;
int relaystate3 = 0;
int relaystate4 = 0;
int relaystate5 = 0;
int relaystate6 = 0;


void setup() {
  pinMode(switch0, INPUT_PULLUP);
  pinMode(switch1, INPUT_PULLUP);
  pinMode(switch2, INPUT_PULLUP);
  pinMode(switch3, INPUT_PULLUP);
  pinMode(switch4, INPUT_PULLUP);
  pinMode(switch5, INPUT_PULLUP);
  pinMode(relay0, INPUT_PULLUP);
  pinMode(relay1, INPUT_PULLUP);
  pinMode(relay2, INPUT_PULLUP);
  pinMode(relay3, INPUT_PULLUP);
  pinMode(relay4, INPUT_PULLUP);
  pinMode(relay5, INPUT_PULLUP);
  pinMode(relay6, INPUT_PULLUP);
  
  digitalWrite(relay0, LOW);
  digitalWrite(relay1, LOW);
  digitalWrite(relay2, LOW);
  digitalWrite(relay3, LOW);
  digitalWrite(relay4, LOW);
  digitalWrite(relay5, LOW);
  digitalWrite(relay6, LOW);
  digitalWrite(switchstate0, HIGH);
  digitalWrite(switchstate1, HIGH);
  digitalWrite(switchstate2, HIGH);
  digitalWrite(switchstate3, HIGH);
  digitalWrite(switchstate4, HIGH);
  digitalWrite(switchstate5, HIGH);

  Serial.begin(9600);
}

void loop() {
  while(relaystate6 == 0){
    switchstate0 = digitalRead(switch0);
    switchstate1 = digitalRead(switch1);
    switchstate2 = digitalRead(switch2);
    switchstate3 = digitalRead(switch3);
    switchstate4 = digitalRead(switch4);
    switchstate5 = digitalRead(switch5);
    
    if(switchstate5 == LOW){
      deactivateRelays();
      updateRelayState(off, off, off, off, off);
      switchMode(relaystate6);
      Serial.println("Switched Mode, Relay 6 On");
      while(digitalRead(switch5) == LOW){
        delay(1);
        }
      delay(250);
    }else if(switchstate0 == LOW && relaystate0 == 0){
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
  while(relaystate6 == 1){
    switchstate0 = digitalRead(switch0);
    switchstate1 = digitalRead(switch1);
    switchstate2 = digitalRead(switch2);
    switchstate3 = digitalRead(switch3);
    switchstate4 = digitalRead(switch4);
    switchstate5 = digitalRead(switch5);

    if(switchstate5 == LOW){
      deactivateRelays();
      updateRelayState(off, off, off, off, off);
      switchMode(relaystate6);
      Serial.println("Switched Mode, Relay 6 Off");
      while(digitalRead(switch5) == LOW){
        delay(1);
        }
      delay(250);
    }else if(switchstate0 == LOW){
      if(relaystate0 == 0){
        digitalWrite(relay0, HIGH);
        relaystate0 = on;
        Serial.println("Relay 0 On");
      }else{
        digitalWrite(relay0, LOW);
        relaystate0 = off;
        Serial.println("Relay 0 Off");
      }
      while(digitalRead(switch0) == LOW){
        delay(1);
      }
      delay(250);
    }else if(switchstate1 == LOW){
      if(relaystate1 == 0){
        digitalWrite(relay1, HIGH);
        relaystate1 = on;
        Serial.println("Relay 1 On");
      }else{
        digitalWrite(relay1, LOW);
        relaystate1 = off;
        Serial.println("Relay 1 Off");
      }
      while(digitalRead(switch1) == LOW){
        delay(1);
      }
      delay(250);
     }else if(switchstate2 == LOW){
      if(relaystate2 == 0){
        digitalWrite(relay2, HIGH);
        relaystate2 = on;
        Serial.println("Relay 2 On");
      }else{
        digitalWrite(relay2, LOW);
        relaystate2 = off;
        Serial.println("Relay 2 Off");
      }
      while(digitalRead(switch2) == LOW){
        delay(1);
      }
      delay(250);
     }else if(switchstate3 == LOW){
      if(relaystate3 == 0){
        digitalWrite(relay3, HIGH);
        relaystate3 = on;
        Serial.println("Relay 3 On");
      }else{
        digitalWrite(relay3, LOW);
        relaystate3 = off;
        Serial.println("Relay 3 Off");
      }
      while(digitalRead(switch3) == LOW){
        delay(1);
      }
      delay(250);
     }else if(switchstate4 == LOW){
      if(relaystate4 == 0){
        digitalWrite(relay4, HIGH);
        relaystate4 = on;
        Serial.println("Relay 4 On");
      }else{
        digitalWrite(relay4, LOW);
        relaystate4 = off;
        Serial.println("Relay 4 Off");
      }
      while(digitalRead(switch4) == LOW){
        delay(1);
      }
      delay(250);
     }
    makeSwitchHigh();  
    }
}

void makeSwitchHigh(){
  digitalWrite(switchstate0, HIGH); 
  digitalWrite(switchstate1, HIGH);
  digitalWrite(switchstate2, HIGH);
  digitalWrite(switchstate3, HIGH);
  digitalWrite(switchstate4, HIGH);
  digitalWrite(switchstate5, HIGH);
  }

void switchMode(int rS){
  if(rS == 0){
    relaystate6 = 1;
    digitalWrite(relay6, HIGH);
    }else if(rS == 1){
      relaystate6 = 0;
      digitalWrite(relay6, LOW);
      }
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
