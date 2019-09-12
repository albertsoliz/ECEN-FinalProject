#include "Wire.h"
#include "RTClib.h"
RTC_DS3231 rtc;

int hour, month, day, year, minute,second=0;
int lasthour, lastmonth,lastday,lastyear,lastminute,lastsecond = 0;
int A4Read, A5Read;
unsigned long timer4, timer5, timeRstCheck;
bool tripChecker4, tripChecker5;
unsigned long timeTracked4, timeTracked5, maxTimeTracked4, maxTimeTracked5 = 0;
String maxTimeTracked4str = "0 ms ";
String maxTimeTracked5str = "0 ms ";
String str, message;
String SetDate;
bool MachineStatus;


//unsigned long lastSample = 0;
//unsigned long sampleSum = 0;
//int sampleCount = 0;
//int sensitivity =.066;
//float vpc = 0.0048828125;
//float offstV = 2715;
//float ccurent;
int fraudTracker;
int TrueMachineState=0;


/////////////////////////////////////////////////
float vpc = .0048828125;  // 5 V/ 4096 points
float vpc12 = 0.001220703125;// 5/4096
float sens = .066;
int sample, out, Vref;
float Voltage, Current;
long lastTime = 0;
long sampleSum = 0;
int sampleCount;
float mean,rmsSample;

//////////////////////////////////

void setup() {
  // put your setup code here, to run once:
Serial.begin(115200);
//Serial.begin(9600);
rtc.begin();
//rtc.adjust(DateTime(F(__DATE__), F(__TIME__)));    
pinMode(P1_4,INPUT);
pinMode(P1_5, INPUT);
pinMode(P1_3,OUTPUT);
pinMode(P1_7, INPUT);
pinMode(P1_6, INPUT);
pinMode(P1_0, INPUT);
Serial.print("L Started");
}



// the goal is to read when an L is sent and then give all the information back
// information
// longest time safety hall sensor was tripped
// longest time power supply hall sensor was tripped
// The current time and date on the RTC
// if any time reprogrammings happened
// the date of next turn off


void loop() {
  // put your main code here, to run repeatedly: 
  //////////////////////////Hall sense timing

if(millis()> lastTime+8)
{
//  sampleSum += sq(analogRead(P1_0)/4 -512); //squares and adds the sample
  sampleSum += sq(analogRead(P1_0) - analogRead(P1_7)); //squares and adds the sample
//  Serial.println(analogRead(P1_0));
//  Serial.println("7");
//  Serial.println(analogRead(P1_7));
//  delay(300);
  sampleCount++;
  lastTime = millis();
}

if(sampleCount>=125)
{
  //Averaging stuff.
 mean = sampleSum/sampleCount;
 rmsSample = sqrt(mean); // take the square root of the mean
// Serial.println("The final RRMSValue is: " + String(rmsSample));
 Voltage = rmsSample*vpc;
// Serial.println("The final Voltage is: " + String(Voltage));
 Current = Voltage/sens;
//Serial.println("The final RMS Amperage is: " + String(Current));
 sampleSum = 0;
 sampleCount = 0;
}



  A4Read = analogRead(P1_4);
  A5Read = analogRead(P1_5);
  if(A4Read<500 & tripChecker4 == false)
{
  timer4 = millis();
  tripChecker4 = true;
}
else if(A4Read<500 & tripChecker4 == true)
{
  timeTracked4 = millis() - timer4;
  if(maxTimeTracked4<timeTracked4)
  {maxTimeTracked4 = timeTracked4;}
}
else if(A4Read>500 & tripChecker4 == true)
{
  timeTracked4 = millis()-timer4;
  tripChecker4 = false;
  if(maxTimeTracked4<timeTracked4)
  {
    maxTimeTracked4 = timeTracked4;
    maxTimeTracked4str = String(maxTimeTracked4);
  }
}
if(maxTimeTracked4>3600000)
{
  maxTimeTracked4 = 3600000;
  maxTimeTracked4str = "over an hour";

}


if(A5Read<500 & tripChecker5 == false)
{
  timer5 = millis();
  tripChecker5 = true;
}
else if(A5Read<500 & tripChecker5 == true)
{
  timeTracked5 = millis() - timer5;
  if(maxTimeTracked5<timeTracked5)
  {maxTimeTracked5 = timeTracked5;}
}
else if(A5Read>500 & tripChecker5 == true)
{
  timeTracked5 = millis()-timer5;
  tripChecker5 = false;
  if(maxTimeTracked5<timeTracked5)
  {
    maxTimeTracked5 = timeTracked5;
    maxTimeTracked5str = String(maxTimeTracked5);
  }
}
if(maxTimeTracked5>3600000)
{
  maxTimeTracked5 =3600000;
  maxTimeTracked4str = String("over an hour");
}
///////////////////////////////// this is where hall sense timing ends


////////////////////////////////////////////////////////////RTC
DateTime now = rtc.now();
hour = (now.hour());
month = (now.month());
day = (now.day());
year= (now.year());
minute = (now.minute());
second = (now.second());

if (digitalRead(P1_6) == HIGH)
{
  rtc.adjust(DateTime(F(__DATE__), F("08:50:00")));    
}
if (compareTimes()== true)
{

}
else 
{
  timeRstCheck++;
}

lastyear = year;
lastmonth = month;
lastday = day;
lasthour = hour;
lastminute = minute;
lastsecond = second;
////////////////////////////////////////////////// this is where rtc ends

//////////////////////////////////////////////// this is where serial coms starts
if(Serial.available()>0)
{
  str = Serial.readString();
  //////////data logging stuff happens
  if(str == "L")
  {
    Serial.print("L");
    Serial.print("Safety snsr off: "); Serial.print(maxTimeTracked4str);Serial.print(" Pwr snsr off: "); Serial.print(maxTimeTracked5str); 
    Serial.print("Today is "); Serial.print(month); Serial.print("-"); Serial.print(day); Serial.print("-"); Serial.print(year);Serial.print("   "); Serial.print(hour); Serial.print(":"); Serial.print(minute);
    Serial.print(" number of time resets:");Serial.print(timeRstCheck);Serial.print(" amps:"); Serial.print(Current);
    //also add date of next turn off
  }
  /// this is where the stopping date comes from
  else if (str.charAt(0) == 'T')
  {
    SetDate= str.substring(1);    
  }
}

//Machine Status
if (maxTimeTracked5 < 10000 && maxTimeTracked4 < 10000 && timeRstCheck==0)
{digitalWrite(P1_3, HIGH);}
else {digitalWrite(P1_3, LOW);}
}




























/// functions
// compares nows time with last time
bool compareTimes()
{
  if (year > lastyear)
  {
    return true;
  }
  else if (year<lastyear)
  {
    return false;
  }
  else 
  {
    if(month > lastmonth)
    {return true;}
    else if(month<lastmonth)
    {return false;}
    else
    {
        if(day>lastday)
        {return true;}
        else if (lastday<day)
        {return false;}
         else
          {
            if(hour>lasthour)
            {return true;}
            else if(hour<lasthour)
            { return false;}
            else
              {
                if(minute>lastminute)
                {return true;}
                else if(minute<lastminute)
                { return false;}
                else
                  {
                    if(second>lastsecond)
                    {return true;}
                    else if(second<lastsecond)
                    { return false;}
                    else
                    {//probably should be true
                      return true;
                    }
                  }
              }
          }
    }
  }
}

         
