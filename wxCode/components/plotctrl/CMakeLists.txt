# ---------------------------------------------------------------------------
# CMake build file
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Project name, sets ${wxPlotCtrl_SOURCE_DIR} and ${wxPlotCtrl_BINARY_DIR}

project( wxPlotCtrl )

# ===========================================================================
# General settings for CMake
# ===========================================================================

# ---------------------------------------------------------------------------
# Use a minimum version of CMake of 2.8, >= 2.8.3 is prefered

cmake_minimum_required( VERSION 2.8 )

# ---------------------------------------------------------------------------
# Setup the CMake environment

include( build/CMakeProject.cmake )
include( build/CMakewxAppLib.cmake )

# Specify what wxWidgets libs we need to link to. Note: 'core' must be before 'base'.
# If you call this CMakeLists.txt from another one you may have already called FIND_WXWIDGETS()
if (NOT DEFINED wxWidgets_COMPONENTS)
    set(wxWidgets_COMPONENTS adv core base) # minimal set for static lib/dll
    #set(wxWidgets_COMPONENTS gl xrc xml net media propgrid richtext aui stc html adv core base) # for multilib/dll
    #set(wxWidgets_COMPONENTS stc mono) # for monolithic
endif()

FIND_WXWIDGETS(wxWidgets_COMPONENTS) # Ok to call multiple times
# wxWidgets include (this will do all the magic to configure everything)
include( "${wxWidgets_USE_FILE}" )

# ===========================================================================
# Add subdirectories containing CMakeLists.txt files or specify projects
# ===========================================================================

# ---------------------------------------------------------------------------
# We link to the wxThings library that is part of wxCode
# ---------------------------------------------------------------------------

# Start with a test to see if this is a full wxCode SVN checkout
# If it fails, leave wxThings_ROOT_DIR empty to make it clear that it needs to be set.

if ("${wxThings_ROOT_DIR}" STREQUAL "")
    if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../wxthings/include/wx/things/genergdi.h)
        if (EXISTS ${CMAKE_CURRENT_SOURCE_DIR}/../wxthings/CMakeLists.txt)
            get_filename_component(wxThings_ROOT_DIR "${CMAKE_CURRENT_SOURCE_DIR}/../wxthings" ABSOLUTE)
        endif()
    endif()
endif()

set( wxThings_ROOT_DIR ${wxThings_ROOT_DIR} CACHE PATH "Specify the path to the root directory wxThings library" FORCE)

if (NOT EXISTS ${wxThings_ROOT_DIR}/include/wx/things/genergdi.h)
    MESSAGE(FATAL_ERROR "Unable to find \"wx/things/genergdi.h\", please set variable -DwxThings_ROOT_DIR=/path/to/wxThings")
endif()

# add_subdirectory() errors on multiple calls, only add it once per project
# We also have to specify a binary dir, so we'll make it a child dir of our binary dir.
if (NOT TARGET wxThingsLib)
    add_subdirectory(${wxThings_ROOT_DIR} ${CMAKE_CURRENT_BINARY_DIR}/wxthings)
endif()

# ---------------------------------------------------------------------------

