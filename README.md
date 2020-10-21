## Overview 
A simple smart sports ground watering management and information system implemented using multiple particle argon boards. The system consists of 2 sensor nodes to collect environmental data, and a cluster head that process the data. The cluster head also is a publisher of MQTT protocol where a workstation or PC is a subscriber. 
![](https://github.com/emiltan97/simple-outdoor-iot-system/blob/master/img/overview.jpg)
## Running the program 
To run the program simply load the .ino file in each folder using the (particle web ide)[https://build.particle.io/build/new] into the argon boards. 
## Hardware 
### Alpha 
Alpha is the sensor node 1 that is connected to the following sensors : 
- (soil moisture sensor)[https://www.elecfreaks.com/learn-en/microbitOctopus/sensor/octopus_ef04027.html]
- (DHT11 temperature and humidity sensor)[https://core-electronics.com.au/dht11-temperature-and-humidity-sensor.html]
- (LDR)[https://www.elecfreaks.com/octopus-analog-photocell-brick-obphotocell.html]
This node also equipped with a fan as the actuator. 
Alpha board : 
![](https://github.com/emiltan97/simple-outdoor-iot-system/blob/master/img/alpha.jpg)
Alpha program flow : 
![](https://github.com/emiltan97/simple-outdoor-iot-system/blob/master/img/alpha.jpeg)
### Beta 
Beta is the sensor node 2 that is connected to the following sensors : 
- (rain sensor)[https://www.elecfreaks.com/octopus-rain-steam-sensor.html]
- (water level sensor)[https://www.elecfreaks.com/octopus-water-level-sensor.html]
- (pir sensor)[https://www.elecfreaks.com/octopus-pir-sensor-brick.html]
Beta board : 
![](https://github.com/emiltan97/simple-outdoor-iot-system/blob/master/img/beta.jpg)
Beta program flow : 
![](https://github.com/emiltan97/simple-outdoor-iot-system/blob/master/img/beta.jpeg)
### Gamma
Gamma is the cluster head of the system that is equipped with no sensors. 
Gamma program flow : 
![](https://github.com/emiltan97/simple-outdoor-iot-system/blob/master/img/gamma.jpeg)
