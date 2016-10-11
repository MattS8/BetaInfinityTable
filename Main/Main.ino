// Use Arduino/Genuino Mega or Mega 2560 for the Board: selection
// Pulse Width controll of 12 digital outputs for four letters each with an RGB
// LED strand. Variables format for color is Cxy where x is the letter and y is the color.
// CBRed is the color variable for letter B, LED color Red.
// Pin names are Letter followed by the LED color.
// Floating Point color variables are letter followed by color followed by C, ie. BRedC
// Max brightness set by Rmaxon, Gmaxon and Bmaxon variables (0 to 255)
// Potentiometer with 10 detents is read by Analog input A0 which is assigned name: analogPin.
// Potentiometer values are used to select different routines.
//

/**
 * This struct represents a letter.
 *  Each letter has: 
 *    - 3 ints for the pins associated with it
 *    - 3 ints for the color values (R,G,B) associated with it
 */
typedef struct {
  int redPin;       // Pin for red light
  int greenPin;     // Pin for green light
  int bluePin;      // Pin for blue light
  int redVal;       // Color value for red
  int greenVal;     // Color value for green
  int blueVal;      // Color value for blue
} LETTER;

LETTER B = {12, 13, 11, 0, 0, 0};   // B has red pin = 12, green pin = 13, blue pin = 11
LETTER E = {9, 10, 8, 0, 0, 0};     // E has red pin = 9, green pin = 10, blue pin = 8
LETTER T = {6, 7, 5, 0, 0, 0};      // T has red pin = 6, green pin = 7, blue pin = 5
LETTER A = {3, 4, 2, 0, 0, 0};      // A has red pin = 3, green pin = 4, blue pin = 2

int analogPin = A0; // Read voltage from pot to select routine to run

int cycl[12];       // R,G,B for B,E,T,A
int index = 0;      //

int y = 0;          //
int dly = 250;      // Delay variable
float BRedC;        // Temporary variable
float BGreenC;      // Temporary variable
float BBlueC;       // Temporary variable
float ERedC;        // Temporary variable
float EGreenC;      // Temporary variable
float EBlueC;       // Temporary variable
float TRedC;        // Temporary variable
float TGreenC;      // Temporary variable
float TBlueC;       // Temporary variable
float ARedC;        // Temporary variable
float AGreenC;      // Temporary variable
float ABlueC;       // Temporary variable
float incR = 0;     // Incremental value for Red cycle, one for all four letters
float incG = 0;     // Incremental value for Green cycle, one for all four letters
float incB = 0;     // Incremental value for Blue cycle, one for all four letters
float incR1 = .05;  // Incremental value for Red cycle, one for all four letters
float incG1 = .04;  // Incremental value for Green cycle, one for all four letters
float incB1 = .03;  // Incremental value for Blue cycle, one for all four letters
float incRa = 0;    // Intermediate variable for random function (currently commented out and not used)
float incGa = 0;    // Intermediate variable for random function (currently commented out and not used)
float incBa = 0;    // Intermediate variable for random function (currently commented out and not used)
int val;            // Analog read 0 to 1023. Used to select which routine to run
/* Old Color Values
int CBRed = 0;      // Color value for BRed
int CBGreen = 0;    // Color value for BGreen
int CBBlue = 0;     // Color value for BBlue
int CERed = 0;      // Color value for ERed
int CEGreen = 0;    // Color value for EGreen
int CEBlue = 0;     // Color value for EBlue
int CTRed = 0;      // Color value for TRed
int CTGreen = 0;    // Color value for TGreen
int CTBlue = 0;     // Color value for TBlue
int CARed = 0;      // Color value for ARed
int CAGreen = 0;    // Color value for AGreen
int CABlue = 0;     // Color value for ABlue
*/
int maxon = 75;     // Max brightness used in Random(maxon)
int tmp_maxon = 70;  // Temporary storage of maxon
int Rmaxon = 75;    // Max brightness for all Red LEDs
int Gmaxon = 75;    // Max brightness for all Green LEDs
int Bmaxon = 75;    // Max brightness for all Blue LEDs
unsigned long timeRGB;  // Time variable for RGB parameter randomizer
unsigned long RtimeRGB; // Random portion of the timeout to randomize the RGB color cycle parameters
unsigned long tmp;  // Temporary variable used in random number generator

