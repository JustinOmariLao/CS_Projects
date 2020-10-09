#include<stdio.h>
#include<stdlib.h>
#include<ctype.h>
#include<assert.h>
#include<string.h>

#define MAX_LENGTH 100

void extract_chars(char* s, char* a, char* d, char* p, char* w) {
    //Initializing indices for line and each type of character
    int i = 0, al = 0, nu = 0, pu = 0, ws = 0;
    while(s[i] != '\0' && i < MAX_LENGTH) {
        if( isalpha((int)s[i]) )
        {
            a[al++] = s[i];
            i++;
        }
        else if(isdigit((int)s[i]))
        {
            d[nu++] = s[i];
            i++;
        }
        else if(ispunct((int)s[i]))
        {
            p[pu++] = s[i];
            i++;
        }
        else
        {
            w[ws++] = s[i];
            i++;
        }
        //Terminating output arrays by making them into valid C strings with '\0'
        a[al] = '\0';
        d[nu] = '\0';
        p[pu] = '\0';
        w[ws] = '\0';
    }
}

int main(int argc, char* argv[]) {
    FILE* in;
    FILE* out;

    //Creating strings for the input line, alphabetic, numeric, punctuation, and whitespace lines
    char* input;
    char* alpha;
    char* numeric;
    char* punct;
    char* wspace;

    if( argc != 3) {
        return (EXIT_FAILURE); /* If it the command line doesn't contain 2 arguments then the system will exit */
    }

    //Opening 'in' file to read, opening 'out' file to write
    in  = fopen(argv[1], "r");
    out = fopen(argv[2], "w");

    //Allocation of each onto a heap
    input   = calloc(MAX_LENGTH + 1, sizeof(char));
    alpha   = calloc(MAX_LENGTH + 1, sizeof(char));
    numeric = calloc(MAX_LENGTH + 1, sizeof(char));
    punct   = calloc(MAX_LENGTH + 1, sizeof(char));
    wspace  = calloc(MAX_LENGTH + 1, sizeof(char));
    assert(input != NULL && alpha != NULL && numeric != NULL && punct != NULL && wspace != NULL);

    //Reads each line in 'in' file, extracting characters until NULL
    int lineNum = 1;
    while (fgets(input, MAX_LENGTH, in) != NULL) {
        extract_chars(input, alpha, numeric, punct, wspace);
        fprintf(out,"line %d contains:\n",lineNum);
        if(strlen(alpha) > 1) {
            fprintf(out, "%d alphabetic characters: %s\n", (int)strlen(alpha), alpha);
        }
        else {
            fprintf(out, "%d alphabetic character: %s\n", (int)strlen(alpha), alpha);
        }
        if(strlen(numeric) > 1)
        {
            fprintf(out, "%d numeric characters: %s\n", (int)strlen(numeric), numeric);
        }
        else
        {
            fprintf(out, "%d numeric character: %s\n", (int)strlen(numeric), numeric);
        }
        if(strlen(punct) > 1)
        {
            fprintf(out, "%d punctuation characters: %s\n", (int)strlen(punct), punct);
        }
        else
        {
            fprintf(out, "%d punctuation character: %s\n", (int)strlen(punct), punct);
        }
        if(strlen(wspace) > 1)
        {
            fprintf(out, "%d whitespace characters: %s\n", (int)strlen(wspace), wspace);
        }
        else
        {
            fprintf(out, "%d whitespace character: %s\n", (int)strlen(wspace), wspace);
        }
        lineNum++;
    }

    //Freeing allocated heap memory
    free(input);
    input = NULL;
    free(alpha);
    alpha = NULL;
    free(numeric);
    numeric = NULL;
    free(punct);
    punct = NULL;
    free(wspace);
    wspace = NULL;

    //Closing the in & out files
    fclose(in);
    fclose(out);

    return EXIT_SUCCESS;
}
