#include<stdio.h>
#include<stdlib.h>
#include<string.h>
#include<assert.h>
#include "Dictionary.h"

int isEmpty(Dictionary D);

//Initial Size
static const int startingSize = 101;

typedef struct NodeObj {
    char* key;
    char* value;
    struct NodeObj* next;
} NodeObj;

typedef NodeObj* Node;

// newNode()
// creates a newNode emulating Java's new operator
Node newNode(char* k, char* v) {
    Node N = malloc(sizeof(NodeObj));
    assert(N != NULL);
    N -> key = k;
    N -> value = v;
    N -> next = NULL;
    return N;
}

// freeNode()
// frees the heap memory associated with the Dictionary
void freeNode(Node* pN) {
    if (pN != NULL && *pN != NULL) {
        free(*pN);
        *pN = NULL;
    }
}

typedef struct DictionaryObj {
    Node* pairsArray;
    int numItems;
} DictionaryObj;

// newDictionary()
// constructor for the Dictionary type
Dictionary newDictionary(void) {
    Dictionary D = malloc(sizeof(DictionaryObj));
    assert(D != NULL);
    D -> pairsArray = calloc(startingSize, sizeof(Node));
    D -> numItems = 0;
    return D;
}

// freeDictionary()
// destructor for  the Dictionary type
void freeDictionary(Dictionary *pD) {
    if (pD != NULL && *pD != NULL) {
        if (isEmpty(*pD))
        {
            makeEmpty(*pD);
        }
        for(int i = 0; i <= startingSize; i++) {
            while ((*pD) -> pairsArray[i] != NULL) {
                Node N = (*pD) -> pairsArray[i];
                (*pD) -> pairsArray[i] = N -> next;
                free(N);
                freeNode(&N);
                N = NULL;
            }
        }
        free((*pD) -> pairsArray);
        free(*pD);
        *pD = NULL;
    }
}

/*void doubleItemArray(Dictionary D) {
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
} */

int countChars(Dictionary D){
    int count = 0;

    for(int i = 0; i < startingSize; i++) {
        Node N = D -> pairsArray[i];
        while (N != NULL) {
            count = count + strlen(N -> key) + strlen(N -> value) + 2;
            N = N -> next;
        }
    }
    return count;
}

char* toStringInOrder(char* buf, Dictionary D){
    for(int i = 0; i < startingSize; i++) {
        Node N = D -> pairsArray[i];
        while (N != NULL) {
            strcpy(buf, N -> key);
            buf += strlen(N -> key);
            *buf = ' ';
            buf++;
            strcpy(buf, N -> value);
            buf += strlen(N -> value);
            *buf = '\n';
            buf++;
            N = N -> next;
        }
    }
    return buf;
}

// rotate_left()
// rotate the bits in an unsigned int
unsigned int rotate_left(unsigned int value, int shift) {
    int sizeInBits = 8*sizeof(unsigned int);
    shift = shift & (sizeInBits - 1);
    if ( shift == 0 )
        return value;
    return (value << shift) | (value >> (sizeInBits - shift));
}

// pre_hash()
// turn a string into an unsigned int
unsigned int pre_hash(char* input) {
    unsigned int result = 0xBAE86554;
    while (*input) {
        result ^= *input++;
        result = rotate_left(result, 5);
    }
    return result;
}

// hash()
// turns a string into an int in the range 0 to tableSize-1
int hash(char* key) {
    return pre_hash(key)%startingSize;
}

// findKey()
// finds a certain key in the hash pairsArray
Node findKey(Dictionary D, char* k) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling findKey() on NULL Dictionary reference\n");
        return EXIT_SUCCESS;
    }

    if (D -> pairsArray == NULL) {
        return NULL;
    }
    Node N = D -> pairsArray[hash(k)];
    while (N != NULL) {
        if (strcmp(N -> key, k) == 0) {
            return N;
        }
        else {
            N = N -> next;
        }
    }
    return NULL;
}

// isEmpty()
// returns 1(true) if S is empty or 0 (false) otherwise
int isEmpty(Dictionary D) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling isEmpty() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    return (D -> numItems == 0);
}

// size()
// Return the number of (key, value) pairs in Dictionary D.
int size(Dictionary D) {
    return (D -> numItems);
}

// lookup()
// If D contains a pair whose key matches argument k, then return the
// corresponding value, otherwise return NULL.
char* lookup(Dictionary D, char* k) {
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling lookup() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    Node N = findKey(D, k);
    if (N != NULL) {
        return N -> value;
    }
    return NULL;
}

// insert()
// Insert the pair (k,v) into D.
// Pre: lookup(D, k)==NULL (D does not contain a pair whose first member is k.)
void insert(Dictionary D, char* k, char* v) {
    int i = hash(k);
    if (D == NULL) {
        fprintf(stderr,"Dictionary Error: calling insert() on NULL Dictionary reference\n");
        exit(EXIT_SUCCESS);
    }

    if (lookup(D, k) != NULL) {
        fprintf(stderr, "Dictionary Error: cannot insert() duplicate keys\n");
        exit(EXIT_SUCCESS);
    }

    /*
    // increase physical size of array if necessary
    if ((D -> numItems) == (D -> physicalSize)) {
        doubleItemArray(D);
    } */

    else {
        if (D -> pairsArray[i] == NULL) {
            Node N = newNode(k, v);
            D -> pairsArray[i] = N;
        }
        else {
            Node N = newNode(k, v);
            N -> next = D -> pairsArray[i];
            D -> pairsArray[i] = N;
        }
        D -> numItems++;
    }
}

// delete()
// Remove pair whose first member is the argument k from D.
// Pre: lookup(D,k)!=NULL (D contains a pair whose first member is k.)
void delete(Dictionary D, char* k) {
    int i = hash(k);
    if (D == NULL) {
        fprintf(stderr,"Dictionary Error: calling isEmpty() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    if (lookup(D, k) == NULL) {
        fprintf(stderr, "Dictionary Error: cannot delete() empty list\n");
        exit(EXIT_FAILURE);
    }
    else {
        Node N = findKey(D, k);
        if (N == D -> pairsArray[i]) {
            Node A = D -> pairsArray[i];
            D -> pairsArray[i] = A->next;
            freeNode(&A);
        }
        else {
            Node H = D -> pairsArray[i];
            while (H != NULL) {
                Node A = H -> next;
                if (strcmp(A -> key, k) == 0) {
                    break;
                }
                H = H->next;
            }

            Node A = H -> next;
            Node S = A -> next;
            if (S != NULL) {
                H -> next = S;
                A -> next = NULL;
                freeNode(&A);
            }
            else {
                H -> next = NULL;
                freeNode(&A);
            }
        }
    }
    D -> numItems--;
}

// makeEmpty()
// Reset D to the empty state, the empty set of pairs.
void makeEmpty(Dictionary D) {
    for (int i = 0; i < startingSize; i++) {
        while (D -> pairsArray[i] != NULL) {
            Node N = D -> pairsArray[i];
            D -> pairsArray[i] = N -> next;
            freeNode(&N);
            N = NULL;
        }
    }
    D -> numItems = 0;
}

// printDictionary()
// pre: none
// prints a text representation of D to the file pointed to by out
char* DictionaryToString(Dictionary D) {
    char* str;
    int n;
    if (D == NULL) {
        fprintf(stderr, "Dictionary Error: calling printDictionary() on NULL Dictionary reference\n");
        exit(EXIT_FAILURE);
    }

    n = countChars(D);
    str = calloc(n + 1, sizeof(char));
    toStringInOrder(str, D);
    str[n] = '\0';
    return str;
}

