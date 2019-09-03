#ifndef SOLVE_H 
#include <matrices.h>
#include <imatrices.h>

void solve_4(matrix Ma, int m, int n, const vector F, intvector nietnul);
void solve_2(const matrix A,const vector F );
void solve_3(const matrix A,const vector F );
void solve_gauss(const matrix A,const vector F );
void frontsolver(matrix A, vector f, vector res);
#endif 
#define SOLVE_H

