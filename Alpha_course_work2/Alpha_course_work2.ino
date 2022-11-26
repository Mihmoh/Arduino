#define PIN_LEDR 13
#define PIN_LEDY 12
#define PIN_LEDG 11
#define PIN_LEDB 9
#define PIN_PHOTO_SENSORL A0
#define PIN_PHOTO_SENSORC A1
#define PIN_PHOTO_SENSORR A2
#define in1 4
#define in2 5
#define in3 6
#define in4 7

#include <Wire.h>
#include <LiquidCrystal_I2C.h>
LiquidCrystal_I2C lcd(0x3F, 16, 2);

byte CharMount[] = {
  B00000,
  B00000,
  B00000,
  B00100,
  B01110,
  B11111,
  B11111,
  B11111
};

byte CharPlain[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B11111,
  B11111,
  B11111
};

byte CharPit[] = {
  B00000,
  B00000,
  B00000,
  B00000,
  B00000,
  B10001,
  B11011,
  B11111
};

int dl = 5;
const int PIN_BUTTON = 10;
boolean buttonFlag = false;
boolean buttonPressed = 0;
boolean buttonRealeased = 0;
int buttonState = 1;
int mounts = 0;
int pits = 0;
int start = 0;
int finish = 0;
int counter = 0;
int valL = 0;
int valC = 0;
int valR = 0;

void setup() {
  Serial.begin(9600);
  pinMode(PIN_LEDR, OUTPUT);
  pinMode(PIN_LEDY, OUTPUT);
  pinMode(PIN_LEDG, OUTPUT);
  pinMode(PIN_LEDB, OUTPUT);
  pinMode(in1, OUTPUT);
  pinMode(in2, OUTPUT);
  pinMode(in3, OUTPUT);
  pinMode(in4, OUTPUT);
  pinMode(PIN_BUTTON, INPUT_PULLUP);
  lcd.init();
  lcd.backlight();
  lcd.createChar(0, CharMount);
  lcd.createChar(1, CharPlain);
  lcd.createChar(2, CharPit);
  lcd.setCursor(0, 0);
  lcd.print("Push button");
  lcd.setCursor(0, 1);
  lcd.print("to begin");
  lcd.home();
  lcd.setCursor(0, 1);
}

