#include <stdio.h>
#include <stdlib.h>

#define MAX_SIZE 100
#define SUBSET_USAGE "Usage: Subset n k (n and k are ints satisfying 0<=k<=n<=100)"

void printSet(int B[MAX_SIZE + 1], int n) {
    int j;
    int comma = 0;
    int emptyset = 1;
    printf("{");
    for (j = 1; j <= n; j++) {
        if (B[j] == 1) {
            if (comma == 1) {
                printf(", ");
            }
            emptyset = 0;
            printf("%d", j);
            comma = 1;
        }
    }
    if (emptyset) {
        printf(" ");
    }
    printf("}\n");
}

void printSubsets(int B[], int n, int k, int i) {
    if (k == 0) {
        printSet(B, n);
    }
    else if (k > n - i + 1) {
	return;
    }
    else {
        B[i] = 1;
        printSubsets(B, n, k - 1, i + 1);
        B[i] = 0;
        printSubsets(B, n, k, i + 1);
    }
}

//Using the CommandLineArguments.c example as reference for main
int main(int argc, char *argv[]) {
    int n, k, i, status;
    char c;
    int B[MAX_SIZE + 1] = {0};

    if (argc != 3) {
        //Since argc is 1 + how many arguments we have (this being 2), argc should equal 3.
        printf("%s", SUBSET_USAGE);
        return EXIT_SUCCESS;
    }
    for(i = 1; i < argc; i++) {
        //Parse argv[i] as int
        status = sscanf(argv[i], "%d%c", &n, &c);
        if (status == 1) {
            continue; //if it is an int then skip to the next step
        }
        printf("%s", SUBSET_USAGE);
        return EXIT_SUCCESS;
    }

    n = atoi(argv[1]);
    k = atoi(argv[2]);
    if ((k < 0) || (n < k) || (n > MAX_SIZE)) {
        printf("%s", SUBSET_USAGE);
        return EXIT_SUCCESS;
    }
    printSubsets(B, n, k , 1);
    return EXIT_SUCCESS;
}
