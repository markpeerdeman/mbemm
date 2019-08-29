/* ------------------------------------------ */
/* emath_io.c - General communication tools   */
/*   for Elastomath. Make sure that communi-  */
/*   cation is established in the right way.  */
/*   Source for SGI implementation only.      */
/* ------------------------------------------ */
/* History                                    */
/*   04-03-96 : Start of implementation       */
/* ------------------------------------------ */
/* Written by Alex Bron - (c) 1996 BroncoSoft */
/* ------------------------------------------ */

#include <stdio.h>
#include <emath_io.h>

void emprintf(char *message)
{
	fprintf(stderr,"%s",message);
}
