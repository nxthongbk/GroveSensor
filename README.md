# Integrate Grove Sensors, Actuators with MangoH and Grove IoT Exapansion Card

This project demonstrates how to integrate Grove Sensor with sensors, actuators MangoH and Grove IoT Exapansion Card.


## Prerequisites

* A mangOH Red board.
* A Grove IoT Expansion card.
* Grove sensors: See detail of sensor on wiki page: http://wiki.seeedstudio.com/ 

Grove Alcohol Sensor
------------------
Grove - Alcohol Sensor is built with MQ303A semiconductor alcohol sensor. It has good sensitivity and fast response to alcohol. It is suitable for making Breathalyzer. This Grove implements all the necessary circuitry for MQ303A like power conditioning and heater power supply. This sensor outputs a voltage inversely proportional to the alcohol concentration in air.


## Setup
1. Insert Grove IoT Expansion card into Mangoh Red
1. Jump 5V Pin on Grove IoT Card
1. Connect Grove Alcohol Sensor with A0 connector on Grove card



## How To Run

1. Build the AlcoholSensorService app by running ```mkapp -t wp85 groveSensor.adef``` in GroveSensorToCloud directory.
1. Run ```instapp groveSensor.wp85.update 192.168.2.2``` to install the app.

