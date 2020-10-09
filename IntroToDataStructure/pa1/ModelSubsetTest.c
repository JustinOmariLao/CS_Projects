//./ModelSubsetTest -v > SubsetTest-out.txt &>> SubsetTest-out.txt
#define _GNU_SOURCE
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <ctype.h>
#include <fcntl.h>
#include <unistd.h>
#include <signal.h>
#include <setjmp.h>
#include <stdbool.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <time.h>
#define bool _Bool
#define main main_ignore
#include "Subset.c"
#undef main

static uint8_t testsPassed;
static volatile sig_atomic_t testStatus;
static uint8_t disable_exit_handler;
jmp_buf test_crash;
int test_count;

#define FIRST_TEST setToString1_test
#define MAXSCORE 40
#define PNTSPERTEST 8
#define BREAKER_TEST_RANGE 20000
const char origtempstr[20*BREAKER_TEST_RANGE];
enum Test_e {
  setToString1_test = 0,
  setToString2_test,
  printSubsets1_test,
  printSubsets2_test,
  printSubsets3_test,

  NUM_TESTS,
};

char* strtrim(char *str) {
   // printf("4444:%s\n",str);
    char *ptr = malloc((strlen(str)+1)*sizeof(char));
    int i,j=0;
    for(i=0;str[i]!='\0';i++)
    {
        if (!isspace(str[i]) && str[i] != '\t'&&str[i] != '\n') 
        ptr[j++]=str[i];
    } 
    ptr[j]='\0';
   
     return ptr;
 
}
char *testName(int test) {

        if (test == setToString1_test)
            return "setToString1_test";
        if (test == setToString2_test)
            return "setToString2_test";
        if (test == printSubsets1_test)
            return "printSubsets1_test";
        if (test == printSubsets2_test)
            return "printSubsets2_test";
        if (test == printSubsets3_test)
            return "printSubsets3_test";

        return "";
    }

bool strcmpn(int ss[], char* s,int n) {
        //char* temp = NULL;
        //itoa()
       
        int SIZE = 500;

        char buffer[SIZE];
        memset( buffer, '\0', sizeof(char)*SIZE );
        //char *buffer = NULL;
        //fflush(stdout);
        // freopen("/dev/null", "a", stdout);
        // setbuf(stdout, buffer);
        // printSet(ss,  n);
        // freopen ("/dev/tty", "a", stdout);
        //printSet(ss,  n);
        int stdout_save = dup(STDOUT_FILENO); //save the stdout state
        freopen("NUL", "a", stdout); //redirect stdout to null pointer
        setvbuf(stdout, buffer, _IOFBF, 1024); //set buffer to stdout
        printSet(ss,  n);
        freopen("NUL", "a", stdout); //redirect stdout to null again
        dup2(stdout_save, STDOUT_FILENO); //restore the previous state of stdout
        setvbuf(stdout, NULL, _IONBF, 1024); //disable buffer to print to screen instantly
           //printf("22222\n");
        //fflush(stdout);
       char *buffer_trim =strtrim(buffer);
       //printf("333 %s:%s\n", buffer,s);
       char *s_trim = strtrim(s);
       // printf("333 %s:%s\n", buffer_trim,s_trim );
        if(strcmp(buffer_trim,s_trim) == 0)
            return 1;
         //printf("your result: %s",buffer);
         //printf("correct: %s",s);
        return 0;

    }


