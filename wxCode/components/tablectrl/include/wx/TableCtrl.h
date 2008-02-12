////////////////////////////////////////////////////////////////////////////////
// SDG!                                                                       //
//                                                                            //
// Name:        TableCtrl.h                                                   //
// Purpose:                                                                   //
// Author:      Jan Knepper                                                   //
// Created:     1998                                                          //
// Copyright:   (c) 1998-2008 Jan Knepper                                     //
// Licence:     wxWidgets licence                                             //
////////////////////////////////////////////////////////////////////////////////



#ifndef TABLECTRL_H
#define TABLECTRL_H



#define CURSOR_RESIZELR       1000



//#define IDM_NOSORT            1000
#define IDM_PROPERTY          1001
#define IDM_RESET             1002
#define IDM_VISUAL_SQL        1003

#define IDM_NOSORT            1999
//#define IDM_SORT              2000
#define IDM_SUBTABLE          3000



//#define IDM_EXCHANGE_FAST     1011
//#define IDM_EXCHANGE_MERGE    1012
//#define IDM_EXCHANGE_ROLL     1013
//#define IDM_EXCHANGE_SQUASH   1014



#define RNS_RESIZELR                      "ResizeLR"
#define RNS_RESIZETB                      "ResizeTB"

//#define RNS_TABLE_CHECKBOX_UNCHECKED      "Table_CheckBox_Unchecked"
//#define RNS_TABLE_CHECKBOX_CHECKED        "Table_CheckBox_Checked"

#define RNS_TABLETOOLBAR                  "TableToolBar"



#define IDM_TABLECTRL                     10000

#define IDM_TABLECTRL_BEGIN               10000
#define IDM_TABLECTRL_END                 19999

#define IDM_TABLECTRL_EDIT                10000
#define IDM_TABLECTRL_INSERT              10001
#define IDM_TABLECTRL_UPDATE              10002
#define IDM_TABLECTRL_DELETE              10003
#define IDM_TABLECTRL_COPY                10004
#define IDM_TABLECTRL_SHOW                10005

#define IDM_TABLECTRL_SUBTABLE            11000

#define IDM_TABLECTRL_SORT_BEGIN          12000
#define IDM_TABLECTRL_SORT_END            12999

#define IDM_TABLECTRL_SORT                12000
#define IDM_TABLECTRL_SORT_0              12001
#define IDM_TABLECTRL_SORT_ASCENDING      12100
#define IDM_TABLECTRL_SORT_ASCENDING_0    12101
#define IDM_TABLECTRL_SORT_DESCENDING     12200
#define IDM_TABLECTRL_SORT_DESCENDING_0   12201

#define IDM_TABLECTRL_FIND                13000

#define IDM_TABLECTRL_SEARCH              14000

#define IDM_TABLECTRL_SELECT              15000



const DWORD             ITCS_MULTISELECT           = 0x00000001L;
const DWORD             ITCS_DRAGANDDROP           = 0x00000002L;
const DWORD             ITCS_GRIDVERTICAL          = 0x00000004L;
const DWORD             ITCS_GRIDHORIZONTAL        = 0x00000008L;
const DWORD             ITCS_GRIDLINES             = ITCS_GRIDVERTICAL | ITCS_GRIDHORIZONTAL;
const DWORD             ITCS_CHECKBOXES            = 0x00000010L;
const DWORD             ITCS_FOCUSRECT             = 0x00000020L;
const DWORD             ITCS_SELECTRANDOM          = 0x00000040L;
const DWORD             ITCS_SPREADSHEET           = 0x00000080L;
const DWORD             ITCS_VSCROLLER             = 0x00000100L;



const DWORD             ITCF_REFRESH_DEFAULT       = 0x00000000L;
const DWORD             ITCF_REFRESH_FOCUS_FIRST   = 0x00000001L;
const DWORD             ITCF_REFRESH_RESET         = 0x00000002L;
const DWORD             ITCF_REFRESH_RELATION      = 0x00000004L;



class  wxTableCtrl;
class  wxTableEvent;



#endif
