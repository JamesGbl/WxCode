 
wxLatexDC ReadMe
----------------
 
Website: http://wxcode.sourceforge.net/components/wxlatex
Author: Christoph Schmidt-Hieber
Version: 0.1
Description:
wxLatexDC is a device context that will generate a LaTeX2e file from graphics and text that
were drawn onto it. It makes use of PSTricks (http://tug.org/PSTricks) to create Tex-compatible
PostScript macros. I've shamelessly copied large parts of the code from the wxSVGFileDC and
wxPostScriptDC classes. You can create a wxLatexDC in exactly the same way as a wxSVGFileDC.
At present, only a small part of the DC drawing functions have been implemented. 
The major advantage of using LaTeX output rather than directly writing PostScript is that 
typesetting can be done in LaTeX, so that graphics seamlessly integrate into documents. A 
positive side effect is that the PSTricks macros are more easily readable than plain PostScript.
The obvious disadvantage is that wxLatexDC can't know which font type and size will be used in
the final document, so that all the GetTextExtent() functions will not work. To still be able to
position Text precisely at the location that you want to have it, you can make use of
wxLatexDC::DrawLabelLatex() which uses the same syntax as wxDC::DrawLabel(). This allows you to 
set your text alignment (left, right, top, bottom, horizontal and vertical center) relative to
the wxRect that is passed as an argument.



Installation - general
----------------------

Since wxLatexDC uses the new pImpl idiom, it will only work with wxWidgets >= 2.9.0
that you can check out from the subversion repository.



Installation - win32
--------------------
 
When building on win32, you can use the makefiles in the BUILD folder using the
WX_DIR, WX_DEBUG, WX_UNICODE and WX_SHARED to match your wxWidgets build.
Examples:
 
> nmake -fmakefile.vc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1
> make -fmakefile.bcc WX_DIR=c:\path\to\my\wxWidgets WX_UNICODE=0/1 WX_DEBUG=0/1 WX_SHARED=0/1


     
Installation - unix
-------------------

When building on a Linux/GNU-based system, type

./configure
make

to build against the default build of wxWidgets (the one listed as 'default' 
by wx-config --list). Type "./configure --help" for more info.



Sample program
--------------

The sample program is derived from the printing sample coming with the standard
wxWidgets distribution. The only change that you will find is a new entry
("Latex") in the file menu that allows you to export the drawing to LaTeX /
PSTricks.
There is a very simple wrapper file (wrapper.tex) in the sample directory
to generate a postscript. Note that pdflatex doesn't work easily with PSTricks,
so you will have to use ps2pdf to create a pdf.


ChangeLog
---------
 
0.1 - first version of wxLatexDC at wxCode
