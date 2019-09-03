/* restr.h
 *
 * restraints header file
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: Yellow
 *
 */

#ifndef RESTR_H
#include <stdio.h>
#include <stdlib.h>

/* restraint_dll() commands: */
#define START      0
#define END        1
#define SEARCH     2
#define APPEND     3
#define NEW        4
#define KILL       5

/* restraint types: */
#define TRACTION     0      /* node traction force */
#define DISPLACEMENT 1      /* node displacement */
#define IMPLICIT_TRACTION 2 /* free node (traction is zero) */

typedef struct restrainttp {
  struct restrainttp *PREV;
  struct restrainttp *NEXT;
  int type;              /* TRACTION || DISPLACEMENT */
  int label;         	 /* node label to which the restraint is applied */
  double f1;             /* traction forces */
  double f2;
  double f3;
  double u1;             /* nodal displacements */
  double u2;
  double u3;
} *restraint, restrainttype;

extern restraint restraint_dll( restraint thedll,const int command,const int label );
extern void fill_restraints( restraint r, FILE *fp);

#define RESTR_H
#endif
