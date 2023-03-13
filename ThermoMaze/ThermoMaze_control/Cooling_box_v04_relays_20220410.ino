// This script is for controlling the on and off of the heating peltier for the ThermoMaze behavioral experiment.
// The script will turn on each of the heated spot using milli function. 

// Set the digital output pin number on the Arduino that will be turned on to control the relay (heat spots)

const int relay_01 =  4; // black
const int relay_02 =  5; // red
const int relay_03 =  6; // blue
const int relay_04 =  7; // yellow

// Set the digital output pin number on the Arduino that will be turned on to send TTL pulse to Intan
const int IntanTTL_01 = 9; // red to intan 2
const int IntanTTL_02 = 10; // orange to intan 3
const int IntanTTL_03 = 11; // yellow to intan 4
const int IntanTTL_04 = 12; // green to intan 5


// Generally, you should use "unsigned long" for variables that hold time
// The value will quickly become too large for an int to store
unsigned long previousMillis_heat_loop = 0;  // will store last time into the loop

// set time constants
const long second = 1000; // 1 s = 1000 ms
const long minute = 60 * second; // 1 min = 60 s

// Other variables
const long heat_switch_interval = 7 * minute;         // interval at which to blink (milliseconds)
int isNewLoop = 1;
int nLoopsRemained = 3;//number of loops in total to iterate through the 4 spots
int buttonState = 0;
int isInLoop = 0; // boo to indicate whether is still in loop. 0 - no ; 1 - yes
const int arduinoSwitch = 8;
int ignoreButton = 0;// boo to indicate that button state has been read and should ignore if it == 1

void setup() {
  // set the serial port for communication with the PC
  Serial.begin(9600);

  // set the digital pin as output:
  pinMode(relay_01, OUTPUT);
  pinMode(relay_02, OUTPUT);
  pinMode(relay_03, OUTPUT);
  pinMode(relay_04, OUTPUT);

  pinMode(IntanTTL_01, OUTPUT);
  pinMode(IntanTTL_02, OUTPUT);
  pinMode(IntanTTL_03, OUTPUT);
  pinMode(IntanTTL_04, OUTPUT);

  pinMode(arduinoSwitch, INPUT);
}

void loop() {
    unsigned long currentMillis = millis();

  //TESTING: arduino high -> relay high -> light off
  // read the state of the pushbutton value:
  buttonState = digitalRead(arduinoSwitch);

  if (buttonState == 1 && ignoreButton == 0) {
    isInLoop = 1; // set the boo to 1 once the switch is turned on
    Serial.println("botton is on, turn it off!");
    previousMillis_heat_loop = currentMillis;
    ignoreButton = 1;
  }
  if (isInLoop == 1 && nLoopsRemained > 0) {
    //set variables for each loop

    if (currentMillis - previousMillis_heat_loop >= 3 * heat_switch_interval) {
      digitalWrite(relay_01, LOW);
      digitalWrite(IntanTTL_01, LOW);
      digitalWrite(relay_02, LOW);
      digitalWrite(IntanTTL_02, LOW);
      digitalWrite(relay_03, LOW);
      digitalWrite(IntanTTL_03, LOW);
      digitalWrite(relay_04, HIGH);
      digitalWrite(IntanTTL_04, HIGH);
      if (currentMillis - previousMillis_heat_loop >= 4 * heat_switch_interval) {
        // reset previousMillis_heat_loop when the four spots are visited
        previousMillis_heat_loop = currentMillis;
        // deduct number of loops remaining
        nLoopsRemained --; // number of loops remained reduced by 1
        isNewLoop = 1; // for the new loop
        Serial.println("Another loop over, check if button is turned off:");
        Serial.println(buttonState);
      }
    } else if (currentMillis - previousMillis_heat_loop >= 2 * heat_switch_interval) {
      digitalWrite(relay_01, LOW);
      digitalWrite(IntanTTL_01, LOW);
      digitalWrite(relay_02, LOW);
      digitalWrite(IntanTTL_02, LOW);
      digitalWrite(relay_03, HIGH);
      digitalWrite(IntanTTL_03, HIGH);
      digitalWrite(relay_04, LOW);
      digitalWrite(IntanTTL_04, LOW);
    } else if (currentMillis - previousMillis_heat_loop >= heat_switch_interval) {
      digitalWrite(relay_01, LOW);
      digitalWrite(IntanTTL_01, LOW);
      digitalWrite(relay_02, HIGH);
      digitalWrite(IntanTTL_02, HIGH);
      digitalWrite(relay_03, LOW);
      digitalWrite(IntanTTL_03, LOW);
      digitalWrite(relay_04, LOW);
      digitalWrite(IntanTTL_04, LOW);
    } else {
      if (isNewLoop == 1) {
        previousMillis_heat_loop = currentMillis;
        isNewLoop = 0;
        Serial.println("new loop");
        Serial.println(currentMillis);
      }
      digitalWrite(relay_01, HIGH);
      digitalWrite(IntanTTL_01, HIGH);
      digitalWrite(relay_02, LOW);
      digitalWrite(IntanTTL_02, LOW);
      digitalWrite(relay_03, LOW);
      digitalWrite(IntanTTL_03, LOW);
      digitalWrite(relay_04, LOW);
      digitalWrite(IntanTTL_04, LOW);
    }
  } else {
    if (isInLoop == 1) {
      digitalWrite(relay_01, LOW);
      digitalWrite(IntanTTL_01, LOW);
      digitalWrite(relay_02, LOW);
      digitalWrite(IntanTTL_02, LOW);
      digitalWrite(relay_03, LOW);
      digitalWrite(IntanTTL_03, LOW);
      digitalWrite(relay_04, LOW);
      digitalWrite(IntanTTL_04, LOW);
      Serial.println("end of session");
      isInLoop = 0;
    }

  }
}
