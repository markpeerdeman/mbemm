
#include <globals.h>
#include <parse.h>
#include <m_io.h>
#include <linear.h>
#include <solve.h>
#include <matrices.h>
#include <post.h>
#include <memstat.h>
#include <scale.h>

/*** GLOBAL VARIABELES: ******************************************************/

int do_linear(char *argv)
{ mesh mesh1;
  WRITE_SPECIAL = NO;
  WRITE_UNIVERSAL = NO;
  READ_SOLUTION = NO;

  /* initialize global datastructures: */
  mesh1=(mesh)malloc(sizeof(meshtype));
  mesh1->mesh_p = point_dll((point)NULL,NEW,0);
  mesh1->mesh_e = element_dll((element)NULL,NEW,0);
  mesh1->mesh_t = restraint_dll((restraint)NULL,NEW,0); mesh1->mesh_t->type=TRACTION;
  mesh1->mesh_d = restraint_dll((restraint)NULL,NEW,0); mesh1->mesh_d->type=DISPLACEMENT;

  /* parse the inputdeck: */
  parse(argv,mesh1); 
  scalemesh(mesh1);

  /* do the linear analysis: */
  linear3(mesh1); 

if (READ_SOLUTION==NO)
  { solve_gauss(main_E,main_f);
    printvector(main_f,"solution vector");
  }
  if (READ_SOLUTION==YES)
  { fprintf(stderr,"Parsing solution vector...\n"); 
    parse_solution(argv,main_f);
    fprintf(stderr,"Solution vector read.\n");
  }


  /* do the postprocessing: */
  post(mesh1,0); 

  /* kill the globals: */
  point_dll(mesh1->mesh_p,KILL,0);
  element_dll(mesh1->mesh_e,KILL,0);
  restraint_dll(mesh1->mesh_t,KILL,0);
  restraint_dll(mesh1->mesh_d,KILL,0);
  free(mesh1);

  delmatrix(main_E);
  delvector(main_f);  
  delvector(main_u);
   
  evaluate_level(FINISH,41);
  return (int)1;
} /* main */
