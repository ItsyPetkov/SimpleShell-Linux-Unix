#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 512

void displayPrompt();
void readInput();
void exitCheck(char *input);
void parseInput(char *input);
void externalCommandexec(char *input);
void runShell(char *input);

int terminate = 0;
extern char **environ;

int main(void){
	displayPrompt();
	return 0;
}

void displayPrompt(){
	while (terminate == 0){
		printf(">");
		readInput();
	}		
}

void readInput(){
	char input[BUFFER_SIZE];
    	if (fgets(input,BUFFER_SIZE,stdin)==NULL){
        	exit(0);
    	}
	runShell(input);
}

void exitCheck(char *input){
	if(strncmp(input, "exit", 4)==0){
		exit(0);
	}
}

void parseInput(char *input){
	char *token;
	token=strtok(input," |><&\t\n;");
	printf("Tokens\n");
	while(token != NULL){
		printf("'%s' " ,token);
		token = strtok(NULL," |><&\t\n;");
	}
	printf("\n");
}

void runShell(char *input){
	exitCheck(input);
	if(input[0] != '\n'){
		parseInput(input);
	}
	externalCommandexec(input);

}

void externalCommandexec(char *input){
	char * args[2];	
	pid_t  pid;
 	pid = fork();
	
	args[0]="ls";
	args[1]=NULL;
 	if (pid == 0){
		execvp(args[0],args);
	}else{
		wait(NULL);
		exit(0);
	}
    		

}