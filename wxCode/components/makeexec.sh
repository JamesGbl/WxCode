#!/bin/sh

# the following tells SVN to make executable some files required
# to build the components on Unix systems
# Remember to do a svn commit after running this comamnd!
svn propset svn:executable ON */configure */build/config.* */build/*.sh
