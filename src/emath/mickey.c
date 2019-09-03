/***************************************************************************** 
 * mickey.c
 *
 * mickey's mainline
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: Red
 *****************************************************************************/

#include <globals.h>
#include <parse.h>
#include <m_io.h>
#include <linear.h>
#include <bcapp.h>
#include <solve.h>
#include <matrices.h>
#include <post.h>
#include <memstat.h>
#include <do_linear.h>
#include <do_contact.h>
#include <mcheck.h>

/*** GLOBAL VARIABELES: ******************************************************/

double modulus_of_elasticity=0.0;
double poissons_ratio=0.0;

matrix main_E; vector main_f; vector main_u;

int  WRITE_UNIVERSAL;
int  WRITE_SPECIAL;
int  READ_SOLUTION;

int allocated_matrices=0;
int freed_matrices=0;
int allocated_vectors=0;
int freed_vectors=0;

/*****************************************************************************/
/* This is the entrypoint of the software from the OS: */
int main(const int argc, char *argv[] )
{ WRITE_SPECIAL = NO;
  WRITE_UNIVERSAL = NO;
  READ_SOLUTION = NO;
#include <text.h>

  if (argc>1000) {}; /* this is pure nonsense to pull the compiler's leg */

  /* do_meshcheck(argv[1]);  */
  
  if (argc==2) do_linear(argv[1]);
  if (argc==3) do_contact(argv[1],argv[2]);
   
  evaluate_level(FINISH,41);
  return (int)1;
} /* main */
