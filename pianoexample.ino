// // Sketch for knock sensor in pull-up resistor configuration
// // Pinout:   https://startingelectronics.org/pinout/knock-sensor/
// // Tutorial: https://startingelectronics.org/tutorials/arduino/modules/knock-sensor/
// // Change pin number that knock sensor is connected to here
// #define KNOCK_PIN 4

// void setup() {
//   pinMode(LED_BUILTIN, OUTPUT);       // on-board LED, usually pin 13
//   pinMode(KNOCK_PIN, INPUT);          // knock sensor pin set to input

// }

// void loop() {
//   if (digitalRead(KNOCK_PIN)) {       // knock detected?
//     // knock not detected with pull-up resistor
//     digitalWrite(LED_BUILTIN, LOW);   // switch LED off
//   }
//   else {
//     // knock detected with pull-up resistor
//     digitalWrite(LED_BUILTIN, HIGH);  // switch LED on
//     delay(2000);                      // leave LED on for period
//   }
// }


// this is code for rotaru thing
/*
 * Created by ArduinoGetStarted.com
 *
 * This example code is in the public domain
 *
 * Tutorial page: https://arduinogetstarted.com/tutorials/arduino-rotary-encoder
 */

// #include <ezButton.h>  // the library to use for SW pin

// #define CLK_PIN 5
// #define DT_PIN 6
// #define SW_PIN 7
// #define LED_PIN 13 // me
// #define DIRECTION_CW 0   // clockwise direction
// #define DIRECTION_CCW 1  // counter-clockwise direction

// int counter = 0;
// int direction = DIRECTION_CW;
// int CLK_state;
// int prev_CLK_state;

// ezButton button(SW_PIN);  // create ezButton object that attach to pin 4

// void setup() {
//   Serial.begin(9600);
//   pinMode(LED_PIN, OUTPUT); // me added


//   // configure encoder pins as inputs
//   pinMode(CLK_PIN, INPUT);
//   pinMode(DT_PIN, INPUT);
//   button.setDebounceTime(50);  // set debounce time to 50 milliseconds

//   // read the initial state of the rotary encoder's CLK pin
//   prev_CLK_state = digitalRead(CLK_PIN);
// }

// void loop() {
//   button.loop();  // MUST call the loop() function first

//   // read the current state of the rotary encoder's CLK pin
//   CLK_state = digitalRead(CLK_PIN);

//   // If the state of CLK is changed, then pulse occurred
//   // React to only the rising edge (from LOW to HIGH) to avoid double count
//   if (CLK_state != prev_CLK_state && CLK_state == HIGH) {
//     // if the DT state is HIGH
//     // the encoder is rotating in counter-clockwise direction => decrease the counter
//     if (digitalRead(DT_PIN) == HIGH) {
//       counter--;
//       direction = DIRECTION_CCW;
//       digitalWrite(LED_PIN, LOW); // me
      


//     } else {
//       // the encoder is rotating in clockwise direction => increase the counter
//       counter++;
//       direction = DIRECTION_CW;
//       digitalWrite(LED_PIN, HIGH); // me


//     }

//     Serial.print("DIRECTION: ");
//     if (direction == DIRECTION_CW)
//       Serial.print("Clockwise");
       
    
//     else
//       Serial.print("Counter-clockwise");
      

//     Serial.print(" | COUNTER: ");
//     Serial.println(counter);
//   }

//   // save last CLK state
//   prev_CLK_state = CLK_state;

//   if (button.isPressed()) {
   
   

//     Serial.println("The button is pressed");
    

//   }
  
// }

// this is foor tilt switch it works

// int tiltPin = 2;      // pin number for tilt switch signal 
// int ledPin =  13;     // pin number of LED 
// int tiltState = 0;    // variable for reading the tilt switch status

// void setup() {  
//   pinMode(ledPin, OUTPUT);  // set the LED pin as output      
//   pinMode(tiltPin, INPUT);  // set the tilt switch pin as input
// }

// void loop(){
//   // get the tilt switch state
//   tiltState = digitalRead(tiltPin);

//   // check if tilt switch is tilted.
//   if (tiltState == HIGH) {     
//     digitalWrite(ledPin, HIGH);  
//   } 
//   else {
//     digitalWrite(ledPin, LOW); 
//   }
// }


//
#include "MIDIUSB.h"