void setup()
{
  incR = random(1000, 100) / 1000; // Randomize the starting point for Red intensity
  incG = random(1000, 100) / 1000; // Randomize the starting point for Green intensity
  incB = random(1000, 100) / 1000; // Randomize the starting point for Blue intensity
  RtimeRGB = millis();        // Set RtimeRGB to current time, one timer for all three colors
  timeRGB = 10000;            // 10 seconds to start with
  Serial.begin(9600);
  randomSeed(analogRead(1));
  pinMode (B.redPin, OUTPUT);      //
  pinMode (B.greenPin, OUTPUT);    //
  pinMode (B.bluePin, OUTPUT);     //
  pinMode (E.redPin, OUTPUT);      //
  pinMode (E.greenPin, OUTPUT);    //
  pinMode (E.bluePin, OUTPUT);     //
  pinMode (T.redPin, OUTPUT);      //
  pinMode (T.greenPin, OUTPUT);    //
  pinMode (T.bluePin, OUTPUT);     //
  pinMode (A.redPin, OUTPUT);      //
  pinMode (A.greenPin, OUTPUT);    //
  pinMode (A.bluePin, OUTPUT);     //
  analogWrite(B.redPin, 0);        // set Red B 0
  analogWrite(B.greenPin, 0);      // set Green B 0
  analogWrite(B.bluePin, 0);       // set Bllue B 0
}


void  print_RGBValues()
// Print out values for Colors
{
  //Serial.print(" B Red: ");Serial.print(CBRed);Serial.print("\t");Serial.print("B Green: ");Serial.print(CBGreen);Serial.print("\t");Serial.print("B Blue: ");Serial.print(CBBlue);
  //Serial.print(" B Green: ");Serial.print(DcycleBG);//Serial.print("\t");Serial.print("E Green: ");Serial.print(DcycleEG);Serial.print("\t");Serial.print("E Blue: ");Serial.print(DcycleEB);
  //Serial.print(" B Blue: ");Serial.print(DcycleBB);//Serial.print("\t");Serial.print("E Green: ");Serial.print(DcycleTG);Serial.print("\t");Serial.print("T Blue: ");Serial.print(DcycleTB);
  //Serial.print("A Red: ");Serial.print(DcycleAR);//Serial.print("\t");Serial.print("E Green: ");Serial.print(DcycleAG);Serial.print("\t");Serial.print("A Blue: ");Serial.print(DcycleAB);
  //Serial.print("  timeRGB: ");Serial.print(timeRGB);Serial.print(" RtimeRGB: ");Serial.print(RtimeRGB);//Serial.print(" Current Time: ");Serial.print(millis());
  //Serial.print("  incR: ");Serial.print(incR);Serial.print(" incR1: ");Serial.print(incR1);Serial.print(" incB: ");Serial.print(incB);Serial.print(" incB1: ");Serial.print(incB1);Serial.print(" incG: ");Serial.print(incG);Serial.print(" incG1: ");Serial.print(incG1);
  //Serial.println();
}

void sinloop()
// Randomly increment RGB color values based on a sin function
{
  incR = incR + incR1;                          // Increment BRed increment value by incR1 value (typically .01)
  BRedC = (sin(incR) + 1) / 2;                  // sin function (offset to range 0 to 1)
  B.redVal = map(BRedC * 99, 0, 100, 2, Rmaxon);   // Normalize sin values and map to new range
  incG = incG + incG1;                          // Increment BGreen increment value by incG1 value (typically .01)
  BGreenC = (sin(incG) + 1) / 2;                // sin function (offset to range 0 to 1)
  B.greenVal = map(BGreenC * 99, 0, 100, 2, Gmaxon); // Normalize sin values and map to new range
  incB = incB + incB1;                          // Increment BBlue increment value by incB1 value (typically .01)
  BBlueC = (sin(incB) + 1) / 2;                 // Absolute value of sin function
  B.blueVal = map(BBlueC * 99, 0, 100, 2, Bmaxon); // Normalize sin values and map to new range
  // Since there is only one set of inc variables I made the other three letters E, T & A = to B
  E.redVal = B.redVal;
  T.redVal = B.redVal;
  A.redVal = B.redVal;
  E.greenVal = B.greenVal;
  T.greenVal = B.greenVal;
  A.greenVal = B.greenVal;
  E.blueVal = B.blueVal;
  T.blueVal = B.blueVal;
  A.blueVal = B.blueVal;
  //delay(10);
}

