//ModelDictionaryTest.c
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

#define bool _Bool
//#define main main_ignore
#include "Dictionary.h"
//#undef main

static uint8_t testsPassed;
static volatile sig_atomic_t testStatus;
static uint8_t disable_exit_handler;
jmp_buf test_crash;
int test_count;

    // static int test_count;

    // static bool verbose;
#define FIRST_TEST isEmpty_test1
#define MAXSCORE 80
#define PNTSPERTEST 10
#define BREAKER_TEST_RANGE 1000
const char origtempstr[20*BREAKER_TEST_RANGE];
enum Test_e {
     
    isEmpty_test1 = 0,
    size_test1,
    size_test2,
    lookup_test1,
    lookup_test2,
    lookup_test3,
    toString_test1,
    exception_test1,

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

	if (test == isEmpty_test1) return "isEmpty_test1";
    if (test == size_test1) return "size_test1";
    if (test == size_test2) return "size_test2";
    if (test == lookup_test1) return "lookup_test1";
    if (test == lookup_test2) return "lookup_test2";
    if (test == lookup_test3) return "lookup_test3";
    if (test == toString_test1) return "toString_test1";
    if (test == exception_test1) return "exception_test1";

    return "";
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
  if (testStatus == 255||totalScore==0) totalScore = 5;
  if (testStatus != 255 && argc == 2) {
    printf("\nYou passed %d out of %d tests\n", testsPassed, NUM_TESTS);
  } else if (argc == 2) {
    printf("\nRecieving charity points for a premature program end\n");
  }

  printf("\nYou will receive %d out of %d possible points on the Hashtable Dictionary\n\n",
      totalScore, MAXSCORE);

  exit(0);
}

// char* newlookup(Dictionary D, char* k) {
// 	if (!D) {
// 		printErr(NULL_REF_ERR, "lookup()"); 
// 		exit(EXIT_FAILURE); 
// 	}
	
// 	Node N; 
// 	for (N=D->head; N!=NULL; N=N->next) {
// 		if (strcmp(N->key, k) == 0) { return N->value; }
// 	}
// 	// associated k-v pair not found 
// 	return NULL;
// }

