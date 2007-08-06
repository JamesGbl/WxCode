 # Name of project
 PROJECT(main)
 
 # Root for overall project
 SET(PROJROOT ${main_SOURCE_DIR}/../..)
 
 # Libraries to use
 SET(wxWidgets_USE_LIBS base core xml adv html xrc)
 
 # Find wxWidgets
 FIND_PACKAGE(wxWidgets)
 IF(wxWidgets_FOUND)
 
   # Sort out include files & paths
   INCLUDE(${wxWidgets_USE_FILE})
   INCLUDE_DIRECTORIES(${mvtst_SOURCE_DIR}
                       ${wxWidgets_INCLUDE_DIRS} 
                       ${PROJROOT}/include)
   
   # Other directories
   ADD_SUBDIRECTORY(../../src/rl cbuild)
   ADD_SUBDIRECTORY(../../src/utils-wx cbuild)
   ADD_SUBDIRECTORY(../../src/utils cbuild)
   
   SET(EXECUTABLE_OUTPUT_PATH ${PROJROOT}/samples/)   
   
   # Name of exec and source files it depends on
   ADD_EXECUTABLE(rlmain WIN32 rlmain.cpp main.h)
   
   # and for each of your dependant executable/library targets:
   TARGET_LINK_LIBRARIES(rlmain rl utils_wx utils ${wxWidgets_LIBRARIES})
 ELSE(wxWidgets_FOUND)
 
   # For convenience. When we cannot continue, inform the user
   MESSAGE("wxWidgets not found!")
   
 ENDIF(wxWidgets_FOUND)
 