void RGBloop (int val, int dly1)
// Change from Red to Green to Blue. maxon needs to be set to 1 for this routine
{
  if (millis() - timeRGB >= (0))     // 0 seconds into cycle
  {
    BRedC = 1; BGreenC = 0; BBlueC = 0; //
    ERedC = 1; EGreenC = 0; EBlueC = 0; //
    TRedC = 1; TGreenC = 0; TBlueC = 0; //
    ARedC = 1; AGreenC = 0; ABlueC = 0; //
  }
  //goto skipit;
  if (millis() - timeRGB >= (dly1))     // 5 seconds into cycle
  {
    BRedC = 0; BGreenC = 1; BBlueC = 0; //
    ERedC = 0; EGreenC = 1; EBlueC = 0; //
    TRedC = 0; TGreenC = 1; TBlueC = 0; //
    ARedC = 0; AGreenC = 1; ABlueC = 0; //
  }
  if (millis() - timeRGB >= (2 * dly1))   // 10 seconds into cycle
  {
    BRedC = 0; BGreenC = 0; BBlueC = 1; //
    ERedC = 0; EGreenC = 0; EBlueC = 1; //
    TRedC = 0; TGreenC = 0; TBlueC = 1; //
    ARedC = 0; AGreenC = 0; ABlueC = 1; //
  }
skipit:
  if (millis() - timeRGB >= (3 * dly1)) // Time to re-eandomize RtimeRGB & RtimeRGB
  {
    //RtimeRGB=random(10000);           // Randomize RtimeRGB
    timeRGB = millis();               // Set timeRGB to now
  }
  MAP_0_1();  // Map to PWM range (0 to maxon)
}

void RGBCycle (int val)  // Sequence through Letters and colors
// Change from Red to Green to Blue
{
  for (index = 1; index < 13; index++)       // Increment from 1 to 12
  {
    tmp = random(0, maxon);
    cycl[index] = tmp;
  }
  {
    BRedC = cycl[1]; BGreenC = cycl[2]; BBlueC = cycl[3]; //
    ERedC = cycl[4]; EGreenC = cycl[5]; EBlueC = cycl[6]; //
    TRedC = cycl[7]; TGreenC = cycl[8]; TBlueC = cycl[8]; //
    ARedC = cycl[10]; AGreenC = cycl[11]; ABlueC = cycl[12]; //
    MAP_0_1();
  }
}

void ramp(int val1)
// Ramp all four letters Red, Green, then Blue from 1 to maxon by color
{
  for (B.redVal = 1; B.redVal < Rmaxon; B.redVal++)       // Ramp from 1 to Rmaxon
  {
    analogWrite(B.redPin, B.redVal);
    analogWrite(E.redPin, B.redVal);
    analogWrite(T.redPin, B.redVal);
    analogWrite(A.redPin, B.redVal);
    delay (75);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      return;
    }
  }
  analogWrite(B.redPin, 0);
  analogWrite(E.redPin, 0);
  analogWrite(T.redPin, 0);
  analogWrite(A.redPin, 0);

  for (B.greenVal = 1; B.greenVal < Gmaxon; B.greenVal++) // Ramp from 1 to Gmaxon
  {
    analogWrite(B.greenPin, B.greenVal);
    analogWrite(E.greenPin, B.greenVal);
    analogWrite(T.greenPin, B.greenVal);
    analogWrite(A.greenPin, B.greenVal);
    delay (75);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      return;
    }
  }
  analogWrite(B.greenPin, 0);
  analogWrite(E.greenPin, 0);
  analogWrite(T.greenPin, 0);
  analogWrite(A.greenPin, 0);

  for (B.blueVal = 1; B.blueVal < Bmaxon; B.blueVal++)    // Ramp from 1 to Bmaxon
  {
    analogWrite(B.bluePin, B.blueVal);
    analogWrite(E.bluePin, B.blueVal);
    analogWrite(T.bluePin, B.blueVal);
    analogWrite(A.bluePin, B.blueVal);
    delay (75);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      return;
    }
  }
  analogWrite(B.bluePin, 0);
  analogWrite(E.bluePin, 0);
  analogWrite(T.bluePin, 0);
  analogWrite(A.bluePin, 0);
}

