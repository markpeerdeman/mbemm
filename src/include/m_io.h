/* m_io.h
 *
 * mickey's communication channels with the real world...
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: Red
 */

#ifndef M_IO_H
#include <stdio.h>

/* types of severity levels: */
#define GREEN  0  /* just a status message */
#define YELLOW 1  /* warning, execution can be proceeded */
#define RED    2  /* error, execution is terminated */
#define FINISH 4  /* mickey ready, normal exit */
#define BEGIN  5  /* mickey is running */

extern void evaluate_level(const int status,const int messagenumber );

#define M_IO_H
#endif
