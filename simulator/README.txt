Inside are two folders - one for the base unit and one for the mobile

The base unit is very rudimentary to demo how the simulator will allow the back
and forth communication between the base and mobile unit. 

As we develop our code, the setup will allow us test the communication
to determine if it is working. 

As we proceed with development, the scenario script.txt can be updated/changed.

HOW TO USE:

1. Determine which COM ports your USB cables are using. 
Open base.cpp and change line 63 to reflect the correct COM port
Open mobile.c and change line 218 to the correct COM port. 

Compile the files in each folder. (A README is inside each folder)

Open two different terminal and start base in one and mobile in the other.
I would start base first. 
It will tell you that it is open and waiting for data.

Start mobile and it will sit and wait for notifications.

The way I work the demo of the simulator, you will see the first 11
vehicles loaded into the vector and printed, then in the mobile unit,
you will see notifications appear. 

