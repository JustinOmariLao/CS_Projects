#---------------------------------------------------
#       Makefile for Dictionary.c with Macros
#---------------------------------------------------
ASSIGNMENT     = pa3
CLIENT         = DictionaryTest
ADT            = Dictionary
SOURCE         = $(CLIENT).c
OBJECT         = $(CLIENT).o
ADT_SOURCE     = $(ADT).c
ADT_OBJECT     = $(ADT).o
ADT_HEADER     = $(ADT).h
COMPILE        = gcc -c -std=c99 -Wall
LINK           = gcc -o
REMOVE         = rm -f
FILES          = $(SOURCES) Makefile README $(ADT_SOURCE) $(ADT_HEADER)
SUBMIT         = submit cse015-pt.f19 $(ASSIGNMENT)
MEMCHECK       = valgrind --leak-check=full

$(CLIENT) : $(OBJECT) $(ADT_OBJECT)
	$(LINK) $(CLIENT) $(OBJECT) $(ADT_OBJECT)
$(OBJECT) : $(SOURCE) $(ADT_HEADER)
	$(COMPILE) $(SOURCE)
$(ADT_OBJECT) : $(ADT_SOURCE) $(ADT_HEADER)
	$(COMPILE) $(ADT_SOURCE)
clean :
	$(REMOVE) $(CLIENT) $(OBJECT) $(ADT_OBJECT)
submit : $(FILES)
	$(SUBMIT) $(FILES)
check:
	ls /afs/cats.ucsc.edu/class/cse015-pt.f19/pa3/jlao3
memcheck : $(CLIENT)
	$(MEMCHECK) $(CLIENT)