int runTest(int test) {

    Dictionary A = newDictionary();
    Dictionary B = newDictionary();
   // final int BREAKER_TEST_RANGE = 1000;

   
      if(test == isEmpty_test1) {
        if (size(A)!=0 || size(B)!=0) return 1;
        insert(A, "test", "123");
        if (size(A)==0 || size(B)!=0) return 2;
      } else if(test == size_test1) {
        if (size(A) != size(B)) return 1;
        insert(A, "test", "123");
        if (size(A) == size(B)) return 2;
        insert(B,"test", "123");
        if (size(A) != size(B) ) return 3;
      } else if(test == size_test2) {
        for (int i = 0; i < BREAKER_TEST_RANGE; i++) {
        	char *curstr = calloc(1000,sizeof(char));
        	char *curstr2 = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
        	sprintf(curstr2, "%s%d","test" , i + 1);
        	//printf("%s",curstr);
          insert(A,curstr, curstr2);
        }
        for (int i = 0; i < BREAKER_TEST_RANGE / 10; i++) {
        	char *curstr = calloc(1000,sizeof(char));
        	char *curstr2 = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
        	sprintf(curstr2, "%s%d","test" , i + 1);
          insert(B,curstr, curstr2);
        }
        if (size(A)  != BREAKER_TEST_RANGE || size(B)  != BREAKER_TEST_RANGE / 10) return 1;
        for (int i = BREAKER_TEST_RANGE / 10; i < BREAKER_TEST_RANGE; i++) {
        	//char *curstr = (i + 1 + "");
        	char *curstr = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
        	char *curstr2 = calloc(1000,sizeof(char));
        	sprintf(curstr2, "%s%d","test" , i + 1);
          insert(B,curstr, curstr2);
        }
        if (size(A)  != size(B) ) return 2;
        for (int i = BREAKER_TEST_RANGE * 9 / 10; i < BREAKER_TEST_RANGE; i++) {
        	//char *curstr = (i + 1 + "");
        	char *curstr = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
          delete(A,curstr);
        }
        if (size(A)  != BREAKER_TEST_RANGE * 9 / 10 || size(B)  != BREAKER_TEST_RANGE) return 3;
        for (int i = BREAKER_TEST_RANGE * 9 / 10 - 1; i >= 0; i--) {
        	char *curstr = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
          delete(A,curstr);
        }
        makeEmpty(B);
        if (size(A)  != 0 || size(B)  != 0) return 4;
      } else if(test == lookup_test1) {
        // should be NULL
        if (lookup(A,"test")!=lookup(B,"test")) return 1;
        	insert(A,"test", "123");
        if (strcmp(lookup(A,"test"),"123")!=0 || lookup(B,"test") != NULL) return 2;
        	insert(B,"test", "123");
        if (strcmp(lookup(A,"test"),(lookup(B,"test")))!=0) return 3;
      } else if(test == lookup_test2) {
        for (int i = 0; i < BREAKER_TEST_RANGE; i++) {
        	char *curstr = calloc(1000,sizeof(char));
        	char *curstr2 = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
        	sprintf(curstr2, "%s%d","test" , i + 1);
            insert(A,curstr, curstr2);
          if (strcmp(lookup(A,curstr),curstr2)!=0) return 1;
        }
        for (int i = 0; i < BREAKER_TEST_RANGE / 10; i++) {
        	char *curstr = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
        	char *cursvalue = calloc(1000,sizeof(char));
        	sprintf(cursvalue, "%s%d","test" , i + 1);
          insert(B,curstr,cursvalue);
          if (strcmp(lookup(B,curstr),(cursvalue))!=0) return 2;
        }
        for (int i = BREAKER_TEST_RANGE / 10; i < BREAKER_TEST_RANGE; i++) {
            char *curstr = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
        	char *cursvalue = calloc(1000,sizeof(char));
        	sprintf(cursvalue, "%s%d","test" , i + 1);
          if (lookup(B,curstr) != NULL) return 3;
          insert(B,curstr, cursvalue);
          if (strcmp(lookup(B,curstr),cursvalue)!=0) return 4;
        }
        for (int i = BREAKER_TEST_RANGE * 9 / 10; i < BREAKER_TEST_RANGE; i++) {
        	char *curstr = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
          delete(A,curstr);
          if (lookup(A,curstr) != NULL) return 5;
        }
        for (int i = BREAKER_TEST_RANGE * 9 / 10 - 1; i >= 0; i--) {
        	char *curstr = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
          delete(A,curstr);
          if (lookup(A,curstr) != NULL) return 6;
        }
        makeEmpty(B);
        for (int i = 0; i < BREAKER_TEST_RANGE; i++) {
        	char *curstr = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
          if (lookup(A,curstr) != NULL) return 7;
        }
      } else if(test == lookup_test3) {
       insert(A,"repeat", "123");
        if (strcmp(lookup(A,"repeat"),"123")!=0) return 1;
        delete(A,"repeat");
        if (lookup(A,"repeat") != NULL) return 2;
       insert(A,"repeat", "123");
        if (strcmp(lookup(A,"repeat"),"123")!=0) return 3;
        for (int i = 0; i < BREAKER_TEST_RANGE; i++) {
        	char *curstr = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
        	char *cursvalue = calloc(1000,sizeof(char));
        	sprintf(cursvalue, "%s%d","test" , i + 1);
         	insert(A,curstr, cursvalue);
        }
        	char *cursvalue1 = calloc(1000,sizeof(char));
        	sprintf(cursvalue1, "%s%d","" , (BREAKER_TEST_RANGE / 2));
        	delete(A, cursvalue1 );
        if (lookup(A,cursvalue1) != NULL) return 4;
        	char *cursvalue2 = calloc(1000,sizeof(char));
        	sprintf(cursvalue2, "%s%d","" , (BREAKER_TEST_RANGE / 2+1));

        	char *cursvalue3 = calloc(1000,sizeof(char));
        	sprintf(cursvalue3, "%s%d","test" , (BREAKER_TEST_RANGE / 2+1));
        if (strcmp(lookup(A,cursvalue2),
              cursvalue3)!=0) return 5;

        char *cursvalue4 = calloc(1000,sizeof(char));
        sprintf(cursvalue4, "%s%d","" , (BREAKER_TEST_RANGE / 2-1));

        char *cursvalue5 = calloc(1000,sizeof(char));
        sprintf(cursvalue5, "%s%d","test" , (BREAKER_TEST_RANGE / 2-1));
        if (strcmp(lookup(A,cursvalue4),cursvalue5)!=0) return 6;

        char *cursvalue6 = calloc(1000,sizeof(char));
        sprintf(cursvalue6, "%s%d","" , (BREAKER_TEST_RANGE));
        delete(A,cursvalue6);

        char *cursvalue7 = calloc(1000,sizeof(char));
        sprintf(cursvalue7, "%s%d","" , (BREAKER_TEST_RANGE-1));
		char *cursvalue8 = calloc(1000,sizeof(char));
        sprintf(cursvalue8, "%s%d","test" , (BREAKER_TEST_RANGE-1));
        if (strcmp(lookup(A,cursvalue7),cursvalue8)!=0) return 7;
      } else if(test == toString_test1) {
        if (strcmp(DictionaryToString(A),DictionaryToString(B))!=0) return 1;
       		insert(A,"1", "one");
       		insert(B,"2", "two");
       		insert(A,"2", "two");
        	insert(B,"1", "one");
        if (strcmp(DictionaryToString(A),DictionaryToString(B))==0) return 2;
        	delete(B,"2");
        	insert(B,"2", "two");
        if (strcmp(DictionaryToString(A),DictionaryToString(B))!=0) return 1;
      } else if(test == exception_test1) {
        bool failed = false;
        
        
       	
      
        atexit(exit_attempt_handler);
        signal(SIGSEGV, segfault_handler);
        
      	uint8_t fail_type = setjmp(test_crash);
            

      	if (fail_type != 0) goto fail_jmp;
        if (fail_type == 0) goto fail_jmp1;
        insert(A,"test", "123");
       	insert(A,"test", "123");
      
        //printf("%d\n",size(A));
       	//if(value==0)
       	fail_jmp:
       	failed = true;
       	//printf("111111\n");
        if (failed) return 2;
       	fail_jmp1:
       	failed = false;   	
       
       //printf("2222\n");
        //printf("%d\n",size(A));
        failed = true;
        for (int i = 0; i < BREAKER_TEST_RANGE; i++) {
        	char *curstr = calloc(1000,sizeof(char));
        	sprintf(curstr, "%d%s", i+1, "");
        	char *cursvalue = calloc(1000,sizeof(char));
        	sprintf(cursvalue, "%s%d","test" , i + 1);
         insert(A,curstr,cursvalue);
        }
        //printf("ssss\n");
        // for (int i = BREAKER_TEST_RANGE - 1; i >= 0; i--) {
         char *curstr = calloc(1000,sizeof(char));
         sprintf(curstr, "%d%s", 2+1, "");
        uint8_t fail_type1 = setjmp(test_crash);
		if (fail_type1 != 0) goto fail_jmp3;
		if (fail_type1 == 0) goto fail_jmp4;
          delete(A,curstr);
         
          delete(A,curstr);
          
        fail_jmp3:
          failed = true;
          if (failed) return 3;
        fail_jmp4:
        	failed = false;  
		  return 0;        //}
      }
    
    return 0;
  }
  int main(int argc, char* argv[]) {
  	test_count = 0;  
        //int test_status = 0;
        disable_exit_handler = 0;
        testsPassed = 0;
        atexit(exit_attempt_handler);
        signal(SIGSEGV, segfault_handler);
		printf("\nList of tests passed/failed:\n");
        for (uint8_t i = FIRST_TEST; i < NUM_TESTS; i++) {

            uint8_t fail_type = setjmp(test_crash);
            if (fail_type != 0) goto fail_jmp;
            testStatus = runTest(i);
            fail_jmp:
            printf("Test %s %s", testName(i),testStatus == 0 ? "PASSED" : "FAILED");
                if (testStatus == 255) {
                    printf(": due to a %s\n", fail_type == 1 ? "segfault" : fail_type == 2 ?"program exit" : "program interruption");
                    printf("\nWARNING: Program will now stop running tests\n\n");
                    end_program(argc);

                } else if (testStatus == 254) {
                    printf(": undefined test\n");
                } else if (testStatus != 0) {
                    printf(": test %d\n", testStatus);
                } else {
                    printf("\n");
                }

            if (testStatus == 0) {
                testsPassed++;
            }

        }
        printf("passed %d\n",testsPassed);
        end_program(argc);
        return 0;



  }