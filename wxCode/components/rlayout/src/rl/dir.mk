LOG += !rl/dir.mk 
SRCS := RlManager.cpp RlPainter.cpp RlDialogMgr.cpp 

# Only include if we don't have RL_NO_STATE
#ifneq ($(findstring RL_NO_STATE, $(DEFS)),RL_NO_STATE)
    SRCS += RlDialog.cpp RlFromXml.cpp 
#endif

include $(GEN_BUILD_DIR)/subdir.mk 
     
