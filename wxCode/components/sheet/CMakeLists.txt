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

set( wxSheet_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE PATH "Root dir of wxSheet" FORCE)

# ---------------------------------------------------------------------------
# CMake build file for wxSheet Library
# ---------------------------------------------------------------------------

if (NOT TARGET wxSheetLib)

set( wxSheet_VERSION         "1.2.0" )
set( wxSheet_MAJOR_VERSION   "1" )
set( wxSheet_MINOR_VERSION   "2" )
set( wxSheet_RELEASE_VERSION "0" )

# ---------------------------------------------------------------------------

ADD_CPPCHECK_TEST(wxSheet_CppCheck)

set( DOXYGEN_PROJECT_NAME        "wxSheet" )
set( DOXYGEN_PROJECT_NUMBER      "${wxSheet_VERSION}" )
set( DOXYGEN_OUTPUT_DIRECTORY    "${CMAKE_BINARY_DIR}/doc-wxSheet" )
set( DOXYGEN_STRIP_FROM_PATH     "${wxSheet_ROOT_DIR}/include" )
set( DOXYGEN_STRIP_FROM_INC_PATH "${wxSheet_ROOT_DIR}/include" )
set( DOXYGEN_INPUT               "${wxSheet_ROOT_DIR}/include" )
set( DOXYGEN_FILE_PATTERNS       "*.h *.hpp" )
set( DOXYGEN_PREDEFINED          "${DOXYGEN_PREDEFINED_WXWIDGETS}  WXDLLIMPEXP_DATA_SHEET(x)=x")

ADD_DOXYGEN( wxSheet_doxygen
             ${wxSheet_ROOT_DIR}/build/CMake-doxygen.in
             ${CMAKE_BINARY_DIR}/doxygen_wxSheet.cfg
             ${CMAKE_BINARY_DIR} )

# ---------------------------------------------------------------------------

include_directories(${wxSheet_ROOT_DIR}/include/)

set( WXSHEET_PROPERTIES_DEFINE_SYMBOL )
if ( BUILD_SHARED_LIBS_WIN_DLLS )
    set( WXSHEET_PROPERTIES_DEFINE_SYMBOL PROPERTIES DEFINE_SYMBOL WXMAKINGDLL_SHEET)
endif()

ADD_LIBRARY_FULL(wxSheetLib
    HEADERS
        ${wxSheet_ROOT_DIR}/include/wx/sheet/pairarr.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheet.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetatr.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetdef.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetedg.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetedt.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetevt.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetren.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetsel.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetspt.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheettbl.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/sheetval.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/srtobjar.h
        ${wxSheet_ROOT_DIR}/include/wx/sheet/wx24defs.h
        ${wxSheet_ROOT_DIR}/src/precomp.h
    SOURCES
        ${wxSheet_ROOT_DIR}/src/precomp.cpp
        ${wxSheet_ROOT_DIR}/src/sheet.cpp
        ${wxSheet_ROOT_DIR}/src/sheetatr.cpp
        ${wxSheet_ROOT_DIR}/src/sheetedg.cpp
        ${wxSheet_ROOT_DIR}/src/sheetedt.cpp
        ${wxSheet_ROOT_DIR}/src/sheetevt.cpp
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
    ${WXSHEET_PROPERTIES_DEFINE_SYMBOL}
    PROPERTIES FOLDER "wxSheet")

WXLIKE_LIBRARY_NAMES( wxSheetLib wxsheet "${wxSheet_VERSION}")

# ---------------------------------------------------------------------------

install(TARGETS wxSheetLib
        EXPORT  wxSheet_export
        RUNTIME DESTINATION bin
        LIBRARY DESTINATION lib
        ARCHIVE DESTINATION lib)

install(EXPORT wxSheet_export
        DESTINATION share/wxsheet/
        FILE wxSheetConfig.cmake)

export( TARGETS wxSheetLib
        FILE "${CMAKE_BINARY_DIR}/build/wxSheetConfig.cmake")

install(FILES
        ${wxSheet_ROOT_DIR}/docs/ChangeLog.txt
        ${wxSheet_ROOT_DIR}/docs/README.txt
        ${wxSheet_ROOT_DIR}/docs/readme.htm
        DESTINATION "doc")

install(FILES
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
        DESTINATION "include/wx/sheet")

# ---------------------------------------------------------------------------
# CMake build file for wxSheet sample
# ---------------------------------------------------------------------------

ADD_EXECUTABLE_FULL( wxSheetDemo WIN32 MACOSX_BUNDLE
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





