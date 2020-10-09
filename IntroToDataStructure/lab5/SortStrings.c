//-----------------------------------------------------------------------------
// SortStrings.c
//-----------------------------------------------------------------------------

#include<stdio.h>
#include<stdlib.h>
#include<string.h>

void printArray(FILE* out, char** A, int n) {
    int i;
    for(i = 0; i <= n; i++) {
        fprintf(out, "%s", A[i]);
    }
    printf("\n");
}

void swap(char** A, int i, int j) {
    char* temp;
    temp = A[i];
    A[i] = A[j];
    A[j] = temp;
}

int Partition(char** A, int p, int r) {
    int i, j;
    char* x;
    x = A[r];
    i = p - 1;
    for (j = p; j < r; j++){
        if (strcmp(A[j], x) < 0) {
            i++;
            swap(A, i, j);
        }
    }
    swap(A, i+1, r);
    return(i + 1);
}

void QuickSort(char** A, int p, int r) {
    int q;
    if (p < r) {
        q = Partition(A, p, r);
        QuickSort(A, p, q - 1);
        QuickSort(A, q + 1, r);
    }
}

int main(int argc, char** argv){
    FILE* in;
    FILE* out;
    char** stringArray;
    int i = 0, n;

    if( argc != 3) {
        printf("Incomplete call, specify input and output file\n");
        return (EXIT_FAILURE); /* If it the command line doesn't contain 2 arguments then the system will exit */
    }

    in = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    fscanf(in, "%d", &n);
    stringArray = (char **) calloc(n, sizeof(char *));
    for (i = 0; i <= n; i++) {
        stringArray[i] = (char *) calloc(100, sizeof(char));
    }

    for (i = 0; i <= n; i++) {
        fgets(stringArray[i], 100, in);
    }
    QuickSort(stringArray, 0, n);
    printArray(out, stringArray, n);

    for (i = 0; i <= n; i++) {
        free(stringArray[i]);
    }
    free(stringArray);
    stringArray = NULL;

    fclose(in);
    fclose(out);
    return(EXIT_SUCCESS);
}