#include <stdlib.h>     // for exit
#include <stdio.h>      // for perror
#include <unistd.h>     // for fork, pipe
#include <sys/wait.h>   // for wait
#include <iostream>     // for cerr, cout
#define MAXSIZE 4096 
using namespace std;

int main( void ) {
   enum {RD, WR}; // pipe fd index RD=0, WR=1
   int n, fd[2];
   pid_t pid;

   if( pipe(fd) < 0 ) // 1: pipe created
      perror("pipe error");
   else {
      switch(fork()) {
         case -1:
            perror("fork error");
         case 0:
         {
            close(fd[RD]);// 4: child's fd[0] closed
            dup2(fd[WR], 1); // stdout --> childs pipe write 
            execlp("/bin/ls","ls","-l", NULL);
            // Never returns here!!
         }

         default:
         {
            close(fd[WR]); // 3: close parent's write end of pipe
            dup2(fd[RD], 0); // stdin --> parent's pipe read 
            wait( NULL );

            char buf[MAXSIZE];
            n = read(fd[RD], buf, MAXSIZE); // use this raw read!
            buf[n] = '\0'; // make sure cstring is terminated
            cout << buf;   // write to stdout
            cout << "Parent Done!" << endl;
         }
      }
   }
   exit(EXIT_SUCCESS);
}