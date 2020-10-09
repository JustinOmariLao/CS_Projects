#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define USAGE_ERROR "Usage: Queens [-v] number"
#define OPTION_ERROR "Option: -v verbose output, print all solutions"

void placeQueen(int** B, int n, int i, int j) {
    int k, l, qAttackLR, qAttackRL;
    B[i][j] = 1;
    B[i][0] = j;
    for (k = i + 1; k <= n; k++) {
        for (l = 1; l <= n; l++) {
            if (l == j) {
                B[k][l] = B[k][l] - 1; //Indicates Queen attacking column of B[i][1 <= j <= n]
            }
            qAttackLR = i - j;
            if ((qAttackLR) == (k - l)) {
                B[k][l] = B[k][l] - 1; //Indicates Queen attack diagonal from Top Left to Bottom Right
            }
            qAttackRL = i + j;
            if ((qAttackRL) == (k + l)) {
                B[k][l] = B[k][l] - 1; //Indicates Queen attack diagonal from Top Right to Bottom Left
            }
        }
    }
}

void removeQueen(int** B, int n, int i, int j) {
    int k, l, qAttackLR, qAttackRL;
    B[i][j] = 0;
    B[i][0] = 0;
    for (k = i + 1; k <= n; k++) {
        for (l = 1; l <= n; l++) {
            if (l == j) {
                B[k][l] = B[k][l] + 1; //Indicates Queen removal from column of B[i][1 <= j <= n]
            }
            qAttackLR = i - j;
            if (qAttackLR == (k - l)) {
                B[k][l] = B[k][l] + 1; //Indicates Queen removal from diagonal from Top Left to Bottom Right
            }
            qAttackRL = i + j;
            if (qAttackRL == (k + l)) {
                B[k][l] = B[k][l] + 1; //Indicates Queen removal from diagonal from Top Right to Bottom Left
            }
        }
    }
}

void printBoard(int** B, int n) {
    int i;
    int comma = 0;
    printf("(");
    for (i = 1; i <= n; i++) {
        if (comma == 1) {
            printf(", ");
        }
        printf("%d", B[i][0]);
        comma = 1;
    }
    printf(")\n");
}

int findSolutions(int** B, int n, int i, char* mode) {
    int sum = 0;
    if (i > n) {
        if (strcmp(mode, "verbose") == 0) {
            printBoard(B, n);
            sum = sum + 1;
        }
        else {
            sum = sum + 1;
        }
        return sum;
    }
    else {
        for (int j = 1; j <= n; j++) {
            if (B[i][j] >= 0) {
                placeQueen(B, n, i, j);
                sum = sum + findSolutions(B, n, i + 1, mode);
                removeQueen(B, n, i, j);
            }
        }
    }
    return sum;
}

int main(int argc, char* argv[]) {
    int i, n, status, sum;
    char c;
    char* mode = "";

    /* If the command line doesn't contain an argument then the system will exit */
    if ((argc != 3) && (argc != 2)) {
	printf("%s\n", USAGE_ERROR);
        printf("%s\n", OPTION_ERROR);
        return EXIT_SUCCESS;
    }

    for(i = 1; i < argc; i++) {
        //Parse argv[i] as int
        if (argc == 3) {
            status = sscanf(argv[2], "%d%c", &n, &c);
            if (status == 1) {
                continue; //if it is an int then skip to the next step
            }
	    printf("%s\n", USAGE_ERROR);
            printf("%s\n", OPTION_ERROR);
            return EXIT_SUCCESS;
        }
        if (argc == 2) {
            status = sscanf(argv[1], "%d%c", &n, &c);
            if (status == 1) {
                continue; //if it is an int then skip to the next step
            }
	    printf("%s\n", USAGE_ERROR);
            printf("%s\n", OPTION_ERROR);
            return EXIT_SUCCESS;
        }
    }

    if (argc == 3) {
        if (strcmp(argv[1], "-v") == 1) {
	    printf("%s\n", USAGE_ERROR);
            printf("%s\n", OPTION_ERROR);
            return EXIT_SUCCESS;
        }
        mode = "verbose";
        n = atoi(argv[2]);
    }
    else {
        n = atoi(argv[1]);
    }

    int** B = (int **) calloc(n + 1, sizeof(int *));
    for (int i = 1; i <= n; i++) {
        B[i] = (int *) calloc(n + 1, sizeof(int));
    }

    sum = findSolutions(B, n, 1, mode);
    printf("%d-Queens has %d solutions\n", n, sum);

    for(int i = 0; i < n + 1; ++i) {
        free(B[i]);
    }
    free(B);
    B = NULL;

    return 0;
}
