This project contains firmware for two arduino boards used to control a flight simulator panel, as well as an accompanying LUA script that is run through FSUIPC to interface to either FSX or P3D.

#summary Arduino controlled panel for interfacing with a flight simulator, either FSX or P3D.
# Introduction #

This project handles the source code for my prototype arduino flight simulator panel. The project consists of two arduino boards, one MEGA and one Uno which are responsible for handling switch and button inputs and displaying radio and autopilot output on seven-segment displays.

<a href='http://www.youtube.com/watch?feature=player_embedded&v=gcI6D6CItCo' target='_blank'><img src='http://img.youtube.com/vi/gcI6D6CItCo/0.jpg' width='425' height=344 /></a>


The project contains three arduino sketches:
  * **switches**: This is the main sketch for the Mega board
  * **extra\_switches**: This is the sketch for the Uno board which is responsible for handling rotary encoding.
  * **AdvButton**: This is no modifications to the available arduino but library to make it easily usable in the above sketches.

Upon it also contains a Lua script, **ardswitch.lua**. This is run through FSUIPC and handle serial input and output from the Arduino Mega board.

Updates are most easily forgotten by following me on G+, frankose@gmail.com.

# Details #

The current version of the source code supports the following controls:
  * Two selectable radio banks with master and slave displays, buttons to switch master/slave, and rotary encoded to change input on bank one slave.
  * ADF frequency/DME distance/DME time combined display with rotary encoder to change ADF frequency.
  * Fully featured autopilot control with heading, OBS1, autopilot altitude and vertical speed displays with rotary controls to input commands.
  * Turn on and off lost autopilot together with all common controller functions such as navigation mode, heading mode, altitude mode, speed hold mode, and approach mode
  * Flaps lever.
  * Spoiler lever/rotary knob
  * Rudder trim.
  * Aileron trim.
  * Gear lever.
  * Magneto rotary knob
  * Common electrical systems such as main battery, avionics, altimeter, fuel pump, de-ice, and pitot heat.
  * Common lights: panel, Beacon, navigation, strobe, taxi, and landing lights.

The arduino sketches and Lua script easily extendable, most additional switch functionality requires only a single line for each of the arduino sketch and Lua script.

Final panel installed in cockpit.
![https://lh6.googleusercontent.com/-ILVW6uUzico/Uy9IJzqZjDI/AAAAAAAATjM/9o2DzEVML_I/w1598-h899-no/20140323_212134.jpg](https://lh6.googleusercontent.com/-ILVW6uUzico/Uy9IJzqZjDI/AAAAAAAATjM/9o2DzEVML_I/w1598-h899-no/20140323_212134.jpg)

Final panel, only missing a few knobs.
![https://lh3.googleusercontent.com/-sGcIzU7jsrM/UIcAa25qpiI/AAAAAAAAOtY/NYVVBUkQdxI/w1598-h901-no/2012-10-23.jpg](https://lh3.googleusercontent.com/-sGcIzU7jsrM/UIcAa25qpiI/AAAAAAAAOtY/NYVVBUkQdxI/w1598-h901-no/2012-10-23.jpg)
![https://lh5.googleusercontent.com/-UjQokTCzZpo/UIcAa9EUV5I/AAAAAAAAOsU/L5CE4Lz2xfY/w1598-h901-no/2012-10-23.jpg](https://lh5.googleusercontent.com/-UjQokTCzZpo/UIcAa9EUV5I/AAAAAAAAOsU/L5CE4Lz2xfY/w1598-h901-no/2012-10-23.jpg)
![https://lh4.googleusercontent.com/-OaQyOFp9qH4/UIcAa1DpLpI/AAAAAAAASi0/-98Qfw4ZK7g/w1598-h901-no/2012-10-23.jpg](https://lh4.googleusercontent.com/-OaQyOFp9qH4/UIcAa1DpLpI/AAAAAAAASi0/-98Qfw4ZK7g/w1598-h901-no/2012-10-23.jpg)
![https://lh4.googleusercontent.com/-kIiIN2oEhr0/UIcAax7FioI/AAAAAAAAOtI/V__U_YcgyAA/w1598-h901-no/2012-10-23.jpg](https://lh4.googleusercontent.com/-kIiIN2oEhr0/UIcAax7FioI/AAAAAAAAOtI/V__U_YcgyAA/w1598-h901-no/2012-10-23.jpg)

Below is a few images from my own prototype panel.

![http://farm9.staticflickr.com/8427/7865336144_c61c93b081_b.jpg](http://farm9.staticflickr.com/8427/7865336144_c61c93b081_b.jpg)
![https://lh4.googleusercontent.com/-MmBw-99c0WQ/T_6ndmYHTxI/AAAAAAAAB9U/fYXCCkRSEM4/s1481/IMAG0401.jpg](https://lh4.googleusercontent.com/-MmBw-99c0WQ/T_6ndmYHTxI/AAAAAAAAB9U/fYXCCkRSEM4/s1481/IMAG0401.jpg)
![https://lh4.googleusercontent.com/-SD3vS9hchVs/T_6nWBNnvVI/AAAAAAAAB9U/jg9N7jFPO7I/s1676/IMAG0400.jpg](https://lh4.googleusercontent.com/-SD3vS9hchVs/T_6nWBNnvVI/AAAAAAAAB9U/jg9N7jFPO7I/s1676/IMAG0400.jpg)