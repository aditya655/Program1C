#include <stdlib.h>     // for exit
#include <stdio.h>      // for perror
#include <unistd.h>     // for fork, pipe
#include <sys/wait.h>   // for wait
#include <iostream>     // for cerr, cout
using namespace std;

int main( void ) {
   enum {RD, WR}; // pipe fd index RD=0, WR=1
   int n, fd[2];
   pid_t pid;

   if( pipe(fd) < 0 ) // 1: pipe created
      perror("pipe error");
   else if ((pid = fork()) < 0) // 2: child forked
      perror("fork error");
   else if (pid == 0) {
      // ---------- CHILD SECTION ----------
      close(fd[WR]);// 4: child's fd[1] closed
      dup2(fd[RD], 0); // stdin(0) --> childs pipe read 

      char buf[256];
      n = read(fd[RD], buf, 256); // use this raw read!
      // cin >> buf; <-- *caution with cin and white space!
      cout << buf;      // write to stdout
      cout << "Child Done!" << endl;
   }
   else {
      // ---------- PARENT SECTION -----------
      close(fd[RD]); // 3: close parent's read end of pipe
      dup2(fd[WR], 2); // stderr(2) --> parent's pipe write 

      cerr <<  "Hello my child" << endl;
      wait( NULL );
      cout << "Parent Done!" << endl;
   }
   exit(EXIT_SUCCESS);
}