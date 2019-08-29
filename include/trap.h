/* ------------------------------------------ */
/* trap.h - Definitions for message handler.  */
/*   Handle all errors and warnings with one  */
/*   trap mechanism; let user know which      */
/*   module went wrong.                       */
/* ------------------------------------------ */
/* History                                    */
/*   04-02-96 : Start of implementation       */
/* ------------------------------------------ */
/* Written by Alex Bron - (c) 1996 BroncoSoft */
/* ------------------------------------------ */

#ifndef TRAPPER
#define TRAPPER

/* Definition of trap levels */
#define ERROR		000
#define SYSERR		001
#define WARNING		010
#define INFORMATION	020

/* Definition of module names */
#define TRAP		000
#define DYNARRAY	001

/* Trapping functions */
extern void trap(int module, int level, int message, char *function);
extern void traceback(int module, char *function);

#endif
