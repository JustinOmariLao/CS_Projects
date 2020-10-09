#include<stdlib.h>
#include<stdio.h>
#include<string.h>
#include <assert.h>
#include "IntegerQueue.h"
#include "Job.h"

#define startingSize 10
typedef struct NodeObj {
    int x;
    struct NodeObj* next;
} NodeObj;

typedef NodeObj* Node;

Node newNode(int a) {
    Node N = malloc(sizeof(NodeObj));
    assert(N != NULL);
    N -> x = a;
    N -> next = NULL;
    return N;
}

void freeNode(Node* pN) {
    if (pN != NULL && *pN != NULL)
    {
        free(*pN);
        *pN = NULL;
    }
}

typedef struct IntegerQueueObj {
    Node front;
    Node* back;
    int numItems;
    int physicalSize;
} IntegerQueueObj;

typedef IntegerQueueObj* IntegerQueue;

// newDictionary()
// constructor for the Dictionary type
IntegerQueue newIntegerQueue(void)
{
    IntegerQueue Q = malloc(sizeof(IntegerQueueObj));
    assert(Q != NULL);
    Q -> front = NULL;
    Q -> back = calloc(startingSize, sizeof(IntegerQueueObj));
    Q -> numItems = 0;
    Q -> physicalSize = startingSize;
    return Q;
}

// freeIntegerQueue()
// Destructor for the Queue ADT
void freeIntegerQueue(IntegerQueue* pQ) {
    if (pQ != NULL && *pQ != NULL)
    {
        if (isEmpty(*pQ))
        {
            dequeueAll(*pQ);
        }
        free((*pQ) -> back);
        free(*pQ);
        *pQ = NULL;
    }
}
void doubleItemArray(IntegerQueue Q) {
    int i;

    //Double size of Node* left
    Node* newBack;
    Node* oldBack = Q -> back;
    newBack = calloc(Q -> physicalSize * 2, sizeof(NodeObj));
    for(i = 0; i < (Q -> numItems - 1); i++) {
        newBack[i] = oldBack[i];
    }
    Q -> back = newBack;
    Q -> physicalSize = 2*Q -> physicalSize;
    free(oldBack);
}

int countSize(IntegerQueue Q) {
    int counter = 0;
    if (isEmpty(Q) || Q == NULL) {
        return 0;
    }

    //printf("Start of counter is %d\n", counter);
    Node H = Q -> front;
    if (H -> x >= 100) {
        counter = counter + 5;
    }
    else if (H -> x >= 10) {
        counter = counter + 4;
    }
    else counter = counter + 3;

    Node N = Q -> back[0];
    while (N != NULL) {
        if (H -> x >= 100) {
            counter = counter + 5;
        }
        else if (N -> x >= 10) {
            counter = counter + 4;
        }
        else counter = counter + 3;
        //printf("Counter[0] = %d\n", counter);
        N = N -> next;
    }
    //printf("Counter[Final] = %d\n", counter);
    return counter;
}

char* toStringInOrder(char* buf, Node N, Node H) {
    sprintf(buf, "%d ", N -> x);
    if (N -> x >= 100) {
        buf += 4;
    }
    else if (N -> x >= 10) {
        buf += 3;
    }
    else buf += 2;
    *buf = ' ';
    buf++;

    while (H != NULL) {
        sprintf(buf, "%d ", H -> x);
        if (N -> x >= 100) {
            buf += 4;
        }
        else if (H -> x >= 10) {
            buf += 3;
        }
        else buf += 2;
        *buf = ' ';
        buf++;
        H = H -> next;
    }
    return buf;
}
// ADT operations -------------------------------------------------------------

// isEmpty()
// Returns 1 (true) if Queue Q is empty, 0 (false) otherwise.
int isEmpty(IntegerQueue Q) {
    if (Q == NULL ) {
        fprintf(stderr, "IntegerQueue Error: isEmpty() called on NULL IntegerQueue reference");
        exit(EXIT_FAILURE);
    }
    if (Q -> numItems != 0) {
        return 0;
    }
    else {
        return 1;
    }
}

// length()
// Returns the number of elements in Q
int length(IntegerQueue Q) {
    if (Q == NULL ) {
        fprintf(stderr, "IntegerQueue Error: length() called on NULL IntegerQueue reference");
        exit(EXIT_FAILURE);
    }

    return Q -> numItems;
}

