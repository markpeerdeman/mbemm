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
#   UserIO  - User interface for in- and output
#
# (c) 1996 by A.J. Bron
# Elastomath (c) 1995 by Mark S. Peerdeman and Douwe Overdijk

PROJECT = .
GENERAL = $(PROJECT)/general
IFACE   = $(PROJECT)/interface
EMATH   = $(PROJECT)/emath
INCLUDE = $(PROJECT)/include
OBJECT  = $(PROJECT)/object
LIBRARY = $(PROJECT)/lib

all: elastomath

general_stuff:
	@(cd $(GENERAL); make all)

interfaces: general_stuff
	@(cd $(IFACE); make all)

elastomath: general_stuff
	@(cd $(EMATH); make all)

clean:
	@(cd $(GENERAL); make clean)
	@(cd $(IFACE); make clean)
	@(cd $(EMATH); make clean)
