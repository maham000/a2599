//This code was adapted witht the help of the following references:
//Code for rotary encoder : 
//https://arduinogetstarted.com/tutorials/arduino-rotary-encoder#google_vignette

//Code for knock sensor: 
//https://startingelectronics.org/tutorials/arduino/modules/knock-sensor/

//Code for tilt switch : 
//https://arduinomodules.info/ky-020-tilt-switch-module/#google_vignette
//Code for photo resistors:
//https://lab.arts.ac.uk/books/physical-computing/page/using-arduino-leonardo-to-send-usb-midi-data
// and library  :  https://www.arduino.cc/reference/en/libraries/midiusb/ 
#include "MIDIUSB.h"
#include <ezButton.h>  // the library to use for SW pin //rotary encoder code


const int photoresistorPin1 = A0; // Pin connected to the first photoresistor
const int photoresistorPin2 = A1; // Pin connected to the second photoresistor
const int photoresistorPin3 = A2; // Pin connected to the second photoresistor
const int photoresistorPin4 = A3; // Pin connected to the second photoresistor

const int threshold = 70; // Adjust this value as needed for your lighting conditions
bool note1Playing = false; // Flag for the first note
bool note2Playing = false; // Flag for the second note
bool note3Playing = false; // Flag for the second note
bool note4Playing = false; // Flag for the second note

// knock
bool knockPlaying = false; // Flag for knock sound

//tilt switch
int tiltPin = 2;      // pin number for tilt switch signal 
int ledPin =  13;     // pin number of LED just for testing purposes
int tiltState = 0;    // variable for reading the tilt switch status
//end tilt switch
//rotary encoder
#define CLK_PIN 5
#define DT_PIN 6
#define SW_PIN 7
#define LED_PIN 13 // me
#define DIRECTION_CW 0   // clockwise direction
#define DIRECTION_CCW 1  // counter-clockwise direction

int counter = 0;
int direction = DIRECTION_CW;
int CLK_state;
int prev_CLK_state;

ezButton button(SW_PIN);  // create ezButton object that attach to pin 4
// midi encoder code
// Define octave limits
const int minOctave = -2; // Minimum octave
const int maxOctave = 2;  // Maximum octave
int octaveOffset = 0; // Current octave offset
//end rotary encoder

// knock sensor
#define KNOCK_PIN 4

// end
void setup() {
  Serial.begin(115200);

  pinMode(ledPin, OUTPUT);  // set the LED pin as output     just for testing 
  pinMode(tiltPin, INPUT);  // set the tilt switch pin as input
  // knock sensor
  pinMode(KNOCK_PIN, INPUT);          // knock sensor pin set to input
  //end
// rotary 

  // configure encoder pins as inputs
  pinMode(CLK_PIN, INPUT);
  pinMode(DT_PIN, INPUT);
  button.setDebounceTime(50);  // set debounce time to 50 milliseconds

  // read the initial state of the rotary encoder's CLK pin
  prev_CLK_state = digitalRead(CLK_PIN);

  //end
}

