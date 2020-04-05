TO USE:

It is important that this program be used in Windows only. It
uses Windows.h which is solely available in Windows. It calls
the Win32 API which what is being used to open/close/read/write
to the serial ports.

1. Find your COM ports. 
2. Open Mobile.c and scroll to line 218. You will need to change
   which COM port your computer is using. 
3. To compile:
   I installed MinGW on my computer (Minimalist GNU for Windows)
   The files compile fine using MinGW. As you see below, Visual Studio
   requires several changes.
   
	gcc -o mobile Mobile.c serial.c serialport.h
	
4. If you use Visual Studio, there are several adjustments you need
   to make because of the older style C code
   a. I initially got the following warning:
      warning C4133: 'function': incompatible types - from 'char [48]' to 'LPCWSTR'
	  
	  To fix this: Project -> Properties -> Advanced -> Character Set -> 
	                  change to Multi-Byte Character Set or Not Set
	  Reference for this fix: https://stackoverflow.com/questions/33001284/incompatible-with-parameter-of-type-lpcwstr
	b.Visual Studio causes errors for use of functions that it considers
	  unsafe such as fopen, sprintf and strcpy. In order to suppress
	  this at the very top, before all the #includes add:
		#define _CRT_SECURE_NO_WARNINGS
		
		Reference: https://stackoverflow.com/questions/22450423/how-to-use-crt-secure-no-warnings
	c. Visual Studio also will not compile getch found in line 373.
       You have to change this to _getch.
		
	   Reference: https://ourcodeworld.com/articles/read/638/how-to-solve-c-plusplus-error-c4996-getch-the-posix-name-for-this-item-is-deprecated-instead-use-the-iso-c-and-cplusplus-conformant-name-getch

5. Start mobile in a separate terminal. 
6. To stop the program just type in x