void Random_RGB(int val1)
// Write random Red, Green, Blue to B,E,T,A
// maxon needs to be set to 1 for this routine
{
  int b[3], e[3], t[3], a[3];
  for (index = 0; index < 3; index++) // Clear out  BETA array
  {
    b[index] = 0;
    e[index] = 0;
    t[index] = 0;
    a[index] = 0;
  }
  index = random(0, 3); // Random number between 1 and 3 inclusive
  //Serial.print("Index: ");Serial.println(index);
  b[index] = 1;
  B.redVal = b[0]; B.greenVal = b[1]; B.blueVal = b[2];
  index = random(0, 3);
  e[index] = 1;
  E.redVal = e[0]; E.greenVal = e[1]; E.blueVal = e[2];
  index = random(0, 3);
  t[index] = 1;
  T.redVal = t[0]; T.greenVal = t[1]; T.blueVal = t[2];
  index = random(0, 3);
  a[index] = 1;
  A.redVal = a[0]; A.greenVal = a[1]; A.blueVal = a[2];
  MAP_0_1();  // Map to PWM range (0 to maxon)
}

void BETA_Seq_G(int val1)
// Sequence B BE BET BETA Green, Then BETA ETA TA A
// maxon needs to be set to 1 for this routine
{
  for (y = 0; y < 2; y++) // two loops 0 and 1
  {
    BRedC = 0; BGreenC = 0; BBlueC = 0; ERedC = 0; EGreenC = 0; EBlueC = 0; TRedC = 0; TGreenC = 0; TBlueC = 0; ARedC = 0; AGreenC = 0; ABlueC = 0;
    x = y;
    BGreenC = EGreenC = TGreenC = AGreenC = x; // x = 0 or 1 for all off or All On
    x = !y;      // swap 0 for 1 or 1 for 0
    MAP_0_1();
    writeLEDs();  // All on/Off depending on x
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      return;
    }
    BGreenC = x; // B Green
    MAP_0_1();
    writeLEDs();
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      return;
    }
    EGreenC = x; // B E Green
    MAP_0_1();
    writeLEDs();
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      return;
    }
    TGreenC = x; // B E T Green
    MAP_0_1();
    writeLEDs();
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      return;
    }
    AGreenC = x; // B E T A Green
    MAP_0_1();
    writeLEDs();
    delay(dly);
    if (x == 1)
    {
      Blink(val1);
      val = analogRead(analogPin);
      if (val >= val1 + 5 || val < val1 - 5)
      {
        return;
      }
      Blink(val1);
      val = analogRead(analogPin);
      if (val >= val1 + 5 || val < val1 - 5)
      {
        return;
      }
      Blink(val1);
    }
  }
}

void BETA_Seq_R(int val1)
// Sequence B BE BET BETA Red, Then BETA ETA TA A
// maxon needs to be set to 1 for this routine
{

  for (y = 0; y < 2; y++) // two loops 0 and 1
  {
    BRedC = 0; BGreenC = 0; BBlueC = 0; ERedC = 0; EGreenC = 0; EBlueC = 0; TRedC = 0; TGreenC = 0; TBlueC = 0; ARedC = 0; AGreenC = 0; ABlueC = 0;
    x = y;
    BRedC = ERedC = TRedC = ARedC = x; // x = 0 or 1 for all off or All On
    x = !y;      // swap 0 for 1 or 1 for 0
    MAP_0_1();
    writeLEDs();  // All on/Off depending on x
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      return;
    }
    BRedC = x; // B Red
    MAP_0_1();
    writeLEDs();
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      return;
    }
    ERedC = x; // B E Red
    MAP_0_1();
    writeLEDs();
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      return;
    }
    TRedC = x; // B E T Red
    MAP_0_1();
    writeLEDs();
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      return;
    }
    ARedC = x; // B E T A Red
    MAP_0_1();
    writeLEDs();
    delay(dly);
    if (x == 1)
    {
      Blink(val1);
      val = analogRead(analogPin);
      if (val >= val1 + 5 || val < val1 - 5)
      {
        return;
      }
      Blink(val1);
      val = analogRead(analogPin);
      if (val >= val1 + 5 || val < val1 - 5)
      {
        return;
      }
      Blink(val1);
    }
  }
}

