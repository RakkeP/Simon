////////////////////////////////////////////////////////////////////////////////////////
// OVERVIEW
////////////////////////////////////////////////////////////////////////////////////////
/*
DESCRIPTION: This code is for implenting my version of the classic Simon memory game.
https://en.wikipedia.org/wiki/Simon_(game)

AUTHOR: Pekka Ratilainen

COMMENTS: This is made for memory practice in hopes to help those with memory issues.
Inspired by Crash Course Arduino and Microcontroller Development by Andre LaMothe:
https://www.udemy.com/course/crash-course-arduino-and-microcontroller-development/

Even though Arduino supports C++11 this is purposefully written in C-style apart 
from using the ready made Arduino methods and classes in order to possible port fully to C 
some day if needed. Full C++ version might be implemented as well.




HISTORY: 

*/


////////////////////////////////////////////////////////////////////////////////////////
// INCLUDES 
////////////////////////////////////////////////////////////////////////////////////////


////////////////////////////////////////////////////////////////////////////////////////
// PROTOTYPES
////////////////////////////////////////////////////////////////////////////////////////

void playSequence(struct soundItem* sequence, uint8_t tonesToPlay);
void waitForButton();
bool checkPlayerResponse(struct soundItem* sequence);
void playMissSound();
void playFanfare();
void generateRandomSequence(uint8_t* sequence);

////////////////////////////////////////////////////////////////////////////////////////
// DEFINES AND MACROS
////////////////////////////////////////////////////////////////////////////////////////

// Debugging prints on/off
#define DEBUGMODE 1

// LEDS
#define LED_BLUE 13
#define LED_GREEN 12
#define LED_RED 11
#define LED_YELLOW 10

// BUTTONS
#define BUTTON_BLUE 7
#define BUTTON_GREEN 6
#define BUTTON_RED 5
#define BUTTON_YELLOW 4

// NOTE LENGTHS
#define NOTE_EIGHT 125
#define NOTE_QUARTER 250
#define NOTE_HALF 500
#define NOTE_LONG 1000

// BUZZER
#define BUZZER 3
// My led order           Original Simon tunes
#define BLUE_TONE 415     //Blue – 415 Hz – G#4 (true pitch 415.305 Hz)
#define GREEN_TONE 310    //Yellow – 310 Hz – D#4 (true pitch 311.127 Hz)
#define RED_TONE 252      //Red – 252 Hz ‐ B3 (true pitch 247.942 Hz)
#define YELLOW_TONE 209   //Green – 209 Hz – G#3 (true pitch 207.652 Hz)

// Calculate array length (C-style)
#define ARRAYLENGTH(ar) (sizeof(ar) / (sizeof(ar[0])))

// Structure which holds the correct led to lit up and note length NOT IN USE
struct soundItem {
    uint8_t ledColor;
    uint16_t noteLength;
};
////////////////////////////////////////////////////////////////////////////////////////
// GLOBALS
////////////////////////////////////////////////////////////////////////////////////////

char sequenceOne[] = { LED_BLUE, LED_GREEN, LED_RED, LED_YELLOW};
char sequenceTwo[] = { LED_BLUE, LED_GREEN, LED_RED, LED_YELLOW,LED_YELLOW,LED_RED,LED_GREEN,LED_BLUE};
struct soundItem randomSequenceEasy[4];
struct soundItem randomSequenceMedium[6];
struct soundItem randomSequenceHard[8];
struct soundItem *sequence;
uint8_t notesToPlay = 1;
uint8_t sequenceSize;
uint8_t arrayLength;
bool generateNewSequence = true;
uint8_t difficulty = 0;
int16_t noteLengths[] = {NOTE_EIGHT, NOTE_QUARTER, NOTE_HALF, NOTE_LONG};

////////////////////////////////////////////////////////////////////////////////////////
// FUNCTIONS
////////////////////////////////////////////////////////////////////////////////////////


