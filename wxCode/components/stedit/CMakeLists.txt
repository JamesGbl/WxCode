# ---------------------------------------------------------------------------
# CMake build file
# ---------------------------------------------------------------------------

# ---------------------------------------------------------------------------
# Project name, sets ${wxStEdit_SOURCE_DIR} and ${wxStEdit_BINARY_DIR}

project( wxStEdit )

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

set( wxStEdit_ROOT_DIR ${CMAKE_CURRENT_SOURCE_DIR} CACHE INTERNAL "Root dir of wxStEdit" FORCE)

# ---------------------------------------------------------------------------
# CMake build file for wxStEdit Library
# ---------------------------------------------------------------------------

if (NOT TARGET wxStEditLib)

# The user can provide their own "wx/stedit/setup.h" by changing this path
if (NOT DEFINED wxStEdit_SETUP_INCLUDE_PATH)
    set(wxStEdit_SETUP_INCLUDE_PATH ${CMAKE_BINARY_DIR}/include CACHE INTERNAL "Path to the configured wxStEdit setup.h" FORCE)
endif()

#
if (NOT EXISTS ${CMAKE_BINARY_DIR}/include/wx/stedit/setup.h)
    configure_file(${wxStEdit_ROOT_DIR}/include/wx/stedit/setup0.h
                   ${CMAKE_BINARY_DIR}/include/wx/stedit/setup.h COPYONLY)
endif()

include_directories(${wxStEdit_ROOT_DIR}/include/ ${wxStEdit_SETUP_INCLUDE_PATH})


ADD_LIBRARY_FULL(wxStEditLib
    HEADERS
        ${CMAKE_BINARY_DIR}/include/wx/stedit/setup.h

        ${wxStEdit_ROOT_DIR}/include/wx/stedit/pairarr.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/setup0.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/steart.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/stedefs.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/stedit.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/stedlgs.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/steexprt.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/stefindr.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/steframe.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/stelangs.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/stemenum.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/stenoteb.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/steopts.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/steprefs.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/steprint.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/steshell.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/stesplit.h
        ${wxStEdit_ROOT_DIR}/include/wx/stedit/stestyls.h
        ${wxStEdit_ROOT_DIR}/src/precomp.h
        ${wxStEdit_ROOT_DIR}/src/wxext.h
        ${wxStEdit_ROOT_DIR}/src/stedlgs_wdr.h
    SOURCES
        ${wxStEdit_ROOT_DIR}/src/precomp.cpp
        ${wxStEdit_ROOT_DIR}/src/steart.cpp
        ${wxStEdit_ROOT_DIR}/src/stedit.cpp
        ${wxStEdit_ROOT_DIR}/src/stedlgs.cpp
        ${wxStEdit_ROOT_DIR}/src/stedlgs_wdr.cpp
        ${wxStEdit_ROOT_DIR}/src/steexprt.cpp
        ${wxStEdit_ROOT_DIR}/src/stefindr.cpp
        ${wxStEdit_ROOT_DIR}/src/steframe.cpp
        ${wxStEdit_ROOT_DIR}/src/stelangs.cpp
        ${wxStEdit_ROOT_DIR}/src/stemenum.cpp
        ${wxStEdit_ROOT_DIR}/src/stenoteb.cpp
        ${wxStEdit_ROOT_DIR}/src/steopts.cpp
        ${wxStEdit_ROOT_DIR}/src/steprefs.cpp
        ${wxStEdit_ROOT_DIR}/src/steprint.cpp
        ${wxStEdit_ROOT_DIR}/src/steshell.cpp
        ${wxStEdit_ROOT_DIR}/src/stesplit.cpp
        ${wxStEdit_ROOT_DIR}/src/stestyls.cpp
        ${wxStEdit_ROOT_DIR}/src/wxext.cpp
    PRECOMPILED_INCLUDE precomp.h
    PRECOMPILED_HEADER  ${wxStEdit_ROOT_DIR}/src/precomp.h
    PRECOMPILED_SOURCE  ${wxStEdit_ROOT_DIR}/src/precomp.cpp
    LINK_LIBRARIES
        ${wxWidgets_LIBRARIES}
    PROPERTIES DEFINE_SYMBOL "WXMAKINGDLL_STEDIT"
    PROPERTIES FOLDER "wxStEdit")

# ---------------------------------------------------------------------------

install(FILES
        ${wxStEdit_ROOT_DIR}/docs/ChangeLog.txt
        ${wxStEdit_ROOT_DIR}/docs/readme.txt
        ${wxStEdit_ROOT_DIR}/docs/readme.htm
        ${wxStEdit_ROOT_DIR}/docs/wxstedit.htm
        DESTINATION "doc")

# ---------------------------------------------------------------------------
# CMake build file for wxStEdit sample
# ---------------------------------------------------------------------------

ADD_EXECUTABLE_FULL( wxStEdit WIN32
    HEADERS
        ${wxStEdit_ROOT_DIR}/samples/stedit/readme_htm.hpp
        #${wxStEdit_ROOT_DIR}/samples/stedit/stedoc.h
        ${wxStEdit_ROOT_DIR}/samples/stedit/wxstedit.h
        ${wxStEdit_ROOT_DIR}/samples/stedit/wxstedit_htm.hpp
    SOURCES
        #${wxStEdit_ROOT_DIR}/samples/stedit/link.cpp
        #${wxStEdit_ROOT_DIR}/samples/stedit/stedoc.cpp
        ${wxStEdit_ROOT_DIR}/samples/stedit/wxstedit.cpp
        ${wxStEdit_ROOT_DIR}/samples/stedit/wxstedit.rc
    LINK_LIBRARIES
        wxStEditLib
        ${wxWidgets_LIBRARIES}
    PROPERTIES FOLDER "wxStEdit")

if (BUILD_SHARED_LIBS)
    APPEND_TARGET_PROPERTIES(wxStEdit PROPERTIES COMPILE_DEFINITIONS "WXUSINGDLL_STEDIT")
endif()

endif(NOT TARGET wxStEditLib)
