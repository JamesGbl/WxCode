# ---------------------------------------------------------------------------
# CMake build file
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Project name, sets ${wxThings_SOURCE_DIR} and ${wxThings_BINARY_DIR}

project( wxThings )

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

set( WXTHINGS_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Root dir of wxThings" FORCE)

# ---------------------------------------------------------------------------
# CMake build file for wxThings Library
# ---------------------------------------------------------------------------

if (NOT TARGET wxThingsLib)

include_directories( ${WXTHINGS_DIR}/include/ )

# We use some functions from GTK directly
if ("${wxWidgets_PLATFORM}" STREQUAL "gtk2")
    find_package(GTK2)
    include_directories( ${GTK2_INCLUDE_DIRS} )
elseif ("${wxWidgets_PLATFORM}" STREQUAL "gtk")
    find_package(GTK)
    include_directories( ${GTK_INCLUDE_DIRS} )
endif()

# ---------------------------------------------------------------------------
# wxFileBrowser uses /wx/generic/filedlgg.h which is not available in MSW.
# For MSW DLLs we need to replace WXDLLEXPORT with WXDLLIMPEXP_THINGS in
# wx/include/wx/generic/filedlgg.h

if (NOT EXISTS ${CMAKE_CURRENT_BINARY_DIR}/include/wx/generic/filedlgg.h)

    # Include this dir first so our filedlgg.h is found first.
    #include_directories( BEFORE ${CMAKE_CURRENT_BINARY_DIR}/include/ )

    # Use configure since it'll create the dirs for us too.
    configure_file(${wxWidgets_ROOT_DIR}/include/wx/generic/filedlgg.h
                   ${CMAKE_CURRENT_BINARY_DIR}/include/wx/generic/filedlgg.h COPYONLY)

    file(READ ${wxWidgets_ROOT_DIR}/include/wx/generic/filedlgg.h WXFILEDLGG_H_STRING)

    string(REPLACE "class WXDLLEXPORT wxGenericFileDialog"
                   "class WXDLLIMPEXP_THINGS wxGenericFileDialog"
                   WXFILEDLGG_H_STRING "${WXFILEDLGG_H_STRING}")

    string(REPLACE "class WXDLLEXPORT wxFileData"
                   "class WXDLLIMPEXP_THINGS wxFileData"
                   WXFILEDLGG_H_STRING "${WXFILEDLGG_H_STRING}")

    string(REPLACE "class WXDLLEXPORT wxFileCtrl"
                   "class WXDLLIMPEXP_THINGS wxFileCtrl"
                   WXFILEDLGG_H_STRING "${WXFILEDLGG_H_STRING}")

    file(WRITE ${CMAKE_CURRENT_BINARY_DIR}/include/wx/generic/filedlgg.h "${WXFILEDLGG_H_STRING}")
endif()

# ---------------------------------------------------------------------------
# wxFileBrowser uses /wx/generic/filedlgg.h which is not available in MSW.
# For MSW DLLs we cannot get at wxGetAvailableDrives() in wx/src/generic/dirctrlg.cpp
# which wx/src/generic/filedlgg.cpp requires. We simply cut out that function
# and paste it into our code to get it.
# If this breaks we can resort to some other means, but this works for wx 2.8 and 2.9

if ( ${BUILDING_DLLS} )

    file(READ ${wxWidgets_ROOT_DIR}/src/generic/dirctrlg.cpp WXDIRCTRLG_CPP_STRING)

    string(FIND "${WXDIRCTRLG_CPP_STRING}" "// wxGetAvailableDrives, for WINDOWS, DOS, OS2, MAC" WXDIRCTRLG_CPP_STRING_START)
    string(FIND "${WXDIRCTRLG_CPP_STRING}" "// wxIsDriveAvailable"                               WXDIRCTRLG_CPP_STRING_END)

    math(EXPR WXDIRCTRLG_CPP_STRING_LEN "${WXDIRCTRLG_CPP_STRING_END} - ${WXDIRCTRLG_CPP_STRING_START}")

    string(SUBSTRING "${WXDIRCTRLG_CPP_STRING}" "${WXDIRCTRLG_CPP_STRING_START}" "${WXDIRCTRLG_CPP_STRING_LEN}" CMAKE_CONFIGURE_WXTHINGS_FILEDLGG)

else()

    set(CMAKE_CONFIGURE_WXTHINGS_FILEDLGG "Non DLL linking : wxGetAvailableDrives() is available in dirctrlg.cpp")

endif()

configure_file(${WXTHINGS_DIR}/src/filedlgg.cpp
               ${CMAKE_CURRENT_BINARY_DIR}/src/filedlgg.cpp @ONLY)

# ---------------------------------------------------------------------------

ADD_LIBRARY_FULL(wxThingsLib
    HEADERS
        ${WXTHINGS_DIR}/include/wx/things/block.h
        ${WXTHINGS_DIR}/include/wx/things/bmpcombo.h
        ${WXTHINGS_DIR}/include/wx/things/dropdown.h
        ${WXTHINGS_DIR}/include/wx/things/filebrws.h
        ${WXTHINGS_DIR}/include/wx/things/genergdi.h
        ${WXTHINGS_DIR}/include/wx/things/geometry.h
        ${WXTHINGS_DIR}/include/wx/things/matrix2d.h
        ${WXTHINGS_DIR}/include/wx/things/medsort.h
        ${WXTHINGS_DIR}/include/wx/things/menubtn.h
        ${WXTHINGS_DIR}/include/wx/things/optvalue.h
        ${WXTHINGS_DIR}/include/wx/things/range.h
        ${WXTHINGS_DIR}/include/wx/things/spinctld.h
        ${WXTHINGS_DIR}/include/wx/things/thingdef.h
        ${WXTHINGS_DIR}/include/wx/things/toggle.h
    SOURCES
        ${WXTHINGS_DIR}/src/block.cpp
        ${WXTHINGS_DIR}/src/bmpcombo.cpp
        ${WXTHINGS_DIR}/src/dropdown.cpp
        ${WXTHINGS_DIR}/src/filebrws.cpp

        #${WXTHINGS_DIR}/src/filedlgg.cpp
        ${CMAKE_CURRENT_BINARY_DIR}/src/filedlgg.cpp

        ${WXTHINGS_DIR}/src/genergdi.cpp
        ${WXTHINGS_DIR}/src/geometry.cpp
        ${WXTHINGS_DIR}/src/matrix2d.cpp
        ${WXTHINGS_DIR}/src/menubtn.cpp
        ${WXTHINGS_DIR}/src/optvalue.cpp
        ${WXTHINGS_DIR}/src/range.cpp
        ${WXTHINGS_DIR}/src/spinctld.cpp
        ${WXTHINGS_DIR}/src/toggle.cpp
    LINK_LIBRARIES
        ${wxWidgets_LIBRARIES}
    PROPERTIES DEFINE_SYMBOL "WXMAKINGDLL_THINGS"
    PROPERTIES FOLDER "wxThings")

ADD_TARGET_INCLUDE_PATHS(wxThingsLib ${CMAKE_CURRENT_BINARY_DIR}/include/)

# ---------------------------------------------------------------------------

install(FILES
    ${WXTHINGS_DIR}/docs/readme.txt
    DESTINATION "doc")

# ---------------------------------------------------------------------------
# CMake build file for wxThings sample
# ---------------------------------------------------------------------------

ADD_EXECUTABLE_FULL( wxThingsDemo WIN32
    HEADERS ""
    SOURCES
        ${WXTHINGS_DIR}/samples/things/thingsdemo.cpp
        ${WXTHINGS_DIR}/samples/things/thingsdemo.rc
    LINK_LIBRARIES
        wxThingsLib
        ${wxWidgets_LIBRARIES}
    PROPERTIES FOLDER "wxThings")

# ---------------------------------------------------------------------------
# CMake build file for wxThings FileBrowser sample
# ---------------------------------------------------------------------------

ADD_EXECUTABLE_FULL( wxFileBrowser WIN32
    HEADERS ""
    SOURCES
        ${WXTHINGS_DIR}/samples/filebrws/wxfilebrowser.cpp
        ${WXTHINGS_DIR}/samples/filebrws/wxfilebrowser.rc
    LINK_LIBRARIES
        wxThingsLib
        ${wxWidgets_LIBRARIES}
    PROPERTIES FOLDER "wxThings")

endif(NOT TARGET wxThingsLib)