///////////////////////////////////////////////////////////////////////////////////////
// SUMMARY 
// Generates random sequence of tunes to provided sequence.
///////////////////////////////////////////////////////////////////////////////////////
// PARAMETERS
// sequence - Sequence to be filled with random tones.
///////////////////////////////////////////////////////////////////////////////////////
// RETURNS
// Nothing.
///////////////////////////////////////////////////////////////////////////////////////
void generateRandomSequence(struct soundItem* sequence, uint8_t arrayLength)
{
  // Pin 0 is not connected to anything and analog read will return a random value.
  // This value is used as a seed for random number generator. 
  randomSeed(analogRead(0));
  long randomLedColor = 0;
  int randomNoteLength = 0;
  
  #if DEBUGMODE
    int16_t analogValue = analogRead(0);
    Serial.println("//// //// //// RANDOM //// //// ////");
    Serial.print("Random analogValue is ");
    Serial.println(analogValue);
  #endif
     
  for(int i = 0; i < arrayLength; i++)
  {
    randomLedColor = random(LED_YELLOW, LED_BLUE+1);
    sequence[i].ledColor = randomLedColor;
    randomNoteLength = random(0, 4);                                                                                                                                               
        
    sequence[i].noteLength = noteLengths[randomNoteLength];
  }

  #if DEBUGMODE
  for(int i = 0; i < arrayLength; i++)
  {
    Serial.print("Sequence[");
    Serial.print(i);
    Serial.print("].ledColor is ");
    Serial.print(sequence[i].ledColor);
    Serial.print(" Note length is ");
    Serial.println(sequence[i].noteLength);
  }
  Serial.println("//// //// //// RANDOM ENDS //// //// ////");
  #endif
  
}

void playFanfare()
{
  tone(BUZZER, YELLOW_TONE, 250);
  delay(250);
  tone(BUZZER, YELLOW_TONE, 250);
  delay(250);
  tone(BUZZER, YELLOW_TONE, 250);
  delay(250);
  tone(BUZZER, GREEN_TONE, 500);
  delay(500);
  tone(BUZZER, YELLOW_TONE, 250);
  delay(250);
  tone(BUZZER, GREEN_TONE, 1000);
  delay(4000);
}

void playMissSound()
{
  for(int i= BLUE_TONE; i > 0; i--)
  {
     tone(BUZZER, i, 50);
     delay(10);
  }
}

void playSequence(struct soundItem* sequence){

  #if DEBUGMODE
  Serial.print("Playsequence ");
  Serial.print("notesToPlay: ");
  Serial.println(notesToPlay);
  #endif
  for(uint8_t soundIndex = 0; soundIndex < notesToPlay; soundIndex++)
  {
    #if DEBUGMODE
    Serial.print("SoundIndex: ");
    Serial.println(soundIndex);
    #endif
    if(sequence[soundIndex].ledColor == LED_BLUE)
    {
      digitalWrite(LED_BLUE, HIGH); // turn the LED on (HIGH is the voltage level)
      //delay(500);
      //digitalWrite(LED_BLUE, LOW);
      tone(BUZZER, BLUE_TONE, sequence[soundIndex].noteLength);
      delay(sequence[soundIndex].noteLength);
      digitalWrite(LED_BLUE, LOW);  // turn the LED off by making the voltage LOW
      delay(500);  
    }
    else
    {
      digitalWrite(LED_BLUE, LOW);  // turn the LED off by making the voltage LOW
    }
  
    if(sequence[soundIndex].ledColor == LED_GREEN)
    {
      digitalWrite(LED_GREEN, HIGH); // turn the LED on (HIGH is the voltage level)
      //delay(500);
      //digitalWrite(LED_GREEN, LOW);
      tone(BUZZER, GREEN_TONE, sequence[soundIndex].noteLength);
      delay(sequence[soundIndex].noteLength);
      digitalWrite(LED_GREEN, LOW);
      delay(500);
    }
    else
    {
      digitalWrite(LED_GREEN, LOW);  // turn the LED off by making the voltage LOW
    }
  
    if(sequence[soundIndex].ledColor == LED_RED)
    {
      digitalWrite(LED_RED, HIGH); // turn the LED on (HIGH is the voltage level)
      //delay(500);
      //digitalWrite(LED_RED, LOW);
      tone(BUZZER, RED_TONE, sequence[soundIndex].noteLength);
      delay(sequence[soundIndex].noteLength);
      digitalWrite(LED_RED, LOW);
      delay(500);
    }
    else
    {
      digitalWrite(LED_RED, LOW);  // turn the LED off by making the voltage LOW
    }
  
    if(sequence[soundIndex].ledColor == LED_YELLOW)
    {
      digitalWrite(LED_YELLOW, HIGH); // turn the LED on (HIGH is the voltage level)
      //delay(500);
      //digitalWrite(LED_YELLOW, LOW);
      tone(BUZZER, YELLOW_TONE, sequence[soundIndex].noteLength);
      delay(sequence[soundIndex].noteLength);
      digitalWrite(LED_YELLOW, LOW);
      delay(500);
    }
    else
    {
      digitalWrite(LED_YELLOW, LOW);  // turn the LED off by making the voltage LOW
    }
  } 
  //delay(500);        
}

