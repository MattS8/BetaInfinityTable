// Pulse Width controll of 12 digital outputs for four letters each with an RGB
// LED strand Infinity Table
//
#define DEFAULT_CYCLE .5
#define NUM_PINS 12
#define TRUE 1

struct Delay            // Holds value of Duty cycle and the associated pin
{
  float dCycle;         // Duty cycle (between 0 and 1)
  int pin;              // Associated pin
};
typedef struct Delay Delay;

struct Letter           // Holds values for each letter
{
  int rPin;             // Red pin
  int gPin;             // Green pin
  int bPin;             // Blue pin
  Delay rDcycle;        // Duty cycle for Red pin
  Delay gDcycle;        // Duty cycle for Green pin
  Delay bDcycle;        // Duty cycle for Blue pin
};
typedef struct Letter Letter;

Letter B = {2, 3, 4, {DEFAULT_CYCLE, 2}, {DEFAULT_CYCLE, 3}, {DEFAULT_CYCLE, 4}};            // Set up B letter with defaul values
Letter E = {5, 6, 7, {DEFAULT_CYCLE, 5}, {DEFAULT_CYCLE, 6}, {DEFAULT_CYCLE, 7}};            // Set up E letter with defaul values
Letter T = {8, 9, 10, {DEFAULT_CYCLE, 8}, {DEFAULT_CYCLE, 9}, {DEFAULT_CYCLE, 10}};          // Set up T letter with defaul values
Letter A = {11, 12, 13, {DEFAULT_CYCLE, 11}, {DEFAULT_CYCLE, 12}, {DEFAULT_CYCLE, 13}};      // Set up A letter with defaul values
Delay *delays[NUM_PINS] = {&(B.rDcycle), &(B.gDcycle), &(B.bDcycle),
                           &(E.rDcycle), &(E.gDcycle), &(E.bDcycle),
                           &(T.rDcycle), &(T.gDcycle), &(T.bDcycle),
                           &(A.rDcycle), &(A.gDcycle), &(A.bDcycle)
                          };

int All_On = 1;         // Force Lights On (No Music Present)
int analogPin = 0;      // Analog input for loop selector
int val = 0;            // Variable to store pot value 0-1023

float incR = 0;         // Incremental value for Red cycle
float incG = 0;         // Incremental value for Green cycle
float incB = 0;         // Incremental value for Blue cycle
float incR1 = .01;      // Incremental value for Red cycle
float incG1 = .01;      // Incremental value for Green cycle
float incB1 = .01;      // Incremental value for Blue cycle

float incRa = 0;          // Intermediate variable for random function
float incGa = 0;          // Intermediate variable for random function
float incBa = 0;          // Intermediate variable for random function
int looptime = 5000;    // Cycle counter for how long to wait for music off to trigger RGB cycle
int rnd_cnt = 0;        // Cycle counter to re-randomize the RGB increment values
int CRed;               // Max values for color organ Red
int CGreen;             // Max values for color organ Green
int CBlue;              // Max values for color organ Blue
unsigned long timeRGB;  // Time variable for RGB parameter randomizer
unsigned long RtimeRGB; // Random portion of the timeout to randomize the RGB color cycle parameters

void setup()
{
  incR = random(1000, 100) / 1000; // Randomize the starting point for Red intensity
  incG = random(1000, 100) / 1000; // Randomize the starting point for Green intensity
  incB = random(1000, 100) / 1000; // Randomize the starting point for Blue intensity
  RtimeRGB = millis();        // Set RtimeRGB to current time, one timer for all three colors
  timeRGB = 10000;            // 10 seconds to start with
  Serial.begin(9600);
  pinMode (B.rPin, OUTPUT);             //
  pinMode (B.gPin, OUTPUT);           //
  pinMode (B.bPin, OUTPUT);            //
  pinMode (E.rPin, OUTPUT);             //
  pinMode (E.gPin, OUTPUT);           //
  pinMode (E.bPin, OUTPUT);            //
  pinMode (T.rPin, OUTPUT);             //
  pinMode (T.gPin, OUTPUT);           //
  pinMode (T.bPin, OUTPUT);            //
  pinMode (A.rPin, OUTPUT);             //
  pinMode (A.gPin, OUTPUT);           //
  pinMode (A.bPin, OUTPUT);            //
  digitalWrite(B.rPin, LOW);        // set Red B low
  digitalWrite(B.gPin, LOW);        // set Green B low
  digitalWrite(B.bPin, LOW);        // set Bllue B low
  digitalWrite(E.rPin, LOW);        // set Red E low
  digitalWrite(E.gPin, LOW);        // set Green E low
  digitalWrite(E.bPin, LOW);        // set Bllue E low
  digitalWrite(T.rPin, LOW);        // set Red T low
  digitalWrite(T.gPin, LOW);        // set Green T low
  digitalWrite(T.bPin, LOW);        // set Bllue T low
  digitalWrite(A.rPin, LOW);        // set Red A low
  digitalWrite(A.gPin, LOW);        // set Green A low
  digitalWrite(A.bPin, LOW);        // set Bllue A low
  //analogReference (INTERNAL);  // Internal reference 1.1 V instead of the default of 5 V if input levels are low.
}

