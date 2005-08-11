##---------------------------------------------------------------------------
## $RCSfile: CMakeLists.txt,v $
## $Source: /home/ulrich/cvs-wxcode/wxCode/components/treemultictrl/contrib/src/treemultictrl/CMakeLists.txt,v $
## $Revision: 1.2 $
## $Date: 2005-08-11 18:02:13 $
##---------------------------------------------------------------------------
## Author:      Jorgen Bodde
## Copyright:   (c) Jorgen Bodde
## License:     wxWidgets License
##---------------------------------------------------------------------------

# Add files to library treemultictrl

ADD_LIBRARY(treemultictrl TreeMultiItemBase.cpp TreeMultiItemNode.cpp TreeMultiItemWindow.cpp
                          TreeMultiItemRoot.cpp wxTreeMultiCtrl.cpp
                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/wxTreeMultiCtrl.h
                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/TreeMultiItemBase.h 
                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/TreeMultiItemNode.h
                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/TreeMultiItemRoot.h 
                          ${TREEMULTICTRL_SOURCE_DIR}/contrib/include/wx/treemultictrl/TreeMultiItemWindow.h )

IF(LINUX)
  ADD_DEFINITIONS( -DLINUX=1 )
ENDIF(LINUX)

# if the checkbox view functionality is not desired, we 
# won't include it in the compile process. This way the 
# control can remain small if new features are applied.

IF(USE_CHECKBOXVIEW)
  ADD_DEFINITIONS( -DCHECKBOXVIEW=1 )
ELSE(USE_CHECKBOXVIEW)
  ADD_DEFINITIONS( -DCHECKBOXVIEW=0)
ENDIF(USE_CHECKBOXVIEW)