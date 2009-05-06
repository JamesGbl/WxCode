#------------------------------------------------------------------------------
#           Name: for_loop.py
#         Author: Kevin Harris
#  Last Modified: 02/13/04
#    Description: This Python script demonstrates how to use for loops.
#------------------------------------------------------------------------------

# Simply iterate the items in a list

list = ['one', 'two', 'three']

for x in list:
    print x
    
print ""

# For more traditonal index-based iteration, use range()

for i in range(10):
    print i,

print "\n"

# To iterate over the indices of a list, combine range() and len(). 

list = ['one', 'two', 'three', 'four', 'five']

for i in range( len(list) ):
    print i, list[i]

raw_input( '\n\nPress Enter to exit...' )
