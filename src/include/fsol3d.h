/* fsol3d.h
 *
 * headerfile for the fundamental solution calculations
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: Red
 */

extern void makeUFandTF( const matrix u, const matrix t,
		  const double s1, const double s2, const double s3,
		  const double c1, const double c2, const double c3,
		  const double n1, const double n2, const double n3 );
/* matrices u and p are filled with the fundamental solutions,
 * s1, s2 and s3 are the sourcepoint coordinates,
 * c1, c2 and c3 are the collocation point coordinates
 * n1, n1 and n3 are the normal vector components
 */

extern double delta(const int i,const int j);
