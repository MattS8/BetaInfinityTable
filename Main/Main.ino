// Pulse Width controll of 12 digital outputs for four letters each with an RGB 
// LED strand Infinity Table
//

struct Pin 
{
  int r;
  int g;
  int b;
};
typedef struct Pin Pin;

Pin bPins = {2, 3, 4};

int BRed=2;             // 
int BGreen=3;           // 
int BBlue=4;            // 
int ERed=5;             // 
int EGreen=6;           // 
int EBlue=7;            // 
int TRed=8;             // 
int TGreen=9;           // 
int TBlue=10;           // 
int ARed=11;            // 
int AGreen=12;          // 
int ABlue=13;           // 
int All_On=1;           // Force Lights On (No Music Present)
int analogPin=0;        // Analog input for loop selector
int val=0;              // Variable to store pot value 0-1023
float Period=1000;      // Period for PWM in micro seconds (1,000=1kHz)
float DcycleBR=.5;      // Duty cycle B Red (between 0 and 1)
float DcycleBG=.5;      // Duty cycle B Green (between 0 and 1)
float DcycleBB=.5;      // Duty cycle B Blue (between 0 and 1)
float DcycleER=.5;      // Duty cycle E Red (between 0 and 1)
float DcycleEG=.5;      // Duty cycle E Green (between 0 and 1)
float DcycleEB=.5;      // Duty cycle E Blue (between 0 and 1)
float DcycleTR=.5;      // Duty cycle T Red (between 0 and 1)
float DcycleTG=.5;      // Duty cycle T Green (between 0 and 1)
float DcycleTB=.5;      // Duty cycle T Blue (between 0 and 1)
float DcycleAR=.5;      // Duty cycle A Red (between 0 and 1)
float DcycleAG=.5;      // Duty cycle A Green (between 0 and 1)
float DcycleAB=.5;      // Duty cycle A Blue (between 0 and 1)
float incR=0;           // Incremental value for Red cycle
float incG=0;           // Incremental value for Green cycle
float incB=0;           // Incremental value for Blue cycle
float incR1=.01;        // Incremental value for Red cycle 
float incG1=.01;        // Incremental value for Green cycle 
float incB1=.01;        // Incremental value for Blue cycle 

float incRa=0;            // Intermediate variable for random function
float incGa=0;            // Intermediate variable for random function
float incBa=0;            // Intermediate variable for random function
int looptime=5000;      // Cycle counter for how long to wait for music off to trigger RGB cycle
int rnd_cnt=0;          // Cycle counter to re-randomize the RGB increment values
int CRed;               // Max values for color organ Red
int CGreen;             // Max values for color organ Green
int CBlue;              // Max values for color organ Blue
unsigned long timeRGB;  // Time variable for RGB parameter randomizer 
unsigned long RtimeRGB; // Random portion of the timeout to randomize the RGB color cycle parameters

void setup()
{
  incR=random(1000,100)/1000; // Randomize the starting point for Red intensity
  incG=random(1000,100)/1000; // Randomize the starting point for Green intensity
  incB=random(1000,100)/1000; // Randomize the starting point for Blue intensity
  RtimeRGB=millis();          // Set RtimeRGB to current time, one timer for all three colors
  timeRGB=10000;              // 10 seconds to start with
 Serial.begin(9600);
 pinMode (BRed, OUTPUT);             // 
 pinMode (BGreen, OUTPUT);           // 
 pinMode (BBlue, OUTPUT);            //
 pinMode (ERed, OUTPUT);             // 
 pinMode (EGreen, OUTPUT);           // 
 pinMode (EBlue, OUTPUT);            //
 pinMode (TRed, OUTPUT);             // 
 pinMode (TGreen, OUTPUT);           // 
 pinMode (TBlue, OUTPUT);            //
 pinMode (ARed, OUTPUT);             // 
 pinMode (AGreen, OUTPUT);           // 
 pinMode (ABlue, OUTPUT);            //
 digitalWrite(BRed,LOW);         // set Red B low
 digitalWrite(BGreen,LOW);       // set Green B low
 digitalWrite(BBlue,LOW);        // set Bllue B low
 digitalWrite(ERed,LOW);         // set Red E low
 digitalWrite(EGreen,LOW);       // set Green E low
 digitalWrite(EBlue,LOW);        // set Bllue E low
 digitalWrite(TRed,LOW);         // set Red T low
 digitalWrite(TGreen,LOW);       // set Green T low
 digitalWrite(TBlue,LOW);        // set Bllue T low
 digitalWrite(ARed,LOW);         // set Red A low
 digitalWrite(AGreen,LOW);       // set Green A low
 digitalWrite(ABlue,LOW);        // set Bllue A low                        
 //analogReference (INTERNAL);  // Internal reference 1.1 V instead of the default of 5 V if input levels are low. 
}

