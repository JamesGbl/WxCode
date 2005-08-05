##---------------------------------------------------------------------------
## $RCSfile: CMakeLists.txt,v $
## $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/CMakeLists.txt,v $
## $Revision: 1.1 $
## $Date: 2005-08-05 18:23:28 $
##---------------------------------------------------------------------------
## Author:      Jorgen Bodde
## Copyright:   (c) Jorgen Bodde
## License:     wxWidgets License
##---------------------------------------------------------------------------

PROJECT( TREEMULTICTRL )

# Set predefined dir where the library is found
SET (LIBRARY_OUTPUT_PATH ${TREEMULTICTRL_BINARY_DIR}/contrib/lib/ CACHE PATH "Single output directory for building all libraries.")

# wxWidgets defines for the Linux environment
INCLUDE (${TREEMULTICTRL_SOURCE_DIR}/bin/FindwxWin.cmake)
INCLUDE (${CMAKE_ROOT}/Modules/FindDoxygen.cmake OPTIONAL)    

SET(CMAKE_CXX_FLAGS "${CMAKE_WX_CXX_FLAGS}")
SET(CMAKE_CXX_FLAGS_DEBUG "${CMAKE_CXX_FLAGS_DEBUG} -D__WXDEBUG__ -DWXDEBUG=1")

INCLUDE_DIRECTORIES(${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl
                    ${TREEMULTICTRL_SOURCE_DIR}/contrib/src/treemultictrl
                    ${WXWINDOWS_INCLUDE_DIR})

LINK_DIRECTORIES( ${WXWINDOWS_LINK_DIRECTORIES})

OPTION( MAKE_DOXYGEN     "Generate doxygen documentation for the wxTreeMultiCtrl"                                    ON )
OPTION( USE_CHECKBOXVIEW "Enable checkbox view functionality (if wanted, checkboxes can be placed in front of items" ON )

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
