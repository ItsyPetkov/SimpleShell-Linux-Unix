#define _GNU_SOURCE
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <sys/wait.h>

#define BUFFER_SIZE 512

void exitCheck(char *input);
void parseInput(char *input);
void externalCommandexec(char * tokens[]);
void runShell();
void changeDirectory(char * tokens[]);
void setdefaultPath();
void getdefaultPath();
void startShell();

int terminate = 0;
char* PATH;
extern char **environ;

int main(void){
	startShell();
	runShell();
	return 0;
}

void exitCheck(char *input){
	if(strncmp(input, "exit", 4)==0){
		setdefaultPath();
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

void runShell(){
	char input[BUFFER_SIZE];
	while (terminate == 0){
		printf(">");
		memset(input,'\0',BUFFER_SIZE);
		if (fgets(input,BUFFER_SIZE,stdin)==NULL){
			printf("\n");
			setdefaultPath();
        		exit(0);
    		}
		exitCheck(input);
		if(input[0] != '\n'){
			parseInput(input);
		}
	}
	
}

void externalCommandexec(char * tokens[]){
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

void setdefaultPath(){
	setenv("PATH",PATH,1);
	chdir(PATH);
}

void getdefaultPath(){
	PATH=getenv("PATH");
}

void startShell(){
	getdefaultPath();
	chdir(getenv("HOME"));
}

void changeDirectory(char * tokens[]){

	
}




