##---------------------------------------------------------------------------
## $RCSfile: CMakeLists.txt,v $
## $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/contrib/CMakeLists.txt,v $
## $Revision: 1.2 $
## $Date: 2005-08-16 17:51:50 $
##---------------------------------------------------------------------------
## Author:      Jorgen Bodde
## Copyright:   (c) Jorgen Bodde
## License:     wxWidgets License
##---------------------------------------------------------------------------

SUBDIRS( src/treemultictrl samples/treemultictrl )

# Only when we are allowed to play with XML
IF(USE_XMLMAPPER)
  SUBDIRS( samples/xmlmapper )
ENDIF(USE_XMLMAPPER)
  