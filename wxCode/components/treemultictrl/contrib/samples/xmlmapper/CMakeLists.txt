##---------------------------------------------------------------------------
## $RCSfile: CMakeLists.txt,v $
## $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/contrib/samples/xmlmapper/CMakeLists.txt,v $
## $Revision: 1.1 $
## $Date: 2005-08-16 19:37:09 $
##---------------------------------------------------------------------------
## Author:      Jorgen Bodde
## Copyright:   (c) Jorgen Bodde
## License:     wxWidgets License
##---------------------------------------------------------------------------

SET(SAMPLE_SRCS MultiCtrlTestXML.cpp)

IF(WIN32)
    SET(SAMPLE_SRCS ${SAMPLE_SRCS} MultiCtrlTestXML.rc)
ENDIF(WIN32)

ADD_EXECUTABLE(xmlmapper WIN32 ${SAMPLE_SRCS})

IF(LINUX)
  ADD_DEFINITIONS( -DLINUX=1 )
ENDIF(LINUX)

# Include XML wrapper definition
ADD_DEFINITIONS(-DwxUSE_TMC_XMLMAPPER)

# if the checkbox view functionality is not desired,
# exclude this part from the sample

IF(USE_CHECKBOXVIEW)
  ADD_DEFINITIONS( -DCHECKBOXVIEW=1 )
ELSE(USE_CHECKBOXVIEW)
  ADD_DEFINITIONS( -DCHECKBOXVIEW=0)
ENDIF(USE_CHECKBOXVIEW)

LINK_DIRECTORIES(${TREEMULTICTRL_BINARY_DIR}/contrib/lib ${LINK_DIRECTORIES} ${TINYXML_LIB_PATH} )

TARGET_LINK_LIBRARIES(xmlmapper treemultictrl ${WXWIDGETS_LIBRARIES} ${TINYXML_LIB} )