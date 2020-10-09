//-----------------------------------------------------------------------------
// IntegerStackTest.c
// Test client for the IntegerStack ADT
//-----------------------------------------------------------------------------
#include<stdlib.h>
#include<stdio.h>
#include"IntegerStack.h"


int main(){
    int i, n = 45;
    IntegerStack A = newIntegerStack();
    IntegerStack B = newIntegerStack();

    for( i=1; i<=n; i++){
        push(A, i);
        push(B, i);
    }

    printIntegerStack(stdout, A);
    printIntegerStack(stdout, B);
    printf("%s\n", equals(A, B)?"true":"false");

    for( i=1; i<=n/2; i++){
        printf("%d ", pop(B));
    }
    printf("\n");
    printIntegerStack(stdout, B);
    printf("%s\n", equals(A, B)?"true":"false");

    for( i=1; i<=n/2; i++){
        printf("%d ", peek(A));
        pop(A);
    }
    printf("\n");
    printIntegerStack(stdout, A);
    printf("%s\n", equals(A, B)?"true":"false");

    popAll(A);
    printf("%s\n", equals(A, B)?"true":"false");
    popAll(B);
    printf("%s\n", equals(A, B)?"true":"false");

    freeIntegerStack(&A);
    freeIntegerStack(&B);

    return EXIT_SUCCESS;
}
