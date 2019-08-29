/* ------------------------------------------ */
/* messages.c - Messages for all trap calls   */
/* ------------------------------------------ */
/* History                                    */
/*   04-04-96 : Start of implementation       */
/* ------------------------------------------ */
/* Written by Alex Bron - (c) 1996 BroncoSoft */
/* ------------------------------------------ */

#include <messages.h>

char *messages(int message)
{
	char *buffer;
	
	switch(message) {
		case 200 :
			buffer = "Memory allocation error for Dynamic Arrays";
			break;
		case 201 :
			buffer = "Attempt to erase non-existing element";
			break;
		case 202 :
			buffer = "Attempt to clear non-existing index";
			break;
		case 203 :
			buffer = "Attempt to add zero-length element to Dynamic Array";
			break;
		case 204 :
			buffer = "Warning: array did not have any elements";
			break;
		default :
			buffer = "Unknown system error occurred...";
			break;
	}
	return buffer;
}