void loop() {
  int velocity; // Declare velocity

  int lightLevel1 = analogRead(photoresistorPin1); // Read the first photoresistor
  int lightLevel2 = analogRead(photoresistorPin2); // Read the second photoresistor
  int lightLevel3 = analogRead(photoresistorPin3); // Read the second photoresistor
  int lightLevel4 = analogRead(photoresistorPin4); // Read the second photoresistor
 // get the tilt switch state
 // adapted from : //https://arduinomodules.info/ky-020-tilt-switch-module/#google_vignette
  tiltState = digitalRead(tiltPin);

  // check if tilt switch is tilted.
  if (tiltState == HIGH) {     
    //digitalWrite(ledPin, HIGH);  
    velocity = 100; // then set it to 100
  } 
  else {
    //digitalWrite(ledPin, LOW); 
    velocity = 60; // else set to 60
  }

  //knock sensor
// adapted from: //https://startingelectronics.org/tutorials/arduino/modules/knock-sensor/
 // Knock sensor
  if (digitalRead(KNOCK_PIN)) { // Knock detected?
    if (!knockPlaying) {
      knockPlaying = true; // Set flag to indicate the knock sound is playing
      noteOn(0, 68, velocity); // Send note on (C4)
      MidiUSB.flush();
      delay(500); // Duration of the knock sound
      noteOff(0, 68, velocity); // Send note off
      MidiUSB.flush();
    }
  } else {
    knockPlaying = false; // Reset flag when no knock is detected
  }
  octaveOffset = constrain(counter / 4, minOctave, maxOctave); // decide offset and make it be between min or max for rotary

// photoresitor adapted code from: //https://lab.arts.ac.uk/books/physical-computing/page/using-arduino-leonardo-to-send-usb-midi-data
  // Check the first photoresistor
  if (lightLevel1 > threshold) { // If light level
        Serial.println(lightLevel1);

    if (!note1Playing) { // Check if the first note is not already playing
      Serial.println("Sending note on for note 1");
      noteOn(0, 60 + octaveOffset * 12, velocity); // Channel 0, note number for first LDR
      MidiUSB.flush();
      note1Playing = true; // Set flag to indicate the note is playing
       delay(1000);
    }
  } else { 
    if (note1Playing) { // Check if the note is currently playing
      Serial.println("Sending note off for note 1");
      noteOff(0, 60+ octaveOffset * 12, velocity); // Channel 0, note number for first LDR
      MidiUSB.flush();
      note1Playing = false; 
    }
  }

  //Check the second photoresistor
  if (lightLevel2 > threshold) { 
    if (!note2Playing) { // Check if the second note is not already playing
      Serial.println("Sending note on for note 2");
      noteOn(0, 62+ octaveOffset * 12, velocity); // Channel 0, note number for second LDR (D)
      MidiUSB.flush();
      note2Playing = true;
      delay(500);

    }
  } else { 
    if (note2Playing) { // Check if the note is currently playing
      Serial.println("Sending note off for note 2"); // test
      noteOff(0, 62+ octaveOffset * 12, velocity); // Channel 0, note number for second LDR
      MidiUSB.flush();
      note2Playing = false; 
    }
  }
  // Check the third photoresistor
  if (lightLevel3 > threshold) { 
    if (!note3Playing) { // Check if the  note is not already playing
      Serial.println("Sending note on for note 3");
      noteOn(0, 64+ octaveOffset * 12, velocity); // Channel 0, note number for second LDR (D)
      MidiUSB.flush();
      note3Playing = true; 
      delay(500);

    }
  } else { 
    if (note3Playing) { // Check if the note is currently playing
      Serial.println("Sending note off for note 3");
      noteOff(0, 64+ octaveOffset * 12, velocity); // Channel 0, note number for second LDR
      MidiUSB.flush();
      note3Playing = false; 
    }
  }
  // Check the fourth photoresistor
  if (lightLevel4 > threshold) { 
    if (!note4Playing) { // Check if the  note is not already playing
      Serial.println("Sending note on for note 4");
      noteOn(0, 66+ octaveOffset * 12, velocity); // Channel 0, note number for second LDR (D)
      MidiUSB.flush();
      note4Playing = true; 
      delay(500);

    }
  } else { 
    if (note4Playing) { 
      Serial.println("Sending note off for note 4");
      noteOff(0, 66+ octaveOffset * 12, velocity); // Channel 0, note number for second LDR
      MidiUSB.flush();
      note4Playing = false; 
    }
  }

  //rotary
  // adapted from : //https://arduinogetstarted.com/tutorials/arduino-rotary-encoder#google_vignette
  button.loop();  // MUST call the loop() function first

  // read the current state of the rotary encoder's CLK pin
  CLK_state = digitalRead(CLK_PIN);

  if (CLK_state != prev_CLK_state && CLK_state == HIGH) {
          // the encoder is rotating in counterclockwise direction => decrease the counter

    if (digitalRead(DT_PIN) == HIGH) {
      counter--;
      direction = DIRECTION_CCW;
      digitalWrite(LED_PIN, LOW); // me
      

    } else {
      // the encoder is rotating in clockwise direction => increase the counter
      counter++;
      direction = DIRECTION_CW;
      digitalWrite(LED_PIN, HIGH); // me


    }

    Serial.print("DIRECTION: ");
    if (direction == DIRECTION_CW)
      Serial.print("Clockwise");
       
    
    else
      Serial.print("Counter-clockwise");
      

    Serial.print(" | COUNTER: ");
    Serial.println(counter); // testing
  }

  // save last CLK state
  prev_CLK_state = CLK_state;

  if (button.isPressed()) {
   
    Serial.println("The button is pressed");
    octaveOffset = 0; // Reset to default octave
    counter = 0; // Reset rotary position
    
  }
  //end
 }

void noteOn(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOn = {0x09, 0x90 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOn);
}

void noteOff(byte channel, byte pitch, byte velocity) {
  midiEventPacket_t noteOff = {0x08, 0x80 | channel, pitch, velocity};
  MidiUSB.sendMIDI(noteOff);
}

void controlChange(byte channel, byte control, byte value) {
  midiEventPacket_t event = {0x0B, 0xB0 | channel, control, value};
  MidiUSB.sendMIDI(event);
}
