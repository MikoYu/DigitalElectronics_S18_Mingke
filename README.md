# Digital Electronics - S18 Mingke

## Week 2 - Homework
### Find an Arduino project
*Find (online) an project of any sort using Arduino that you find interesting. Write a brief description and put a link in the README.md file in your Github repository.*

#### Project: Noisy Jelly
https://pinaffo-pluvinage.com/papier-machine-1/

The project is a "noisy chemistry lab" which let its gamers make jellies on their own and put them on a board so that they can make noise by touching the jellies. The game board is a capacitive sensor that can detect the variations of the jelly's shape, salt concentration, the distance and the strength of the finger contact; it translates these information to amusing audio signals. The fully working prototype was made with Arduino and Max/Msp.

Related project: https://pinaffo-pluvinage.com/jelly-safari/

### Learn about sensors
*Read about 3 of the sensors on [the Adafruit Sensors guide](http://www.ladyada.net/learn/sensors/). Describe (briefly) what you've learned in your Github READ.md file.*

#### Force Sensitive Resistor (FSR)
https://learn.adafruit.com/force-sensitive-resistor-fsr

FSRs can detect physical pressure roughly at a low cost. The FSR is made of two layers (one with active element, the other with semiconductor) seperated by a spacer. The more it's pressed, the more of the active elements contact with the semiconductor, thus the smaller the resistance is. It is suitable when you need to know whether there is pressure or not. But it doesn't tell precisely how much force it perceived.

Statistics (for Interlink 402)
- Resistance range: Infinite/open circuit (no pressure), 100KΩ (light pressure), 200Ω (max. pressure)
- Force range: 0 - 20 lb. (0 - 100 Newtons) applied evenly over the 0.125 sq in surface area
- Power supply: Any voltage, < 1mA 

#### Temperature Sensor (TMP36)
https://learn.adafruit.com/tmp36-temperature-sensor

Temperature sensors can tell the ambient temperature precisely with its solid-state technique --  the voltage across a diode increases to temperature proportionally at a known rate (and the voltage is independent of the power supply). To use it, connect left pin to power, the right pin to ground, and the middle pin will provide the temperature-dependent analog voltage.

Statistics
- Temperature range: -40°C to 150°C / -40°F to 302°F
- Output range: 0.1V (-40°C) to 2.0V (150°C) but accuracy decreases after 125°C
- Power supply: 2.7 - 5.5V, 0.05 mA
Formula
- Temp in °C = [(Vout in mV) - 500] / 10

#### IR Sensor
https://learn.adafruit.com/ir-sensor

IR sensors/detectors are microchips with a photocell that is most sensitive to infrared light (of around 38kHz, 940nm). It is used for TV and DVD players to detect IR pulses from remote controls.
To test it, connect pin 1 (output) to a visible LED and a resistor, pin 2 to ground, and pin 3 to power supply.

Statistics
- Output: 0V (low) on detection of 38KHz carrier, 5V (high) otherwise
- Sensitivity range: 800nm to 1100nm with peak response at 940nm. Frequency range is 35KHz to 41KHz with peak detection at 38KHz
- Power supply: 3 - 5V DC, 3mA
