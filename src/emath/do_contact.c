
#include <globals.h>
#include <parse.h>
#include <m_io.h>
#include <contact.h>
#include <solve.h>
#include <matrices.h>
#include <imatrices.h>
#include <post.h>
#include <memstat.h>

/*****************************************************************************/
void find_contactnodes(cmesh theCmesh)
{ double mindist=1.0E6;
  double curdist;
  element e;
  point p;
  point q;

  fprintf(stderr,"* Running automatic contact detection\n");
  fprintf(stderr,"  Candidate contact node list:\n");
  fprintf(stderr,"  cnodes\n");

  /* 1: determine smallest element size */
  e=(element)element_dll(theCmesh->merged_mesh->mesh_e,START,0);
  while (e!=(element)NULL)
  { if (e->z1<mindist) mindist=e->z1;
    if (e->z2<mindist) mindist=e->z2;
    if (e->z3<mindist) mindist=e->z3;
    e=e->NEXT;
  }

  /* 2: divide this size by 20.0: */
  mindist/=2.0;

  p=(point)point_dll(theCmesh->merged_mesh->mesh_p,START,0);
  while (p!=(point)NULL)
  { q=(point)point_dll(theCmesh->merged_mesh->mesh_p,START,0);
    while (q!=(point)NULL)
    { curdist=sqrt( (p->x-q->x)*(p->x-q->x) +
                    (p->y-q->y)*(p->y-q->y) +
                    (p->z-q->z)*(p->z-q->z) );
      if ( (curdist<mindist) && (p->label!=q->label) && (p->label<=theCmesh->nr_of_nodes_basemesh) )
      { fprintf(stderr,"  % 5d  % 5d\n",p->label,q->label-
                 theCmesh->nr_of_nodes_basemesh);
      }
      q=q->NEXT;
    }
    p=p->NEXT;
  }


  fprintf(stderr,"  end_cnodes\n\n");
} /* find_contact nodes */


/*****************************************************************************/
void makeCmesh(mesh mesh1, mesh mesh2, cmesh meshC)
{ int i;
  point p, q;
  element e, f;
  restraint dt, dt2;

  /* STEP 1: Put mesh1 into the cmesh-structure: */
             meshC->merged_mesh=mesh1;

  /* STEP 2: Count the numbers of the items in mesh1 and put into cmesh: */
  /*      A: Count nodes: */
             p=(point)point_dll(mesh1->mesh_p,START,0); i=0;
             while (p!=(point)NULL) { i++; p=p->NEXT; }
             mesh1->nr_of_nodes=i; meshC->nr_of_nodes_basemesh=i;
  /*      B: Count elements: */
             e=(element)element_dll(mesh1->mesh_e,START,0); i=0;
             while (e!=(element)NULL) { i++; e=e->NEXT; }
             mesh1->nr_of_elements=i; meshC->nr_of_elements_basemesh=i;
  /*      C: Count tractions: */
             dt=(restraint)restraint_dll(mesh1->mesh_t,START,0); i=0;
             while (dt!=(restraint)NULL) { i++; dt=dt->NEXT; }
             mesh1->nr_of_tractions=i; meshC->nr_of_tractions_basemesh=i;
  /*      D: Count displacements: */
             dt=(restraint)restraint_dll(mesh1->mesh_d,START,0); i=0;
             while (dt!=(restraint)NULL) { i++; dt=dt->NEXT; }
             mesh1->nr_of_displacements=i; meshC->nr_of_displacements_basemesh=i;

  /* STEP 3: Renumber mesh 2: */
  /*      A: Renumber nodes: */
             p=(point)point_dll(mesh2->mesh_p,START,0); 
             while (p!=(point)NULL) 
             { p->label+=meshC->nr_of_nodes_basemesh;
               p=p->NEXT; 
             }
  /*      B: Renumber elements: */
             e=(element)element_dll(mesh2->mesh_e,START,0);
             while (e!=(element)NULL) 
             { e->label+=meshC->nr_of_elements_basemesh;
               e->p1+=meshC->nr_of_nodes_basemesh;
               e->p2+=meshC->nr_of_nodes_basemesh;
               e->p3+=meshC->nr_of_nodes_basemesh;
               e=e->NEXT; 
             }
  /*      C: Renumber tractions: */
             dt=(restraint)restraint_dll(mesh2->mesh_t,START,0);
             while (dt!=(restraint)NULL) 
             { dt->label+=meshC->nr_of_nodes_basemesh;
               dt=dt->NEXT; 
             }
  /*      C: Renumber displacements: */
             dt=(restraint)restraint_dll(mesh2->mesh_d,START,0);
             while (dt!=(restraint)NULL) 
             { dt->label+=meshC->nr_of_nodes_basemesh;
               dt=dt->NEXT; 
             }

  /* STEP 4: Append mesh 2 to mesh 1 in the contact mesh: */
  /*      A: Append points: */
             p=(point)point_dll(mesh1->mesh_p,END,0);
             q=(point)point_dll(mesh2->mesh_p,START,0);
             p->NEXT=q; q->PREV=p;
  /*      B: Append elements: */
             e=(element)element_dll(mesh1->mesh_e,END,0);
             f=(element)element_dll(mesh2->mesh_e,START,0);
             e->NEXT=f; f->PREV=e;
  /*      C: Append tractions: */
             dt =(restraint)restraint_dll(mesh1->mesh_t,END,0);
             dt2=(restraint)restraint_dll(mesh2->mesh_t,START,0);
             if ((dt!=(restraint)NULL)&&(dt2!=(restraint)NULL))
             { dt->NEXT=dt2; dt2->PREV=dt; }
  /*      D: Append displacements: */
             dt =(restraint)restraint_dll(mesh1->mesh_d,END,0);
             dt2=(restraint)restraint_dll(mesh2->mesh_d,START,0);
             if ((dt!=(restraint)NULL)&&(dt2!=(restraint)NULL))
             { dt->NEXT=dt2; dt2->PREV=dt; }

} /* makeCmesh */


