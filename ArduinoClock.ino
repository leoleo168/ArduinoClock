//時間模組
#include <DS3232RTC.h>    //http://github.com/JChristensen/DS3232RTC
#include <Time.h>         //http://www.arduino.cc/playground/Code/Time  
#include <Wire.h>         //http://arduino.cc/en/Reference/Wire (included with Arduino IDE)


//7段顯示器+MAX7219
#include "LedControl.h"
LedControl lc=LedControl(7,6,5,1);  //(Datain pin,CLK pin,LOAD pin,模組數量)

//日期與時間陣列
int mydays[7];
int mytime[5];

const int buttonPin = 10;  //日期pin

void setup() {
  Serial.begin(9600);
  //時間模組
  setSyncProvider(RTC.get);   // the function to get the time from the RTC
  if(timeStatus() != timeSet) 
      Serial.println("Unable to sync with the RTC");
  else
      Serial.println("RTC has set the system time");  
    
    
    
    /*
   The MAX72XX is in power-saving mode on startup,
   we have to do a wakeup call
   */
  lc.shutdown(0,false);
  /* Set the brightness to a medium values */
  lc.setIntensity(0,8);
  /* and clear the display */
  lc.clearDisplay(0);

  pinMode(buttonPin, INPUT_PULLUP); //日期pin

}

void loop() {
  digitalClockDisplay();  //Serial用
  //delay(1000);

  
  //讀取時間放入陣列
  mytime[5] = hour()/10;
  mytime[4] = hour()%10;
  mytime[3] = minute()/10;
  mytime[2] = minute()%10;
  mytime[1] = second()/10;
  mytime[0] = second()%10;

  lc.setDigit(0,7,mytime[5],false);
  lc.setDigit(0,6,mytime[4],false);
  lc.setChar(0,5,'-',false);          //文字顯示
  lc.setDigit(0,4,mytime[3],false);
  lc.setDigit(0,3,mytime[2],false);
  lc.setChar(0,2,'-',false);          //文字顯示
  lc.setDigit(0,1,mytime[1],false);
  lc.setDigit(0,0,mytime[0],false);

  //delay(5000);
  //lc.clearDisplay(0);


  //按下開關顯示日期
  if(digitalRead(buttonPin) == LOW){
    lc.clearDisplay(0); //清除畫面

      //讀取日期放入陣列
    mydays[7] = year()/1000;
    mydays[6] = year()/100%10;
    mydays[5] = year()/10%100%10;
    mydays[4] = year()%10;
    mydays[3] = month()/10;
    mydays[2] = month()%10;
    mydays[1] = day()/10;
    mydays[0] = day()%10;

    lc.setDigit(0,7,mydays[7],false);
    lc.setDigit(0,6,mydays[6],false);
    lc.setDigit(0,5,mydays[5],false);
    lc.setDigit(0,4,mydays[4],true);
    lc.setDigit(0,3,mydays[3],false);
    lc.setDigit(0,2,mydays[2],true);
    lc.setDigit(0,1,mydays[1],false);
    lc.setDigit(0,0,mydays[0],false);

    delay(3000);//顯示時間
    lc.clearDisplay(0); //清除畫面
  }


  


}

void digitalClockDisplay(void)  //Serial檢查用
{
    // digital clock display of the time
    Serial.print(hour());
    printDigits(minute());
    printDigits(second());
    Serial.print(' ');
    Serial.print(day());
    Serial.print(' ');
    Serial.print(month());
    Serial.print(' ');
    Serial.print(year()); 
    Serial.println(); 
}

void printDigits(int digits)
{
    // utility function for digital clock display: prints preceding colon and leading 0
    Serial.print(':');
    if(digits < 10)
        Serial.print('0');
    Serial.print(digits);
}
