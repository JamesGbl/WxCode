#------------------------------------------------------------------------------
#           Name: create_directory.py
#         Author: Kevin Harris
#  Last Modified: 02/13/04
#    Description: This Python script demonstrates how to create a single
#                 new directory as well as delete a directory and everything 
#                 it contains. The script will fail if encountewrs a read-only
#                 file
#------------------------------------------------------------------------------

import os

#------------------------------------------------------------------------------
# Name: deleteDir()
# Desc: Deletes a directory and its content recursively.
#------------------------------------------------------------------------------
def deleteDir( dir ):
    
    for name in os.listdir( dir ):
        
        file = dir + "/" + name
        
        if not os.path.isfile( file ) and os.path.isdir( file ):
            deleteDir( file ) # It's another directory - recurse in to it...
        else:
            os.remove( file ) # It's a file - remove it...
            
    os.rmdir( dir )

#------------------------------------------------------------------------------
# Script entry point...
#------------------------------------------------------------------------------

# Creating a new directory is easy...

os.mkdir( "test_dir" )

# Pause for a moment so we can actually see the directory get created.
raw_input( 'A directory called "tes_dir" was created.\n\nPress Enter to delete it.' )

# Deleting it can be a little harder since it may contain files, so we'll need 
# to write a function to help us out here.

deleteDir( "test_dir" );
