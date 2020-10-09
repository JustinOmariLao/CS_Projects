#include <stdlib.h>
#include <stdio.h>
#include "IntegerStack.h"

// Private Types and Functions ------------------------------------------------

// InitialSize
static const int InitialSize = 1;

// IntegerListObj
typedef struct IntegerStackObj {
    int* item;         // array of IntegerStack items
    int numItems;      // number of items in this IntegerStack
    int physicalSize;  // current length of underlying array
} IntegerStackObj;

// doubleItemArray
// Doubles the physical size of the underlying array S->item.
void doubleItemArray(IntegerStack S) {
    int i;
    int* newArray;
    int* oldArray = S -> item;
    S -> physicalSize *= 2;
    newArray = calloc(S -> physicalSize, sizeof(int));
    for(i = 0; i < (S -> numItems); i++) {
        newArray[i] = oldArray[i];
    }
    S -> item = newArray;
    free(oldArray);
}

// arrayIndex()
// transforms an IntegerStack index to an Array index
int arrayIndex(int stackIndex){
    return stackIndex;
}

// Constructors-Destructors ---------------------------------------------------

// newIntegerStack()
// Constructor for the IntegerStack ADT
IntegerStack newIntegerStack() {
    IntegerStack S = malloc(sizeof(IntegerStackObj));
    S -> item = calloc(InitialSize, sizeof(int));
    S -> numItems = 1;
    S -> physicalSize = InitialSize;
    return S;
}

// freeIntegerStack()
// Destructor for the Stack ADT
void freeIntegerStack(IntegerStack* pS){
    if( pS!=NULL && *pS!=NULL ){
        free((*pS)->item);
        free(*pS);
        *pS = NULL;
    }
}

// ADT operations -------------------------------------------------------------

// isEmpty()
// Returns true (1) if S is empty, false (0) otherwise.
int isEmpty(IntegerStack S) {
    if( S == NULL ){
        fprintf(stderr, "IntegerStack Error: isEmpty() called on NULL IntegerStack reference");
        exit(EXIT_FAILURE);
    }
    return (S -> numItems == 0);
}

// push()
// Pushes x on top of S.
void push(IntegerStack S, int x) {
    if( S == NULL ){
        fprintf(stderr, "IntegerStack Error: pushh() called on NULL IntegerStack reference");
        exit(EXIT_FAILURE);
    }

    // increase physical size of array if necessary
    if( (S -> numItems) == (S -> physicalSize) ){
        doubleItemArray(S);
    }

    S -> item[(S -> numItems)] = x;
    S -> numItems++;
}

// pop()
// Deletes and returns integer at top of S.
// Pre: !isEmpty(S)
int pop(IntegerStack S) {
    int returnInteger;
    if( S == NULL ){
        fprintf(stderr, "IntegerStack Error: pop() called on NULL IntegerStack reference");
        exit(EXIT_FAILURE);
    }

    if( S -> numItems == 0 ){
        fprintf(stderr, "Stack Error: calling pop() on empty Stack\n");
        exit(EXIT_FAILURE);
    }

    // increase physical size of array if necessary
    if( (S -> numItems) == (S -> physicalSize) ){
        doubleItemArray(S);
    }

    returnInteger = S -> item[S -> numItems - 1];
    S -> item[(S -> numItems)] == NULL;
    S -> numItems--;
    return returnInteger;
}

// peek()
// Returns integer at top of S.
// Pre: !isEmpty(S)
int peek(IntegerStack S) {
    int returnInteger;
    if( S == NULL ){
        fprintf(stderr, "IntegerStack Error: peek() called on NULL IntegerStack reference");
        exit(EXIT_FAILURE);
    }

    // increase physical size of array if necessary
    if( (S -> numItems) == (S -> physicalSize) ){
        doubleItemArray(S);
    }

    returnInteger = S -> item[S -> numItems - 1];
    return returnInteger;
}

// popAll()
// Resets S to the empty state.
void popAll(IntegerStack S) {
    if( S == NULL ){
        fprintf(stderr, "IntegerStack Error: popAll() called on NULL IntegerStack reference");
        exit(EXIT_FAILURE);
    }
    if( S -> numItems == 0 ) {
        fprintf(stderr, "Stack Error: calling popAll() on empty Stack\n");
        exit(EXIT_FAILURE);
    }

    // increase physical size of array if necessary
    if( (S -> numItems) == (S -> physicalSize) ){
        doubleItemArray(S);
    }

    while(S -> numItems > 0) {
        pop(S);
    }
}

// Other Operations -----------------------------------------------------------

// printIntegerStack()
// Prints a space separated list of integers in S, from top to bottom, to the
// Filestream out.
void printIntegerStack(FILE* out, IntegerStack S) {
    int i;
    if(S == NULL) {
        fprintf(stderr, "IntegerList Error: printIntegerStack() called on NULL IntegerStack reference");
        exit(EXIT_FAILURE);
    }

    for(i = S -> numItems - 1; i != NULL; i--) fprintf(out, "%d ", S -> item[i]);
    fprintf(out, "\n");
}

// equals()
// Returns true (1) if S and T are matching sequences of integers, false (0)
// otherwise.
int equals(IntegerStack S, IntegerStack T) {
    int i, eq;
    if (S == NULL || T == NULL ){
        fprintf(stderr, "IntegerStack Error: equals() called on NULL IntegerStack reference");
        exit(EXIT_FAILURE);
    }

    eq = ((S -> numItems) == (T -> numItems));
    for (i = 1; eq && i <= (S -> numItems); i++) {
        eq = ((S -> item[arrayIndex(i)]) == (T -> item[arrayIndex(i)]));
    }
    return eq;
}