void waitForButton()
{
  while(digitalRead(BUTTON_BLUE))
  {
    // Do nothing wait for the BLUE button press
  }
  delay(1000);
}

bool checkPlayerResponse(struct soundItem* sequence)
{
  for(int sequenceIndex = 0; sequenceIndex < notesToPlay; sequenceIndex++)
  {
    #if DEBUGMODE
    Serial.print("Check player response sequenceIndex is ");
    Serial.println(sequenceIndex);
    #endif
    // Wait for button press
    while(digitalRead(BUTTON_BLUE) && digitalRead(BUTTON_GREEN) && digitalRead(BUTTON_RED) && digitalRead(BUTTON_YELLOW)) {}
    
    if(sequence[sequenceIndex].ledColor == LED_BLUE)
    {
      // Blue button pressed
      if(!digitalRead(BUTTON_BLUE))
      {
            #if DEBUGMODE
            Serial.println("Blue pressed!");
            #endif
            digitalWrite(LED_BLUE, HIGH);
            tone(BUZZER, BLUE_TONE, 500);
            // Delay a bit in order to wait for pin (button) state set to HIGH again
            delay(250);
            digitalWrite(LED_BLUE, LOW);
      } 
      if(!digitalRead(BUTTON_GREEN) || !digitalRead(BUTTON_RED) || !digitalRead(BUTTON_YELLOW))
      {
        #if DEBUGMODE
        Serial.println("Other pressed! Blue...");
        #endif
        playMissSound();
        return false;
      }      
    }
    if(sequence[sequenceIndex].ledColor == LED_GREEN)
    {
      // Green button pressed
      if(!digitalRead(BUTTON_GREEN))
      {
            #if DEBUGMODE
            Serial.println("Green pressed!");
            #endif
            digitalWrite(LED_GREEN, HIGH);
            tone(BUZZER, GREEN_TONE, 500);
            // Delay a bit in order to wait for pin (button) state set to HIGH again
            delay(250);
            digitalWrite(LED_GREEN, LOW);
      } 
      if(!digitalRead(BUTTON_BLUE) || !digitalRead(BUTTON_RED) || !digitalRead(BUTTON_YELLOW))
      {
        #if DEBUGMODE
        Serial.println("Other pressed! Green...");
        #endif
        playMissSound();
        return false;
      }      
    }
    if(sequence[sequenceIndex].ledColor == LED_RED)
    {
      // Green button pressed
      if(!digitalRead(BUTTON_RED))
      {
            #if DEBUGMODE
            Serial.println("Red pressed!");
            #endif
            digitalWrite(LED_RED, HIGH);
            tone(BUZZER, RED_TONE, 500);
            // Delay a bit in order to wait for pin (button) state set to HIGH again
            delay(250);
            digitalWrite(LED_RED, LOW);
      } 
      if(!digitalRead(BUTTON_BLUE) || !digitalRead(BUTTON_GREEN) || !digitalRead(BUTTON_YELLOW))
      {
        #if DEBUGMODE
        Serial.println("Other pressed! Red...");
        #endif
        playMissSound();
        return false;
      }      
    }
    if(sequence[sequenceIndex].ledColor == LED_YELLOW)
    {
      // Green button pressed
      if(!digitalRead(BUTTON_YELLOW))
      {
            #if DEBUGMODE
            Serial.println("Yellow pressed!");
            #endif
            digitalWrite(LED_YELLOW, HIGH);
            tone(BUZZER, YELLOW_TONE, 500);
            // Delay a bit in order to wait for pin (button) state set to HIGH again
            delay(250);
            digitalWrite(LED_YELLOW, LOW);
      } 
      if(!digitalRead(BUTTON_BLUE) || !digitalRead(BUTTON_GREEN) || !digitalRead(BUTTON_RED))
      {
        #if DEBUGMODE
        Serial.println("Other pressed! Yellow...");
        #endif
        playMissSound();
        return false;
      }      
    }
        
  }
  delay(1000);
  // Sequence completed without errors
  return true;
}

