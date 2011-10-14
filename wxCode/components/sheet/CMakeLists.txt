# ---------------------------------------------------------------------------
# CMake build file
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Project name, sets ${wxSheet_SOURCE_DIR} and ${wxSheet_BINARY_DIR}

project( wxSheet )

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

set( wxSheet_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Root dir of wxSheet" FORCE)

# ---------------------------------------------------------------------------
# CMake build file for wxSheet Library
# ---------------------------------------------------------------------------

if (NOT TARGET wxSheetLib)

include_directories(${wxSheet_ROOT_DIR}/include/)

ADD_LIBRARY_FULL(wxSheetLib
    HEADERS
        ${wxSheet_ROOT_DIR}/include/wx/sheet/pairarr.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheet.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetatr.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetdef.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetedg.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetedt.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetren.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetsel.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetspt.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheettbl.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetval.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/srtobjar.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/wx24defs.h
    SOURCES
        ${wxSheet_ROOT_DIR}/src/sheet.cpp
        ${wxSheet_ROOT_DIR}/src/sheetatr.cpp
        ${wxSheet_ROOT_DIR}/src/sheetedg.cpp
        ${wxSheet_ROOT_DIR}/src/sheetedt.cpp
        ${wxSheet_ROOT_DIR}/src/sheetren.cpp
        ${wxSheet_ROOT_DIR}/src/sheetsel.cpp
        ${wxSheet_ROOT_DIR}/src/sheetspt.cpp
        ${wxSheet_ROOT_DIR}/src/sheettbl.cpp
        ${wxSheet_ROOT_DIR}/src/sheetval.cpp

    PRECOMPILED_INCLUDE precomp.h
    PRECOMPILED_HEADER  ${wxSheet_ROOT_DIR}/src/precomp.h
    PRECOMPILED_SOURCE  ${wxSheet_ROOT_DIR}/src/precomp.cpp

    LINK_LIBRARIES
        ${wxWidgets_LIBRARIES}
    PROPERTIES DEFINE_SYMBOL "WXMAKINGDLL_SHEET"
    PROPERTIES FOLDER "wxSheet")

# ---------------------------------------------------------------------------

install(FILES
        ${wxSheet_ROOT_DIR}/docs/ChangeLog.txt
        ${wxSheet_ROOT_DIR}/docs/README.txt
        ${wxSheet_ROOT_DIR}/docs/readme.htm
        DESTINATION "doc")

# ---------------------------------------------------------------------------
# CMake build file for wxSheet sample
# ---------------------------------------------------------------------------

ADD_EXECUTABLE_FULL( wxSheetDemo WIN32
    HEADERS
        ${wxSheet_ROOT_DIR}/samples/sheet/sheetdemo.h
    SOURCES
        ${wxSheet_ROOT_DIR}/samples/sheet/sheetdemo.cpp
        ${wxSheet_ROOT_DIR}/samples/sheet/sheetdemo.rc
    LINK_LIBRARIES
        wxSheetLib
        ${wxWidgets_LIBRARIES}
    PROPERTIES FOLDER "wxSheet")

endif(NOT TARGET wxSheetLib)