void BETA_Seq_B(int val1)
// Sequence B BE BET BETA Blue, Then BETA ETA TA A
// maxon needs to be set to 1 for this routine
{

  for (y = 0; y < 2; y++) // two loops 0 and 1
  {
    BRedC = 0; BGreenC = 0; BBlueC = 0; ERedC = 0; EGreenC = 0; EBlueC = 0; TRedC = 0; TGreenC = 0; TBlueC = 0; ARedC = 0; AGreenC = 0; ABlueC = 0; // All Off
    x = y;
    BBlueC = EBlueC = TBlueC = ABlueC = x; // x = 0 or 1 for all off or All On
    x = !y;      // swap 0 for 1 or 1 for 0
    MAP_0_1();
    writeLEDs();  // All on/Off depending on x
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      break;
    }
    BBlueC = x; // B Blue
    MAP_0_1();
    writeLEDs();
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      break;
    }
    EBlueC = x; // B E Blue
    MAP_0_1();
    writeLEDs();
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      break;
    }
    TBlueC = x; // B E T Blue
    MAP_0_1();
    writeLEDs();
    delay(dly);
    val = analogRead(analogPin);
    if (val >= val1 + 5 || val < val1 - 5)
    {
      break;
    }
    ABlueC = x; // B E T A Blue
    MAP_0_1();
    writeLEDs();
    delay(dly);
    if (x == 1)
    {
      Blink(val1);
      val = analogRead(analogPin);
      if (val >= val1 + 5 || val < val1 - 5)
      {
        break;
      }
      Blink(val1);
      val = analogRead(analogPin);
      if (val >= val1 + 5 || val < val1 - 5)
      {
        break;
      }
      Blink(val1);
    }
  }
}

void Pong_Blue(int val1)
// Sequence B E T A T E B
// maxon needs to be set to 1 for this routine
{
  BRedC = 0; BGreenC = 0; BBlueC = 0; ERedC = 0; EGreenC = 0; EBlueC = 0; TRedC = 0; TGreenC = 0; TBlueC = 0; ARedC = 0; AGreenC = 0; ABlueC = 0; // All Off

  MAP_0_1();
  writeLEDs();  // All on/Off depending on x
  //delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  EBlueC = 0; // E Blue off
  BBlueC = 1; // B Blue on
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  EBlueC = 1; // E Blue on
  BBlueC = 0; // B Blue off
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  TBlueC = 1; // T Blue on
  EBlueC = 0; // E Blue off
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  ABlueC = 1; // A Blue on
  TBlueC = 0; // T Blue off
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  ABlueC = 0; // A Blue off
  TBlueC = 1; // T Blue on
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  TBlueC = 0; // T Blue off
  EBlueC = 1; // E Blue on
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 2)
  {
    return;
  }
  EBlueC = 0; // E Blue off
  BBlueC = 1; // B Blue on
  MAP_0_1();
  writeLEDs();
  //delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 2)
  {
    return;
  }
}

void Pong_Green(int val1)
// Sequence B E T A T E B
// maxon needs to be set to 1 for this routine
{
  BRedC = 0; BGreenC = 0; BBlueC = 0; ERedC = 0; EGreenC = 0; EBlueC = 0; TRedC = 0; TGreenC = 0; TBlueC = 0; ARedC = 0; AGreenC = 0; ABlueC = 0; // All Off

  MAP_0_1();
  writeLEDs();  // All on/Off depending on x
  //delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  EGreenC = 0; // E Green off
  BGreenC = 1; // B Green on
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  EGreenC = 1; // E Green on
  BGreenC = 0; // B Green off
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  TGreenC = 1; // T Green on
  EGreenC = 0; // E Green off
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  AGreenC = 1; // A Green on
  TGreenC = 0; // T Green off
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  AGreenC = 0; // A Green off
  TGreenC = 1; // T Green on
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  TGreenC = 0; // T Green off
  EGreenC = 1; // E Green on
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 2)
  {
    return;
  }
  EGreenC = 0; // E Green off
  BGreenC = 1; // B Green on
  MAP_0_1();
  writeLEDs();
  //delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 2)
  {
    return;
  }
}