/*****************************************************************************/
int do_contact(char *name1, char *name2)
{ mesh  mesh1;  /* mesh for body 1 */
  mesh  mesh2;  /* mesh for body 2 */
  cmesh meshC;  /* contact mesh for both bodies */
  element e;
  intvector nietnul;
  int i;
  int n, m;
  point po;

  WRITE_SPECIAL = NO;
  WRITE_UNIVERSAL = NO;
  READ_SOLUTION = NO;




  /* initialize global datastructures for mesh 1: */
  mesh1=(mesh)malloc(sizeof(meshtype));
  mesh1->mesh_p = point_dll((point)NULL,NEW,0);
  mesh1->mesh_e = element_dll((element)NULL,NEW,0);
  mesh1->mesh_t = restraint_dll((restraint)NULL,NEW,0); mesh1->mesh_t->type=TRACTION;
  mesh1->mesh_d = restraint_dll((restraint)NULL,NEW,0); mesh1->mesh_d->type=DISPLACEMENT;

  /* initialize global datastructures for mesh 2: */
  mesh2=(mesh)malloc(sizeof(meshtype));
  mesh2->mesh_p = point_dll((point)NULL,NEW,0);
  mesh2->mesh_e = element_dll((element)NULL,NEW,0);
  mesh2->mesh_t = restraint_dll((restraint)NULL,NEW,0); mesh2->mesh_t->type=TRACTION;
  mesh2->mesh_d = restraint_dll((restraint)NULL,NEW,0); mesh2->mesh_d->type=DISPLACEMENT;

  /* initialize global datastructures for the contactmesh: */
  meshC=(cmesh)malloc(sizeof(cmeshtype));

  /* parse the inputdeck: */
  parse(name1,mesh1); 
  parse(name2,mesh2); 


  /* join both meshes to a contact mesh: */
  makeCmesh(mesh1,mesh2,meshC);
  /* parse contact nodes: */
  parse_contact(name1,meshC);
  /* make the system dimensionless: */
  scalemesh(meshC->merged_mesh);

  printf("nr_of_nodes_basemesh %d\n",meshC->nr_of_nodes_basemesh);
  printf("nr_of_elements_basemesh %d\n",meshC->nr_of_elements_basemesh);

  find_contactnodes(meshC);

  contact3(meshC);

  /***/  
  po=meshC->merged_mesh->mesh_p;
  m=meshC->nr_of_nodes_basemesh*3;
  n=main_E->cols-m; 
  nietnul=(intvector)newivector(n+m);
  verwijdernullen(m, n, main_E, nietnul,po);


  if (READ_SOLUTION==NO)
  { solve_4(main_E, m, n, main_f, nietnul);
    printvector(main_f,"solution vector");
  }
  if (READ_SOLUTION==YES)
  { fprintf(stderr,"Parsing solution vector...\n"); 
	parse_solution(name1,main_f);
	fprintf(stderr,"Solution vector read.\n");
  }

  post(meshC->merged_mesh,meshC->nr_of_nodes_basemesh); 

  delivector(nietnul);
  delmatrix(main_E); delvector(main_f);
  point_dll(meshC->merged_mesh->mesh_p,KILL,0);
  element_dll(meshC->merged_mesh->mesh_e,KILL,0);
  restraint_dll(meshC->merged_mesh->mesh_t,KILL,0);
  restraint_dll(meshC->merged_mesh->mesh_d,KILL,0);
  free(mesh1); free(mesh2); free(meshC);
  evaluate_level(FINISH,41);
  return (int)1;
} /* do_contact */
