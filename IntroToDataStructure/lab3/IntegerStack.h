//-----------------------------------------------------------------------------
// IntegerStack.h
// Header file for the IntegerStack ADT
//-----------------------------------------------------------------------------
#include<stdlib.h>
#include<stdio.h>

#ifndef _Stack_H_INCLUDE_
#define _Stack_H_INCLUDE_


// Exported type --------------------------------------------------------------

// IntegerStack
// Exported reference type
typedef struct IntegerStackObj* IntegerStack;


// Constructors-Destructors ---------------------------------------------------

// newIntegerStack()
// Constructor for the IntegerStack ADT
IntegerStack newIntegerStack();

// freeIntegerStack()
// Destructor for the Stack ADT
void freeIntegerStack(IntegerStack* pS);


// ADT operations -------------------------------------------------------------

// isEmpty()
// Returns true (1) if S is empty, false (0) otherwise.
int isEmpty(IntegerStack S);

// push()
// Pushes x on top of S.
void push(IntegerStack S, int x);

// pop()
// Deletes and returns integer at top of S.
// Pre: !isEmpty(S)
int pop(IntegerStack S);

// peek()
// Returns integer at top of S.
// Pre: !isEmpty(S)
int peek(IntegerStack S);

// popAll()
// Resets S to the empty state.
void popAll(IntegerStack S);


// Other Operations -----------------------------------------------------------

// printIntegerStack()
// Prints a space separated list of integers in S, from top to bottom, to the
// filestream out.
void printIntegerStack(FILE* out, IntegerStack S);

// equals()
// Returns true (1) if S and T are matching sequences of integers, false (0)
// otherwise.
int equals(IntegerStack S, IntegerStack T);


#endif
