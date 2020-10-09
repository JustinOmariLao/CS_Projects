#include<stdio.h>
#include<stdlib.h>

int sum(int x, int y) {
    return x + y;
}

int diff(int x, int y) {
    return x - y;
}

int prod(int x, int y) {
    return x * y;
}

int quot(int x, int y) {
    return x / y;
}

int rem(int x, int y) {
    return x % y;
}

typedef int (*fptr_t) (int, int);

int apply(fptr_t fp, int x, int y) {
    return fp(x, y);
}

int compute(fptr_t fcn[5], int* A, int* idx, int n) {
    int i, returned, total = 0;
    for (i = 0; i <= n - 1; i++) {
        returned = apply(fcn[idx[i]], A[i], A[i + 1]);
        A[i + 1] = returned;
        total = returned;
    }
    return total;
}

int main(int argc, char* argv[]) {
    // testCompute();  // output should be 86
    FILE *inputFile;
    char *input;
    int *idx, *A;
    int n, i;

    if (argc != 2) {
        return (EXIT_FAILURE);
    }

    int (*fcn[5])(int, int);
    fcn[0] = sum;
    fcn[1] = diff;
    fcn[2] = prod;
    fcn[3] = quot;
    fcn[4] = rem;

    inputFile = fopen(argv[1], "r");
    fscanf(inputFile, "%d", &n);

    //Allocates and reads the op-code into idx
    input = calloc(n, sizeof(char));
    idx = calloc(n, sizeof(int));
    for (i = 0; i <= n - 1; i++){
	fscanf(inputFile, "%d", &idx[i]);
    }

    //Allocates and reads the operands into A
    A = calloc(n + 1, sizeof(int));
    for (i = 0; i <= n; i++) {
        fscanf(inputFile, "%d", &A[i]);
    }

    printf("%d\n", compute(fcn, A, idx, n));

    free(input);
    input = NULL;
    free(idx);
    idx = NULL;
    free(A);
    A = NULL;
   
    fclose(inputFile);
}