void writeLEDsOn()
// Function to turn on all LED FET drivers based on RGB values
{
  // Write all RED LEDs
  digitalWrite(BRed,HIGH);
  delayMicroseconds (Period*DcycleBR);
  digitalWrite(BRed,LOW);
  delayMicroseconds (Period-(Period*DcycleBR));
  digitalWrite(ERed,HIGH);
  delayMicroseconds (Period*DcycleER);
  digitalWrite(ERed,LOW);
  delayMicroseconds (Period-(Period*DcycleER));
  digitalWrite(TRed,HIGH);
  delayMicroseconds (Period*DcycleTR);
  digitalWrite(TRed,LOW);
  delayMicroseconds (Period-(Period*DcycleTR));
  digitalWrite(ARed,HIGH);
  delayMicroseconds (Period*DcycleAR);
  digitalWrite(ARed,LOW);
  delayMicroseconds (Period-(Period*DcycleAR));

  // Write all Green LEDs
  digitalWrite(BGreen,HIGH);
  delayMicroseconds (Period*DcycleBG);
  digitalWrite(BGreen,LOW);
  delayMicroseconds (Period-(Period*DcycleBG));
  digitalWrite(EGreen,HIGH);
  delayMicroseconds (Period*DcycleEG);
  digitalWrite(EGreen,LOW);
  delayMicroseconds (Period-(Period*DcycleEG));
  digitalWrite(TGreen,HIGH);
  delayMicroseconds (Period*DcycleTG);
  digitalWrite(TGreen,LOW);
  delayMicroseconds (Period-(Period*DcycleTG));
  digitalWrite(AGreen,HIGH);
  delayMicroseconds (Period*DcycleAG);
  digitalWrite(AGreen,LOW);
  delayMicroseconds (Period-(Period*DcycleAG));

  // Write all Blue LEDs
  digitalWrite(BBlue,HIGH);
  delayMicroseconds (Period*DcycleBB);
  digitalWrite(BBlue,LOW);
  delayMicroseconds (Period-(Period*DcycleBB));
  digitalWrite(EBlue,HIGH);
  delayMicroseconds (Period*DcycleEB);
  digitalWrite(EBlue,LOW);
  delayMicroseconds (Period-(Period*DcycleEB));
  digitalWrite(TBlue,HIGH);
  delayMicroseconds (Period*DcycleTB);
  digitalWrite(TBlue,LOW);
  delayMicroseconds (Period-(Period*DcycleTB));
  digitalWrite(ABlue,HIGH);
  delayMicroseconds (Period*DcycleAB);
  digitalWrite(ABlue,LOW);
  delayMicroseconds (Period-(Period*DcycleAB));
}

void  writeLEDsOff()
{
  // Write all RED LEDs Off
  digitalWrite(BRed,LOW);
  digitalWrite(ERed,LOW);
  digitalWrite(TRed,LOW);
  digitalWrite(ARed,LOW);
  // Write all Green LEDs Off
  digitalWrite(BGreen,LOW);
  digitalWrite(EGreen,LOW);
  digitalWrite(TGreen,LOW);
  digitalWrite(AGreen,LOW);
  // Write all Blue LEDs Off
  digitalWrite(BBlue,LOW);
  digitalWrite(EBlue,LOW);
  digitalWrite(TBlue,LOW);
  digitalWrite(ABlue,LOW);
}

