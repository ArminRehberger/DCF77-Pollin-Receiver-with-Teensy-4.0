#include <DCF77.h>       //https://github.com/thijse/Arduino-DCF77
#include <TimeLib.h>

#define DCF_PIN 23         // Connection pin to DCF 77 device --> pinMode(DCF_PIN, INPUT);
#define DCF_INTERRUPT 23	 // Interrupt associated with pin. Must be the same then DCF_PIN. --> attachInterrupt(DCF_INTERRUPT, int0handler, CHANGE);

//time_t time;
DCF77 DCF = DCF77(DCF_PIN, DCF_INTERRUPT, true); // DCF pin, DCF interrupt pin, OnRisingFlank

void setup()
{
  Serial.begin(9600);
  pinMode(LED_BUILTIN, OUTPUT); // LED on board
 
  delay(2000);  
  Serial.println("1   - binary 1 corresponding to long pulse");
  Serial.println("0   - binary 0 corresponding to short pulse");
  Serial.println("BF  - Buffer is full at end of time-sequence. This is good");
  Serial.println("EoB - Buffer is full before at end of time-sequence");
  Serial.println("EoM - Buffer is not yet full at end of time-sequence");
  DCF.Start();
}

void loop()
{
  static unsigned long previousMillis = 0;
  static unsigned long currentMillis = 0;
  time_t DCFtime;
  bool DCFPinInput ;

  currentMillis = millis();
  if(currentMillis >= previousMillis)
  {
    previousMillis = currentMillis + 1000;
    DCFtime = DCF.getTime();
    if(DCFtime!=0)
    {
      setTime(DCFtime);
      Serial.print(" DCF: ");    
      digitalClockDisplay(DCFtime);
    }	
    else
    {
      Serial.print(" running: ");
      digitalClockDisplay(now());
    }
  }

  // DFC input pin to LED
  DCFPinInput = digitalRead(DCF_PIN);
  digitalWrite(LED_BUILTIN, DCFPinInput);
}

void digitalClockDisplay(time_t _time)
{
  tmElements_t tm;   
  breakTime(_time, tm);

  Serial.print("Time: ");
  Serial.print(tm.Hour);
  Serial.print(":");
  printDigits(tm.Minute);
  Serial.print(":");
  printDigits(tm.Second);
  Serial.print(" Date: ");
  Serial.print(tm.Day);
  Serial.print(".");
  Serial.print(tm.Month);
  Serial.print(".");
  Serial.println(tm.Year+1970);
}

void printDigits(int digits)
{
  // utility function for digital clock display: prints preceding colon and leading 0
  Serial.print(":");
  if(digits < 10)
    Serial.print('0');
  Serial.print(digits);
}


