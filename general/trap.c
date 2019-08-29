/* ------------------------------------------ */
/* trap.c - Message handler for Elastomath    */
/*   Handle messages including module name,   */
/*   function name and severity level.        */
/* ------------------------------------------ */
/* History                                    */
/*   04-03-96 : Start of implementation       */
/* ------------------------------------------ */
/* Written by Alex Bron - (c) 1996 BroncoSoft */
/* ------------------------------------------ */

/* System includes */
#include <stdio.h>

/* User includes */
#include <trap.h>
#include <messages.h>
#include <emath_io.h>

static char modules[3][40] = {
	"general//trap.c",
	"general//dynarray.c",
	"general//mesh.c"
};

char buffer[80];

void trap(int module, int level, int message, char *function)
{
	switch(level) {
		case ERROR:
			sprintf(buffer,"General error in module %s",modules[module]);
			emprintf(buffer);
			break;
		case SYSERR:
			sprintf(buffer,"System error in module %s",modules[module]);
			emprintf(buffer);
			break;
		case WARNING:
			sprintf(buffer,"General warning in module %s",modules[module]);
			emprintf(buffer);
			break;
		case INFORMATION:
			sprintf(buffer,"General information for module %s",modules[module]);
			emprintf(buffer);
			break;
	}
	sprintf(buffer,", function %s\n", function);
	emprintf(buffer);
	emprintf(messages(message));
}

void traceback(int module, char *function)
{
	sprintf(buffer,"Returning from %s; module %s\n",function,modules[module]);
	emprintf(buffer);
}
