/**
 * Color-Mixer  -  A project to mix colors on a LED strip with the arduino.
 * Copyright (C) 2021  Mastaschief, Rivallix
 * 
 * This program is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <https://www.gnu.org/licenses/>.
*/


#include <Arduino.h>
#include <FastLED.h>
#include <LiquidCrystal_I2C.h>

#define NUM_POTS 3     // Set the number of potentiometers (only 3 because of RGB, but you could add one for the general brightness)
#define POT_PIN_1 A1   // Potentiometer for red value
#define POT_PIN_2 A2   // Potentiometer for green value
#define POT_PIN_3 A3   // Potentiometer for blue value
#define NUM_LEDS 6     // This value sets number of all LEDs
#define NUM_RGB_LEDS 3 // This value only the 3 control LEDs
#define DATA_PIN 3     // Data pin for the LCD display
#define CLOCK_PIN 13   // Clock pin for the LCD display

CRGB leds[NUM_LEDS];
LiquidCrystal_I2C lcd(0x27, 16, 2);

uint8_t pots[NUM_POTS] = { POT_PIN_1, POT_PIN_2, POT_PIN_3 };
uint16_t mapedValues[3];
bool act = false;
bool serialRGBDebug = false; // Set this to true if you want serial RGB output


void clear()
{
  for (int i = 0; i < NUM_LEDS; i++)
  {
    leds[i] = CRGB::Black;
  }

  FastLED.show();
  lcd.clear();
}

uint8_t checkForUint(int test)
{
  if (test > 255)
  {
    test = 255;
  }
  else if (test < 0)
  {
    test = 0;
  }
  return test;
}

void compareValues()
{
  for (uint8_t i = 0; i < 3; i++)
  {
    uint16_t help = map(analogRead(pots[i]), 950, 20, 0, 255);
    help = checkForUint(help);
    if (help < (mapedValues[i] - 2) || help > (mapedValues[i] + 2))
    {
      act = true;
      mapedValues[i] = help;
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
  // Print the RGB Value of the potentiometers (normaly only for debugging)
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
  lcd.print("LED-Mixer V0.2.2");

  delay(3000);

  lcd.clear();

  lcd.print("R:");
  lcd.setCursor(7, 0);
  lcd.print("G:");
  lcd.setCursor(13, 0);
  lcd.print("B:");
}

void loop()
{
  act = false;
  compareValues();

  if (act)
  {
    leds[0].setRGB(0, 0, mapedValues[2]);   // Blue
    leds[1].setRGB(0, mapedValues[1], 0);   // Green
    leds[2].setRGB(mapedValues[0], 0, 0);   // Red
    displayValues();
    
    for (uint8_t i = NUM_RGB_LEDS; i < NUM_LEDS; i++)
    {
      leds[i].setRGB(mapedValues[0], mapedValues[1], mapedValues[2]);
    }

    FastLED.show();
  }
  
  if (serialRGBDebug == true)
  {
    printRGBSerial();
  }

  delay(1);  // Delay for stability
}
