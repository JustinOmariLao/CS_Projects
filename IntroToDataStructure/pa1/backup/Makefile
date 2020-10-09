#---------------------------------------------------
#       Makefile for Subset.c with Macros
#---------------------------------------------------
ASSIGNMENT = pa1
EXEBIN     = Subset
SOURCES    = $(EXEBIN).c
OBJECTS    = $(EXEBIN).o
FLAGS      = -std=c99 -Wall
FILES      = $(SOURCES) Makefile README
SUBMIT     = submit cse015-pt.f19 $(ASSIGNMENT)
$(EXEBIN) : $(OBJECTS)
	gcc -o $(EXEBIN) $(OBJECTS)
$(OBJECTS) : $(SOURCES)
	gcc -c $(FLAGS) $(SOURCES)
clean :
	rm -f $(EXEBIN) $(OBJECTS)
submit : $(FILES)
	$(SUBMIT) $(FILES)
check : 
	ls /afs/cats.ucsc.edu/class/cse015-pt.f19/pa1/jlao3
