/* points.h
 *
 * point header file
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: Yellow
 *
 */

#ifndef POINTS_H

/* point_dll() commands: */
#define START      0
#define END        1
#define SEARCH     2
#define APPEND     3
#define NEW        4
#define KILL       5

#define NODE      11
#define CNODE     15

typedef struct pointtp {
  struct pointtp *PREV;
  struct pointtp *NEXT;
  int label;             /* point label */
  int type;              /* NODE || CNODE */
  int conn;              /* connected to nodelabel */
  double x;              /* point positions */
  double y;
  double z;
  double fx;
  double fy;
  double fz;
  double dx;
  double dy;
  double dz; /* post_structs */
  double n1;
  double n2;
  double n3; /* normal in the node */
  int  calculated;
} *point, pointtype;

extern point point_dll( point thedll,const int command,const int label );
extern point newpoint( void ); 
extern void  delpoint(const point p );

#define POINTS_H
#endif