const int photoresistorPin1 = A0; // Pin connected to the first photoresistor
const int photoresistorPin2 = A1; // Pin connected to the second photoresistor
const int photoresistorPin3 = A2; // Pin connected to the second photoresistor
const int photoresistorPin4 = A3; // Pin connected to the second photoresistor

const int threshold = 70; // Adjust this value as needed for your lighting conditions
bool note1Playing = false; // Flag for the first note
bool note2Playing = false; // Flag for the second note
bool note3Playing = false; // Flag for the second note
bool note4Playing = false; // Flag for the second note

void setup() {
  Serial.begin(115200);
//  pinMode(Shock, INPUT); 	// define input for sensor signal
 // Serial.println("No noise");
}

void loop() {
  int lightLevel1 = analogRead(photoresistorPin1); // Read the first photoresistor
  int lightLevel2 = analogRead(photoresistorPin2); // Read the second photoresistor
  int lightLevel3 = analogRead(photoresistorPin3); // Read the second photoresistor
  int lightLevel4 = analogRead(photoresistorPin4); // Read the second photoresistor

  // Check the first photoresistor
  if (lightLevel1 > threshold) { // If covered (low light)
        Serial.println(lightLevel1);

    if (!note1Playing) { // Check if the first note is not already playing
      Serial.println("Sending note on for note 1");
      noteOn(0, 48, 64); // Channel 0, note number for first LDR (middle C)
      MidiUSB.flush();
      note1Playing = true; // Set flag to indicate the note is playing
       delay(1000);
    }
  } else { // If not covered (high light)
    if (note1Playing) { // Check if the note is currently playing
      Serial.println("Sending note off for note 1");
      noteOff(0, 48, 64); // Channel 0, note number for first LDR
      MidiUSB.flush();
      note1Playing = false; // Reset flag
    }
  }

  //Check the second photoresistor
  if (lightLevel2 > threshold) { // If covered (low light)
    if (!note2Playing) { // Check if the second note is not already playing
      Serial.println("Sending note on for note 2");
      noteOn(0, 50, 64); // Channel 0, note number for second LDR (D)
      MidiUSB.flush();
      note2Playing = true; // Set flag to indicate the note is playing
      delay(500);

    }
  } else { // If not covered (high light)
    if (note2Playing) { // Check if the note is currently playing
      Serial.println("Sending note off for note 2");
      noteOff(0, 50, 64); // Channel 0, note number for second LDR
      MidiUSB.flush();
      note2Playing = false; // Reset flag
    }
  }
  // Check the third photoresistor
  if (lightLevel3 > threshold) { // If covered (low light)
    if (!note3Playing) { // Check if the second note is not already playing
      Serial.println("Sending note on for note 3");
      noteOn(0, 54, 64); // Channel 0, note number for second LDR (D)
      MidiUSB.flush();
      note3Playing = true; // Set flag to indicate the note is playing
      delay(500);

    }
  } else { // If not covered (high light)
    if (note3Playing) { // Check if the note is currently playing
      Serial.println("Sending note off for note 3");
      noteOff(0, 54, 64); // Channel 0, note number for second LDR
      MidiUSB.flush();
      note3Playing = false; // Reset flag
    }
  }
  // Check the fourth photoresistor
  if (lightLevel4 > threshold) { // If covered (low light)
    if (!note4Playing) { // Check if the second note is not already playing
      Serial.println("Sending note on for note 4");
      noteOn(0, 53, 64); // Channel 0, note number for second LDR (D)
      MidiUSB.flush();
      note4Playing = true; // Set flag to indicate the note is playing
      delay(500);

    }
  } else { // If not covered (high light)
    if (note4Playing) { // Check if the note is currently playing
      Serial.println("Sending note off for note 4");
      noteOff(0, 53, 64); // Channel 0, note number for second LDR
      MidiUSB.flush();
      note4Playing = false; // Reset flag
    }
  }
  //garbage didnt work
 // val = digitalRead(Shock); // read and assign the value of digital interface 3 to val
  //Serial.println(Shock);
// 	if(val == HIGH) // when sensor detects a signal, the LED flashes
// 	{
// 	//	digitalWrite(Led, LOW);
// 	}
// 	else
// 	{
// 		digitalWrite(Led, HIGH);
// 	}
  //garbage didnt work

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