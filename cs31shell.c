/*
* Swarthmore College, CS 31
* Copyright (c) 2021 Swarthmore College Computer Science Department,
* Swarthmore PA
*/

/*
Students: Nader Ahmed & Ibrahim Hassouna
Lab: Making a shell
Date: 11/23/2021
*/

#include <stdlib.h>

#include <string.h>

#include <stdio.h>

#include <unistd.h>

#include <ctype.h>

#include <sys/types.h>

#include <sys/wait.h>

#include "parsecmd.h"

/* The maximum size of your circular history queue. */
#define MAXHIST 10

/*
* A struct to keep information one command in the history of
* command executed
*/
struct histlist_t {
  unsigned int cmd_num;
  char cmdline[MAXLINE]; // command line for this process
};

static unsigned int commandID = 0;
static int start = 0;
static struct histlist_t queue[MAXHIST];
static int queue_next = 0; // the next place to insert in the queue
static int queue_size = 0; // the number of items in the queue

//functions prototypes
void run(char** args,int bg);
void add_history(struct histlist_t val);
void print_history(void);
void child_handler2(int sig);
void findAndExecute(int num);
void update(char * cmdline);
void freeArgs(char** args);

int main(int argc, char ** argv) {
  char cmdline[MAXLINE];
  char ** args;
  int bg = 0;

  // add a call to signal to register your SIGCHLD signal handler.
  signal(SIGCHLD, child_handler2);


  while (1) {
    // (1) print the shell prompt
    printf("cs31shell> ");
    fflush(stdout);

    // (2) read in the next command entered by the user
    if ((fgets(cmdline, MAXLINE, stdin) == NULL) && ferror(stdin)) {
      perror("fgets error");
    }
    if (feof(stdin)) {
      /* End of file (ctrl-D) */
          printf("\n");
      fflush(stdout);
      exit(0);
    }
    args = parse_cmd_dynamic(cmdline, & bg);
    if (args[0]==NULL){
      continue;
    }

    if (strcmp(args[0], "exit") == 0){
      return 0;
    }
    else if (strcmp(args[0], "history") == 0) { //history command
      update(& cmdline[0]);
      print_history();
    } else if (args[0][0] == '!') { //!num command
      int id = atoi(strstr(args[0], & args[0][1]));
      findAndExecute(id);
    } else {
      // if not a built in command then execute
      update(& cmdline[0]);
      run(args,bg);
    }
    freeArgs(args);
  }
  return 0;
}
//find command by ID, execute it and add it to history
void findAndExecute(int num) {
  int found=0;
  for (int i = 0; i < queue_size; i++) {
    if (queue[i].cmd_num == num) {
      // we found the ID
      found=1;
      int bg;
      char** args=parse_cmd_dynamic(queue[i].cmdline, & bg);
      update( & queue[i].cmdline[0]);
      if (strcmp(args[0], "history") == 0) { //history command
        print_history();
      }
      else{
        run(args, bg);
      }
      freeArgs(args);
    }
  }
  if (found==0){
    printf("Command not found \n");
  }
}
// create an history object using the cmdline data and add it to history
void update(char * cmdline) {
  struct histlist_t line;
  strcpy(line.cmdline, cmdline);
  line.cmd_num = commandID++;
  add_history(line);
}
// add an element to the hsitory queue
void add_history(struct histlist_t val) {
  queue[queue_next] = val;

  if (queue_size < MAXHIST) {
    queue_size++;
  }
  queue_next = (queue_next + 1) % MAXHIST;
  if (queue_size == MAXHIST) {
    start = queue_next;
  }
}
// print the history using  start and size
void print_history(void) {
  for (int i = start; i < queue_size + start; i++) {
    printf("%d %s \n", queue[i % MAXHIST].cmd_num, queue[i % MAXHIST].cmdline);
  }
}
// execute when child exits to reap it
void child_handler2(int sig) {
  int child_status=0;
  pid_t pid;
  while ((pid = waitpid(-1, & child_status, WNOHANG)) > 0) {
  }
}

// run a program by creating a child and using execvp function
void run(char** args,int bg){
  int status=0;
  int pid = fork(); // create new process
  if (pid == 0) {
    /* child */
    if (execvp(args[0], args) < 0) {
      printf("%s:Command not found.\n", args[0]);
      exit(0);
    }
  } else {
    if (bg==0){ // if in foreground wait for the child to exits
      waitpid(pid, & status, 0);
    }
  }
}
// free the dynamic memmory allocated in args
void freeArgs(char** args) {
  int index = 0;
  while (args[index] != NULL) {

    free(args[index]);
    index++;
  }
  free(args);
}
