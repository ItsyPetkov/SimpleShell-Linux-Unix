#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

//Test 
void displayPrompt();
void readInput();
void exitCheck(char *input);
void parseInput(char *input);
void externalCommandexec(char *input);
void runShell(char *input);

int terminate = 0;

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
	char input[512];
    	if (fgets(input,512,stdin)==NULL){
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
	token=strtok(input," |><&\t;");
	printf("Tokens\n");
	while(token != NULL){
		printf("'%s' " ,token);
		token = strtok(NULL," |><&\t;");
	}
}

void runShell(char *input){
	exitCheck(input);
	if(input[0] != '\n'){
		parseInput(input);
	}
	//externalCommandexec(input);

}

void externalCommandexec(char *input){
	char * args[3];	
	args[0] = "/bin/sh";
	args[2] = NULL;

	pid_t instruction;
	instruction = fork();

	if(instruction < 0){
		fprintf(stderr, "Invalid Program! Fork Failed!");
		exit(-1);
	}
	else if(instruction == 0){
		execve(args[0], args, NULL);
	}
	else{
		wait(NULL);
		exit(0);
	}
}
