#---------------------------------------------------
#       Makefile for ArithmeticOperations.c with Macros
#---------------------------------------------------
ASSIGNMENT     = lab4
CLIENT         = ArithmeticOperations
SOURCES        = $(CLIENT).c
OBJECTS        = $(CLIENT).o
COMPILE        = gcc -c -std=c99 -Wall
LINK           = gcc -o
REMOVE         = rm -f
FILES          = $(SOURCES) Makefile README
SUBMIT         = submit cse015-pt.f19 $(ASSIGNMENT)
MEMCHECK       = valgrind --leak-check=full

$(CLIENT) : $(OBJECTS)
	$(LINK) $(CLIENT) $(OBJECTS)
$(OBJECT) : $(SOURCE)
	$(COMPILE) $(FLAGS) $(SOURCES)
clean :
	$(REMOVE) $(CLIENT) $(OBJECTS)
memcheck : $(CLIENT)
	valgrind --leak-check=full $(CLIENT) in1
submit : $(FILES)
	$(SUBMIT) $(FILES)
check:
	ls /afs/cats.ucsc.edu/class/cse015-pt.f19/lab4/jlao3