void  print_RGBValues()
                            // Print out values for audio channel
{
  Serial.print(" B Red: ");Serial.print(DcycleBR);//Serial.print("\t");Serial.print("B Green: ");Serial.print(DcycleBG);Serial.print("\t");Serial.print("B Blue: ");Serial.print(DcycleBB);
  Serial.print(" B Green: ");Serial.print(DcycleBG);//Serial.print("\t");Serial.print("E Green: ");Serial.print(DcycleEG);Serial.print("\t");Serial.print("E Blue: ");Serial.print(DcycleEB);
  Serial.print(" B Blue: ");Serial.print(DcycleBB);//Serial.print("\t");Serial.print("E Green: ");Serial.print(DcycleTG);Serial.print("\t");Serial.print("T Blue: ");Serial.print(DcycleTB);
  //Serial.print("A Red: ");Serial.print(DcycleAR);//Serial.print("\t");Serial.print("E Green: ");Serial.print(DcycleAG);Serial.print("\t");Serial.print("A Blue: ");Serial.print(DcycleAB);
  Serial.print("  timeRGB: ");Serial.print(timeRGB);Serial.print(" RtimeRGB: ");Serial.print(RtimeRGB);//Serial.print(" Current Time: ");Serial.print(millis());
  Serial.print("  incR: ");Serial.print(incR);Serial.print(" incR1: ");Serial.print(incR1);Serial.print(" incB: ");Serial.print(incB);Serial.print(" incB1: ");Serial.print(incB1);Serial.print(" incG: ");Serial.print(incG);Serial.print(" incG1: ");Serial.print(incG1);
  Serial.println();
}

void sinloop()
// Randomly increment RGB color values based on a sin function
{
incR = incR + incR1;                      // Increment B Red increment value by incR1 value (typically .01)
DcycleBR=abs(sin(incR));                  // Absolute value of sin function
incG = incG + incG1;                      // Increment Green increment value by incG1 value
DcycleBG=abs(sin(incG));                  // Absolute value of sin function
incB = incB + incB1;                      // Increment Blue increment value by incB1 value
DcycleBB=abs(sin(incB));                  // Absolute value of sin function
if (millis() - timeRGB >=(10000 + RtimeRGB))      // 10 seconds minimum, 5 + RtimeRGB maximum (randomize RtimeRGB)
  {
  incR=random(3000)/3000; incRa=random(1,10);incR1 = incRa / 1000 +.001;  // incR range 0.1 to 1, incR1 range is .01 to .11
  incG=random(3000)/3000; incGa=random(1,10);incG1 = incGa / 1000 +.001;  // incG range 0.1 to 1, incG1 range is .01 to .11
  incB=random(3000)/3000; incBa=random(1,10);incB1 = incBa / 1000 +.001;  // incB range 0.1 to 1, incB1 range is .01 to .11
  RtimeRGB=random(10000);timeRGB=millis();
  }
}

void RGBloop()
// Change from Red to Green to Blue
{
if (millis() - timeRGB >=(10000 + RtimeRGB))      // 10 seconds into cycle
  {
    DcycleBR=1;                  // Absolute value of sin function
    DcycleBG=0;                  // Absolute value of sin function
    DcycleBB=0;                  // Absolute value of sin function
  }
if (millis() - timeRGB >=(20000 + RtimeRGB))      // 20 seconds into cycle
  {
    DcycleBR=0;                  // Absolute value of sin function
    DcycleBG=1;                  // Absolute value of sin function
    DcycleBB=0;                  // Absolute value of sin function
  }
if (millis() - timeRGB >=(30000 + RtimeRGB))      // 30 seconds into cycle
  {
    DcycleBR=0;                  // Absolute value of sin function
    DcycleBG=0;                  // Absolute value of sin function
    DcycleBB=1;                  // Absolute value of sin function
  }
 if (millis() - timeRGB >=(40000 + RtimeRGB))    // Time to re-randomize RtimeRGB
  {
    RtimeRGB=random(10000);   // randomize RtimeRGB
    timeRGB=millis();         // Set timeRGB to now
  }
}
void loop()
// Main loop
{
  {
    val = analogRead(analogPin);    // read the input pin
    //Serial.print("Pot_Value: ");Serial.println(val);
    //print_RGBValues();   // Debug print RGB sin values routine
    if (val>=0 && val<100)
    {
      RGBloop();           // Cycle Red, Green, Blue 
      writeLEDsOn();       // Write the cycle routine RGB values to the LEDs
    }
    if (val>=100 && val<200)
    {
      sinloop();           // Randomly increment RGB color values based on a sin function
      writeLEDsOn();       // Write the cycle routine RGB values to the LEDs
    }
    if (val>=200 && val<1024)
    {
       //Ligts Out
      writeLEDsOff();       // Write the cycle routine RGB values to the LEDs  
    }

  }
  //delay(20);
}