void Pong_Red(int val1)
// Sequence B E T A T E B
// maxon needs to be set to 1 for this routine
{
  BRedC = 0; BGreenC = 0; BBlueC = 0; ERedC = 0; EGreenC = 0; EBlueC = 0; TRedC = 0; TGreenC = 0; TBlueC = 0; ARedC = 0; AGreenC = 0; ABlueC = 0; // All Off

  MAP_0_1();
  writeLEDs();  // All on/Off depending on x
  //delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  ERedC = 0; // E Red off
  BRedC = 1; // B Red on
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  ERedC = 1; // E Red on
  BRedC = 0; // B Red off
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  TRedC = 1; // T Red on
  ERedC = 0; // E Red off
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  ARedC = 1; // A Red on
  TRedC = 0; // T Red off
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  ARedC = 0; // A Red off
  TRedC = 1; // T Red on
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  TRedC = 0; // T Red off
  ERedC = 1; // E Red on
  MAP_0_1();
  writeLEDs();
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 2)
  {
    return;
  }
  ERedC = 0; // E Red off
  BRedC = 1; // B Red on
  MAP_0_1();
  writeLEDs();
  //delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 2)
  {
    return;
  }
}
void Blink(int val1)
{
  // Get store the current values of BETA colors in an array and turn all off.
  // Then back to their previous values using dly
  cycl[1] = B.redVal;  // Store current RGB values
  cycl[2] = B.greenVal;
  cycl[3] = B.blueVal;
  cycl[4] = E.redVal;
  cycl[5] = E.greenVal;
  cycl[6] = E.blueVal;
  cycl[7] = T.redVal;
  cycl[8] = T.greenVal;
  cycl[9] = T.blueVal;
  cycl[10] = A.redVal;
  cycl[11] = A.greenVal;
  cycl[12] = A.blueVal;
  writeLEDsOff();   // All off
  delay(dly);
  val = analogRead(analogPin);
  if (val >= val1 + 5 || val < val1 - 5)
  {
    return;
  }
  B.redVal = cycl[1];  // Restore previous RGB values
  B.greenVal = cycl[2];
  B.blueVal = cycl[3];
  E.redVal = cycl[4];
  E.greenVal = cycl[5];
  E.blueVal = cycl[6];
  T.redVal = cycl[7];
  T.greenVal = cycl[8];
  T.blueVal = cycl[9];
  A.redVal = cycl[10];
  A.greenVal = cycl[11];
  A.blueVal = cycl[12];
  writeLEDs();      // Write previous RGB values
  delay(dly);

}
void MAP_0_1()
// Remap color variables from 0,maxon to 0,(Rmaxon, Gmaxon, Bmaxon) values
{
  B.redVal = map(BRedC, 0, maxon, 0, Rmaxon);     // Map to PWM range (0 to maxon)
  B.greenVal = map(BGreenC, 0, maxon, 0, Gmaxon); // Map to PWM range (0 to maxon)
  B.blueVal = map(BBlueC, 0, maxon, 0, Bmaxon);   // Map to PWM range (0 to maxon)
  E.redVal = map(ERedC, 0, maxon, 0, Rmaxon);     // Map to PWM range (0 to maxon)
  E.greenVal = map(EGreenC, 0, maxon, 0, Gmaxon); // Map to PWM range (0 to maxon)
  E.blueVal = map(EBlueC, 0, maxon, 0, Bmaxon);   // Map to PWM range (0 to maxon)
  T.redVal = map(TRedC, 0, maxon, 0, Rmaxon);     // Map to PWM range (0 to maxon)
  T.greenVal = map(TGreenC, 0, maxon, 0, Gmaxon); // Map to PWM range (0 to maxon)
  T.blueVal = map(TBlueC, 0, maxon, 0, Bmaxon);   // Map to PWM range (0 to maxon)
  A.redVal = map(ARedC, 0, maxon, 0, Rmaxon);     // Map to PWM range (0 to maxon)
  A.greenVal = map(AGreenC, 0, maxon, 0, Gmaxon); // Map to PWM range (0 to maxon)
  A.blueVal = map(ABlueC, 0, maxon, 0, Bmaxon);   // Map to PWM range (0 to maxon)
}

