##---------------------------------------------------------------------------
## $RCSfile: CMakeLists.txt,v $
## $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/CMakeLists.txt,v $
## $Revision: 1.3 $
## $Date: 2005-08-11 20:37:32 $
##---------------------------------------------------------------------------
## Author:      Jorgen Bodde
## Copyright:   (c) Jorgen Bodde
## License:     wxWidgets License
##---------------------------------------------------------------------------

##---------------------------------------------------
## Please set your wxWidgets configuration here
##--------------------------------------------------- 

SET(WXW_USE_DEBUG OFF)
SET(WXW_USE_UNICODE OFF)
SET(WXW_USE_SHARED OFF)
SET(WXW_USE_UNIV OFF)
SET(WXW_USE_MONO OFF)
SET(WXW_FILE_VERSION "26")
SET(WXW_VERSION "2.6")

PROJECT( TREEMULTICTRL )

IF(WIN32)
    INCLUDE (${TREEMULTICTRL_SOURCE_DIR}/bin/FindwxW.cmake)
ELSE(WIN32)
    INCLUDE (${TREEMULTICTRL_SOURCE_DIR}/bin/FindwxWin.cmake)
    
    # sync flags 
    SET(WXWIDGETS_FOUND            ${WXWINDOWS_FOUND})
    SET(WXWIDGETS_INCLUDE_DIR      ${WXWINDOWS_INCLUDE_DIR})
    SET(WXWIDGETS_LINK_DIRECTORIES ${WXWINDOWS_LINK_DIRECTORIES})
    SET(WXWIDGETS_LIBRARIES        ${WXWINDOWS_LIBRARIES})
ENDIF(WIN32)

## Finding Doxygen
INCLUDE (${CMAKE_ROOT}/Modules/FindDoxygen.cmake OPTIONAL)    

# Set predefined dir where the library is found
SET (LIBRARY_OUTPUT_PATH ${TREEMULTICTRL_BINARY_DIR}/contrib/lib/ CACHE PATH "Single output directory for building all libraries.")

SET(CMAKE_CXX_FLAGS "${CMAKE_WX_CXX_FLAGS}")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__WXDEBUG__ -DWXDEBUG=1")

INCLUDE_DIRECTORIES(${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl
                    ${TREEMULTICTRL_SOURCE_DIR}/contrib/src/treemultictrl
                    ${WXWIDGETS_INCLUDE_DIR})

LINK_DIRECTORIES( ${WXWIDGETS_LINK_DIRECTORIES})

OPTION( MAKE_DOXYGEN     "Generate doxygen documentation for the wxTreeMultiCtrl"                                    ON  )
OPTION( USE_CHECKBOXVIEW "Enable checkbox view functionality (if wanted, checkboxes can be placed in front of items" OFF )
OPTION( USE_XMLMAPPER    "Enable XML mapping functionality where wxTreeMultiCtrl can show LibXML2 data"              ON )

# If option Doxygen, create config script
CONFIGURE_FILE(${TREEMULTICTRL_SOURCE_DIR}/doc/Doxyfile.in
               ${TREEMULTICTRL_BINARY_DIR}/docs/Doxyfile
               @ONLY IMMEDIATE)                  

OPTION( MAKE_DOXYGEN     "Generate doxygen documentation for the treemultictrl"                                      ON )

IF(MAKE_DOXYGEN)    
	ADD_CUSTOM_TARGET(api-docs ALL ${DOXYGEN}
					  ${TREEMULTICTRL_BINARY_DIR}/docs/Doxyfile DEPENDS
					  ${TREEMULTICTRL_SOURCE_DIR}/doc/Doxyfile.in )  
ENDIF(MAKE_DOXYGEN)                   

SUBDIRS(contrib)