void loop() {

  buttonState = digitalRead(PIN_BUTTON);

  if (buttonPressed == 0 && buttonState == 0)
  {
    buttonPressed = 1;
  }

  if (buttonPressed == 1 && buttonState == 1)
  {
    buttonRealeased = 1;
  }

  if (buttonPressed == 1 && buttonRealeased == 1 && buttonFlag == 0)
  {
    buttonFlag = 1;
    buttonPressed = 0;
    buttonRealeased = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Processing...");
    lcd.setCursor(0, 1);
  }

  if (buttonPressed == 1 && buttonRealeased == 1 && buttonFlag == 1)
  {
    buttonFlag = 0;
    buttonPressed = 0;
    buttonRealeased = 0;
    mounts = 0;
    pits = 0;
    start = 0;
    finish = 0;
    counter = 0;
    valL = 0;
    valC = 0;
    valR = 0;
    lcd.clear();
    lcd.setCursor(0, 0);
    lcd.print("Push button");
    lcd.setCursor(0, 1);
    lcd.print("to begin");
    lcd.home();
    lcd.setCursor(0, 1);
  }

  //Serial.print("buttonState = ");
//  Serial.println(buttonState);
//  Serial.print("buttonPressed = ");
//  Serial.println(buttonPressed);
//  Serial.print("buttonRealeased = ");
//  Serial.println(buttonRealeased);
//  Serial.print("buttonFlag = ");
//  Serial.println(buttonFlag);
  
  if (buttonFlag == true)
  {
    digitalWrite(PIN_LEDB, HIGH);
    start = millis();
    finish = millis();
    while(finish - start <= 3000)
    {
      valL = analogRead(PIN_PHOTO_SENSORL);
      valC = analogRead(PIN_PHOTO_SENSORC);
      valR = analogRead(PIN_PHOTO_SENSORR);
      finish = millis();
    }

    Serial.print("valL  ");
    Serial.println(valL);
    Serial.print("valC  ");
    Serial.println(valC);
    Serial.print("valR  ");
    Serial.println(valR);
    if (valL > 500) {
      digitalWrite(PIN_LEDR, LOW);
    } else {
      digitalWrite(PIN_LEDR, HIGH);
      mounts++;
      lcd.write(0);
    }
    if (valC > 500) {
      digitalWrite(PIN_LEDY, LOW);
    } else {
      digitalWrite(PIN_LEDY, HIGH);
      lcd.write(1);
    }
    if (valR > 500) {
      digitalWrite(PIN_LEDG, LOW);
    } else {
      digitalWrite(PIN_LEDG, HIGH);
      pits++;
      lcd.write(2);
    }

    start = millis();
    finish = millis();
    while(finish - start <= 3000)
    {
      digitalWrite(in1, HIGH); 
      digitalWrite(in2, LOW); 
      digitalWrite(in3, LOW); 
      digitalWrite(in4, HIGH);
      delay(dl);
  
      digitalWrite(in1, HIGH); 
      digitalWrite(in2, HIGH); 
      digitalWrite(in3, LOW); 
      digitalWrite(in4, LOW);
      delay(dl);
  
      digitalWrite(in1, LOW); 
      digitalWrite(in2, HIGH); 
      digitalWrite(in3, HIGH); 
      digitalWrite(in4, LOW);
      delay(dl);
  
      digitalWrite(in1, LOW); 
      digitalWrite(in2, LOW); 
      digitalWrite(in3, HIGH); 
      digitalWrite(in4, HIGH);
      delay(dl);

      finish = millis();
    }

    counter++;    
  }
  else
  {
    digitalWrite(PIN_LEDB, LOW);
  }

  if(counter == 15)
  {
    digitalWrite(PIN_LEDR, LOW);
    digitalWrite(PIN_LEDY, LOW);
    digitalWrite(PIN_LEDG, LOW);
    lcd.setCursor(0,0);
    lcd.print("Final surface: ");
    while(finish - start <= 10000)
    {
      finish = millis();    
    }
    lcd.clear();
    lcd.setCursor(0,0);
    lcd.print("Mounts: ");
    lcd.print(String(mounts));
    lcd.setCursor(0,1);
    lcd.print("Pits: ");
    lcd.print(String(pits));
    start = millis();
    finish = millis();
    while(finish - start <= 5000)
    {
      finish = millis();    
    }
    if(mounts > 0 || pits > 0)
    {
      while(buttonFlag)
      {
        lcd.setCursor(0,0);
        lcd.print("The surface");
        lcd.setCursor(0,1);
        lcd.print("is rough");
        buttonState = digitalRead(PIN_BUTTON);
        if (buttonPressed == 0 && buttonState == 0)
        {
          buttonPressed = 1;
        }
      
        if (buttonPressed == 1 && buttonState == 1)
        {
          buttonRealeased = 1;
        }
      
        if (buttonPressed == 1 && buttonRealeased == 1 && buttonFlag == 0)
        {
          buttonFlag = 1;
          buttonPressed = 0;
          buttonRealeased = 0;
        }
      
        if (buttonPressed == 1 && buttonRealeased == 1 && buttonFlag == 1)
        {
          buttonFlag = 0;
          buttonPressed = 0;
          buttonRealeased = 0;
        }
      }
    }
    else
    {
       while(buttonFlag)
      {
        lcd.setCursor(0,0);
        lcd.print("The surface");
        lcd.setCursor(0,1);
        lcd.print("is smooth");
        buttonState = digitalRead(PIN_BUTTON);
        if (buttonPressed == 0 && buttonState == 0)
        {
          buttonPressed = 1;
        }
      
        if (buttonPressed == 1 && buttonState == 1)
        {
          buttonRealeased = 1;
        }
      
        if (buttonPressed == 1 && buttonRealeased == 1 && buttonFlag == 0)
        {
          buttonFlag = 1;
          buttonPressed = 0;
          buttonRealeased = 0;
        }
      
        if (buttonPressed == 1 && buttonRealeased == 1 && buttonFlag == 1)
        {
          buttonFlag = 0;
          buttonPressed = 0;
          buttonRealeased = 0;          
        }
      }
    }
  mounts = 0;
  pits = 0;
  start = 0;
  finish = 0;
  counter = 0;
  valL = 0;
  valC = 0;
  valR = 0;
  }
  
  Serial.print("Counter =  ");
  Serial.println(counter);
}
