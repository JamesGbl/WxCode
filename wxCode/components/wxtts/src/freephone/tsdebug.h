/////////////////////////////////////////////////////////////////////////////
// Name:        tsdebug.h
// Purpose:     Misc debug functions and macros
// Author:      Vadim Zeitlin
// Modified by:
// Created:     29/01/98
// RCS-ID:      $$
// Copyright:   (c) 1998 Vadim Zeitlin <zeitlin@dptmaths.ens-cachan.fr>
// Licence:   	wxWindows license
/////////////////////////////////////////////////////////////////////////////

#ifndef   _TSDEBUG_H_
#define   _TSDEBUG_H_

#include  <assert.h>

// ----------------------------------------------------------------------------
/** 
  @name Debugging macros 

  All debugging macros rely on ASSERT() which in turn calls user-defined
  OnAssert() function. To keep things simple, it's called even when the
  expression is TRUE (i.e. everything is ok) and by default does nothing: just
  returns the same value back. But if you redefine it to do something more sexy
  (popping up a message box in your favourite GUI, sending you e-mail or 
  whatever) it will affect all ASSERTs, FAILs and CHECKs in your code.
  <BR>
  <BR>
  <b>Warning</b>: if you don't like advices on programming style, don't read 
  further! ;-)
  <BR>
  <BR>
  Extensive use of these macros is recommended! Remember that ASSERTs are
  disabled in final (without DEBUG defined) build, so they add strictly
  nothing to your program's code. On the other hand, CHECK macros do stay
  even in release builds, but in general are not much of a burden, while
  a judicious use of them might increase your program's stability.

  @memo Debugging macros (replacement for standard assert()) and more.
  */
// ----------------------------------------------------------------------------
//@{

/** @name Macros which are completely disabled in 'release' mode */
//@{
#ifdef  __DEBUG__
  /**
  this function may be redefined to do something non trivial and is called
  whenever one of debugging macros fails (i.e. condition is false in an
  assertion)
  @param   szFile and nLine - file name and line number of the ASSERT
           szMsg            - optional message explaining the reason
  */
  void tsOnAssert(const char *szFile, int nLine, const char *szMsg = NULL);

  /// generic assert macro
  #define   TSASSERT(cond)   if ( !(cond) ) tsOnAssert(__FILE__, __LINE__)
  /// assert with additional message explaining it's cause
  #define   TSASSERT_MSG(x, m)  if ( !(x) ) tsOnAssert(__FILE__, __LINE__, m)
#else
  // nothing to do in release modes (hopefully at this moment there are
  // no more bugs ;-)
  #define   TSASSERT(cond)
  #define   TSASSERT_MSG(x, m)
#endif  //DEBUG

  /// special form of assert: always triggers it (in debug mode)
#define   TSFAIL                 TSASSERT(0)
  /// FAIL with some message
#define   TSFAIL_MSG(msg)        TSASSERT_MSG(0, msg)
//@}

// NB: these macros work also in release mode!

/** 
  These macros must be used only in invalid situation: for example, an
  invalid parameter (NULL pointer) is passed to a function. Instead of
  dereferencing it and causing core dump the function might try using
  CHECK( p != NULL ) or CHECK( p != NULL, return LogError("p is NULL!!") )

  @name Macros which remain even in 'release' mode 
*/
//@{
  /// check that expression is true, "return" if not (also FAILs in debug mode)
#define   TSCHECK(x, rc)            if (!(x)) {TSFAIL; return rc; }
  /// as TSCHECK but with a message explaining why we fail
#define   TSCHECK_MSG(x, rc, msg)   if (!(x)) {TSFAIL_MSG(msg); return rc; }
  /// check that expression is true, perform op if not
#define   TSCHECK2(x, op)           if (!(x)) {TSFAIL; op; }
  /// as TSCHECK2 but with a message explaining why we fail
#define   TSCHECK2_MSG(x, op, msg)  if (!(x)) {TSFAIL_MSG(msg); op; }
  /// special form of TSCHECK2: as TSCHECK, but for use in void functions
  //  NB: there is only one form (with msg parameter) and it's intentional:
  //      there is no other way to tell the caller what exactly went wrong
  //      from the void function (of course, the function shouldn't be void
  //      to begin with...)
#define   TSCHECK_RET(x, msg)       if (!(x)) {TSFAIL_MSG(msg); return; }
//@}

//@}

#endif  // _TSDEBUG_H_

