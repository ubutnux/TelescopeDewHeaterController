# TelescopeDewHeaterController
This is a relatively easy to build for an Arduino based automatic dew heater controller for telescopes. It heats the lens/mirror on your telescope to stop the dreaded dew. 

In this build, the power drive to the heater is automatically controlled by the difference between the ambient dew point and the current temperature of the heater on your lens/mirror. The heater power can also be set manually if desired. 

The ambient temperature/humidity sensor is a DHT22, and the heater temperture is sensed by a DS18B20.

Any arduino can be used, I use the Uno board. So all the components can be attached with pre-made connectors. There is NO circuit board making. Just a few resistors oldered here and there

You can use commercially available dew straps (although I haven’t tested my system with these, so no responsibility taken), or you can make your own with nichrome wire.
