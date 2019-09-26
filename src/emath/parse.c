/*****************************************************************************
 * parse.c
 *
 * input deck file parser
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: Red
 *****************************************************************************/

#include <ctype.h>
#include <string.h>
#include <globals.h>
#include <scale.h>
#include <cylgeom.h>

#include <m_io.h>
#include <tril.h>
#include <parse.h>

#define theJohn '{'
#define theAnita '}'

extern double modulus_of_elasticity;
extern double poissons_ratio;

/***************************************************************************/
int fgetc_sc(FILE *fp)
/* this function gets a character from stream fp, skipping everything between
 * the characters named 'theJohn' and 'theAnita', defined above.
 */
{ char c;
  int johns=0;
  do
  { c=(char)fgetc(fp);
	if (feof(fp)) return (int)' ';
	if (c==theJohn)  johns++;
	if (c==theAnita) johns--;
  } while( johns!=0 );
  if (c==theAnita) c=' ';
  return (int)c;
} /* fgetc_sc */

/***************************************************************************/
void read_keyword( FILE *fp, char *data)
/* This function reads a keyword string from the stream fp. The keyword is
 * returned (capitalized), terminated with a \0 string terminator. When EOF is
 * encountered, the keyword 'END' is returned.
 */
{ int ch;
  int i=0;
  /* search begin of next keyword: */
  do
  { ch=fgetc_sc(fp);
	if (feof(fp))
	{ /* evaluate_level(GREEN,52); */
	  data[0]='E'; data[1]='N'; data[2]='D'; data[3]=(char)0;
	  return;
	}
  }
  while (isspace(ch));
  /* read keyword in the datastring: */
  while ( !isspace(ch) )
  { data[i++]=(char)toupper(ch);
    if (i>79) evaluate_level(YELLOW,58);
    ch=toupper(fgetc_sc(fp));
  }
  data[i]=(char)0;
} /* read_keyword */

/***************************************************************************/
void read_points(FILE *fp, mesh theMesh)
/* this function reads the point data into the the mesh's point dll. Data is
 * read until the keyword 'END_NODES' is encountered. Syntax errors are NOT
 * processed!
 */
{ static char theKeyword[80];
  /* evaluate_level(GREEN,54); */
  while (1)
  { read_keyword(fp,theKeyword);
    if (!strcmp(theKeyword,"END_NODES")) return;
    theMesh->mesh_p=point_dll(theMesh->mesh_p,APPEND,0);
    theMesh->mesh_p->label =  atoi(theKeyword); 
    read_keyword(fp,theKeyword); theMesh->mesh_p->x = strtod(theKeyword,NULL);
    read_keyword(fp,theKeyword); theMesh->mesh_p->y = strtod(theKeyword,NULL);
    read_keyword(fp,theKeyword); theMesh->mesh_p->z = strtod(theKeyword,NULL);
    theMesh->mesh_p->type=NODE;
  }
} /* read_points */

/***************************************************************************/
void skip_solutionvector(FILE *fp)
{ static char theKeyword[80];
  READ_SOLUTION=YES;
  while (1)
  { read_keyword(fp,theKeyword);
    if (!strcmp(theKeyword,"END_SOLUTION")) return;
  }
} /* skip_solutionvector() */

/***************************************************************************/
void fill_solutionvector(FILE *fp,vector thesolutionvector)
{ static char theKeyword[80];
  int i=1;
  while (1)
  { read_keyword(fp,theKeyword);
    if (!strcmp(theKeyword,"END_SOLUTION")) return;
	else 
	{ V(thesolutionvector,i++)=strtod(theKeyword,NULL);
	  read_keyword(fp,theKeyword); V(thesolutionvector,i++)=strtod(theKeyword,NULL);
	  read_keyword(fp,theKeyword); V(thesolutionvector,i++)=strtod(theKeyword,NULL);
	}
  }
} /* fill_solutionvector() */

