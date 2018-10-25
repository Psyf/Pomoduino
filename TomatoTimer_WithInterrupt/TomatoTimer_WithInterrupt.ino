#define SESSION_TIME_PIN A3
#define BREAK_TIME_PIN A4
#define NUM_SESSIONS_PIN A5
#define START_BUTTON_PIN A2
#define PIEZO_PIN 2

int SESSION_LEDS[] = {5, 6, 9, 10}; 
int BREAK_LEDS[] = {4, 7, 8, 12};
int over = 0; 
int pause; 
    
void setup() {
    pinMode(SESSION_TIME_PIN, INPUT);
    pinMode(BREAK_TIME_PIN, INPUT); 
    pinMode(NUM_SESSIONS_PIN, INPUT);
    attachInterrupt(digitalPinToInterrupt(START_BUTTON_PIN), pause_ISR, HIGH); 
    Serial.begin(9600); 
    
    // INPUT for LEDs
    for (int i=0; i<4; i++) {
        pinMode(SESSION_LEDS[i], OUTPUT);
        pinMode(BREAK_LEDS[i], OUTPUT);
    } 
}

void loop() {
    pause = digitalRead(START_BUTTON_PIN); 
    
    while (pause) {
        pause = digitalRead(START_BUTTON_PIN);   
    }
    
    while (!over) {
        int session_time = getSessTime(SESSION_TIME_PIN); 
        int break_time = getBreakTime(BREAK_TIME_PIN); 
        int num_sessions = getNumSess(NUM_SESSIONS_PIN); 
        Serial.println(session_time); 
        Serial.println(break_time);
        Serial.println(num_sessions); 

        for (int i=0; i<num_sessions; i++) {
            fadeAndHold(SESSION_LEDS[i], session_time);
            tone(PIEZO_PIN, 2800, 1000); 
            hold(BREAK_LEDS[i], break_time); 
            tone(PIEZO_PIN, 3000, 1000);  
        }
        over = 1; 
        tone(PIEZO_PIN, 2800, 3000); 
        

    }
}

// Any output pin can use this
// duration in minutes
void hold(int led, int duration) {
    unsigned long targetTime = millis() + duration*60*1000; 
    digitalWrite(led, HIGH);

    while (millis() < targetTime) continue; 
}

// Only pins with PWM can be used here
// duration in minutes
void fadeAndHold(int led, int duration) { 
     
    unsigned long targetTime = millis() + duration*60*1000; 
    // playHarryPotter(); 
    while (millis() < targetTime) {
        for (int i=0; i<255; i++) {
            analogWrite(led, i); 
            unsigned long curTime = millis(); 
            while (millis() - curTime < 5) continue; 
        }
        for (int i=255; i>0; i--) {
            analogWrite(led, i); 
            unsigned long curTime = millis(); 
            while (millis() - curTime < 5) continue; 
        }
    }
    
    digitalWrite(led, HIGH); 
}

// returns integer from 25 to 40
int getSessTime(int pin) {
    int value = analogRead(pin);
    int output; 
    if (value <= 250) return 25; 
    else if (value > 250 && value <= 500) output = 30;
    else if (value > 500 && value <= 750) output = 35; 
    else if (value > 750 && value <= 1000) output = 40; 
    else output = 45;

    return output;   
}

// returns integer from 5 to 15
int getBreakTime(int pin) {
    int value = analogRead(pin); 
    int output; 
    if (value <= 250) return 5; 
    else if (value > 250 && value <= 500) output = 10;
    else if (value > 500 && value <= 750) output = 15; 
    else if (value > 750 && value <= 1000) output = 20; 
    else output = 25;
    
    return output; 
}

// returns integer from 1 to 4
int getNumSess(int pin) {
    int value = analogRead(pin);
    int output; 
    if (value <= 250) return 1; 
    else if (value > 250 && value <= 500) output = 2;
    else if (value > 500 && value <= 750) output = 3; 
    else output = 4;
    
    return output; 
}

void pause_ISR() {
    pause = 1;  //only called when RISES
    pauseExec(); 
}

void pauseExec() {
    while (pause) {
        pause = digitalRead(START_BUTTON_PIN);   
    }   
}