void writeLEDs()
// Write each color to each Letter
{
  analogWrite(B.redPin, B.redVal);
  analogWrite(B.greenPin, B.greenVal);
  analogWrite(B.bluePin, B.blueVal);
  analogWrite(E.redPin, E.redVal);
  analogWrite(E.greenPin, E.greenVal);
  analogWrite(E.bluePin, B.blueVal);
  analogWrite(T.redPin, T.redVal);
  analogWrite(T.greenPin, T.greenVal);
  analogWrite(T.bluePin, T.blueVal);
  analogWrite(A.redPin, A.redVal);
  analogWrite(A.greenPin, A.greenVal);
  analogWrite(A.bluePin, A.blueVal);
}

void writeLEDsOn()

// Turn on all LED to maxon variables
{
  // Write all RED LEDs On
  analogWrite(B.redPin, Rmaxon);
  analogWrite(E.redPin, Rmaxon);
  analogWrite(T.redPin, Rmaxon);
  analogWrite(A.redPin, Rmaxon);
  // Write all Green LEDs On
  analogWrite(B.greenPin, Gmaxon);
  analogWrite(E.greenPin, Gmaxon);
  analogWrite(T.greenPin, Gmaxon);
  analogWrite(A.greenPin, Gmaxon);
  // Write all Blue LEDs On
  analogWrite(B.bluePin, Bmaxon);
  analogWrite(E.bluePin, Bmaxon);
  analogWrite(T.bluePin, Bmaxon);
  analogWrite(A.bluePin, Bmaxon);
}

void  writeLEDsOff()
{
  // Write all RED LEDs Off
  analogWrite(B.redPin, 0);
  analogWrite(E.redPin, 0);
  analogWrite(T.redPin, 0);
  analogWrite(A.redPin, 0);
  // Write all Green LEDs Off
  analogWrite(B.greenPin, 0);
  analogWrite(E.greenPin, 0);
  analogWrite(T.greenPin, 0);
  analogWrite(A.greenPin, 0);
  // Write all Blue LEDs Off
  analogWrite(B.bluePin, 0);
  analogWrite(E.bluePin, 0);
  analogWrite(T.bluePin, 0);
  analogWrite(A.bluePin, 0);
}

