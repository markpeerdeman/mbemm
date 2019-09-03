/* elements.h
 *
 * element header file
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: Yellow
 *
 */

#ifndef ELEMENTS_H

/* element_dll() commands: */
#define START      0
#define END        1
#define SEARCH     2
#define APPEND     3
#define NEW        4
#define KILL       5
#define SEARCHNODE 6

/* element types: */
#define TRIL       2  /* triangulate linear element, color GRAY_BLUE */ 
#define TRIC      10  /* triangulate contact element, color ORANGE */
#define NONE      19  /* NO element */

#define VERTEX    20
#define EDGE      21
#define FACE      22

/* I-DEAS STANDARD PALETTE:
 * 0  black
 * 1  blue
 * 2  gray_blue
 * 3  light_blue
 * 4  cyan
 * 5  dark_olive
 * 6  dark_green
 * 7  green
 * 8  yellow
 * 9  golden_orange
 * 10 orange
 * 11 red
 * 12 magenta
 * 13 light_magenta
 * 14 pink
 * 15 white
 */

typedef struct elementtp {
  struct elementtp *PREV;
  struct elementtp *NEXT;
  int type;              /* elementtype */
  int label;             /* element label */
  int p1;
  int p2;
  int p3;                
  double p1x; double p1y; double p1z; /* node coordinates for speed */
  double p2x; double p2y; double p2z;
  double p3x; double p3y; double p3z;
  double alpha1;           /* element corner angles */
  double alpha2;
  double alpha3;
  double z1;               /* element side lengths */
  double z2;
  double z3;
  double nx;               /* element normal vector */
  double ny;
  double nz;
  double area;             /* element area */
} *element, elementtype;

extern element element_dll( element thedll, const int command, const int label );
extern element newelement(void);
extern void delelement(const element e);

#define ELEMENTS_H
#endif
