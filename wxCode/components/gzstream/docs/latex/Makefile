# File:     Makefile
# Author:   Julian Smart
# Created:  2002
# Updated:
# Copyright: (c) 2002, Julian Smart
#
# Modified for GNU make Mike Wetherell

WXDIR = ../../../../..
DOCDIR = ..
NAME = wxgz
SRCS = wxgz.tex gzstream.tex

html:     $(DOCDIR)/html/$(NAME).htm
htmlhelp: $(DOCDIR)/htmlhelp/$(NAME).chm
htb:      $(DOCDIR)/htb/$(NAME).htb
winhelp:  $(DOCDIR)/winhelp/$(NAME).hlp
rtf:      $(DOCDIR)/rtf/$(NAME).rtf
docs:     html htb htmlhelp rtf winhelp

$(DOCDIR)/winhelp/$(NAME).hlp: $(DOCDIR)/rtf/$(NAME).rtf $(DOCDIR)/rtf/$(NAME).hpj
	(cd $(DOCDIR)/rtf; \
	rm -f $(NAME).ph; \
	hc $(NAME)); \
	mkdir $(DOCDIR)/winhelp; \
	mv $(DOCDIR)/rtf/$(NAME).hlp $(DOCDIR)/rtf/$(NAME).cnt $(DOCDIR)/winhelp

$(DOCDIR)/rtf/$(NAME).rtf: $(SRCS)
	-mkdir $(DOCDIR)/rtf; \
	tex2rtf $(NAME).tex $(DOCDIR)/rtf/$(NAME).rtf -winhelp

$(DOCDIR)/html/$(NAME).htm: $(SRCS)
	mkdir $(DOCDIR)/html; \
	for x in forward back up contents; do \
	    cp $(WXDIR)/docs/latex/wx/$$x.gif $(DOCDIR)/html; \
	done; \
	tex2rtf $(NAME).tex $(DOCDIR)/html/$(NAME).htm -html; \
	rm -f $(DOCDIR)/html/*.con $(DOCDIR)/html/*.ref *.con *.ref

$(DOCDIR)/htmlhelp/$(NAME).chm: $(DOCDIR)/html/$(NAME).htm
	(cd $(DOCDIR)/html; hhc $(NAME)); \
	mkdir $(DOCDIR)/htmlhelp; \
	mv $(DOCDIR)/html/$(NAME).chm $(DOCDIR)/htmlhelp

# An htb file is a zip file containing the .htm, .gif, .hhp, .hhc and .hhk
# files, renamed to htb.
# This can then be used with e.g. helpview.
# Optionally, a cached version of the .hhp file can be generated with hhp2cached.
$(DOCDIR)/htb/$(NAME).htb: $(DOCDIR)/html/$(NAME).htm
	mkdir $(DOCDIR)/htb; \
	(cd $(DOCDIR)/html; \
	zip -j ../htb/$(NAME).htb *.htm *.gif *.hhp *.hhc *.hhk)

# In order to force document reprocessing
touch$(NAME):
	-touch $(NAME).tex

update: touch$(NAME) docs

clean:
	(cd $(DOCDIR); rm -rf winhelp html rtf htmlhelp htb)

# Start Word, running the GeneratePDF macro. MakeManual.dot should be in the
# Office StartUp folder, and PDFMaker should be installed.
#updatepdf:  # touch$(NAME) pdfrtf
#    start $(WAITFLAG) "winword d:/wx2/wxWindows/docs/latex/pdf/$(NAME).rtf /mGeneratePDF"

