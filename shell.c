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
void setPath(char *path) ;
void getPath();
void restorePath();
void startShell();
void setHome(); 
void commandCheck(char * tokens[]);
void changeDirectory(char * tokens[]);

int terminate = 0;
char* path;
const char* home;
extern char **environ;

int main(void){
	startShell();
	runShell();
	return 0;
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

	commandCheck(tokenarray);
	
}

void runShell(){
	char input[BUFFER_SIZE];
	while (terminate == 0){
		printf(">");
		memset(input,'\0',BUFFER_SIZE);
		if (fgets(input,BUFFER_SIZE,stdin)==NULL){
			printf("\n");
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

void getPath() {
    char *currpath = getenv("PATH");
    printf("PATH : %s\n", currpath);
    
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

void setHome() {
    char directory[512];
    path = getenv("PATH");
    home = getenv("HOME");
    
    
    if (chdir(home) == 0) {
        printf("HOME : %s\n", getcwd(directory, sizeof(directory)));
    } else {
        printf("chdir() error.\n");
    }

}

void startShell(){
	setHome();
	//setPath(path);
	//restorePath();
}

void commandCheck(char * tokens[]){
	
	if(strcmp(tokens[0],"getpath")==0){
		getPath();
	}

	else if(strcmp(tokens[0],"setpath")==0){
		setPath(tokens[1]);
	}

	else if(strcmp(tokens[0],"cd")==0){
		changeDirectory(tokens);
	}
	
	else{
		externalCommandexec(tokens);	
	}
}

void changeDirectory(char * tokens[]){
	if(tokens[1]==NULL){
		chdir(home);
	}else if(chdir(tokens[1])==0){
		char directory[512];
		printf("Directory : %s\n", getcwd(directory, sizeof(directory)));
	}
}

