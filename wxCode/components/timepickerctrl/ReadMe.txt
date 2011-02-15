Below are instructions for building on various platforms.  If you experience problems, or if you have feedback, suggestions or comments, please email me at greenbreen@yahoo.com.  If you do email, please include the string "timepickerctrl" in the subject line so that I can filter the message to the appropriate folder.

DISCLAIMER:  Although I have attempted to make my code as high quality as possible, I am providing it AS IS without any warranty, included an implied warranty.  Use it at your own risk.

Linux:
To build in Linux, open a terminal and cd into the folder "timepickerctrl".  Then run "./configure"; then run "make"; then either run "make install" with root privileges, or run "sudo make install" and enter the appropriate password.  After this last command executes successfully, the component is installed and ready to be linked to by your programs.  On my system, the libraries are put in /usr/local/lib.  By default, the configure script generates a makefile for a shared unicode release library.  You can use "./configure --enable-"FEATURE or "./configure --disable-"FEATURE, where FEATURE is "unicode", "debug", or "shared" to control how the library is created.  For example, you can use "./configure --enable-debug --disable-shared" to generate a makefile for a static debug library.  Then use "make" and (sudo) "make install" to make and install it.  Other configure script options are available; search the configure script using search phrase "Optional Features" for more details.

Windows:
I tested the dsw file created by the bakefile system on Visual C++ 6 running on my Windows Vista system.  The project loaded but failed to build.  I spent some time (maybe an hour) trying to fix the errors, then gave up.  I'm not sure if the errors are a result of my wxWidgets installation or a problem with the workspace and project files.  I'm using wxWidgets 2.8.11.  I was getting an error about wx/setup.h not being present, and indeed, it was not in my wxWidget's include folder, although I did have a wx/setup_inc.h file.  I've built wxWidgets programs on that system before, so I don't think it's a problem with my wxWidgets installation, but I can't be sure.

I don't use Windows now as much as I used to, and I don't plan on solving these build problems unless someone expresses interest in it.  However, if you've experienced similar issues and solved them, I'd love to hear your solution so I can update this file with instructions that work for Windows.

Other platforms:
I currently don't have access to other platforms.  The bakefile system generated build files that can potentially be used on other systems, so give it a try if you have access to these systems.  Please consider providing feedback about your experience if you do try it.
