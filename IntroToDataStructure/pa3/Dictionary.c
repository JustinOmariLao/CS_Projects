#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include "Dictionary.h"

// Private Types and Functions ------------------------------------------------

//Initial Size
static const int startingSize = 1001;

//NodeObj = Node
typedef struct NodeObj {
    char* key;
    char* value;
    struct NodeObj* next;
} NodeObj;

typedef NodeObj* Node;

Node newNode(char* k, char* v) {
    Node N = malloc(sizeof(NodeObj));
    assert(N != NULL);
    N -> key = k;
    N -> value = v;
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

typedef struct DictionaryObj {
    Node* pairsArray;
    int numItems;
    int physicalSize;
} DictionaryObj;

typedef DictionaryObj* Dictionary;

void doubleItemArray(Dictionary D) {
    int i;
    Node* newArray;
    Node* oldArray = D -> pairsArray;
    D -> physicalSize *= 2;
    newArray = calloc(D -> physicalSize, sizeof(int));
    for(i = 0; i < (D -> numItems); i++) {
        newArray[i] = oldArray[i];
    }
    D -> pairsArray = newArray;
    free(oldArray);
}

int countChars(Node R){
    int count = 0;
    while (R != NULL ) {
        // count = count + countChars(R -> next);
        count = count + strlen(R -> key) + strlen(R -> value) + 2;
        R = R -> next;
    }
    return count;
}

char* toStringInOrder(char* buf, Node R){
    while (R != NULL ) {
        strcpy(buf, R -> key);
        buf += strlen(R -> key);
        *buf = ' ';
        buf++;
        strcpy(buf, R -> value);
        buf += strlen(R -> value);
        *buf = '\n';
        buf++;
        R = R -> next;
    }
    return buf;
}

// newDictionary()
// constructor for the Dictionary type
Dictionary newDictionary(void)
{
    Dictionary D = malloc(sizeof(DictionaryObj));
    assert(D != NULL);
    D -> pairsArray = calloc(startingSize, sizeof(Node));
    D -> numItems = 0;
    D -> physicalSize = startingSize;
    return D;
}

// isEmpty()
// Returns true (1) if S is empty, false (0) otherwise.
int isEmpty(Dictionary D) {
    if (D == NULL ) {
        fprintf(stderr, "Dictionary Error: isEmpty() called on NULL IntegerStack reference");
        exit(EXIT_FAILURE);
    }
    return (D -> numItems == 0);
}

// freeDictionary()
// destructor for  the Dictionary type
void freeDictionary(Dictionary *pD)
{
    if (pD != NULL && *pD != NULL)
    {
        if (isEmpty(*pD))
        {
            makeEmpty(*pD);
        }
        free((*pD) -> pairsArray);
        free(*pD);
        *pD = NULL;
    }
}

//Returns a Reference Node to the Node whose key field equals the key parameter
Node findKey(Dictionary D, char* k){
    if (D == NULL)
    {
        exit(EXIT_SUCCESS);
    }

    Node keyPoint;
    keyPoint = D -> pairsArray[0];
    if (D -> pairsArray == NULL)
    {
        return NULL;
    }
    while (keyPoint != NULL) {
        if (strcmp(keyPoint -> key, k) == 0)
        {
            return keyPoint;
        }
        else
        {
            keyPoint = keyPoint -> next;
        }
    }
    return NULL;
}

// ADT operations -------------------------------------------------------------

// size()
// returns the number of (key, value) pairs in D
int size(Dictionary D) {
    return (D -> numItems);
}

char* lookup(Dictionary D, char* k) {
    if (D == NULL)
    {
        fprintf(stderr, "Dictionary Error: calling lookup() on NULL Dictionary reference\n");
        exit(EXIT_SUCCESS);
    }

    Node N = findKey(D, k);
    if (N != NULL)
    {
        return N -> value;
    }
    else return NULL;
}

void insert(Dictionary D, char* k, char* v) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling insert() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }
    if (lookup(D, k) != NULL) {
        fprintf(stderr, "Dictionary Error: cannot insert() duplicate keys\n");
        exit(EXIT_SUCCESS);
    }

    // increase physical size of array if necessary
    if ((D -> numItems) == (D -> physicalSize)) {
        doubleItemArray(D);
    }

    if (D -> numItems == 0) {
        Node N = newNode(k, v);
        D -> pairsArray[0] = N;
    }
    else {
        Node H = D -> pairsArray[0];
        while (H -> next != NULL) {
            H = H -> next;
        }
        H -> next = newNode(k, v);
    }
    D -> numItems++;
}

void delete(Dictionary D, char* k) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling delete() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    if (lookup(D, k) == NULL) {
        fprintf(stderr, "Dictionary Error: cannot delete() non-existent key\n");
    }
    else {
        Node N = findKey(D, k);
        if (N == D -> pairsArray[0]) {
            Node A = D -> pairsArray[0];
            D -> pairsArray[0] = A -> next;
            freeNode(&A);
        }
        else {
            Node B = D -> pairsArray[0];
            while (B != NULL) {
                Node A = B -> next;
                if (strcmp(A -> key, k) == 0) {
                    break;
                }
                B = B -> next;
            }
            Node A = B -> next;
            Node C = A -> next;
            if (C != NULL) {
                B -> next = C;
                A -> next = NULL;
                freeNode(&A);
            }
            else {
                B -> next = NULL;
                freeNode(&A);
            }
        }
    }
    D -> numItems--;
}

void makeEmpty(Dictionary D) {
    for (int i = 0; i < D -> numItems; i++)
    {
        while (D -> pairsArray[i] != NULL)
        {
            Node N;
            N = D -> pairsArray[i];
            D -> pairsArray[i] = N -> next;
            freeNode(&N);
            N = NULL;
        }
    }
    D -> numItems = 0;
}

// Other Operations -----------------------------------------------------------

char* DictionaryToString(Dictionary D) {
    char* str;
    int n;
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling printDictionary() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    n = countChars(D -> pairsArray[0]);
    str = calloc(n + 1, sizeof(char));
    toStringInOrder(str, D -> pairsArray[0]);
    str[n] = '\0';
    return str;
}






