
 # Create a library for this directory
 IF(NOT RL_NO_STATE)
   SET(RL_SRCS RlManager.cpp RlPainter.cpp RlDialog.cpp
       RlDialogMgr.cpp RlFromXml.cpp )
 ELSE(NOT RL_NO_STATE)
   SET(RL_SRCS RlManager.cpp RlPainter.cpp )
 ENDIF(NOT RL_NO_STATE)
 
 ADD_LIBRARY(rl STATIC ${RL_SRCS}) 
 