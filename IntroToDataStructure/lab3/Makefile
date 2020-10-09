#---------------------------------------------------
#       Makefile for IntegerStack.c with Macros
#---------------------------------------------------
ASSIGNMENT     = lab3
CLIENT         = IntegerStackTest
ADT            = IntegerStack
SOURCES        = $(CLIENT).c
OBJECTS        = $(CLIENT).o
ADT_SOURCE     = $(ADT).c
ADT_OBJECT     = $(ADT).o
ADT_HEADER     = $(ADT).h
FLAGS          = -std=c99 -Wall
FILES          = $(SOURCES) Makefile README $(ADT_SOURCE) $(ADT_HEADER)
SUBMIT         = submit cse015-pt.f19 $(ASSIGNMENT)
MEMCHECK       = valgrind --leak-check=full

$(CLIENT) : $(OBJECTS) $(ADT_OBJECT)
	gcc -o $(CLIENT) $(OBJECTS) $(ADT_OBJECT)
$(OBJECTS) : $(SOURCES)
	gcc -c $(FLAGS) $(SOURCES)
$(ADT_OBJECT) : $(ADT_SOURCE) $(ADT_HEADER)
	gcc -c $(FLAGS) $(ADT_SOURCE)
clean :
	rm -f $(CLIENT) $(OBJECTS)
submit : $(FILES)
	$(SUBMIT) $(FILES)
check:
	ls /afs/cats.ucsc.edu/class/cse015-pt.f19/lab3/jlao3
memcheck : $(CLIENT)
	$(MEMCHECK) $(CLIENT)

