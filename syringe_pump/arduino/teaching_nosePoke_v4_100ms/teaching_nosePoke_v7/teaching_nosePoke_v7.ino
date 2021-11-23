// include the library code:
#include <LiquidCrystal.h>
// initialize the library with the numbers of the interface pins
LiquidCrystal lcd(46, 48, 36, 38, 40, 42);

// this constant won't change:
const int  WaterInfraPin = 10;    // water IR sensor input
const int  NoseInfraPin = A0;    // nose IR sensor input
const int piezoPin = 8;           //Specify digital pin on the Arduino that the positive lead of piezo buzzer is attached.
const unsigned long NoseHoldingInterval = 5000; // number of millisecs to hold

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
byte CurrentNoseInfraState;
byte CurrentWaterInfraState;
byte PreviousNoseInfraState;
byte PreviousWaterInfraState;
boolean startFlag = false;
boolean EnableWater = false;
boolean EnableNosePoke = true;
unsigned long startMillis;  // this is a global
unsigned long currentMillis;
int trials = 0;
int NosePoke = 0;
float CorrectPerformance;

void setup() {
  // set up the LCD's number of columns and rows:
  lcd.begin(16, 2);
  lcd.print("Trl  AllPoke  P");
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


void loop()
{
  currentMillis = millis();
  CurrentNoseInfraState = analogRead(NoseInfraPin);  //read the current state of the nose IR
  CurrentWaterInfraState = digitalRead(WaterInfraPin);  //read the current state of the nose IR
  if ((CurrentNoseInfraState == LOW) && (PreviousNoseInfraState == HIGH))  // detect nose poke (it breaks the IR beam -> goes from Hi->Lo)
  {
    NosePoke++;    //increment the number of NosePokes
    // print the number of NosePokes
    // set the cursor to column 6, line 1
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(6, 1);
    lcd.print(NosePoke);
    if (EnableNosePoke == true)
    {
    startMillis = currentMillis;  //save the time that the state change occured
    startFlag = true; //flag that nose poke is in progress
    //Serial.println(startTime);
    //Serial.println("Nose Poke detected!");
    }
  }
  if ((millis() - startMillis) >= NoseHoldingInterval) //if the nosepoke period has elapsed
  {
    if (startFlag == true) //nose poke is taking place
    {
      if (CurrentNoseInfraState < 100)  //if the nose is currently in 
      {
        startFlag = false;  //nose holding is finished
        //Serial.println("Reward can be delivered!");
        tone(piezoPin, 1000, 300);    // give a 1000 Hz tone for positive reinforcemen
        EnableNosePoke = !EnableNosePoke;  //don't let another nose poke until reward is collected
        EnableWater = !EnableWater;   // change EnableWater to true -> animal can receive water reward
        trials++;               //increment the trial
        //Serial.print("Trial: ");
        //Serial.println(trials);
        
        // print the number of trials
        // set the cursor to column 0, line 1
        // (note: line 1 is the second row, since counting begins with 0):
        lcd.setCursor(0, 1);
        lcd.print(trials);
      } //end tone
    } //end nose holding IN PROGRESS check
  }
  if ((CurrentWaterInfraState == LOW) && (PreviousWaterInfraState == HIGH)) 
  {
    if (EnableWater == true)
    {
    //Serial.println("Water Poke detected!");
    EnableNosePoke = !EnableNosePoke;
    EnableWater = !EnableWater;
    digitalWrite(EN, LOW);        //Pull enable pin low to set FETs active and allow motor control
    StepForwardDefault(); //CONSIDER change it to slower water delivery!!! ASK BRIAN
    resetBEDPins();
    CorrectPerformance = trials / NosePoke;
    // print the percentage of correct trials
    // set the cursor to column 0, line 13
    // (note: line 1 is the second row, since counting begins with 0):
    lcd.setCursor(13, 1);
    lcd.print(CorrectPerformance,0);
    }
  }
  PreviousNoseInfraState = CurrentNoseInfraState; //save the previous IR state
  PreviousWaterInfraState = CurrentWaterInfraState; //save the previous IR state
}


//Reset Big Easy Driver pins to default states
void resetBEDPins()
{
  digitalWrite(stp, LOW);
  digitalWrite(dir, LOW);
  digitalWrite(MS1, HIGH);
  digitalWrite(MS2, HIGH);
  digitalWrite(MS3, HIGH);
  digitalWrite(EN, HIGH);
}

//Default microstep mode function
void StepForwardDefault()
{
  digitalWrite(dir, LOW); //Pull direction pin low to move "forward"
  for (x = 1; x < 1000; x++) //Loop the forward stepping enough times for motion to be visible
  {
    digitalWrite(stp, HIGH); //Trigger one step forward
    delay(3);
    digitalWrite(stp, LOW); //Pull step pin low so it can be triggered again
    delay(4);
  }
}
