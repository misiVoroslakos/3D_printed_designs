// this constant won't change:
const int  WaterInfraPin = 10;    // water IR sensor input 
const int  NoseInfraPin = 9;    // nose IR sensor input 
const int piezoPin = 8;           //Specify digital pin on the Arduino that the positive lead of piezo buzzer is attached.

const int NoseHoldingInterval = 100; // number of millisecs to hold
//Declare pin functions on Arduino
#define stp 2
#define dir 3
#define MS1 4
#define MS2 5
#define MS3 6
#define EN  7

//Declare variables for functions
char user_input;
int x;
int y;
int state;

// Variables will change:
int NoseInfraState = 1;
int WaterInfraState = 1;
boolean NosePoke = false;
boolean EnableWater = false;
unsigned long startTime = 0;  // this is a global

void setup() {
  // initialize the button pin as a input:
  pinMode(NoseInfraPin, INPUT);
  pinMode(WaterInfraPin, INPUT);
  pinMode(stp, OUTPUT);
  pinMode(dir, OUTPUT);
  pinMode(MS1, OUTPUT);
  pinMode(MS2, OUTPUT);
  pinMode(MS3, OUTPUT);
  pinMode(EN, OUTPUT);
  resetBEDPins(); //Set step, direction, microstep and enable pins to default states
  // initialize serial communication:
  Serial.begin(9600);
}


void loop() {
  // read the nosePoke IR sensor input pin:
  NoseInfraState = digitalRead(NoseInfraPin);
  WaterInfraState = digitalRead(WaterInfraPin);
  
  if (NoseInfraState == 0) { // when nose breaks the IR beam
    startTime = millis();
    NosePoke = !NosePoke;
    Serial.println("Nose Poke detected!");
  }
  if (millis() - startTime >= NoseHoldingInterval && NosePoke == true) {
    tone(piezoPin, 1000, 200);    // give a 1000 Hz tone for positive reinforcement
    startTime = 0;                // reset timer
    EnableWater = !EnableWater;   // change EnableWater to true -> animal can receive water reward
    NosePoke = !NosePoke;         // change NosePoke to false -> water reward will change it true
  }
  else {
    startTime = 0;
  }

  if (WaterInfraState == 0 && EnableWater == true) {
    Serial.println("Water Poke detected!");
    NosePoke = !NosePoke;         // change NosePoke to true
    digitalWrite(EN, LOW);        //Pull enable pin low to set FETs active and allow motor control
    StepForwardDefault(); //CONSIDER change it to slower water delivery!!! ASK BRIAN
    resetBEDPins();
  }
}


//Reset Big Easy Driver pins to default states
void resetBEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, LOW);
  digitalWrite(MS2, LOW);
  digitalWrite(MS3, LOW);
  digitalWrite(EN, HIGH);
}

//Default microstep mode function
void StepForwardDefault()
{
  Serial.println("Moving forward at default step mode.");
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for(x= 1; x<100; x++)  //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp,HIGH); //Trigger one step forward
    delay(1);
    digitalWrite(stp,LOW); //Pull step pin low so it can be triggered again
    delay(1);
  }
  Serial.println("Enter new option");
  Serial.println();
}