void setup() {
  // initialize digital pins (LEDS) as an output.
  pinMode(LED_BLUE, OUTPUT);
  pinMode(LED_GREEN, OUTPUT);
  pinMode(LED_RED, OUTPUT);
  pinMode(LED_YELLOW, OUTPUT);

  // initialize digital pin (BUTTONS) as an input
  pinMode(BUTTON_BLUE, INPUT);
  pinMode(BUTTON_GREEN, INPUT);
  pinMode(BUTTON_RED, INPUT);
  pinMode(BUTTON_YELLOW, INPUT);

  // initialize the serial communication for debugging purposes
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {

  if(generateNewSequence)
  {
    waitForButton();

    #if DEBUGMODE
    Serial.print("Difficulty: ");
    Serial.println(difficulty);
    #endif
    switch(difficulty)
    {
      case 0:
      arrayLength = ARRAYLENGTH(randomSequenceEasy);
      generateRandomSequence(randomSequenceEasy, arrayLength);
      sequence = randomSequenceEasy;
      break;
      case 1:
      arrayLength = ARRAYLENGTH(randomSequenceMedium);
      generateRandomSequence(randomSequenceMedium, arrayLength);
      sequence = randomSequenceMedium;
      break;
      default:
      arrayLength = ARRAYLENGTH(randomSequenceHard);
      generateRandomSequence(randomSequenceHard, arrayLength);
      sequence = randomSequenceHard;       
    }    
  }
  
  #if DEBUGMODE
    // Wait for serial port to setup
    delay(2000);
    sequenceSize = sizeof(sequence);
    Serial.print("Sequence size in bytes is: ");
    Serial.println(sequenceSize);
    Serial.print("Array lenght is: ");
    Serial.println(arrayLength);
  #endif
  
  playSequence(sequence);                                                          
  bool success = checkPlayerResponse(sequence);       // Ask player to repeat the sequence

  if(success)
  {       
    #if DEBUGMODE
    Serial.println("Answer was success, increment notes to play");
    #endif
    if(notesToPlay <= arrayLength)   // arraysize
    {
      //Increment sequence notes to play
      notesToPlay++;
      generateNewSequence = false;
    }

    // Whole sequence is played successfully -- Generate new sequence and increase difficulty level!
    if (notesToPlay == arrayLength + 1)   // arraysize +1
    {
      #if DEBUGMODE
      Serial.println("Sequence success!!!");
      #endif
      // Play fanfare
      playFanfare();
      notesToPlay = 1;
      generateNewSequence = true;
      difficulty++;
    }      
  }
  else
  {
    notesToPlay = 1;
    generateNewSequence = true;
    difficulty = 0;
  }        
}
  
                 
