see configuration for using the config files with XSTC. Also Command line preprocessor.

XSTC uses a copy constructor, so you can call any waSTC or wxSCI function directly.
also try to make use of these three macros:
     XSTC_EVENT this is the event type. [wxStyledTextEvent]
     XSTC_EVENT_DEF() save as below, but it is for events [wxEVT_STC_]
     XSTC_DEF() use this with #defines it concatonates the prefix so you inlu need to 
                put in the second and third parts. XSTC_DEF(C_DEFAULT)

there are a few other macros that replace functions:
      SetCaretBk    SetCaretLineBackground
      GetCaretBk    GetCaretLineBackground
      GetCaretALPHA GetCaretLineBackAlpha
      SetCaretALPHA SetCaretLineBackAlpha

this project uses doxygen for documentation, so build yourself the docs and study up ;}