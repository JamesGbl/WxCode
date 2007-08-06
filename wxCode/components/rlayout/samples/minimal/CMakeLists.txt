 # Name of project
 PROJECT(minimal)
 
 # Root for overall project
 SET(PROJROOT ${minimal_SOURCE_DIR}/../..)
 
 # Libraries to use
 SET(wxWidgets_USE_LIBS base core)
 
 # Find wxWidgets
 FIND_PACKAGE(wxWidgets)
 IF(wxWidgets_FOUND)
 
   # We turn off support for XML state in the minimal sample
   OPTION(RL_NO_STATE "Exclude support for XML state in wxRlLayout" ON) 
   IF(RL_NO_STATE)
     ADD_DEFINITIONS( -DRL_NO_STATE )
   ENDIF(RL_NO_STATE)
   
   # Sort out include files & paths
   INCLUDE(${wxWidgets_USE_FILE})
   INCLUDE_DIRECTORIES(${wxWidgets_INCLUDE_DIRS} 
                       ${PROJROOT}/include)
   
   # Other directories
   ADD_SUBDIRECTORY(../../src/rl cbuild)
   ADD_SUBDIRECTORY(../../src/utils-wx cbuild)
   
   SET(EXECUTABLE_OUTPUT_PATH ${PROJROOT}/samples)   
   
   # Name of exec and source files it depends on
   ADD_EXECUTABLE(rlminimal WIN32 minimal.cpp)
   
   # and for each of your dependant executable/library targets:
   TARGET_LINK_LIBRARIES(rlminimal rl utils_wx ${wxWidgets_LIBRARIES})
   
 ELSE(wxWidgets_FOUND)
 
   # For convenience. When we cannot continue, inform the user
   MESSAGE("wxWidgets not found!")
   
 ENDIF(wxWidgets_FOUND)
 