int compareFiles(FILE *fp1, FILE *fp2) 
{ 
    char * line1 = NULL;
    size_t len1 = 0;
    char * line2 = NULL;
    size_t len2 = 0;
    ssize_t read1;
    ssize_t read2;
    if (fp1 == NULL)
        exit(EXIT_FAILURE);
    if (fp2 == NULL)
        exit(EXIT_FAILURE);

    while ((read1 = getline(&line1, &len1, fp1)) != -1&&(read2 = getline(&line2, &len2, fp2)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
        char *trim1 =strtrim(line1);
       //printf("333 %s:%s\n", buffer,s);
       char *trim2 = strtrim(line2);
       // printf("333 %s:%s\n", buffer_trim,s_trim );
        if(strcmp(trim1,trim2) != 0)
            return 1;
         //printf("your result: %s",buffer);
         //printf("correct: %s",s);
      
        
    }

    while ((read1 = getline(&line1, &len1, fp1)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
            return 1;
         //printf("your result: %s",buffer);
         //printf("correct: %s",s);     
        
    }
    while ((read2 = getline(&line2, &len2, fp2)) != -1) {
        //printf("Retrieved line of length %zu:\n", read);
            return 1;
         //printf("your result: %s",buffer);
         //printf("correct: %s",s);     
        
    }
    free(line1);
    free(line2);
    return 0;
} 
int CheckResult(char* file1, char* file2) {
    FILE *fp1 = fopen(file1, "r"); 
    FILE *fp2 = fopen(file2, "r"); 
  
    if (fp1 == NULL || fp2 == NULL) 
    { 
       printf("Error : Files not open\n"); 
       return 255;
    }
   int status =  compareFiles(fp1, fp2);
    fclose(fp1);
    fclose(fp2);

    return status;

}
void segfault_handler(int signal) { // everyone knows what this is
  testStatus = 255;
  longjmp(test_crash, 1);
}

void exit_attempt_handler(void) { // only I decide when you are done
  if (disable_exit_handler) return; // allow this to be disabled
  testStatus = 255;
  longjmp(test_crash, 2);
}

void abrupt_termination_handler(int signal) { // program killed externally
  testStatus = 255;
  longjmp(test_crash, 3);
}
//at least I got rid of the goto
void end_program(uint8_t argc) {
  disable_exit_handler = 1;

  uint8_t totalScore = (MAXSCORE - NUM_TESTS * PNTSPERTEST) +
    testsPassed * PNTSPERTEST;
  if (testStatus == 255||totalScore==0) totalScore = 10;
  if (testStatus != 255 && argc == 2) {
    printf("\nYou passed %d out of %d tests\n", testsPassed, NUM_TESTS);
  } else if (argc == 2) {
    printf("\nRecieving charity points for a premature program end\n");
  }

  printf("\nYou will receive %d out of %d possible points on the Subset Unit Test\n\n",
      totalScore, MAXSCORE);

  exit(0);
}

int runTest(int test) {
        //int A[5]={0,0,0,0,0};
        //int A1[6]={0,0,0,0,0,0};
        //printSet(int B[], int n)
            if (test == setToString1_test) {

                int a[] = { 0, 0, 0, 0 };
                //int a[] = { 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
                int b[] = { 0, 1, 1, 1 };
                int c[] = { 0, 0, 1, 0 };
                int d[1]={0};
                //int e[2000];
                int f[] = { 0, 1 };

                if (!strcmpn(a, "{}\n\0",3) &&
                    !strcmpn(a, "{ }\n\0",3))
                    return 1;
               // printf("first\n");
                if (!strcmpn(b, "{1, 2, 3}\n\0",3))
                    return 2;
                if (!strcmpn(c, "{2}\n\0",3))
                    return 3;
                if (!strcmpn(d, "{}\n\0",1) && 
                    !strcmpn(d, "{ }\n\0",1))
                    return 4;
                if (!strcmpn(f, "{1}\n\0",1))
                    return 5;

                //e[1999] = 1;

            } else if (test == setToString2_test) {
                // not actually hard. there's just nothing to test...

                int a[] = { 0, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1 };
                int b[] = { 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0, 1, 0 };
                int c[] = { 0, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1, 1 };
                //printSet(a,18);
                if (!strcmpn(a, "{2, 4, 6, 8, 10, 12, 14, 16, 18}\n\0",18))
                    return 1;
                if (!strcmpn(b, "{1, 3, 5, 7, 9, 11, 13, 15, 17}\n\0",18))
                    return 2;
                if (!strcmpn(c, "{1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12, 13, 14, 15, 16, 17, 18}\n\0",18))
                    return 3;
                
            } else if (test == printSubsets1_test) {
                int test_status = -1;

                
                char *name = "unit-out1.txt";
                
                // fflush(stdout);
                // int stdout_fd = dup(STDOUT_FILENO);
                // int redir_fd = open(name, O_WRONLY);
                // dup2(redir_fd, STDOUT_FILENO);
                // close(redir_fd);
                // B[1] = 1;
                // printSubsets(B, 4,1, 2);
                // fflush(stdout);
                // dup2(stdout_fd, STDOUT_FILENO);
                // close(stdout_fd);
                int fd = open(name, O_RDWR | O_CREAT, 0666);//S_IRUSR | S_IWUSR
                int stdout_fd = dup(1);//STDOUT_FILENO)
                dup2(fd, 1);   // make stdout go to file
                //dup2(fd, 2);
                int A[5]={0,0,0,0,0};
                A[1] = 1;
                printSubsets(A, 4, 1, 2);
                fflush(stdout);
                dup2(stdout_fd, 1);
                close(fd);
                close(stdout_fd);
                clearerr(stdout);
                test_status = CheckResult("modelunit-out1.txt", "unit-out1.txt");
                //remove(name);
                //if(test_status==0)
                return test_status;

            } else if (test == printSubsets2_test) {
                int test_status = -1;
                clock_t t;
                t = clock();                
                char *name1 = "unit-out4.txt";
                int fd1= open(name1, O_RDWR | O_CREAT, 0666);//S_IRUSR | S_IWUSR
                int stdout_fd1 = dup(STDOUT_FILENO);
                dup2(fd1, 1);   // make stdout go to file
                //dup2(fd, 2);
                int A1[101]={0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0};

                printSubsets(A1, 30, 29, 3);

                fflush(stdout);
                dup2(stdout_fd1, STDOUT_FILENO);
                close(fd1);
                close(stdout_fd1);
                clearerr(stdout);
                t = clock() - t;
                double time_taken = ((double) t)/CLOCKS_PER_SEC;
                //printf("printSubsets() took %f Seconds to execute \n", time_taken);
                test_status = time_taken;
                //test_status = CheckResult("modelunit-out4.txt", "unit-out4.txt");
                // remove(name1);
                //if(test_status==0)
                return test_status;


            } else if (test == printSubsets3_test) {
                int test_status = -1;
                char *name2 = "unit-out5.txt";
   
                int fd2= open(name2, O_RDWR | O_CREAT,0666);//S_IRUSR | S_IWUSR
                int stdout_fd2 = dup(1);
                dup2(fd2, 1);   // make stdout go to file
                
                int A1[6]={0,0,0,0,0,0};
                A1[1] = 1;
                A1[3] = 1;               
                printSubsets(A1, 5, 1, 5);
                fflush(stdout);
                dup2(stdout_fd2, 1);
                close(fd2);
                close(stdout_fd2);
                clearerr(stdout);
                test_status = CheckResult("modelunit-out5.txt", "unit-out5.txt");
                //if(test_status==0)
                //remove(name2);
                return test_status;


                //return test_status;
            }
           
        return 0;
}

int main(int argc, char **argv) {

        if (argc < 2 || (argc == 2 && strcmp(argv[1], "-v") != 0)) {
            printf("Usage: %s [-v]", (argc > 0 ? argv[0] : "./ModelSubsetTest"));
            exit(1);
        }
       printf("\n"); // more spacing
        if (argc == 2) printf("\n"); // consistency in verbose mode

        test_count = 0;


        disable_exit_handler = 0;
        testsPassed = 0;
        atexit(exit_attempt_handler);
        signal(SIGSEGV, segfault_handler);

      
        for (uint8_t i = FIRST_TEST; i < NUM_TESTS; i++) {
            uint8_t fail_type = setjmp(test_crash);
            if (fail_type != 0) goto fail_jmp;
            testStatus = runTest(i);
            //printf("%s %s", testName(i), test_status == 0 ? "PASSED" : "FAILED");
            fail_jmp:
            if (argc == 2) { // it's verbose mode
                 printf("Test %s %s", testName(i),
            testStatus == 0 ? "PASSED" : "FAILED");
            if (testStatus == 255) {
                    printf(": due to a %s\n", fail_type == 1 ? "segfault" : fail_type == 2 ?
            "program exit" : "program interruption");
                    printf("\nWARNING: Program will now stop running tests\n\n");
                 end_program(argc);

            } else if (testStatus == 254) {
                printf(": undefined test\n");
            } else if (testStatus != 0) {
                printf(": test took %d Seconds to execute\n", testStatus);
            } else {
                 printf("\n");
            }
    }
    if (testStatus == 0) {
      testsPassed++;
    }
  }

  end_program(argc);
  return 0;
       
        
}
