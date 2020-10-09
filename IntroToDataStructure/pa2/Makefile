#---------------------------------------------------
#       Makefile for Queens.c with Macros
#---------------------------------------------------
ASSIGNMENT = pa2
EXEBIN = Queens
SOURCES = $(EXEBIN).c
OBJECTS = $(EXEBIN).o
FLAGS = -std=c99 -Wall
FILES = $(SOURCES) Makefile README
SUBMIT = submit cse015-pt.f19 $(ASSIGNMENT)
$(EXEBIN) : $(OBJECTS)
	gcc -o $(EXEBIN) $(OBJECTS)
$(OBJECTS) : $(SOURCES)
	gcc -c $(FLAGS) $(SOURCES)
clean :
	rm -f $(EXEBIN) $(OBJECTS)
memcheck :
	valgrind --leak-check=full $(EXEBIN) -v 8
submit : $(FILES)
	$(SUBMIT) $(FILES)
check:
	ls /afs/cats.ucsc.edu/class/cse015-pt.f19/pa2/jlao3
