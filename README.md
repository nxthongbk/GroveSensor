# Integrate Grove sensors, actuators with MangoH and Grove IoT Expansion Card

This project demonstrates how to integrate Grove sensors, actuators with MangoH Yellow and Grove IoT Expansion Card.


## Prerequisites

* A mangOH Yellow board.
* A Grove IoT Expansion card.
* Grove sensors: See detail of sensor on wiki page: http://wiki.seeedstudio.com/ 

### Grove Alcohol Sensor
------------------
Grove - Alcohol Sensor is built with MQ303A semiconductor alcohol sensor. It has good sensitivity and fast response to alcohol. It is suitable for making Breathalyzer. This Grove implements all the necessary circuitry for MQ303A like power conditioning and heater power supply. This sensor outputs a voltage inversely proportional to the alcohol concentration in air.  
To integrate with Alcohol Sensor (HM3301) we need connect to port A0 on Grove IoT Expansion Card.   
Detail in: http://wiki.seeedstudio.com/Grove-Alcohol_Sensor/  

### The Grove - Laser PM2.5
------------------
The Grove - Laser PM2.5 Sensor (HM3301) is a new generation of laser dust detection sensor, which is used for continuous and real-time detection of dust in the air. 

To integrate with Laser PM2.5 Sensor (HM3301) we need connect to port A0 on Grove IoT Expansion Card.  
Detail in: http://wiki.seeedstudio.com/Grove-Laser_PM2.5_Sensor-HM3301/  

### Grove - Gas Sensor(MQ9)
------------------
The Grove - Gas Sensor(MQ9) module is useful for gas leakage detection (in home and industry). It is suitable for detecting LPG, CO, CH4 
To integrate with Gas Sensor (HM3301) we need connect to port I2C on Grove IoT Expansion Card.   
Detail in: http://wiki.seeedstudio.com/Grove-Gas_Sensor-MQ9/  

### Grove - RGB LED Matrix w/Driver
------------------
Grove - RGB LED Matrix is 64 pixel leds and 255 colors for each pixel  
To integrate with RGB LED Matrix) we need connect to port I2C on Grove IoT Expansion Card.   
Detail in: http://wiki.seeedstudio.com/Grove-RGB_LED_Matrix_w-Driver/  

### Grove - Speech Recognizer  
------------------
Grove speech recognizer is a designed for voice control application such as smart home, smart toy, voice control robot, anything you would like to control through voice, it worth a try. The board includes a Nuvoton ISD9160, a microphone, 1 SPI flash, 1 grove connector,1 speaker connector and 1 led to reflect to your voice.  
To integrate with Speech Recognizer we need connect to port I2C on Grove Expansion Card.   
Detail in: http://wiki.seeedstudio.com/Grove-Speech_Recognizer/ 

### Grove - Grove-125KHz RFID Reader   
------------------
Grove-125KHz RFID Reader is a  module used to read uem4100 RFID card information with two output formats: Uart and Wiegand. It has a sensitivity with maximum 7cm sensing distance.  
To integrate with Grove-125KHz RFID Reader we need connect to port UART on Grove Expansion Card.   
Detail in: http://wiki.seeedstudio.com/Grove-Speech_Recognizer/ 

### Grove - Ultrasonic Ranger
------------------
Grove - Ultrasonic Ranger is a non-contact distance measurement module which works at 40KHz. When we provide a pulse trigger signal with more than 10uS through singal pin, the Grove_Ultrasonic_Ranger will issue 8 cycles of 40kHz cycle level and detect the echo. The pulse width of the echo signal is proportional to the measured distance. Here is the formula: Distance = echo signal high time * Sound speed (340M/S)/2. Grove_Ultrasonic_Ranger's trig and echo singal share 1 SIG pin.
To integrate with Speech Recognizer we need connect to port D5 on Grove Expansion Card.   
Detail in: http://wiki.seeedstudio.com/Grove-Ultrasonic_Ranger/ 

### Grove - Human Presence Sensor (AK9753)
------------------
Grove - Ultrasonic Ranger isThe Grove - Human Presence Sensor can be used to detect the presence of the human body or any other infrared objects. Moreover, it is composed of four quantum IR sensors and an integrated circuit (IC) for characteristic compensation, so it can be used to detect the motion of the IR object and the relative position where the IR object moves. An integral analog-to-digital converter provides 16-bits data outputs. This module is suitable for several feet human detector..
To integrate with Human Presence Sensor we need connect to port I2C on Grove Expansion Card.   
Detail in: http://wiki.seeedstudio.com/Grove-Human_Presence_Sensor-AK9753/


## Setup
1. Insert Grove IoT Expansion card into Mangoh Red
1. Jump 3.3V Pin on Grove IoT Card
1. Connect Grove Sensor to connector on Grove card



## How To Run

1. Copy ```GroveSensorToCloud``` app to mangOH/apps.
1. Add line ```$CURDIR/apps/GroveSensorToCloud/groveSensor``` to yellow.sdef.
1. Make yellow.sdef.