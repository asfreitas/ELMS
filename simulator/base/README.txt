Overview:

This base was written in C++ as opposed to C because it
gives more options on data structures we might use for
the project. 

It is very basic and is just meant to demo opening/reading
writing/serial ports and how this will simulate the back and
forth communication between the base and mobile unit. 

None of the calculations or analysis is done. 

How to compile:
I wrote a Makefile.
I used Mingw.
The command to use Mingw after navigating to your directory is:
mingw32-make

You can clean with the following command:
mingw32-make clean

If you use Visual Studio, it compiles but you must change the
Character set
Project -> Properties ->Configuration Properties -> Advanced -> 
            Character set -> Not Set or use Multi-Byte Character Set
			
The program is in an endless while loop so to exit use ctrl-c.