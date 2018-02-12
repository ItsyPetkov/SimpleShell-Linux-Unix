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
void setHome();
void getPath();
void setPath();
void restorePath();

int terminate = 0;
extern char **environ;
char *path;

int main(void){
    setHome();
    char *path = "USERS:HOME:DOCUMENTS";
    setPath(path);
    restorePath();
	displayPrompt();
    return 0;
    
}

void setHome() {
    char directory[512];
    path = getenv("PATH");
    const char *home = getenv("HOME");
    
    
    if (chdir(home) == 0) {
        printf("HOME : %s\n", getcwd(directory, sizeof(directory)));
    } else {
        printf("chdir() error.\n");
    }

}

void displayPrompt(){
	while (terminate == 0){
		printf(">");
		readInput();
	}		
}

void getPath() {
    const char *path = getenv("PATH");
    printf("PATH : %s\n", path);
    
}

void setPath(char *path) {
    if (setenv("PATH", path, 1) == 0) {
        printf("PATH : %s\n", getenv("PATH"));
    } else {
        puts("setenv() error.");
    }
}

void restorePath() {
    setPath(path);
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


