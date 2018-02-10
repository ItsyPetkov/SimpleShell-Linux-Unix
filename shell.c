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
void externalCommandexec(char * tokens[50]);
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
	int position=0;
	char *token;
	char * tokenarray[50];
	token=strtok(input," |><&\t\n;");
	while(token != NULL){
		tokenarray[position]=token;
		token = strtok(NULL," |><&\t\n;");
		position++;
	}
	for(int i=position;i<50;i++){
		tokenarray[i]=NULL;
	}
	externalCommandexec(tokenarray);	
}

void runShell(char *input){
	exitCheck(input);
	if(input[0] != '\n'){
		parseInput(input);
	}
}

void externalCommandexec(char * tokens[50]){
	pid_t  pid;
 	pid = fork();
	
	if (pid < 0){
		printf("ERROR!! Child Process could not be created\n");
	}
 	if (pid == 0){
		if(execvp(tokens[0],tokens)==-1){
			printf("bash: %s : command not found\n", tokens[0]);
			kill(getpid(),SIGTERM);
		}
	}else{
		wait(NULL);
	}
    		
}