/***************************************************************************/
void skip_C_nodes(FILE *fp)
{ static char theKeyword[80];
  while (1)
  { read_keyword(fp,theKeyword);
    if (!strcmp(theKeyword,"END_CNODES")) return;
  }
} /* skip_C_nodes() */


/***************************************************************************/
void fill_C_points(FILE *fp, cmesh theCMesh)
{ point p; 
  point q;
  int node1label;
  int node2label;
  double length;
  static char theKeyword[80];
  while (1)
  { read_keyword(fp,theKeyword);
    if (!strcmp(theKeyword,"END_CNODES")) return;
    node1label=atoi(theKeyword);
    read_keyword(fp,theKeyword); 
    node2label=atoi(theKeyword)+theCMesh->nr_of_nodes_basemesh;
    p=(point)point_dll(theCMesh->merged_mesh->mesh_p,SEARCH,node1label);
    q=(point)point_dll(theCMesh->merged_mesh->mesh_p,SEARCH,node2label);
    p->type=CNODE; q->type=CNODE;
    p->conn=q->label; q->conn=p->label;
    read_keyword(fp,theKeyword); p->n1=strtod(theKeyword,NULL);
    read_keyword(fp,theKeyword); p->n2=strtod(theKeyword,NULL);
    read_keyword(fp,theKeyword); p->n3=strtod(theKeyword,NULL);
    length=sqrt(p->n1*p->n1+p->n2*p->n2+p->n3*p->n3);
    p->n1/=length;
    p->n2/=length;
    p->n3/=length;
    q->n1=p->n1;
    q->n2=p->n2;
    q->n3=p->n3;
     
  }
} /* fill_C_points */

/***************************************************************************/
void read_elements(FILE *fp, mesh theMesh)
/* This function calls the function fillTRIL() when the keyword 'elements'
 * is encountered. When other elements are implemented, the switch has to be
 * made in this little function.
 */
{ static char theKeyword[80];
  read_keyword(fp,theKeyword);

  /* make a 'switch' here when other element types are implemented: */
  if (!strcmp(theKeyword,"TRIL")) 
  { /* evaluate_level(GREEN,55); */
    fillTRIL(theMesh->mesh_e,fp); 
    return; 
  }
  else evaluate_level(YELLOW,58);
} /* read_elements */

/***************************************************************************/
void read_tractions(FILE *fp, mesh theMesh)
/* This function just calls fill_restraints() for tractions */
{ /* evaluate_level(GREEN,56); */
  theMesh->mesh_t->type=TRACTION;
  fill_restraints(theMesh->mesh_t,fp);
} /* read_tractions */

/***************************************************************************/
void read_displacements(FILE *fp, mesh theMesh)
/* This function just calls fill_restraints() for displacements */
{ /* evaluate_level(GREEN,57); */
  theMesh->mesh_d->type=DISPLACEMENT;
  fill_restraints(theMesh->mesh_d,fp);
} /* read_tractions */

/***************************************************************************/
void read_material(FILE *fp)
/* This function reads the material card */
{ static char theKeyword[80];
  /* evaluate_level(GREEN,62); */
  read_keyword(fp,theKeyword); modulus_of_elasticity = strtod(theKeyword,NULL);
  read_keyword(fp,theKeyword); poissons_ratio = strtod(theKeyword,NULL);

} /* read_material */

/***************************************************************************/
void write_universal_req(void)
/* This function sets the global switch for writing universal files */
{ WRITE_UNIVERSAL=YES;
  WRITE_SPECIAL=NO;
  /*evaluate_level(GREEN,66);*/
} /* write_universal_req */

/***************************************************************************/
void write_special_req(void)
/* This function sets the global switch for writing the special format */
{ WRITE_SPECIAL=YES;
  WRITE_UNIVERSAL=NO;
  /* evaluate_level(GREEN,67); */
} /* write_special_req */

/***************************************************************************/
void large_displacements_req(void)
/* This function sets the global switch for large displacments analysis */
{ LARGE_DISPLACEMENTS=YES;
  evaluate_level(GREEN,69);
} /* large_displacements_req */


