# Prototype Electricity Monitor based on Arduino Uno

## !!! Safety WARNING !!!

This experiment involves high AC voltages and current. Please ensure you know what you are doing before replicating this 
experiment. I will not accept any responsibility for personal injury, losses or damages as a result of attempts to 
replicate this experiment/project.

## Project Background

This prototype was started to see if there is a viable way to deploy sensors in the field that could record electricity 
usage.

The end goal is to integrate a GPRS module for sending the data to a remote server (still TODO as on 2018-12-15).

The objectives of the prototype:

* Get electricity usage
* Get temperature from a temperature probe
* Record the data to a local SD card
* Occasionally submit the data to a remote server (in batches or under certain conditions)

The temperature probe is an idea I am playing with which can be useful in remote generators which tend to die when 
overheating. The overheating itself could be caused by load. I hope these sensors and data recordings will provide more 
data to help users analyse generator failures.

Another idea I have for the temperature probe is to record the actual wire temperature in a electricity distribution 
box. There are a number of factors that could cause high temperatures on the electricity wiring and it may also be 
useful to collect such information.

## Status as on 2018-12-15

The initial experiments in a controlled lab environment produced some encouraging results. The README at this point 
reflects the experiment set-up and status together with some notes.

Once all documentation has been updated, a branch called `experiment_20181215` will be created to preserve all files at 
the point of the experiment.

This section may be removed in future updates.

## Parts List

Below I list the parts used with links and perhaps other useful information such as material cost at the time of the 
experiment.


| Part                                          | Cost (ZAR) | My Supplier Link (Communica)                           | Notes                                                                                                                 |
|-----------------------------------------------|------------|--------------------------------------------------------|-----------------------------------------------------------------------------------------------------------------------|
| Arduino UNO rev 3                             |     305.00 | http://www.communica.co.za/Catalog/Details/P1424521842 | I have a number of UNO's lying around so I used an old one.                                                           |
| Arduino Energy Monitoring Shield              |     150.01 | http://www.communica.co.za/Catalog/Details/P3308369411 |   |
| DS18B20 Temperature Probe Sensor              |      60.01 | http://www.communica.co.za/Catalog/Details/P3958148566 | Pick a waterproof one - just for incase :-)                                                                           |
| SME STACKABLE SD CARD SHIELD V3               |      95.83 | http://www.communica.co.za/Catalog/Details/P2363846356 | I did not include the cost of the SD card as I am sure everyone must have a couple lying around they can use.         |
| USB Cable for the Arduino (aka Printer Cable) |      25.70 | http://www.communica.co.za/Catalog/Details/P1837703657 | I used a spare one - I have a whole lot of assorted cables collected over the years :-)                               |
| 4.7k Resistor                                 |       0.09 | http://www.communica.co.za/Catalog/Details/P3423848296 | I did not buy this online. I used some spairs I have left from a previous resistor kit/pack I purchased from SparkFun |
| Header Pins                                   |       0.46 | http://www.communica.co.za/Catalog/Details/P3333231207 | Used for convienence of connecting the probe to the Arduino.                                                          |
| Strip Grid Experimental Board                 |       0.20 | http://www.communica.co.za/Catalog/Details/P3913707199 | Only used a tiny piece - 3x3 holes required.                                                                          |

I also used some solder. I estimated my total cost just over ZAR660.00

## Final Note

I am still updating the documentation and notes - more commits to follow...