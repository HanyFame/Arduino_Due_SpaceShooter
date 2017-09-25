# Arduino_Due SpaceShooter Game
This is a simple Space/Aracade flying game built with Arduino Due and 2.8" SPI ILI9341 Touch Screen.
## Components and Libraries
* Using ILI9341_due and Utouch library: https://github.com/marekburiak/ILI9341_due, http://www.rinkydinkelectronics.com/library.php?id=92
* Scheduler Library for SAM and SAMD Micrcontrollers to handle multiple loops for continous screen animations: https://playground.arduino.cc/Code/Scheduler
* Any 2.8" SPI ILI9341 LCD with XPT2046 touch controller
* Buzzer for sound
* Arduino Due

This project can't be done by Arduino Uno or any 8 bit Microcontroller, 32Bit Cortex M3 or M4 is really needed to be able to have fast enough screen animations and refresh rates with different threaded loops