void loop()
// Main loop
{
  {
    //goto Skip_all;
    val = analogRead(analogPin);    // read the input pin. 0, 30, 146, 270, 389, 517, 640, 760, 881, 995, 1023
    //midpoints:  15, 88, 208, 330, 453, 579, 700,820, 938, 1009

    if (val >= 0 && val < 15) //Position 0 Expect val=0
    {
      //Ligts Out
      writeLEDsOff();       // Write the cycle routine RGB values to the LEDs
    }

    if (val >= 15 && val < 88) //Position 1 Expect val=30
    {
      // RGB Loop
      // Change from Red to Green to Blue.
      // maxon needs to be set to 1 for this routine
      Rmaxon = 75;
      Gmaxon = 75;
      Bmaxon = 75;
      dly = 250;
      tmp_maxon = maxon;    // Save maxon to temp variable
      maxon = 1;            // maxon needs to be 1 for this sequence
      RGBloop(val, dly);    // Cycle Red, Green, Blue with delay
      maxon = tmp_maxon;    // Restore maxon to original value
      writeLEDs();          // Write the cycle routine RGB values to the LEDs
    }

    if (val >= 88 && val < 208) //Position 2 Expect val=146
    {
      // Sin Loop
      // Randomly increment RGB color values based on a sin function
      Rmaxon = 75;
      Gmaxon = 75;
      Bmaxon = 75;
      sinloop();            // Randomly increment RGB color values based on a sin function
      writeLEDs();          // Write the cycle routine RGB values to the LEDs
    }

    if (val >= 208 && val < 330) //Position 3 Expect val=270
    {
      //Ramp
      // Ramp all four letters Red, Green, then Blue from 1 to maxon by color
      Rmaxon = 75;
      Gmaxon = 75;
      Bmaxon = 75;
      writeLEDsOff();    // Start with all off
      ramp(val);            // Ramp from 0 to max value and straignt back to 0
    }

    if (val >= 339 && val < 453) //Position 4 Expect val=389
    {
      // RGBCycle
      // Randomly Sequence through colors for each letter
      RGBCycle(val);           // Random cycle RGB BETA
      writeLEDs();          // Write the RGBCycle routine RGB values to the LEDs
      delay(200);           // Delay before getting new values
    }

    if (val >= 453 && val < 579) //Position 5 Expect val=517
    {
      // Random_RGB
      // Write random Red, Green, Blue to B,E,T,A
      // maxon needs to be set to 1 for this routine
      Rmaxon = 75;
      Gmaxon = 75;
      Bmaxon = 75;
      tmp_maxon = maxon;    // Save maxon to temp variable
      maxon = 1;            // maxon needs to be 1 for this sequence
      Random_RGB(val);         //
      maxon = tmp_maxon;    // Restore maxon to original value
      writeLEDs();          // Write the RGBCycle routine RGB values to the LEDs
      delay(750);           // Delay before getting new values
    }

    if (val >= 578 && val < 700) //Position 6 Expect val=640
    {
      // BETA_Seq_G, BETA_Seq_R, BETA_Seq_B
      // Sequence  B, BE, BET, BETA, ETA, TA, A for Green, Blue, Red
      dly = 250;            // set delay between changes
      Rmaxon = 75;
      Gmaxon = 75;
      Bmaxon = 75;
      tmp_maxon = maxon;    // Save maxon to temp variable
      maxon = 1;            // maxon needs to be 1 for this sequence
      BETA_Seq_G(val);         // Sequence B BE BET BETA Green, Then BETA ETA TA A
      val = analogRead(analogPin);
      if (val <= 578 || val > 700) // No longer Position 6
      {
        goto skip_BETA_Seq;
      }
      BETA_Seq_B(val);         // Sequence B BE BET BETA Red, Then BETA ETA TA A
      val = analogRead(analogPin);
      if (val <= 578 || val > 700) // No longer Position 6
      {
        goto skip_BETA_Seq;
      }
      BETA_Seq_R(val);         // Sequence B BE BET BETA Blue, Then BETA ETA TA A
      val = analogRead(analogPin);
      if (val <= 578 || val > 700) // No longer Position 6
      {
        goto skip_BETA_Seq;
      }
      skip_BETA_Seq:
      maxon = tmp_maxon;    // Restore maxon to original value
    }


    if (val >= 700 && val < 820) //Position 7 Expect val=760
    {
      // Pong_Blue Pong_Green Pong_Red
      dly = 75;
      Rmaxon = 75;
      Gmaxon = 75;
      Bmaxon = 150;
      tmp_maxon = maxon;    // Save maxon to temp variable
      maxon = 1;            // maxon needs to be 1 for this sequence
      for (y = 0; y < 5; y++) // 5 loops Blue
      {
        Pong_Blue(val);       // Sequence B E T A T E B
        val = analogRead(analogPin);
        if (val <= 700 || val > 820) // No longer Position 7
        {
          goto skip_Pong_Seq;
        }
      }
      for (y = 0; y < 5; y++) // 5 loops Green
      {
        Pong_Green(val);       // Sequence B E T A T E B
        val = analogRead(analogPin);
        if (val <= 700 || val > 820) // No longer Position 7
        {
          goto skip_Pong_Seq;
        }
      }
      for (y = 0; y < 5; y++) // 5 loops Red
      {
        Pong_Red(val);       // Sequence B E T A T E B
        val = analogRead(analogPin);
        if (val <= 700 || val > 820) // No longer Position 7
        {
          goto skip_Pong_Seq;
        }
      }
      skip_Pong_Seq:
      maxon = tmp_maxon;    // Restore maxon to original value
    }

    if (val >= 820 && val < 938) //Position 8 Expect val=881
    {
      // writeLEDsOff - Ligts Out
      writeLEDsOff();       // All LEDs off
    }
    if (val >= 938 && val < 1009) //Position 9 Expect val=995
    {
      // writeLEDsOff - Ligts Out
      writeLEDsOff();       // All LEDs off
    }
    if (val >= 1009) //Position 10 Expect val=1023
    {
      // writeLEDsOn - Ligts On to maxon values (Rmaxon, Gmaxon, Bmaxon)
      Rmaxon = 100;       // Rangge 0 to 255
      Gmaxon = 100;       // Rangge 0 to 255
      Bmaxon = 100;       // Rangge 0 to 255
      writeLEDsOn();      // Write all LEDs to maxon variables
    }
  }
  Skip_all:
  delay(20);
}
