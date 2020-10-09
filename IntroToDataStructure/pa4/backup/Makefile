#------------------------------------------------------------------------------
# Makefile for any client of any two ADTs in C
#------------------------------------------------------------------------------

ASSIGNMENT      = pa4
CLIENT          = Simulation
ADT1            = IntegerQueue
ADT2            = Job
SOURCE          = $(CLIENT).c
OBJECT          = $(CLIENT).o
ADT1_SOURCE     = $(ADT1).c
ADT1_TEST       = $(ADT1)Test.c
ADT1_OBJECT     = $(ADT1).o
ADT1_HEADER     = $(ADT1).h
ADT2_SOURCE     = $(ADT2).c
ADT2_OBJECT     = $(ADT2).o
ADT2_HEADER     = $(ADT2).h
COMPILE         = gcc -c -std=c99 -Wall
LINK            = gcc -o
REMOVE          = rm -f
FILES           = $(SOURCE) Makefile README $(ADT1_SOURCE) $(ADT1_HEADER) $(ADT1_TEST) $(ADT2_HEADER) $(ADT2_SOURCE)
SUBMIT          = submit cse015-pt.f19 $(ASSIGNMENT)
MEMCHECK        = valgrind --leak-check=full --show-leak-kinds=all

$(CLIENT) : $(OBJECT) $(ADT1_OBJECT) $(ADT2_OBJECT)
	$(LINK) $(CLIENT) $(OBJECT) $(ADT1_OBJECT) $(ADT2_OBJECT)

$(OBJECT) : $(SOURCE) $(ADT1_HEADER) $(ADT2_HEADER)
	$(COMPILE) $(SOURCE)

$(ADT1_OBJECT) : $(ADT1_SOURCE) $(ADT1_HEADER)
	$(COMPILE) $(ADT1_SOURCE)

$(ADT2_OBJECT) : $(ADT2_SOURCE) $(ADT2_HEADER)
	$(COMPILE) $(ADT2_SOURCE)

clean :
	$(REMOVE) $(CLIENT) $(OBJECT) $(ADT1_OBJECT) $(ADT2_OBJECT)
submit : $(FILES)
	$(SUBMIT) $(FILES)
check:
	ls /afs/cats.ucsc.edu/class/cse015-pt.f19/pa3/jlao3
memcheck : $(CLIENT)
	$(MEMCHECK) $(CLIENT) test4