void delayPin(int i, int prevDelayTime)
{
  Delay currentDelay = *delays[i];
  float delayTime = (Period * currentDelay.dCycle) - prevDelayTime;

  Serial.print("Delaying for: "); Serial.print(delayTime); Serial.println(" microseconds");
  delayMicroseconds(delayTime);
  digitalWrite(currentDelay.pin, LOW);
}

void writeLEDsOn()  // Function to turn on all LED FET drivers based on RGB values
{
  float prevDelayTime = 0;
  quickSort(0, NUM_PINS - 1);
  digitalWrite(B.rPin, HIGH);
  digitalWrite(B.gPin, HIGH);
  digitalWrite(B.bPin, HIGH);
  digitalWrite(E.rPin, HIGH);
  digitalWrite(E.gPin, HIGH);
  digitalWrite(E.bPin, HIGH);
  digitalWrite(T.rPin, HIGH);
  digitalWrite(T.gPin, HIGH);
  digitalWrite(T.bPin, HIGH);
  digitalWrite(A.rPin, HIGH);
  digitalWrite(A.gPin, HIGH);
  digitalWrite(A.bPin, HIGH);
  for (int i = 0; i < NUM_PINS; i++) {
    delayPin(i, prevDelayTime);
    prevDelayTime += (delays[i]->dCycle * Period) - prevDelayTime;
  }
  if (Period - prevDelayTime > 0)
  {
    delayMicroseconds(Period - prevDelayTime);
  }
}

void  writeLEDsOff()
{
  // Write all RED LEDs Off
  digitalWrite(B.rPin, LOW);
  digitalWrite(E.rPin, LOW);
  digitalWrite(T.rPin, LOW);
  digitalWrite(A.rPin, LOW);
  // Write all Green LEDs Off
  digitalWrite(B.gPin, LOW);
  digitalWrite(E.gPin, LOW);
  digitalWrite(T.gPin, LOW);
  digitalWrite(A.gPin, LOW);
  // Write all Blue LEDs Off
  digitalWrite(B.bPin, LOW);
  digitalWrite(E.bPin, LOW);
  digitalWrite(T.bPin, LOW);
  digitalWrite(A.bPin, LOW);
}

void  print_RGBValues()  // Print out values for audio channel
{
  Serial.print(" B Red: "); Serial.print(B.rDcycle.dCycle); //Serial.print("\t");Serial.print("B Green: ");Serial.print(DcycleBG);Serial.print("\t");Serial.print("B Blue: ");Serial.print(DcycleBB);
  Serial.print(" B Green: "); Serial.print(B.gDcycle.dCycle); //Serial.print("\t");Serial.print("E Green: ");Serial.print(DcycleEG);Serial.print("\t");Serial.print("E Blue: ");Serial.print(DcycleEB);
  Serial.print(" B Blue: "); Serial.print(B.bDcycle.dCycle); //Serial.print("\t");Serial.print("E Green: ");Serial.print(DcycleTG);Serial.print("\t");Serial.print("T Blue: ");Serial.print(DcycleTB);
  //Serial.print("A Red: ");Serial.print(DcycleAR);//Serial.print("\t");Serial.print("E Green: ");Serial.print(DcycleAG);Serial.print("\t");Serial.print("A Blue: ");Serial.print(DcycleAB);
  Serial.print("  timeRGB: "); Serial.print(timeRGB); Serial.print(" RtimeRGB: "); Serial.print(RtimeRGB); //Serial.print(" Current Time: ");Serial.print(millis());
  Serial.print("  incR: "); Serial.print(incR); Serial.print(" incR1: "); Serial.print(incR1); Serial.print(" incB: "); Serial.print(incB); Serial.print(" incB1: "); Serial.print(incB1); Serial.print(" incG: "); Serial.print(incG); Serial.print(" incG1: "); Serial.print(incG1);
  Serial.println();
}

