/*
*   Title: Audio Interface for Digital Radio Modes
*   Author: Sirio Guy - LU3ALT
*   Date: 11/2024
*/

#define DEBUG false   // Enable debug mode (Serial messages for debuging)

#if DEBUG == 1
  #define debug(x) Serial.print(x)
  #define debugln(x) Serial.println(x)
#else
  #define debug(x)
  #define debugln(x)
#endif



// ~~~ DEFINES ~~~

#define AUDIO_IN A7   // Audio IN from USB audio interface

#define SQUELCH_ADJUST A1   // Squelch POT
#define PTT_TIME_ADJUST A0    // PTT time POT

#define PTT 4   // PTT Pin connected to radio
#define PTT_LED 13   // PTT led

#define PREVIA_DELAY 5000 // The delay between transmisions (In MS)



// ~~~ VARIABLES ~~~

int audio_level;
int squelch;
int ptt_time;

bool ptt_on;



// ~~~ SETUP ~~~

void setup(){
  pinMode(AUDIO_IN,        INPUT);
  pinMode(SQUELCH_ADJUST,  INPUT);
  pinMode(PTT_TIME_ADJUST, INPUT);

  pinMode(PTT,       OUTPUT);
  pinMode(PTT_LED,   OUTPUT);

  Serial.begin(9600);

  Serial.print("Radio Interface for Digital Modes");
  debug(" - Debug mode");
  Serial.println();
}



// ~~~ FUNCTIONS ~~~

void readAllInputs(){       // For read all inputs at once
  audio_level   = analogRead(AUDIO_IN);
  squelch       = analogRead(SQUELCH_ADJUST);
  ptt_time      = analogRead(PTT_TIME_ADJUST);
}

void tx(String x){          // Togles transmition on/off
  if(x == "on"){
  digitalWrite(PTT, HIGH);
  digitalWrite(PTT_LED, HIGH);
  }
  
  else if(x == "off"){
    digitalWrite(PTT, LOW);
    digitalWrite(PTT_LED, LOW);
  }
}



// ~~~ LOOP ~~~

void loop(){
  readAllInputs();
  debugln("Listening - PTT Off");

  while(audio_level >= squelch){      // Transmit while rx level pass squelch 
    
    for(int delay;  delay <= ptt_time;  delay++){   // Transmit by a determinated time
      tx("on");
      debugln("Transmitting - PTT On");
    }

    tx("off");      // Stop transmiting
    break;
  }

  debugln("Waiting - PTT Off");
  delay(PREVIA_DELAY);  // Delay between transmisions to desaturate the system
}