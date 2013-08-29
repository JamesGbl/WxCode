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

set( wxVidCap_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Root dir of wxVidCap" FORCE)

# ---------------------------------------------------------------------------
# CMake build file for wxVidCap Library
# ---------------------------------------------------------------------------

if (NOT TARGET wxVidCapLib)

set( wxVidCap_VERSION         "1.0.0" )
set( wxVidCap_MAJOR_VERSION   "1" )
set( wxVidCap_MINOR_VERSION   "0" )
set( wxVidCap_RELEASE_VERSION "0" )

# ---------------------------------------------------------------------------

ADD_CPPCHECK_TEST(wxVidCap_CppCheck)

set( DOXYGEN_PROJECT_NAME        "wxVidCap" )
set( DOXYGEN_PROJECT_NUMBER      "${wxVidCap_VERSION}" )
set( DOXYGEN_OUTPUT_DIRECTORY    "${CMAKE_BINARY_DIR}/doc-wxVidCap" )
set( DOXYGEN_STRIP_FROM_PATH     "${wxVidCap_ROOT_DIR}/include" )
set( DOXYGEN_STRIP_FROM_INC_PATH "${wxVidCap_ROOT_DIR}/include" )
set( DOXYGEN_INPUT               "${wxVidCap_ROOT_DIR}/include" )
set( DOXYGEN_FILE_PATTERNS       "*.h *.hpp" )
set( DOXYGEN_PREDEFINED          "${DOXYGEN_PREDEFINED_WXWIDGETS}  WXDLLIMPEXP_DATA_VIDCAP(x)=x")

ADD_DOXYGEN( wxVidCap_doxygen
             ${wxVidCap_ROOT_DIR}/build/CMake-doxygen.in
             ${CMAKE_BINARY_DIR}/doxygen_wxVidCap.cfg
             ${CMAKE_BINARY_DIR} )

# ---------------------------------------------------------------------------

include_directories( ${wxVidCap_ROOT_DIR}/include/ )

set( WXVIDCAP_PROPERTIES_DEFINE_SYMBOL )
if ( BUILD_SHARED_LIBS_WIN_DLLS )
    set( WXVIDCAP_PROPERTIES_DEFINE_SYMBOL PROPERTIES DEFINE_SYMBOL WXMAKINGDLL_VIDCAP)
endif()

if (WIN32)
    set(wxVidCapLib_EXTRA_LIBS vfw32)
endif()

ADD_LIBRARY_FULL(wxVidCapLib
    HEADERS
        ${wxVidCap_ROOT_DIR}/include/wx/vidcap/vcap_v4l.h
        ${wxVidCap_ROOT_DIR}/include/wx/vidcap/vcap_vfw.h
        ${wxVidCap_ROOT_DIR}/include/wx/vidcap/vcapwin.h
        ${wxVidCap_ROOT_DIR}/src/precomp.h

        ${wxVidCap_ROOT_DIR}/src/vcapconv.h
        ${wxVidCap_ROOT_DIR}/src/vcapdlgs.h
    SOURCES
        ${wxVidCap_ROOT_DIR}/src/precomp.cpp
        ${wxVidCap_ROOT_DIR}/src/vcap_v4l.cpp
        ${wxVidCap_ROOT_DIR}/src/vcap_vfw.cpp
        ${wxVidCap_ROOT_DIR}/src/vcapconv.cpp
        ${wxVidCap_ROOT_DIR}/src/vcapdlgs.cpp
        ${wxVidCap_ROOT_DIR}/src/vcapwin.cpp
    PRECOMPILED_INCLUDE precomp.h
    PRECOMPILED_HEADER  ${wxVidCap_ROOT_DIR}/src/precomp.h
    PRECOMPILED_SOURCE  ${wxVidCap_ROOT_DIR}/src/precomp.cpp
    LINK_LIBRARIES
        ${wxVidCapLib_EXTRA_LIBS}
        ${wxWidgets_LIBRARIES}
    ${WXVIDCAP_PROPERTIES_DEFINE_SYMBOL}
    PROPERTIES FOLDER "wxVidCap")

WXLIKE_LIBRARY_NAMES( wxVidCapLib wxvidcap "${wxVidCap_VERSION}")

# ---------------------------------------------------------------------------

install(TARGETS wxVidCapLib
        EXPORT  wxVidCap_export
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(EXPORT wxVidCap_export
        DESTINATION share/wxvidcap/
        FILE wxVidCapConfig.cmake)

export( TARGETS wxVidCapLib
        FILE "${CMAKE_BINARY_DIR}/build/wxVidCapConfig.cmake")

install(FILES
        ${wxVidCap_ROOT_DIR}/docs/README.txt
        DESTINATION "doc")

install(FILES
        ${wxVidCap_ROOT_DIR}/include/wx/vidcap/vcap_v4l.h
        ${wxVidCap_ROOT_DIR}/include/wx/vidcap/vcap_vfw.h
        ${wxVidCap_ROOT_DIR}/include/wx/vidcap/vcapwin.h
        DESTINATION "include/wx/vidcap")

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
