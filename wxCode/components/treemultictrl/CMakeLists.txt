##---------------------------------------------------------------------------
## $RCSfile: CMakeLists.txt,v $
## $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/CMakeLists.txt,v $
## $Revision: 1.4 $
## $Date: 2005-08-16 17:51:49 $
##---------------------------------------------------------------------------
## Author:      Jorgen Bodde
## Copyright:   (c) Jorgen Bodde
## License:     wxWidgets License
##---------------------------------------------------------------------------

##---------------------------------------------------
## Please set your wxWidgets configuration here
##--------------------------------------------------- 

SET(WXW_USE_DEBUG ON)
SET(WXW_USE_UNICODE OFF)
SET(WXW_USE_SHARED OFF)
SET(WXW_USE_UNIV OFF)
SET(WXW_USE_MONO OFF)
SET(WXW_FILE_VERSION "26")
SET(WXW_VERSION "2.6")

PROJECT( TREEMULTICTRL )

# The new FindWX script works better for  windows, but the 
# old script better for Linux (wx-config should be present
# for windows too)
IF(WIN32)
    INCLUDE (${TREEMULTICTRL_SOURCE_DIR}/bin/FindwxW.cmake)
ELSE(WIN32)
    INCLUDE (${TREEMULTICTRL_SOURCE_DIR}/bin/FindwxWin.cmake)
    
    # sync flags for new old flags
    SET(WXWIDGETS_FOUND            ${WXWINDOWS_FOUND})
    SET(WXWIDGETS_INCLUDE_DIR      ${WXWINDOWS_INCLUDE_DIR})
    SET(WXWIDGETS_LINK_DIRECTORIES ${WXWINDOWS_LINK_DIRECTORIES})
    SET(WXWIDGETS_LIBRARIES        ${WXWINDOWS_LIBRARIES})
ENDIF(WIN32)

# Finding Doxygen
INCLUDE (${CMAKE_ROOT}/Modules/FindDoxygen.cmake OPTIONAL)    

# Set flags for debug and release for compilation
SET(CMAKE_CXX_FLAGS "${CMAKE_WX_CXX_FLAGS}")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__WXDEBUG__ -DWXDEBUG=1")

# Options for building the stuff
OPTION( MAKE_DOXYGEN     "Generate doxygen documentation for the wxTreeMultiCtrl"                                    ON  )
OPTION( USE_CHECKBOXVIEW "Enable checkbox view functionality (if wanted, checkboxes can be placed in front of items" OFF )
OPTION( USE_XMLMAPPER    "Enable XML mapping functionality where wxTreeMultiCtrl can show XML data (using TinyXML)"  OFF )
OPTION( MAKE_DOXYGEN     "Generate doxygen documentation for the treemultictrl"                                      ON )

# Find the tinyXML library if this is not present in the lib dir, I assume
# we simply compile without.

IF(USE_XMLMAPPER)
  SET(TINYXML_LIB_PATH    CACHE PATH "Path for TinyXML library" )
  SET(TINYXML_LIB_INCLUDE CACHE PATH "Path for TinyXML library include files")
ELSE(USE_XMLMAPPER)
  SET(TINYXML_LIB_PATH    "")
  SET(TINYXML_LIB_INCLUDE "")
ENDIF(USE_XMLMAPPER)

IF(TINYXML_LIB_PATH AND TINYXML_LIB_INCLUDE)
  ## Set TinyXML lib when path and include are set
  IF(WXWIDGETS_USE_DEBUG)
    SET(TINYXML_LIB "tinyxmld")
  ELSE(WXWIDGETS_USE_DEBUG)
    SET(TINYXML_LIB "tinyxml")
  ENDIF(WXWIDGETS_USE_DEBUG)

ELSE(TINYXML_LIB_PATH AND TINYXML_LIB_INCLUDE)
  SET(TINYXML_LIB "")
ENDIF(TINYXML_LIB_PATH AND TINYXML_LIB_INCLUDE)

INCLUDE_DIRECTORIES(${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl
                    ${TREEMULTICTRL_SOURCE_DIR}/contrib/src/treemultictrl
                    ${WXWIDGETS_INCLUDE_DIR}
                    ${TINYXML_LIB_INCLUDE} )

LINK_DIRECTORIES( ${WXWIDGETS_LINK_DIRECTORIES} ${TINYXML_LIB_PATH} )

# If option Doxygen, create config script
CONFIGURE_FILE(${TREEMULTICTRL_SOURCE_DIR}/doc/Doxyfile.in
               ${TREEMULTICTRL_BINARY_DIR}/docs/Doxyfile
               @ONLY IMMEDIATE)                  

IF(MAKE_DOXYGEN)    
	ADD_CUSTOM_TARGET(api-docs ALL ${DOXYGEN}
					  ${TREEMULTICTRL_BINARY_DIR}/docs/Doxyfile DEPENDS
					  ${TREEMULTICTRL_SOURCE_DIR}/doc/Doxyfile.in )  
ENDIF(MAKE_DOXYGEN)                   

SUBDIRS(contrib)
