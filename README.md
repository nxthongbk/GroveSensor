# Integrate Grove Sensors, Actuators with MangoH and Grove IoT Expansion Card

This project demonstrates how to integrate Grove Sensor with sensors, actuators MangoH and Grove IoT Exapansion Card.


## Prerequisites

* A mangOH Yellow board.
* A Grove IoT Expansion card.
* Grove sensors: See detail of sensor on wiki page: http://wiki.seeedstudio.com/ 

### Grove Alcohol Sensor
------------------
Grove - Alcohol Sensor is built with MQ303A semiconductor alcohol sensor. It has good sensitivity and fast response to alcohol. It is suitable for making Breathalyzer. This Grove implements all the necessary circuitry for MQ303A like power conditioning and heater power supply. This sensor outputs a voltage inversely proportional to the alcohol concentration in air.  
To integrate with Alcohol Sensor (HM3301) we need connect to port A0 in Grove Expansion Card.  
Detail in: http://wiki.seeedstudio.com/Grove-Alcohol_Sensor/  

### The Grove - Laser PM2.5
------------------
The Grove - Laser PM2.5 Sensor (HM3301) is a new generation of laser dust detection sensor, which is used for continuous and real-time detection of dust in the air. 

To integrate with Laser PM2.5 Sensor (HM3301) we need connect to port A0 in Grove Expansion Card.  
Detail in: http://wiki.seeedstudio.com/Grove-Laser_PM2.5_Sensor-HM3301/  

### Grove - Gas Sensor(MQ9)
------------------
The Grove - Gas Sensor(MQ9) module is useful for gas leakage detection (in home and industry). It is suitable for detecting LPG, CO, CH4 
To integrate with Gas Sensor (HM3301) we need connect to port I2C in Grove Expansion Card.
Detail in: http://wiki.seeedstudio.com/Grove-Gas_Sensor-MQ9/  

### Grove - RGB LED Matrix w/Driver
------------------
Grove - RGB LED Matrix is 64 pixel leds and 255 colors for each pixel  
To integrate with RGB LED Matrix) we need connect to port I2C in Grove Expansion Card. 
Detail in: http://wiki.seeedstudio.com/Grove-RGB_LED_Matrix_w-Driver/  

### Grove - Speech Recognizer  
------------------
Grove speech recognizer is a designed for voice control application such as smart home, smart toy, voice control robot, anything you would like to control through voice, it worth a try. The board includes a Nuvoton ISD9160, a microphone, 1 SPI flash, 1 grove connector,1 speaker connector and 1 led to reflect to your voice.  
To integrate with Speech Recognizer we need connect to port I2C in Grove Expansion Card. 
Detail in: http://wiki.seeedstudio.com/Grove-Speech_Recognizer/ 

### Grove - Grove-125KHz RFID Reader   
------------------
Grove-125KHz RFID Reader is a  module used to read uem4100 RFID card information with two output formats: Uart and Wiegand. It has a sensitivity with maximum 7cm sensing distance.  
Detail in: http://wiki.seeedstudio.com/Grove-Speech_Recognizer/ 


## Setup
1. Insert Grove IoT Expansion card into Mangoh Red
1. Jump 3.3V Pin on Grove IoT Card
1. Connect Grove Sensor to connector on Grove card



## How To Run

1. Build the AlcoholSensorService app by running ```mkapp -t wp77xx groveSensor.adef``` in GroveSensorToCloud directory.
1. Run ```instapp groveSensor.wp77.update 192.168.2.2``` to install the app.

