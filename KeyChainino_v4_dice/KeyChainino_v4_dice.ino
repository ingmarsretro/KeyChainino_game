/****************************************************************************
this is just a simple electronic dice programm 
it displays randomly the number 1 to 6 by pressing both buttons at the same time
  Created by: ingmarsretro  
  
  Hardware:
   - KeyChainino 144 (12×12 matrix, ATmega328p)
   - Two push buttons:
       Button A on pin 3
       Button B on pin 2
  Libraries:
   - KeyChainino.cpp, KeyChainino.h
****************************************************************************/

#include <KeyChainino.h>

// Constants
#define MATRIX_COL 12
#define MATRIX_ROW 12

// Dice display settings
#define ROLL_ITERATIONS 10        // Number of dice rolls in animation
#define ROLL_DELAY 100            // Delay between dice rolls in ms
#define RESULT_DISPLAY_TIME 3000  // Display result for 3 seconds

// Timer1 reload value
unsigned long timer1_count_;

// dice patterns for numbers 1 to 6 represented as 12x12 bitmaps
const byte dicePattern1[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Row 1
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Row 2
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Row 3
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Row 4
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Row 5
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,  // Row 6
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,  // Row 7
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Row 8
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Row 9
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Row 10
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  // Row 11
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   // Row 12
};

const byte dicePattern2[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   
};

const byte dicePattern3[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0  
};

const byte dicePattern4[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   
};

const byte dicePattern5[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 1, 1, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   
};

const byte dicePattern6[] = {
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0, 
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 1, 1, 0, 0, 0, 0, 1, 1, 0, 0,  
  0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0   
};


// Global Variables
bool diceRolling = false;
bool displayingResult = false;
unsigned long resultDisplayStart = 0;
byte finalDiceValue = 0;

// Interrupt Service Routines

ISR(TIMER1_OVF_vect) {
  TIMSK1 &= ~(1 << TOIE1);  // disable Timer1 interrupts
  
  // Check if displaying a result and need to go to sleep
  if (displayingResult && (millis() - resultDisplayStart > RESULT_DISPLAY_TIME)) {
    displayingResult = false;
    KC.goSleep();  //sleep after displaying result
  }
  
  TCNT1 = timer1_count_;   // reload Timer1 counter
  TIMSK1 |= (1 << TOIE1);  // re-enable Timer1 interrupt
}

ISR(INT1_vect) {  // BUTTON A interrupt
  if (!diceRolling && !displayingResult) {
    diceRolling = true;
  }
}

ISR(INT0_vect) {  // BUTTON B interrupt
  if (!diceRolling && !displayingResult) {
    diceRolling = true;
  }
}

// Functions

// Display a the dice pattern based on the value (1-6)
void displayDicePattern(byte value) {
  KC.clear();
  
  switch (value) {
    case 1:
      KC.drawBitmap(dicePattern1);
      break;
    case 2:
      KC.drawBitmap(dicePattern2);
      break;
    case 3:
      KC.drawBitmap(dicePattern3);
      break;
    case 4:
      KC.drawBitmap(dicePattern4);
      break;
    case 5:
      KC.drawBitmap(dicePattern5);
      break;
    case 6:
      KC.drawBitmap(dicePattern6);
      break;
    default:
      break;
  }
  
  KC.display();
}

// dice roll animation
void rollDice() {
  // Animation: show rapidly changing dice values
  for (byte i = 0; i < ROLL_ITERATIONS; i++) {
    byte randomValue = random(1, 7);  // Generate random value between 1-6
    displayDicePattern(randomValue);
    delay(ROLL_DELAY);
  }
  
  // Generate final dice value
  finalDiceValue = random(1, 7);
  displayDicePattern(finalDiceValue);
  
  // Set flags for result display
  diceRolling = false;
  displayingResult = true;
  resultDisplayStart = millis();
}

//Setup and Loop
void setup() {
  randomSeed(analogRead(A0));  // Initialize random seed
  
  KC.init();
  KC.clear();
  KC.display();
  
  pinMode(BUTTON_A, INPUT_PULLUP);
  pinMode(BUTTON_B, INPUT_PULLUP);
  
  // Setup Timer1 for interrupt
  timer1_count_ = 64900;
  noInterrupts();  // disable interrupts for timer setup
  TCCR1A = 0;
  TCCR1B = 0;
  TCNT1 = timer1_count_;
  TCCR1B |= (1 << CS10);   // no prescaler
  TIMSK1 |= (1 << TOIE1);  // enable Timer1 overflow interrupt
  
  // Configure button interrupts
  EICRA |= (1 << ISC01) | (1 << ISC11);  // Falling edge triggers INT0 and INT1
  EIMSK |= (1 << INT0) | (1 << INT1);    // Enable INT0 and INT1 interrupts
  
  interrupts();  // re-enable interrupts
  
  KC.goSleep();  // sleep until button press
}

void loop() {
  //if dice should be rolled, perform the roll animation
  if (diceRolling) {
    rollDice();
  }
}