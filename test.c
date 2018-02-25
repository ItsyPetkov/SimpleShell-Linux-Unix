#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>
#include <math.h>

#define BUFFER_SIZE 512

void parseInput(char *input);
void externalCommandexec(char * tokens[]);
void runShell();
void changeDirectory(char * tokens[]);
void setPath(char *path) ;
void getPath();
void restorePath();
void setHome(); 
void commandCheck(char * tokens[]);
void changeDirectory(char * tokens[]);
void createHistory(char * input);
void printHistory();
void executeHistory(int commandNum);

char* path;
const char* home;
int count = 0;
int historycount=0;

struct hist {
	int commandNumber;
	char string[BUFFER_SIZE];
};

struct hist history[20];

/* */
int main(void){
	setHome();
	runShell();
	return 0;
}

/*parseInput() is a function to parse the string into an array*/
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
    if (tokenarray[0] != NULL) {
        commandCheck(tokenarray);
    }
}

/*runShell() displays >*/
void runShell(){
	char input[BUFFER_SIZE];
	int terminate = 0;
	while (terminate == 0){
		printf(">");
		memset(input,'\0',BUFFER_SIZE);
		if (fgets(input,BUFFER_SIZE,stdin)==NULL){
			printf("\n");
			restorePath();
        		exit(0);
    		}

		if(strncmp(input,"history",7)!=0 && (input[0] != '\n')){
			createHistory(input);
		}

		if(input[0] != '\n') {
			parseInput(input);
		}
	}
	
}

/* */
void externalCommandexec(char * tokens[]){
	pid_t  pid;
 	pid = fork();
	if (pid < 0){
		errno=ECHILD;
		perror("ERROR");
	}
 	if (pid == 0){
		if(execvp(tokens[0],tokens)==-1){
			perror(tokens[0]);
			kill(getpid(),SIGTERM);
		}
	}else{
		wait(NULL);
		
	}
    		
}

/* */
void getPath() {
    char *currpath = getenv("PATH");
    printf("PATH : %s\n", currpath);
    
}

/* */
void setPath(char *path) {
    if (setenv("PATH", path, 1) == 0) {
        printf("PATH : %s\n", getenv("PATH"));
    } else {
        puts("setenv() error.");
    }
}

/* */
void restorePath() {
    setPath(path);
}

/* */
void setHome() {
    char directory[512];
    path = getenv("PATH");
    home = getenv("HOME");
    
    
    if (chdir(home) == 0) {
        printf("HOME : %s\n", getcwd(directory, sizeof(directory)));
    } else {
        printf("chdir() error.\n");	//fix
    }

}

/* */
void commandCheck(char * tokens[]){
	
	/*Calls the function getPath() if correct arguments are provided else displays error*/
	if(strcmp(tokens[0],"getpath")==0){
		if (tokens[1]==NULL){
			getPath();
		}else{
			errno=EINVAL;
			perror(tokens[0]);
		}
	}

	/*Calls the function setPath() if correct arguments are provided else displays error*/
	else if(strcmp(tokens[0],"setpath")==0){
		if (tokens[1]!=NULL && tokens[2]==NULL){
			setPath(tokens[1]);
		}else{
			errno=EINVAL;
			perror(tokens[0]);
		}
	}
	
	/*Calls the function changeDirectory() with the token array*/
	else if(strcmp(tokens[0],"cd")==0){
		changeDirectory(tokens);
	}

	/*Checks whether first command is exit and if there are appropriate parameter*/
	else if(strcmp(tokens[0],"exit")==0){
		if(tokens[1]==NULL){
			restorePath();
			exit(0);
		}else{
			errno=EINVAL;
			perror(tokens[0]);
		}

	}
    
    else if(tokens[0][0] == '!'){
        long size = strlen(tokens[0]);
        int sum = 0;
        
        for (int i = 1; i < size; i++) {
            
            int temp = tokens[0][i] - '0';
            int power = pow(10, size-i-1);
            sum += temp * power;
            temp = 0;
            power = 0;
        }
        executeHistory(sum);
    }

	else if(strcmp(tokens[0], "history") == 0){
		printHistory();
	}
		
	/* */
	else{
		externalCommandexec(tokens);	
	}
}

/* */
void changeDirectory(char *tokens[]){
	if(tokens[1]==NULL){
		chdir(home);
	}else if(chdir(tokens[1])==-1){
		perror(tokens[1]);
	}
}

void createHistory(char * input){
		strcpy(history[count].string, input);
		history[count].commandNumber = historycount+1;
        historycount++;
		count=(count+1)%20;
}

void printHistory(){
	for(int i = count; i < 20; i++){
        if (history[i].commandNumber != 0) {
            printf("%d %s", history[i].commandNumber, history[i].string);
        }
	}
    for (int i = 0;i < count; i++) {
        if (history[i].commandNumber != 0) {
            printf("%d %s", history[i].commandNumber, history[i].string);
        }
    }
}

void executeHistory(int commandNum){
    for(int i = 0; i < 20; i++){
        if(history[i].commandNumber == commandNum){
            parseInput(history[i].string);
        }
    }
}

