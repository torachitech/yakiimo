# yakiimo
Arduino sketch for controlling your electric oven to make "really sweet" roasted sweet potatoes in phases

Change HydrolysisTemp, HydrolysisTime, RoastTemp, RoastTime, and RelayPin as necessary. HydrolysisTemp (degrees Celsius) is already set to 60, which is the optimal temperature for beta-amylase activity. I don't think you need to change this, but if the increase of the temperature inside sweet potatoes is really slow in phase 2 (Yes, I measured it!), you may want to change it to a higher value. Your oven will stay at HydrolysisTemp for HydrolysisTime (sec), which I set to 3600. I don't know how long it takes for beta-amylase in sweet potatoes to produce enough maltose, so you may want to try a couple of different HydrolysisTime. RoastTemp is the temperature (degrees Celsius) at which sweet potatoes are roasted. I set it to 210 because one of the commercial electric yakiimo roasters I saw in a supermarket said 210. I set RoastTime (sec) to 4800 because the commercial roaster said 80 min.

Phases are automatically detected based on the set values and current values. 
In phase 1, your oven will heat up until it reaches HydrolysisTemp.
In phase 2, it stays at HydrolysisTemp and beta-amylase contained in sweet potatoes hopefully converts starch to maltose, which makes roasted sweet potatoes really sweet.
In phase 3, your oven will heat up until it reaches RoastTemp.
In phase 4, it stays at RoastTemp and roasts sweet potatoes all the way.
In phase 5, the relay never turns on. Take out roasted sweet potatoes right after the phase 4 to avoid overcooking.

I used this sketch with the first generation of Intel Galileo board and Adafruit thermocouple amplifier. You can use any type of relay board with an appropriate power rating for your oven.
