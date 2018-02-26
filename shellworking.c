#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

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
int getSum(char *tokens[], int a);
int getPow(int a, int b);
int isDigit(char *tokens[]);

char* path;
const char* home;
int count = 0;
int historycount=0;

struct hist {
	int commandNumber;
	char string[BUFFER_SIZE];
};

struct hist history[20];

/* main() calls the method setHome() and runShell() */
int main(void){
	setHome();
	runShell();
	return 0;
}

/* parseInput() is a function to parse the string into an array */
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

/* runShell() displays >, takes user input, calls createHistory() and parseInput() */
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

		if(strcspn(input,"!")!=0 && (input[0] != '\n')){
			createHistory(input);
		}

		if(input[0] != '\n') {
			parseInput(input);
		}
	}
	
}

/* externalCommandexec() is a function to execute external commands */
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
			kill(getpid(),SIGKILL);
		}
	}else{
		wait(NULL);
		
	}
    		
}

/* getPath() is a function to get the current path of the system */
void getPath() {
    char *currpath = getenv("PATH");
    printf("PATH : %s\n", currpath);
    
}

/* setPath() takes a char pointer as a parameter and sets the path to whatever is in the parameter */
void setPath(char *path) {
    if (setenv("PATH", path, 1) == 0) {
        printf("PATH : %s\n", getenv("PATH"));
    } else {
	perror(path);
    }
}

/* restorePath() restores the path to the original system path */
void restorePath() {
    setPath(path);
}

/* setHome() sets the current working directory to HOME */
void setHome() {
    char directory[512];
    path = getenv("PATH");
    home = getenv("HOME");
    
    
    if (chdir(home) == 0) {
        printf("HOME : %s\n", getcwd(directory, sizeof(directory)));
    } else {
        perror(home);
	printf("Could not change working directory to home.");
    }

}

/* commandCheck() is a function to check what command the user has input */
void commandCheck(char * tokens[]){
	
	/* Calls the function getPath() if correct arguments are provided else displays error */
	if(strcmp(tokens[0],"getpath")==0){
		if (tokens[1]==NULL){
			getPath();
		}else{
			errno=EINVAL;
			perror(tokens[0]);
			printf("Please use getpath without parameters. Use: getpath.\n");
		}
	}

	/* Calls the function setPath() if correct arguments are provided else displays error */
	else if(strcmp(tokens[0],"setpath")==0){
		if (tokens[1]!=NULL && tokens[2]==NULL){
			setPath(tokens[1]);
		}else{
			errno=EINVAL;
			perror(tokens[0]);
			printf("Please use setpath with only one parameter. Use: setpath <path>.\n");
		}
	}
	
	/* Calls the function changeDirectory() with the token array */
	else if(strcmp(tokens[0],"cd")==0){
		changeDirectory(tokens);
	}

	/* Checks whether first command is exit and if there are appropriate parameter */
	else if(strcmp(tokens[0],"exit")==0){
		if(tokens[1]==NULL){
			restorePath();
			exit(0);
		}else{
			errno=EINVAL;
			perror(tokens[0]);
			printf("Please use exit without any parameters. Use: exit.\n");
		}

	}
    
	/* Checks whether the first token is ! and if there are appropriate parameters */
    	else if(strcspn(tokens[0],"!")==0){
		
		/* checks if the second token is - */
		if(tokens[0][1]=='-'){
			if(isDigit(tokens)==1){
				int sum = getSum(tokens,2);
				executeHistory(historycount-sum);
			}else{
				errno=EINVAL;
				perror(tokens[0]);
				printf("Please use !<no> or !-<no> or !!.\n");
			}
		}

		/* checks if the second token is ! */
		else if(tokens[0][1]=='!'){
			executeHistory(historycount);
		}

		/* checks if the second token is NULL */
		else if (tokens[0][1]=='\0'){
			errno=EINVAL;
				perror(tokens[0]);
				printf("Please use !<no> or !-<no> or !!.\n");
		}

		else{
			if(isDigit(tokens)==1){
				int sum = getSum(tokens,1);
				executeHistory(sum);
			}else{
				errno=EINVAL;
				perror(tokens[0]);
				printf("Please use !<no> or !-<no> or !!.\n");
			}
		}

    	}

	/* Checks whether first command is history and if there are appropriate parameter */
	else if(strcmp(tokens[0], "history") == 0){
		if(tokens[1]==NULL){
			printHistory();
		}else{
			errno=EINVAL;
			perror(tokens[0]);
			printf("Please use history without any parameters. Use: history.\n");
		}
		
	}
		
	/* If the command fails to be recognised as an inbuilt command externalCommandexec() is called with the command */
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

/* */
void createHistory(char * input){
		strcpy(history[count].string, input);
		history[count].commandNumber = historycount+1;
       		historycount++;
		count=(count+1)%20;
}

/* */
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

/* */
void executeHistory(int commandNum){
    for(int i = 0; i < 20; i++){
        if(history[i].commandNumber == commandNum){
            parseInput(history[i].string);
        }
    }
}

/* */
int getSum(char *tokens[], int a){
	int sum=0;
	for (int i = a; i < strlen(tokens[0]); i++) {
		int temp = tokens[0][i] - '0';
		int power = getPow(10, strlen(tokens[0])-i-1);
		sum += temp * power;
		temp = 0;
		power = 0;
        }
	return sum;
}

/* */
int getPow(int a, int b){
	int finalNum = 1;
	for(int i=0;i<b;i++){
		finalNum = finalNum * a;
	}
	return finalNum;
}

/* */
int isDigit(char * tokens[]){
	int isdigit=0;
	for(int i=1;i<strlen(tokens[0]);i++){
		if(tokens[0][i]>='0' && tokens[0][i]<='9'){
			isdigit=1;
		}else{
			isdigit=0;
		}
	}
	return isdigit;
}