// enqueue()
// Inserts x at back of Q.
void enqueue(IntegerQueue Q, int x) {
    if (Q == NULL) {
        fprintf(stderr, "IntegerQueue Error: calling enqueue() on NULL IntegerQueue reference\n");
        exit(EXIT_FAILURE);
    }

    // increase physical size of array if necessary
    if ((Q -> numItems - 1) == (Q -> physicalSize)) {
        doubleItemArray(Q);
    }

    Node N = newNode(x);
    if (Q -> front == NULL) {
        Q -> front = N;
        //printf("Correctly put the first one in, now equal to %d\n", Q -> front -> x);
    }
    else {
        Node H = Q -> back[0];
        if (Q -> back[0] == NULL) {
            Q -> back[0] = N;
            //printf("Correctly entered into back[0], now equal to %d\n", Q -> back[0] -> x);
        }
        else {
            while (H -> next != NULL) {
                H = H -> next;
            }
            H -> next = N;
            //printf("Correctly went here since back[%d] is full, back[%d] is now equal to %d\n", i, i + 1, H -> next -> x);
        }
    }
    Q -> numItems++;
}

// dequeue()
// Deletes and returns the item at front of Q.
// Pre: !isEmpty()
int dequeue(IntegerQueue Q) {
    int returnValue, i = 0;
    if (isEmpty(Q)) {
        fprintf(stderr, "IntegerQueue Error: calling dequeue() on NULL IntegerQueue reference\n");
        return EXIT_SUCCESS;
    }

    Node temp = Q -> front;
    returnValue = temp -> x;
    if (Q -> back[0] == NULL) {
        Q -> front = NULL;
        Q -> numItems--;
        return returnValue;
    }
    else {
        Q -> front = Q -> back[0];
        //printf("New front value is %d\n", Q -> front -> x);
        //printf("numItems is %d\n", Q -> numItems);
        Node H = Q -> back[0];
        while (H -> next != NULL) {
            Q -> back[i] = H->next;
            H = H -> next;
            i += 1;
        }
        Q -> back[Q -> numItems - 2] = NULL;
        Q -> numItems--;
        return returnValue;
    }
}

// peek()
// Returns the item at front of Q.
// Pre: !isEmpty()
int peek(IntegerQueue Q) {
    int returnInteger;
    if (isEmpty(Q)  == 1) {
        fprintf(stderr, "IntegerQueue Error: calling peek() on NULL IntegerQueue reference\n");
        return EXIT_SUCCESS;
    }

    Node H = Q -> front;
    returnInteger = H -> x;
    return  returnInteger;
}

// dequeueAll()
// Resets Q to the empty state.
void dequeueAll(IntegerQueue Q) {
    if (Q == NULL) {
        fprintf(stderr, "IntegerQueue Error: calling dequeueAll() on NULL IntegerQueue reference\n");
        exit(EXIT_FAILURE);
    }

    Q -> front = NULL;
    Q -> back[0] = NULL;
    Q -> back = NULL;
    Q -> numItems = 0;
}

// Other Operations -----------------------------------------------------------

// IntegerQueueToString()
// Determines a text representation of IntegerQueue Q. Returns a pointer to a
// char array, allocated from heap memory, containing the integer sequence
// represented by Q. The sequence is traversed from front to back, each integer
// is added to the string followed by a single space. The array is terminated
// by a NUL '\n' character. It is the responsibility of the calling function to
// free this heap memory.
char* IntegerQueueToString(IntegerQueue Q) {
    char* integerString;
    int n;

    n = countSize(Q);
    integerString = calloc(n + 1, sizeof(IntegerQueue));
    //printf("I mean i made it here and n = %d\n", n);
    if (n == 0) {
        integerString[n] = '\0';
        return integerString;
    }
    else toStringInOrder(integerString, Q -> front, Q -> back[0]);
    integerString[n] = '\0';
    return integerString;
}

// equals()
// Returns true (1) if Q and R are matching sequences of integers, false (0)
// otherwise.
int equals(IntegerQueue Q, IntegerQueue R) {
    if (Q == NULL || R == NULL) {
        fprintf(stderr, "IntegerQueue Error: equals() called on NULL IntegerQueue reference");
        exit(EXIT_FAILURE);
    }

    if (Q -> numItems != R -> numItems) {
        return 0;
    }

    //printf("Got here\n");
    Node A = Q -> front;
    Node B = R -> front;
    if (A -> x != B -> x) {
        return 0;
    }
    else {
        Node C = Q -> back[0];
        Node D = R -> back[0];
        while (C != NULL && D != NULL) {
            if (C -> x != D -> x) {
                return 0;
            }
            C = C -> next;
            D = D -> next;
        }
    }
    return 1;
}