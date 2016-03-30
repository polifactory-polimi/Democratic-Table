/*
 * Democratic Table
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
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 *
 */


#include <Adafruit_MPR121.h>
#include <RGBFader.h>

#define CYCLE_TOUCH_PIN       0
#define BRIGHTNESS_TOUCH_PIN  7
#define MPR121_ADDR           0x5A

const uint8_t red_pin = 9, green_pin = 10, blue_pin = 11, brightness_cycles = 50;
const RGB rgb_pins = { .red = red_pin, .green = green_pin, .blue = blue_pin };

Adafruit_MPR121 t = Adafruit_MPR121();
RGBFader *f;

void setup(){
  Serial.begin(9600);

  if (!t.begin(MPR121_ADDR)) {
      Serial.println("MPR121 not found.");
      while(1);
    }
  Serial.println("MPR121 found!");

  f = new RGBFader(rgb_pins, RGBFader::rainbowAndWhite, RGBFader::rainbowAndWhiteSize, 255, 0, 1, 255, true, RGBFader::QUADRATIC);
}

void loop(){
  switch(t.touched()){
    case 1<<CYCLE_TOUCH_PIN:
      f->freezeBrightness = true;
      f->freezeColor = false;
      break;

    case 1<<BRIGHTNESS_TOUCH_PIN:
      if (f->getBrightnessTarget() == 255)
        f->setBrightnessCycle(255, 0, brightness_cycles);
      else
        f->setBrightnessCycle(0, 255, brightness_cycles);

      f->freezeBrightness = false;
      f->freezeColor = true;
      break;

    case 0:
      f->freezeBrightness = true;
      f->freezeColor = true;
      break;
  }

  f->nextStep();
  delay(5);
}