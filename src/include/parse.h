/* parse.h 
 * 
 * headerfile for parse.c 
 * 
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 * 
 */

#ifndef PARSE_H 
#include <stdio.h> 
#include <stdlib.h>

extern void parse(char *name, mesh theMesh ); 
extern void parse_contact(char *name, cmesh theCMesh ); 
extern void parse_solution(char *name, vector thesolutionvector);
extern void read_keyword( FILE *fp, char *theKeyword );

#endif 
#define PARSE_H

