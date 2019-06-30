#include <Arduino.h>

//Sample using LiquidCrystal library
#include <LiquidCrystal.h>

//MQ-3
#include <mq3sensor.h>

#define PIN_MQ3  A1


enum measure_type: uint8_t {
  MQ3_PPM = 0,
  MQ3_MGL,
  MQ3_RATIO
};

void lcd_clear();
void mq3_calibration();
void mq3_print(measure_type mtype);

measure_type curr_mtype;
MQ3sensor mq3n(PIN_MQ3);

/*******************************************************

This program will test the LCD panel and the buttons
Mark Bramwell, July 2010

********************************************************/

// select the pins used on the LCD panel
LiquidCrystal lcd(8, 9, 4, 5, 6, 7);

// define some values used by the panel and buttons
int lcd_key     = 0;
int adc_key_in  = 0;
#define btnRIGHT  0
#define btnUP     1
#define btnDOWN   2
#define btnLEFT   3
#define btnSELECT 4
#define btnNONE   5

// read the buttons
int read_LCD_buttons()
{
 adc_key_in = analogRead(0);      // read the value from the sensor
 // my buttons when read are centered at these valies: 0, 144, 329, 504, 741
 // we add approx 50 to those values and check to see if we are close
 if (adc_key_in > 1000) return btnNONE; // We make this the 1st option for speed reasons since it will be the most likely result
 // For V1.1 us this threshold
 if (adc_key_in < 50)   return btnRIGHT;
 if (adc_key_in < 250)  return btnUP;
 if (adc_key_in < 450)  return btnDOWN;
 if (adc_key_in < 650)  return btnLEFT;
 if (adc_key_in < 850)  return btnSELECT;

 // For V1.0 comment the other threshold and use the one below:
/*
 if (adc_key_in < 50)   return btnRIGHT;
 if (adc_key_in < 195)  return btnUP;
 if (adc_key_in < 380)  return btnDOWN;
 if (adc_key_in < 555)  return btnLEFT;
 if (adc_key_in < 790)  return btnSELECT;
*/


 return btnNONE;  // when all others fail, return this...
}

void setup()
{
  lcd.begin(16, 2);              // start the library
  lcd.setCursor(0,0);
  lcd.print("For calib. press");
  lcd.setCursor(0,1);
  lcd.print("button <Select>");
  delay(3000u);
  mq3n.calibrate();

  lcd_clear();
  lcd.setCursor(0,0);
  lcd.print("Alcohol:");
  curr_mtype = MQ3_MGL;
}

void loop()
{
  lcd_key = read_LCD_buttons();  // read the buttons
  //Init calibration 
  if (lcd_key == btnSELECT) {
    mq3_calibration();
    while (read_LCD_buttons() != btnSELECT) {
      delay(300u);
    }
    lcd_clear();
    lcd.setCursor(0,0);
    lcd.print("Alcohol:");
  }

  if (lcd_key == btnLEFT) {
    if (curr_mtype == MQ3_PPM) curr_mtype = MQ3_RATIO;
    else curr_mtype = (measure_type)((uint8_t)curr_mtype - 1);
  }
  if (lcd_key == btnRIGHT) {
    if (curr_mtype == MQ3_RATIO) curr_mtype = MQ3_PPM;
    else curr_mtype = (measure_type)((uint8_t)curr_mtype + 1);
  }
  if (lcd_key == btnLEFT || lcd_key == btnRIGHT) {
    lcd.setCursor(0,1);
    lcd.print("                ");
  }

  mq3_print(curr_mtype);
  delay(1500u);
}


void lcd_clear()
{
  for( uint8_t i = 0u; i < 2u; i++) {
    lcd.setCursor(0, i);
    lcd.print("                ");
  }
}

void mq3_calibration()
{
  lcd_clear();
  lcd.setCursor(0,0);
  lcd.print("Calib. start");
  lcd.setCursor(0,1);
  lcd.print("Left: ");
  lcd.setCursor(9,1);
  lcd.print("sec");
  
  for(uint8_t i = 60u; i > 0u; i--) {
    lcd.setCursor(6,1);
    if (i < 10) {
      lcd.print("0");
      lcd.setCursor(7,1);
    }
    lcd.print(i);
    delay(1000u);
  }
  mq3n.calibrate();
  lcd_clear();
  lcd.setCursor(0,0);
  lcd.print("Calib. done");
  lcd.setCursor(0,1);
  lcd.print("Ro = ");
  lcd.setCursor(5,1);
  lcd.print( mq3n.getRo() );
}

void mq3_print(measure_type mtype)
{
  lcd.setCursor(0,1);
  switch(mtype)
  {
    case MQ3_PPM:
      lcd.print( mq3n.readAlcoholPpm() );
      lcd.setCursor(8,1);
      lcd.print("PPM");
      break;
    
    case MQ3_MGL:
      lcd.print( mq3n.readAlcoholMgL() );
      lcd.setCursor(8,1);
      lcd.print("mg/L");
      break;
    
    case MQ3_RATIO:
      lcd.print( mq3n.readRatio() );
      lcd.setCursor(8,1);
      lcd.print("[ratio]");
      break;
  }
}
