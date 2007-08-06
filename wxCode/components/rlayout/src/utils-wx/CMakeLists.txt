
 # Create a library for this directory
 IF(NOT RL_NO_STATE)
   # The bigger sample
   SET(UTILS_WX_SRCS LocalMenu.cpp SizerUtils.cpp wxUtils.cpp NamedDir.cpp)
 ELSE(NOT RL_NO_STATE)
   # The samll sample
   SET(UTILS_WX_SRCS SizerUtils.cpp wxUtils.cpp)
 ENDIF(NOT RL_NO_STATE)
 
 ADD_LIBRARY(utils_wx STATIC ${UTILS_WX_SRCS}) 
 