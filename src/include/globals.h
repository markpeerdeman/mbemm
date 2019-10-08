/* globals.h
 *
 * global variabeles for mickey
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: Red
 */

#ifndef GLOBALS_H

#include <stdlib.h>
#include <stdio.h>
#include <math.h>
/* #define double float */ /* compile for floats... */ 

/* #define SGI */
#define ANSI

#define YES 1
#define NO  0

#define PI (acos(-1.0))
#define NU poissons_ratio
#define EMOD modulus_of_elasticity
#define MU (EMOD/2.0/(1.0+NU)) 

extern double modulus_of_elasticity;
extern double poissons_ratio;

#include <matrices.h>
#include <points.h>
#include <elements.h>
#include <restr.h>

extern int CURRENT_STATUS;
extern int WRITE_UNIVERSAL; /* write Ideas Universal file */
extern int WRITE_SPECIAL; /* write Mark's custom format */
extern int WRITE_OBJ; /* write .obj alias wavefront format */
extern int LARGE_DISPLACEMENTS;
extern int READ_SOLUTION;

typedef struct meshtyp {
  point     mesh_p;
  element   mesh_e;
  restraint mesh_d;
  restraint mesh_t;
  int       nr_of_nodes;
  int       nr_of_elements;
  int       nr_of_displacements;
  int       nr_of_tractions;
  double    G_SCALE;
  double    D_SCALE;
  double    T_SCALE;
  double    E_SCALE;
} *mesh, meshtype;

typedef struct cmeshtyp {
  mesh      merged_mesh;
  int       nr_of_nodes_basemesh;
  int       nr_of_elements_basemesh;
  int       nr_of_tractions_basemesh;
  int       nr_of_displacements_basemesh;
} *cmesh, cmeshtype;

extern matrix main_E;
extern vector main_f;
extern vector main_u;

extern int allocated_matrices;
extern int freed_matrices;
extern int allocated_vectors;
extern int freed_vectors;

#endif
#define GLOBALS_H
