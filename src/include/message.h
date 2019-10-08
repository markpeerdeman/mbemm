/***********************************************************************************************
 * message.c
 * message list for the bem software
 * returns a char* depending of the messagenr
 *
 * Copyright (c) 1992 Mark Peerdeman, Douwe Overdijk
 *
 * STATUS: GREEN
 ***********************************************************************************************/

char *message(const int mnr)
{ char *tm;
  switch(mnr) {
  /* messages from points.c: */
  case 10:  tm = "newpoint() unable to allocate memory for pointstructure"; break;
  case 11:  tm = "point_dll(SEARCH) requested point not found"; break;
  case 12:  tm = "point_dll(default) unknown command given"; break;
  /* messages from matrices.c: */
  case 21:  tm = "newmatrix() unable to allocate memory for matrixstructure"; break;
  case 22:  tm = "newmatrix() unable to allocate memory for matrixdata"; break;
  case 23:  tm = "newvector() unable to allocate memory for vectorstructure"; break;
  case 24:  tm = "newvector() unable to allocate memory for vectordata"; break;
  case 25:  tm = "newhypermatrix() unable to allocate memory"; break;
  case 26:  tm = "newhypermatrix() unable to allocate memory for struct"; break;
  /* messages from the solver */
  case 30:  tm = "check_dimensions() conflicting matrix/vectordimensions"; break;
  case 31:  tm = "solver using method 1"; break;
  case 32:  tm = "solver removing singularity"; break;
  case 33:  tm = "solver cannot remove singularity"; break;
  case 34:  tm = "solver successful"; break;
  case 35:  tm = "solver using method 2 (without partial pivoting)"; break;
  case 36:  tm = "solver using method 3 (Iterative solver)"; break;
  case 37:  tm = "solver found zero on diagonal"; break;
  case 38:  tm = "solver using method 4 (compressed contact)"; break;
  case 39:  tm = "solver using Gauss elimination"; break;
  /* Mickey's mainline messages: */
  case 40:  tm = "main() Program started"; break;
  case 41:  tm = "main() Program ready"; break;
  case 42:  tm = "main() Writing output as universal file"; break;
  case 43:  tm = "main() Writing output as Elastomath Special file"; break;
  case 44:  tm = "main() Writing output as .OBJ file"; break;
  /* messages from parsing functions: */
  case 50:  tm = "parse() inputdeck file not found"; break;
  case 51:  tm = "parse() parsing inputdeck"; break;
  case 52:  tm = "parse() inputdeck parsed"; break;
  case 53:  tm = "parse() unrecognized keyword"; break;
  case 54:  tm = "parsing points"; break;
  case 55:  tm = "parsing TRIL elements"; break;
  case 56:  tm = "parsing tractions"; break;
  case 57:  tm = "parsing displacements"; break;
  case 58:  tm = "keyword too long for buffer"; break;
  case 59:  tm = "element type not known"; break;
  case 60:  tm = "parse() leaving parser"; break;
  case 61:  tm = "parse() completing element data"; break;
  case 62:  tm = "parsing material data"; break;
  case 63:  tm = "no material data in deck, using defaults"; break;
  case 65:  tm = "parse() request for writing .OBJ processed"; break;
  case 66:  tm = "parse() request for writing UNIVERSAL processed"; break;
  case 67:  tm = "parse() request for writing SPECIAL processed"; break;
  case 68:  tm = "parse() There is no output requested"; break;
  case 69:  tm = "parse() request for LARGE_DISPLACEMENTS analysis"; break;
  /* messages from elements.c: */
  case 70:  tm = "newelement() unable to allocate memory for elementstructure"; break;
  case 71:  tm = "element_dll(SEARCH) requested element not found"; break;
  case 72:  tm = "element_dll(default) unknown command given"; break;
  /* messages from restr.c: */
  case 80:  tm = "newrestraint() unable to allocate memory for restraintstructure"; break;
  case 81:  tm = "restraint_dll(SEARCH) requested restraint not found"; break;
  case 82:  tm = "restraint_dll(default) unknown command given"; break;
  /* messages from TRIC.c: */
  case 96:  tm = "TRICshapes() wrongsized shapematrix used"; break;
  case 97:  tm = "TRICnormal() called for a non-TRIC element"; break;
  /* messages from TRIL.c: */
  case 98:  tm = "TRILshapes() wrongsized shapematrix used"; break;
  case 99:  tm = "TRILnormal() called for a non-TRIL element"; break;
  /* messages from linear4.c: */
  case 100: tm = "init_linear() entering the linear analysis module"; break;
  case 101: tm = "close_linear() leaving the linear analysis module"; break;
  case 102: tm = "linear3() starting linear analysis"; break;
  case 103: tm = "linear3() creating system of equations"; break;
  case 104: tm = "linear3() system of equations created"; break;
  case 105: tm = "linear3() linear analysis finished"; break;
  case 106: tm = "linear3() node labels are not continuous"; break;
  case 107: tm = "linear3() element labels are not continuous"; break;
  case 108: tm = "init_linear() no memory for integration matrices"; break;
  case 110: tm = "linear3() reading system matrices from file"; break;
  case 111: tm = "linear3() unable to load the system matrices from file"; break;
  case 112: tm = "linear3() input deck and system matrices don't belong"; break;
  /* messages from bcapp.c: */
  case 109: tm = "bcapp() applying boundary conditions"; break;
  /* messages from post.c: */
  case 120: tm = "post() entering post"; break;
  case 121: tm = "post() scaling displacement results with MU"; break;
  case 129: tm = "post() leaving post"; break;
  /* messages from cmesh.c: */
  case 130: tm = "checkmesh() Testing the quality of the mesh"; break;
  case 131: tm = "checkmesh() testing rigid body rotation"; break;
  /* messages from the contact analysis: */
  case 144: tm = "CONTACT> Creating contact nodes..."; break;
  case 145: tm = "CONTACT> Creating contact elements..."; break;


  default:  tm = "message() unknown status, error or warning"; break;
  } /* end switch */
  return tm;
} /* message */
