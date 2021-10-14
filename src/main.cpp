/**
 * LED-RGB-Mixer for Arduino
 * Version 0.2.0
*/


#include <Arduino.h>
#include <FastLED.h>
#include <LiquidCrystal_I2C.h>

#define POT_PIN_1 A1
#define POT_PIN_2 A2
#define POT_PIN_3 A3
#define NUM_POTS 3
#define NUM_LEDS 4                                    // This value sets number of all LEDs
#define NUM_RGB_LEDS 3                                // This value only the 3 control LEDs
#define DATA_PIN 3
#define CLOCK_PIN 13

CRGB leds[NUM_LEDS];
LiquidCrystal_I2C lcd(0x27, 16, 2);

uint8_t pots[NUM_POTS] = {POT_PIN_1, POT_PIN_2, POT_PIN_3};
uint16_t values[NUM_POTS];
int mapedValues[NUM_POTS];

boolean serialRGBDebug = false;                        // Set this to true if you want serial RGB output


void clear()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }

  lcd.clear();
}

void getValues()
{
  for (uint8_t i = 0; i < NUM_POTS; i++)
  {
    values[i] = analogRead(pots[i]);

    mapedValues[i] = map(values[i], 15, 820, 0, 255);

    if (mapedValues[i] > 255)
    {
      mapedValues[i] = 255;
    } else if (mapedValues[i] < 0)
    {
      mapedValues[i] = 0;
    }
  }
}

void displayValues()
{
  for (uint8_t i = 0; i < 3; i++)
  {
    switch (i)
    {
      case 0:
        lcd.setCursor(0, 1);
        lcd.print("   ");
        lcd.setCursor(0, 1);
        break;

      case 1:
        lcd.setCursor(7, 1);
        lcd.print("   ");
        lcd.setCursor(7, 1);
        break;

      case 2:
        lcd.setCursor(13, 1);
        lcd.print("   ");
        lcd.setCursor(13, 1);
        break;
    
      default:
        break;
    }

    lcd.print(mapedValues[i]);
  }
}

void printRGBSerial()
{
  // Print the RGB Value of the potentiometers (only for debugging)
  Serial.print("R: ");
  Serial.print(mapedValues[0]);
  Serial.print("\t");
  Serial.print("G: ");
  Serial.print(mapedValues[1]);
  Serial.print("\t");
  Serial.print("B: ");
  Serial.print(mapedValues[2]);
  Serial.print("\n");
}

void setup()
{
  Serial.begin(9600);

  FastLED.addLeds<NEOPIXEL, DATA_PIN>(leds, NUM_LEDS);

  lcd.init();

  clear();

  lcd.backlight();

  lcd.print("Welcome to");
  lcd.setCursor(0, 1);
  lcd.print("LED-Mixer V0.2.0");

  delay(3000);

  lcd.clear();
  FastLED.show();

  lcd.print("R:");
  lcd.setCursor(7, 0);
  lcd.print("G:");
  lcd.setCursor(13, 0);
  lcd.print("B:");

  pinMode(LED_BUILTIN, OUTPUT);
}

void loop()
{
  getValues();
  
  digitalWrite(LED_BUILTIN, HIGH);
  

  leds[0].setRGB(mapedValues[0], 0, 0);
  leds[1].setRGB(0, mapedValues[1], 0);
  leds[2].setRGB(0, 0, mapedValues[2]);


  for (uint8_t i = NUM_RGB_LEDS; i < NUM_LEDS; i++)
  {
    leds[i].setRGB(mapedValues[0], mapedValues[1], mapedValues[2]);
  }

  FastLED.show();

  displayValues();

  if (serialRGBDebug == true) 
  { 
    printRGBSerial(); 
  }

  delay(1);                                             // Delay for stability
}
