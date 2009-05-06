#------------------------------------------------------------------------------
#           Name: enviroment_variables.py
#         Author: Kevin Harris
#  Last Modified: 02/13/04
#    Description: This Python script demonstrates how to acces enviroment
#                 variables.
#------------------------------------------------------------------------------

import os

envVar = os.environ.get( "PATHEXT" )

print "PATHEXT = " + envVar

raw_input( '\nPress Enter to exit...' )
