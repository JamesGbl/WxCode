# ---------------------------------------------------------------------------
# CMake build file
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Project name, sets ${wxVidCap_SOURCE_DIR} and ${wxVidCap_BINARY_DIR}

project( wxVidCap )

# ===========================================================================
# General settings for CMake
# ===========================================================================

# ---------------------------------------------------------------------------
# Use a minimum version of CMake of 2.8, >= 2.8.3 is prefered

cmake_minimum_required( VERSION 2.8 )

# ---------------------------------------------------------------------------
# Setup the CMake environment

include( build/CMakewxAppLib.cmake )
include( build/CMakeFunctions.txt )

# ===========================================================================
# Add subdirectories containing CMakeLists.txt files or specify projects
# ===========================================================================

set( wxVidCap_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Root dir of wxVidCap" FORCE)

# ---------------------------------------------------------------------------
# CMake build file for wxVidCap Library
# ---------------------------------------------------------------------------
 
if (NOT TARGET wxVidCapLib)

include_directories( ${wxVidCap_ROOT_DIR}/include/ )
 
ADD_LIBRARY_FULL(wxVidCapLib
    HEADERS 
        ${wxVidCap_ROOT_DIR}/include/wx/vidcap/vcap_v4l.h
        ${wxVidCap_ROOT_DIR}/include/wx/vidcap/vcap_vfw.h
        ${wxVidCap_ROOT_DIR}/include/wx/vidcap/vcapwin.h
    SOURCES 
        ${wxVidCap_ROOT_DIR}/src/vcap_v4l.cpp
        ${wxVidCap_ROOT_DIR}/src/vcap_vfw.cpp
        ${wxVidCap_ROOT_DIR}/src/vcapconv.cpp
        ${wxVidCap_ROOT_DIR}/src/vcapconv.h
        ${wxVidCap_ROOT_DIR}/src/vcapdlgs.cpp
        ${wxVidCap_ROOT_DIR}/src/vcapdlgs.h
        ${wxVidCap_ROOT_DIR}/src/vcapwin.cpp
    LINK_LIBRARIES 
        vfw32
        ${wxWidgets_LIBRARIES}
    PROPERTIES DEFINE_SYMBOL "WXMAKINGDLL_VIDCAP"
    PROPERTIES FOLDER "wxVidCap")

# ---------------------------------------------------------------------------

install(FILES 
    ${wxVidCap_ROOT_DIR}/docs/README.txt
    DESTINATION "doc")

# ---------------------------------------------------------------------------
# CMake build file for wxVidCap sample
# ---------------------------------------------------------------------------

ADD_EXECUTABLE_FULL( wxVidCap WIN32
    HEADERS 
        ${wxVidCap_ROOT_DIR}/samples/vidcap/wxvidcap.h
        ${wxVidCap_ROOT_DIR}/samples/vidcap/wxvidcap_wdr.h
    SOURCES 
        ${wxVidCap_ROOT_DIR}/samples/vidcap/wxvidcap.cpp
        ${wxVidCap_ROOT_DIR}/samples/vidcap/wxvidcap_wdr.cpp
        ${wxVidCap_ROOT_DIR}/samples/vidcap/wxvidcap.rc
    LINK_LIBRARIES 
        wxVidCapLib
        ${wxWidgets_LIBRARIES}        
    PROPERTIES FOLDER "wxVidCap")

endif(NOT TARGET wxVidCapLib)
