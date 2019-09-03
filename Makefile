##### #      ###   #### #####  ###  #   #  ###  ##### #   #
#     #     #   # #       #   #   # ## ## #   #   #   #   #
###   #     #####  ###    #   #   # # # # #####   #   #####
#     #     #   #     #   #   #   # #   # #   #   #   #   #
##### ##### #   # ####    #    ###  #   # #   #   #   #   #

# Makefile for the Elastomath project
# Modified to fit a more general approach
# Basic concept: Elastomath is build in a set of utility trees:
#   General - general stuff for all programs related to elastomath
#   Emath   - the real BEM solver
#   MUI     - Marks User Interface or something like that
#
# Elastomath (c) 1995 by Mark S. Peerdeman and Douwe Overdijk

GENERAL = src/general
EMATH   = src/emath
INCLUDE = src/include
OBJECT  = obj
LIBRARY = lib

export PROJECT GENERAL EMATH INCLUDE OBJECT LIBRARY

all: elastomath

general_stuff:
	@(cd $(GENERAL); make all)

elastomath: general_stuff
	@(cd $(EMATH); make all)

clean:
	@(cd $(GENERAL); make clean)
	@(cd $(EMATH); make clean)