void sinloop()  // Randomly increment RGB color values based on a sin function
{
  incR = incR + incR1;                      // Increment B Red increment value by incR1 value (typically .01)
  B.rDcycle.dCycle = abs(sin(incR));                // Absolute value of sin function
  incG = incG + incG1;                      // Increment Green increment value by incG1 value
  B.gDcycle.dCycle = abs(sin(incG));                // Absolute value of sin function
  incB = incB + incB1;                      // Increment Blue increment value by incB1 value
  B.bDcycle.dCycle = abs(sin(incB));                // Absolute value of sin function
  if (millis() - timeRGB >= (10000 + RtimeRGB))     // 10 seconds minimum, 5 + RtimeRGB maximum (randomize RtimeRGB)
  {
    incR = random(3000) / 3000; incRa = random(1, 10); incR1 = incRa / 1000 + .001; // incR range 0.1 to 1, incR1 range is .01 to .11
    incG = random(3000) / 3000; incGa = random(1, 10); incG1 = incGa / 1000 + .001; // incG range 0.1 to 1, incG1 range is .01 to .11
    incB = random(3000) / 3000; incBa = random(1, 10); incB1 = incBa / 1000 + .001; // incB range 0.1 to 1, incB1 range is .01 to .11
    RtimeRGB = random(10000); timeRGB = millis();
  }
}

void RGBloop()  // Change from Red to Green to Blue
{
  if (millis() - timeRGB >= (40000 + RtimeRGB))
  {
    RtimeRGB = random(10000); // randomize RtimeRGB
    timeRGB = millis();       // Set timeRGB to now
  }
  else if (millis() - timeRGB >= (30000 + RtimeRGB))
  {
    B.rDcycle.dCycle = 0;
    B.gDcycle.dCycle = 0;
    B.bDcycle.dCycle = 1;
  }
  else if (millis() - timeRGB >= (20000 + RtimeRGB))
  {
    B.rDcycle.dCycle = 0;
    B.gDcycle.dCycle = 1;
    B.bDcycle.dCycle = 0;
  }
  else if (millis() - timeRGB >= (10000 + RtimeRGB))
  {
    B.rDcycle.dCycle = 1;                // Absolute value of sin function
    B.gDcycle.dCycle = 0;                // Absolute value of sin function
    B.bDcycle.dCycle = 0;                // Absolute value of sin function
  }
}
void loop() // Main loop
{
  val = analogRead(analogPin);    // read the input pin
  //Serial.print("Pot_Value: ");Serial.println(val);
  //print_RGBValues();   // Debug print RGB sin values routine
  if (val >= 0 && val < 100)
  {
    RGBloop();           // Cycle Red, Green, Blue
    writeLEDsOn();       // Write the cycle routine RGB values to the LEDs
  }
  if (val >= 100 && val < 200)
  {
    sinloop();           // Randomly increment RGB color values based on a sin function
    writeLEDsOn();       // Write the cycle routine RGB values to the LEDs
  }
  if (val >= 200 && val < 1024)
  {
    //Ligts Out
    writeLEDsOff();       // Write the cycle routine RGB values to the LEDs
  }
}
//delay(20);

/*
   ----------------------------------------
      Quicksort Algorithm
   ----------------------------------------
*/

void swap(int num1, int num2)
{
  Delay *temp = delays[num1];
  delays[num1] = delays[num2];
  delays[num2] = temp;
}

int partition(int left, int right, float pivot)
{
  int leftPointer = left - 1;
  int rightPointer = right;

  while (TRUE)
  {

    while (delays[++leftPointer]->dCycle < pivot) {
      //do nothing
    }

    while (rightPointer > 0 && delays[--rightPointer]->dCycle > pivot) {
      //do nothing
    }

    if (leftPointer >= rightPointer) {
      break;
    } else {
//      printf("Item Swapped: %f, %f\n", delays[leftPointer]->dCycle, delays[rightPointer]->dCycle);
//      Serial.print(" item swapped : ");
//      Serial.print(delays[leftPointer]->dCycle);
//      Serial.print(", ");
//      Serial.print(delays[rightPointer]->dCycle);
//      Serial.println("");
      swap(leftPointer, rightPointer);
    }

  }
//  printf("Pivot Swapped: %f, %f\n", delays[leftPointer]->dCycle, delays[right]->dCycle);
//  Serial.print("pivot swapped: ");
//  Serial.print(delays[leftPointer]->dCycle);
//  Serial.print(", ");
//  Serial.println(delays[right]->dCycle);
  swap(leftPointer, right);
  return leftPointer;
}

void quickSort(int left, int right)
{
  if (right - left <= 0) {
    return;
  } else {
    float pivot = delays[right]->dCycle;
    int partitionPoint = partition(left, right, pivot);
    quickSort(left, partitionPoint - 1);
    quickSort(partitionPoint + 1, right);
  }
}

void displayDCycles() 
{
  Serial.print("[");
  for (int i = 0; i < NUM_PINS; i++) 
  {
    Serial.print(delays[i]->dCycle);
  }
  Serial.println(" ]");
}

void printline(int count) 
{
  for (int i = 0; i < count - 1; i++) 
  {
    Serial.print("=");
  }
  Serial.println("=");
}
