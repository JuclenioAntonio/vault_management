#include <Keypad.h>
#include "LiquidCrystal.h"
LiquidCrystal lcd(A0,A1,A2,A3,A4,A5);

const byte ROWS = 4; //four rows
const byte COLS = 4; //four columns
char keys[ROWS][COLS] = 
{
  {'1','2','3','A'},
  {'4','5','6','B'},
  {'7','8','9','C'},
  {'S','0','H','D'}
};
byte rowPins[ROWS] = {9, 8, 7, 6}; //connect to the row pinouts of the keypad
byte colPins[COLS] = {5, 4, 3, 2}; //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), rowPins, colPins, ROWS, COLS );

String v_passcode="";

void setup()
{
  Serial.begin(9600);
  lcd.begin(16,2);
  
  lcd.setCursor(0,0);
  lcd.print("JUCLENIO ANTONIO");
  pinMode(13, OUTPUT);

}

void loop()
{
  char key = keypad.getKey();
  
  if (key != NO_KEY)
{
    Serial.println(key); 
     lcd.setCursor(0,1);
     lcd.print(key);


  }
  
  //Tocar o BUZZER
  //tone(13, 200, 100);
  //delay(200);
  
  //Piscar o led
  //digitalWrite(13, HIGH);
  //delay(1000);
  //digitalWrite(13, LOW);
  //delay(1000);
}