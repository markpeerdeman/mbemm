/* tril.h 
* 
* tril element header file 
* 
* Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
* 
* STATUS: Yellow 
* 
*/ 
#ifndef TRIL_H 
#include <stdio.h> 
#include <stdlib.h> 
#include <elements.h> 
#include <points.h> 
#include <matrices.h>

 
extern void fillTRIL(element e, FILE *fp);
 
#define G_MATRIX 10 
#define H_MATRIX 11
 
extern void TRILcopy(const element s, const element d);
extern void TRILcompletion(const  element t, const point thepointlist );

#define TRIL_H 
#endif
