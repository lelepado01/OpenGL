# OpenGL


# Autonomous Software Agents
# Assignment 2

### Introduction

The environment described in the first assignment was partially implemented. 
A sample log of the week can be found in the file *Assignment_2/log.txt*.
To run the scenario, execute *node Assignment_2/scenarioTest.js*.

### People

In particular, the routines for all three people have been described in the file *Assignment_2/scenarioTest.js*, using the observable clock. 
Alice wakes up earlier, followed by the cat, they have breakfast and leave the house, 
although cat stays behind and finds the door shut. 
Bob wakes up at 7:30, and he too has breakfast and leaves.

The devices that have been implemented come into play during this early part of the routine.

### Devices

The device present in the house that have been implemented for this scenario are: 
 - **Cat sensor** in the entrance, allowing the cat too move outside and connected to the smart door (file: *Assignment_2/goals/CatSensor.js*), 
 - **Person sensor** in bedroom, monitoring movement of people, and which is connected to the breakfast agent (file: *Assignment_2/goals/PeopleSensor.js*), 
 - **Smart door**, which opens depending on the presence of the cat in the entrance, and closes after some time on its own  (file: *Assignment_2/devices/SmartDoorDevice.js*)

### Agents

The agents that have been implemented for this scenario are: 
 - **Breakfast agent**: which uses knowledge from the person sensor to decide when to start preparing breakfast (file: *Assignment_2/goals/Breakfast.js* and *Assignment_2/goals/PeopleSensor.js*),
 - **Alarm agent**: a simple agent that handles the alarms in the house, setting them in the evening (file: *Assignment_2/goals/Alarm.js*),
 - **Smart door agent**: which uses knowledge from the cat sensor to decide whether to open the door (file: *Assignment_2/goals/SmartDoor.js* and *Assignment_2/goals/CatSensor.js*).  

### Utilities

For implementing the scenario, two more files have been used to execute common, utility functions. 
The file *Assignment_2/utils/Logger.js* contains a class that allows better formatting of text printed to the console. 
The file *Assignment_2/utils/Time.js* contains utility functions to handle time from the clock class.
