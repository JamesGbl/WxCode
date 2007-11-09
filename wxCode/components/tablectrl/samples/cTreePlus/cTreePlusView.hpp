////////////////////////////////////////////////////////////////////////////////
// Soli Deo Gloria!                                                           //
//                                                                            //
// Name:        cTreePlusView.hpp                                             //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     2007                                                          //
// Copyright:   (c) 2007 Jan Knepper                                          //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef CTREEPLUSVIEW_HPP
#define CTREEPLUSVIEW_HPP



#include "MainFrame.h"



class  cTreePlusView : public  wxApp
{
   private   :
      typedef wxApp  super;

   public    :                            // wxAppConsole
      bool              OnInit         ();
      int               OnExit         ();

   private   :
      MainFrame *       mainframe;

   public    :
      cTreePlusView  ();
      ~cTreePlusView ();

      MainFrame *       GetFrame       ()                { return ( mainframe ); }
};



DECLARE_APP ( cTreePlusView )



#endif


