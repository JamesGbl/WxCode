wxFormatValidator
=================
This name refers to a group of two classes:

wxFormatStringAsNumber
wxNumberValidator

wxFormatStringAsNumber is able to format a number inserting thousands
and decimal separators, prefix, suffix, exponential notation, padding
and some more features, all of them very flexible.
And it has a main advantage: it can also "unformat" the string back.
It works with a wxString, not int, double, etc. So you can format a
number which is many digits long.

wxNumberValidator uses a wxFormatStringAsNumber object to verify the
user's input. Using wxNumberValidator makes possible to format user's
input each time a key is pressed, helping in writing large numbers and
avoiding "typos". The validation can also (or instead of) be done when
the control loses focus. This validator is pretty featured, with range
checking, empty check or replace, colouring, and allowing other strings.

For more information using these classes read the docs (.htm) provided.

You can use this software under wxWindows license.

Usage
-----
By now, I'm not planning to make a library from it, so there's no
makefile, nor Visual projects.
What I do is attach the files to my project:
  forstrnu.h and forstrnu.cpp      if you want just formatting
  plus fvalnum.h and fvalnum.cpp   if you want validation
  plus fvalgred.h and fvalgred.cpp if you want validation in a wxGrid

I made the sample "fvaltest" to show and test every feature.
Again, there's not makefile.




For bugs, comments, etc. mail me at mmarsan@ciccp.es

