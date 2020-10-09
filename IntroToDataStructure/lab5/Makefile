#---------------------------------------------------
#       Makefile for SortStrings.c with Macros
#---------------------------------------------------
ASSIGNMENT  = lab5
EXEBIN      = SortStrings
SOURCES     = $(EXEBIN).c
OBJECTS     = $(EXEBIN).o
FLAGS       = -std=c99 -Wall
FILES       = $(SOURCES) Makefile README
SUBMIT      = submit cse015-pt.f19 $(ASSIGNMENT)
MEMCHECK    = valgrind --leak-check=full

$(EXEBIN) : $(OBJECTS)
	gcc -o $(EXEBIN) $(OBJECTS)
$(OBJECTS) : $(SOURCES)
	gcc -c $(FLAGS) $(SOURCES)
clean :
	rm -f $(EXEBIN) $(OBJECTS)
submit : $(FILES)
	$(SUBMIT) $(FILES)
check:
	ls /afs/cats.ucsc.edu/class/cse015-pt.f19/lab5/jlao3
memcheck : $(EXEBIN)
	$(MEMCHECK) $(EXEBIN) in out
