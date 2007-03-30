# CMAKE BUILD FILE FOR WXRUBBERBAND SAMPLE 
# USE WXIDGETS
# (c) LUCIEN SCHREIBER CREALP 2006


PROJECT (wxRubberSample)

#search all source files
FILE (GLOB SRC_FILES ../src/*.cpp)
FILE (GLOB SRC_SAMPLE ../sample/*.cpp)
SOURCE_GROUP (source FILES ${SRC_FILES} ${SRC_SAMPLE})


# search all headers files
FILE (GLOB HEAD_FILES "../include/wx/rubberband/*.h")
INCLUDE_DIRECTORIES("../include")
SOURCE_GROUP (header FILES ${HEAD_FILES})

#VERSION NUMBER
SET (wxRubberBand_VERSION_MAJOR 1)
SET (wxRubberBand_VERSION_MINOR 0)

# WXWIDGETS
# DEFINE ALL INCLUDES AND SO ON
INCLUDE ("cmake/Use_wxWindows.cmake")

 
# ADD ALSO HEADERS NOT REALY NEEDED JUST FOR VS
# COMPILING

IF (WIN32)

	SOURCE_GROUP (resource FILES ../sample/wxRubberSample.rc)
 	ADD_EXECUTABLE (wxRubberSample WIN32 ${SRC_FILES} ${SRC_SAMPLE} ${HEAD_FILES} ../sample/wxRubberSample.rc)
	TARGET_LINK_LIBRARIES (wxRubberSample ${WXWINDOWS_LIBS})

ELSE (WIN32)
 
 	ADD_EXECUTABLE (wxRubberSample ${SRC_FILES} ${SRC_SAMPLE} ${HEAD_FILES} )

ENDIF (WIN32)


