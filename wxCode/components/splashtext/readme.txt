 
 SplashText ReadMe
 -----------------
 
 Website: http://wxcode.sourceforge.net/components/splashtext
 Author:  Julian Smart
 Version: 1.0 
 
 Description:
 SplashText is a Windows program to draw a simple text string onto
 a bitmap. This bitmap will usually be an application 'splash screen',
 but you could equally use it to label snapshots. Since SplashText
 can be used in batch mode, it can be part of an automated
 distribution or other script.

 SplashText draws text at a specified position in a specified
 font, with user-supplied text foreground and background. The text
 is drawn transparently. Optionally, antialiasing can be
 switched on to give a smoother look to the text.

 In interactive mode (if -b is not specified, or if an error
 occurs) messages are displayed in one window and the resulting
 bitmap is displayed in another.

 Text can be left, centre, or right-justified relative to the
 given position.

 
 Limitations
 =====================================================================
 
 - Currently, original bitmap depth is not respected. It is always
   displayed in 24-bit colour, or other depth appropriate to the
   output file type. Workaround: use another batch image
   processing application to reduce the depth if necessary.
   For example, Image Alchemy at http://www.handmade.com/
   has a demo that can be used on images of 640x480 and below.
   This command reduces a BMP file to 8-bit (256 colours) with
   no dithering:
 
   alchlong test.bmp test2.bmp -w -8 -c256 -d0 -o
 
 - Antialiasing could be better.
 
 - Due to a weakness in the command line parser, if you alter
   options interactively and you remove switches SplashText will
   think that switch is still set.
 
 Options
 =====================================================================
 
 Pass the following options on the command line or by selecting
 File | Run Command. You can also show these options by
 selecting Help | Usage. Note that you can use either the short
 name or the long name. You can also use minus ('-') to precede
 a switch name. Use double quotes to enclose switch values that
 contain spaces.
 
 Usage: splashtext [/h] [/v] [/q] [/b] [/bo] [/it] [/r] [/l]
  [/c] [/a] [/o <str>] [/i <str>] [/t <str>] [/f <str>]
  [/p <num>] [/x <num>] [/y <num>] [/fcol <str>] [/bcol <str>] [/m <num>]
 
   /h  --help                      	show usage
   /v  --verbose                   	be verbose
   /q  --quiet                     	be quiet
   /b  --batch                     	batch mode (no GUI)
   /bo  --bold                     	use a bold font
   /it  --italic                   	use an italic font
   /r  --right-justify             	right justify the text
   /l  --left-justify              	left justify the text (the default)
   /c  --centre                    	centre the text
   /a  --antialias                 	antialias (smooth) the text
   /o  --output=<str>              	output file (finished splash screen)
   /i  --input=<str>               	input file (template splash screen)
   /t  --text=<str>                	text to add to splash screen
   /f  --face=<str>                	facename to use
   /p  --pointsize=<num>           	point size to use
   /x  --x-position=<num>          	x position of text
   /y  --y-position=<num>          	y position of text
   /fcol  --foreground-colour=<str>	text foreground colour in hex
   /bcol  --background-colour=<str>	text background colour in hex
   /m  --magnify=<num>             	magnification factor (amount text
                                         is magnified, then scaled down)
 
 Tips
 =====================================================================
 
 - Use the Windows start /wait command to run SplashText and wait
   until it has finished processing.
 
 - The default value for /m is 4. Using values larger than this
   will cause SplashText to take longer, since a large bitmap must
   be created and manipulated. If you use a low value (such as
   1: no magnification), you will notice that the text is very fuzzy
   since antialiasing affects the text to a greater extent.
 
 - When drawing on a coloured or textured background, choose a
   background text colour that will blend into the background. For
   example, a light grey background (C0C0C0) can be effective.
   Using a dark background may look nasty if the background is
   relatively light.
 