set( wxPlotCtrl_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Root dir of wxPlotCtrl" FORCE)

# ---------------------------------------------------------------------------
# CMake build file for wxPlotCtrl Library
# ---------------------------------------------------------------------------

if (NOT TARGET wxPlotCtrlLib)

set( wxPlotCtrl_VERSION         "1.1.0" )
set( wxPlotCtrl_MAJOR_VERSION   "1" )
set( wxPlotCtrl_MINOR_VERSION   "1" )
set( wxPlotCtrl_RELEASE_VERSION "0" )

# ---------------------------------------------------------------------------

ADD_CPPCHECK_TEST(wxPlotCtrl_CppCheck)

set( DOXYGEN_PROJECT_NAME        "wxPlotCtrl" )
set( DOXYGEN_PROJECT_NUMBER      "${wxPlotCtrl_VERSION}" )
set( DOXYGEN_OUTPUT_DIRECTORY    "${CMAKE_BINARY_DIR}/doc-wxPlotCtrl" )
set( DOXYGEN_STRIP_FROM_PATH     "${wxPlotCtrl_ROOT_DIR}/include" )
set( DOXYGEN_STRIP_FROM_INC_PATH "${wxPlotCtrl_ROOT_DIR}/include" )
set( DOXYGEN_INPUT               "${wxPlotCtrl_ROOT_DIR}/include" )
set( DOXYGEN_FILE_PATTERNS       "*.h *.hpp" )
set( DOXYGEN_PREDEFINED          "${DOXYGEN_PREDEFINED_WXWIDGETS}  WXDLLIMPEXP_DATA_PLOTCTRL(x)=x")

ADD_DOXYGEN( wxPlotCtrl_doxygen
             ${wxPlotCtrl_ROOT_DIR}/build/CMake-doxygen.in
             ${CMAKE_BINARY_DIR}/doxygen_wxPlotCtrl.cfg
             ${CMAKE_BINARY_DIR} )

# ---------------------------------------------------------------------------

# wxThings_ROOT_DIR is set when add_subdirectory( wxThings/build ) has been called
include_directories( ${wxThings_ROOT_DIR}/include/ )
include_directories( ${wxPlotCtrl_ROOT_DIR}/include/ )

# We use some functions from GTK directly
if ("${wxWidgets_PORTNAME}" STREQUAL "gtk2")
    find_package(GTK2)

    if (NOT GTK2_FOUND)
        MESSAGE(WARNING "Unable to find GTK2, you might need to install the gtk2-devel package")
    endif()

    include_directories( ${GTK2_INCLUDE_DIRS} )
elseif ("${wxWidgets_PORTNAME}" STREQUAL "gtk")
    find_package(GTK)

    if (NOT GTK_FOUND)
        MESSAGE(WARNING "Unable to find GTK, you might need to install the gtk-devel package")
    endif()

    include_directories( ${GTK_INCLUDE_DIRS} )
endif()

set( WXPLOTCTRL_PROPERTIES_DEFINE_SYMBOL )
if ( BUILD_SHARED_LIBS_WIN_DLLS )
    set( WXPLOTCTRL_PROPERTIES_DEFINE_SYMBOL PROPERTIES DEFINE_SYMBOL WXMAKINGDLL_PLOTCTRL)
endif()

ADD_LIBRARY_FULL(wxPlotCtrlLib
    HEADERS
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/fourier.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/fparser.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/lm_lsqr.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotctrl.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotcurv.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotdata.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotdefs.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotdraw.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotfunc.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotmark.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotprnt.h

        ${wxPlotCtrl_ROOT_DIR}/src/fparser.hh
        ${wxPlotCtrl_ROOT_DIR}/src/fpconfig.hh
        ${wxPlotCtrl_ROOT_DIR}/src/fptypes.hh
        ${wxPlotCtrl_ROOT_DIR}/src/precomp.h
    SOURCES
        ${wxPlotCtrl_ROOT_DIR}/src/precomp.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/fourier.c
        ${wxPlotCtrl_ROOT_DIR}/src/fparser.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/fpoptimizer.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/lm_lsqr.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotctrl.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotcurv.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotdata.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotdraw.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotfunc.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotmark.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotprnt.cpp
    PRECOMPILED_INCLUDE precomp.h
    PRECOMPILED_HEADER  ${wxPlotCtrl_ROOT_DIR}/src/precomp.h
    PRECOMPILED_SOURCE  ${wxPlotCtrl_ROOT_DIR}/src/precomp.cpp
    PRECOMPILED_SOURCES
        ${wxPlotCtrl_ROOT_DIR}/src/precomp.cpp
        #${wxPlotCtrl_ROOT_DIR}/src/fourier.c
        ${wxPlotCtrl_ROOT_DIR}/src/fparser.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/fpoptimizer.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/lm_lsqr.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotctrl.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotcurv.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotdata.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotdraw.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotfunc.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotmark.cpp
        ${wxPlotCtrl_ROOT_DIR}/src/plotprnt.cpp
    LINK_LIBRARIES
        wxThingsLib
        ${wxWidgets_LIBRARIES}
    ${WXPLOTCTRL_PROPERTIES_DEFINE_SYMBOL}
    PROPERTIES FOLDER "wxPlotCtrl")

WXLIKE_LIBRARY_NAMES( wxPlotCtrlLib wxplotctrl "${wxPlotCtrl_VERSION}")

# ---------------------------------------------------------------------------

install(TARGETS wxPlotCtrlLib
        #EXPORT  wxPlotCtrl_export
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

#install(EXPORT wxPlotCtrl_export
#        DESTINATION share/wxplotctrl/
#        FILE wxPlotCtrlConfig.cmake)

export( TARGETS wxPlotCtrlLib wxThingsLib
        FILE "${CMAKE_BINARY_DIR}/build/wxPlotCtrlConfig.cmake")

install(FILES
    ${wxPlotCtrl_ROOT_DIR}/docs/changelog.txt
    ${wxPlotCtrl_ROOT_DIR}/docs/readme.htm
    ${wxPlotCtrl_ROOT_DIR}/src/fparser.htm
    ${wxPlotCtrl_ROOT_DIR}/src/fparser.txt
    ${wxPlotCtrl_ROOT_DIR}/src/fourier.txt
    DESTINATION "doc")

install(FILES
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/fourier.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/fparser.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/lm_lsqr.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotctrl.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotcurv.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotdata.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotdefs.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotdraw.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotfunc.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotmark.h
        ${wxPlotCtrl_ROOT_DIR}/include/wx/plotctrl/plotprnt.h
        DESTINATION "include/wx/plotctrl")

# ---------------------------------------------------------------------------
# CMake build file for wxPlotCtrl sample
# ---------------------------------------------------------------------------

ADD_EXECUTABLE_FULL( wxPlotCtrlDemo WIN32 MACOSX_BUNDLE
    HEADERS
        ""
    SOURCES
        ${wxPlotCtrl_ROOT_DIR}/samples/plotctrl/wxplotctrl.cpp
        ${wxPlotCtrl_ROOT_DIR}/samples/plotctrl/wxplotctrl.rc
    LINK_LIBRARIES
        wxPlotCtrlLib
        ${wxWidgets_LIBRARIES}
    PROPERTIES FOLDER "wxPlotCtrl")

endif(NOT TARGET wxPlotCtrlLib)