/***************************************************************************/
/***************************************************************************/
void parse(char *name, mesh theMesh)
/* This is the main parse function. This function is called somewhere from
 * the mainline. It reads the inputdeck, identified with the filename 'name'.
 * All database dlls are filled (elements,points(nodes), tractions and disps
 */
{ FILE *fp;
  static char theKeyword[80];
  element elm;
  if ( (fp=fopen(name,"r"))==(FILE *)NULL ) evaluate_level(RED,50);
  evaluate_level(GREEN,51);

  /* do something when a keyword is encountered: */
  while (!feof(fp))
  { read_keyword(fp,theKeyword);
    if (!strcmp(theKeyword,"NODES"))         read_points(fp,theMesh); else
    if (!strcmp(theKeyword,"ELEMENTS"))      read_elements(fp,theMesh); else
    if (!strcmp(theKeyword,"TRACTIONS"))     read_tractions(fp,theMesh); else
    if (!strcmp(theKeyword,"DISPLACEMENTS")) read_displacements(fp,theMesh); else
    if (!strcmp(theKeyword,"MATERIAL"))      read_material(fp); else
    if (!strcmp(theKeyword,"WRITE_UNIVERSAL")) write_universal_req(); else
    if (!strcmp(theKeyword,"WRITE_SPECIAL")) write_special_req(); else
    if (!strcmp(theKeyword,"LARGE_DISPLACEMENTS")) large_displacements_req(); else
    if (!strcmp(theKeyword,"CNODES"))        skip_C_nodes(fp); else
    if (!strcmp(theKeyword,"SOLUTION"))	     skip_solutionvector(fp); else
    if (!strcmp(theKeyword,"END"))           break; else
    evaluate_level(YELLOW,53);
  }

  if ((WRITE_SPECIAL==NO)&&(WRITE_UNIVERSAL==NO))
  { evaluate_level(YELLOW,68);
  }

  if ( (modulus_of_elasticity==0.0) || (poissons_ratio==0.0) )
  { evaluate_level(YELLOW,63);
    modulus_of_elasticity=210000.0;
    poissons_ratio=0.29;
  }


/* evaluate_level(GREEN,60); */
fclose(fp);
} /* parse */

void parse_contact(char *name, cmesh theCMesh)
{ FILE *fp;
  static char theKeyword[80];
  point p;
  fprintf(stderr,"parsing contact...\n");
  if ( (fp=fopen(name,"r"))==(FILE *)NULL ) evaluate_level(RED,50);
  evaluate_level(GREEN,51);
  /* do something when a keyword is encountered: */
  while (!feof(fp))
  { read_keyword(fp,theKeyword);
    if (!strcmp(theKeyword,"CNODES")) fill_C_points(fp,theCMesh); 
  }
  /* calculate the cylinder normals: */
  p = (point)point_dll(theCMesh->merged_mesh->mesh_p,START,0);
  while (p!=(point)NULL)
  { if (p->type==CNODE) 
    { /* calculate_cyl_normal(p); */
      fprintf(stderr,"node %d, x: %f y %f z %f, has normals %f %f %f\n",p->label,
		p->x, p->y, p->z, p->n1, p->n2, p->n3);  
    }
    p=p->NEXT;
  }
  fprintf(stderr,"Contact parsed\n");
} /* parse_contact */


void parse_solution(char *name, vector thesolutionvector)
{ FILE *fp;
  static char theKeyword[80];
  if ( (fp=fopen(name,"r"))==(FILE *)NULL ) evaluate_level(RED,50);
  evaluate_level(GREEN,51);
  /* do something when a keyword is encountered: */
  while (!feof(fp))
  { read_keyword(fp,theKeyword);
    if (!strcmp(theKeyword,"SOLUTION")) fill_solutionvector(fp,thesolutionvector);
  }
} /* parse_solution */
