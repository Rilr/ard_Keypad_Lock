#include <Servo.h>
#include <Keypad.h>

const int ROW_NUM = 4;      //four rows
const int COLUMN_NUM = 3;     //three columns

char keys[ROW_NUM][COLUMN_NUM] = {
  {'1','2','3'},
  {'4','5','6'},
  {'7','8','9'},
  {'*','0','#'}
};

byte pin_rows[ROW_NUM] = {2, 3, 4, 5};      //connect to the row pinouts of the keypad
byte pin_column[COLUMN_NUM] = {8, 9, 10};     //connect to the column pinouts of the keypad

Keypad keypad = Keypad( makeKeymap(keys), pin_rows, pin_column, ROW_NUM, COLUMN_NUM );

Servo lockServo;      //servo for locking mechanism
int lockPos = 15;     //locked position limit
int unlockPos = 75;     //unlocked position limit
boolean locked = true;

int redLEDPin = 13;     //LED board pin-outs
int greenLEDPin = 12;

const String password = "123456";     //cred goes here
String input_password;

void setup(){
  Serial.begin(9600);
  input_password.reserve(32);     //max input characters is 33 (adjustable)
  pinMode(redLEDPin, OUTPUT);     //LED startup sequence
  pinMode(greenLEDPin, OUTPUT);
  digitalWrite(redLEDPin, HIGH);
  digitalWrite(greenLEDPin, HIGH);
  delay(200);
  digitalWrite(redLEDPin, LOW);
  digitalWrite(greenLEDPin, LOW);
  lockServo.attach(7);      //maps servo data pin to the board
  lockServo.write(lockPos);     //Move servo into locked position
  Serial.println("Enter PIN: ");
}

void loop(){
  char key = keypad.getKey();

  if (key){
    Serial.println(key);      //display key and LED keypress
    digitalWrite(greenLEDPin, HIGH);
    delay(10);
    digitalWrite(greenLEDPin, LOW);
    delay(10);

    if(key == '*') {
      input_password = "";      //clears stored creds
      lockServo.write(lockPos);     //set back to locked
      locked = true;
      digitalWrite(redLEDPin, HIGH);      //LED reset indication
      delay(250);
      digitalWrite(redLEDPin, LOW);
      delay(50);
      digitalWrite(redLEDPin, HIGH);
      delay(100);
      digitalWrite(redLEDPin, LOW);
      delay(10);

    } else if(key == '#') {     //button to start password check 
      if(password == input_password) {
        Serial.println("password is correct");
        if (locked == true){      //unlock procedure
          lockServo.write(unlockPos);
          locked = false;
        }
        digitalWrite(greenLEDPin, HIGH);    //green LED sequence
        delay(200);
        digitalWrite(greenLEDPin, LOW);
        delay(200);
        digitalWrite(greenLEDPin, HIGH);
        delay(200);
        digitalWrite(greenLEDPin, LOW);
        delay(200);
      } else {
        Serial.println("password is incorrect, try again");
        digitalWrite(redLEDPin, HIGH);      //red LED sequence
        delay(200);
        digitalWrite(redLEDPin, LOW);
        delay(200);
        digitalWrite(redLEDPin, HIGH);
        delay(200);
        digitalWrite(redLEDPin, LOW);
        delay(200);
      }

      input_password = "";      //clear input password
    } else {
      input_password += key;      //append new character to input password string
    }
  }
}