/* plDevs.h.  Generated automatically by configure.  */
/* -*-C-*- */
/* $Id: plDevs.h,v 1.1.1.1 2003-12-28 21:05:57 wyo Exp $
 * $Log: not supported by cvs2svn $
 * Revision 1.1.1.1  1999/09/01 14:01:40  fhuebner
 * Start of wxPlot
 *
 * Revision 1.2  1999/09/01 13:48:34  fhuebner
 * #if -> #ifdef
 *
 * Revision 1.1.1.1  1999/09/01 12:55:12  fhuebner
 * Start of wxPlot
 *
 * Revision 1.6  1996/10/08  18:38:44  furnish
 * New file for the pbm driver.
 *
 * Revision 1.5  1995/06/12  02:52:31  mjl
 * Changes to allow linuxvga driver to be configured in under Linux.
 * Contributed by Mark Olesen.
 *
 * Revision 1.4  1994/08/26  19:17:21  mjl
 * Define for conex device added; contributed by Mark Olesen.
 *
 * Revision 1.3  1994/08/05  22:23:36  mjl
 * Eliminated dangling comment to get rid of compiler warning on NEC.
 *
 * Revision 1.2  1994/07/25  05:57:32  mjl
 * Added line for new lj_hpgl driver.
 *
 * Revision 1.1  1994/07/19  22:27:37  mjl
 * Header file for holding defines that enable PLplot device drivers.
 * Included by all device drivers and plcore.h.
*/

/*
    plDevs.h.in

    Maurice LeBrun
    IFS, University of Texas at Austin
    18-Jul-1994

    Contains macro definitions that determine what device drivers are
    compiled into the PLplot library.  On a Unix system, typically the
    configure script builds plDevs.h from plDevs.h.in.  Elsewhere, it's
    best to hand-configure a plDevs.h file and keep it with the
    system-specific files.
*/

#ifndef __PLDEVS_H__
#define __PLDEVS_H__

/* All device driver defines (except wxwin) thrown out */
#define PLD_wxwin 1

#endif	/* __PLDEVS_H__ */
