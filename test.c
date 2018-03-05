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
int getPow(int a, int b);
int isDigit(char *tokens[]);
void saveHistory();
void loadHistory();
void addtoHistory(char * input);
int getNum(char *token, int a);
int commandExists(int commandno);
void errorMessage(char * token, int eno);
void startShell();
void endShell();
void addAlias(char *tokens[]);
void printAliases();

char* path;
const char* home;
int count = 0;
int historycount = 0;
int aliasCount = 0;

struct hist {
	int commandNumber;
	char string[BUFFER_SIZE];
};

struct alias {
	char * aliasName;
	char * aliasCommand;
};

struct hist history[20];
struct alias aliases[20];

/* main() calls the method setHome() and runShell() */
int main(void){
	startShell();
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
			endShell();
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
			errorMessage(tokens[0],5);
		}
	}

	/* Calls the function setPath() if correct arguments are provided else displays error */
	else if(strcmp(tokens[0],"setpath")==0){
		if (tokens[1]!=NULL && tokens[2]==NULL){
			setPath(tokens[1]);
		}else{
			errorMessage(tokens[0],6);
		}
	}
	
	/* Calls the function changeDirectory() with the token array */
	else if(strcmp(tokens[0],"cd")==0){
		if(tokens[2]==NULL){
			changeDirectory(tokens);
		}else{
			errorMessage(tokens[0],7);
		}	
	}

	/* Checks whether first command is exit and if there are appropriate parameter */
	else if(strcmp(tokens[0],"exit")==0){
		if(tokens[1]==NULL){
			endShell();
		}else{
			errorMessage(tokens[0],8);
		}

	}
    
	/* Checks whether the first token is ! and if there are appropriate parameters */
    	else if(strcspn(tokens[0],"!")==0){
		
		/* checks if the second token is - */
		if(tokens[0][1]=='-'){
			if(isDigit(tokens)==1){
				if(tokens[1]==NULL){
					int sum = getNum(tokens[0],2);
					if(commandExists(historycount-sum)==1){
						executeHistory(historycount-sum);
					}else{
						errorMessage(tokens[0],2);
					}
				}else{
					errorMessage(tokens[0],2);
				}
			}else{
				errorMessage(tokens[0],2);
			}
		}

		/* checks if the second token is ! */
		else if(tokens[0][1]=='!'){
			if(tokens[1]==NULL){
				if(historycount>0){
					executeHistory(historycount);
				}else{
					errorMessage(tokens[0],4);
				}
			}else{
				errorMessage(tokens[0],3);
			}
				
		}

		/* checks if the second token is NULL */
		else if (tokens[0][1]=='\0'){
			errorMessage(tokens[0],1);
		}

		else{
			if(isDigit(tokens)==1){
				if(tokens[1]==NULL){
					int sum = getNum(tokens[0],1);
					if(commandExists(sum)==1){
						executeHistory(sum);
					}else{
						errorMessage(tokens[0],1);
					}
				}else{
					errorMessage(tokens[0],1);
				}
			}else{
				errorMessage(tokens[0],1);
			}
		}	

    	}

	/* Checks whether first command is history and if there are appropriate parameter */
	else if(strcmp(tokens[0], "history") == 0){
		if(tokens[1]==NULL){
			printHistory();
		}else{
			errorMessage(tokens[0],9);
		}
		
	}

	/* */
	else if(strcmp(tokens[0], "alias") == 0){
		//create alias
		if(tokens[1] == NULL || tokens[2] == NULL || tokens[3] != NULL){
			errorMessage(tokens[0], 10);
		}
		else{
			addAlias(tokens);
		}
	}

	else if(strcmp(tokens[0], "printalias") == 0){
		printAliases();
	}
		
	/* If the command fails to be recognised as an inbuilt command externalCommandexec() is called with the command */
	else{
		externalCommandexec(tokens);	
	}
}

/* changeDirectory() changes the directory according to the user input */
void changeDirectory(char *tokens[]){
	if(tokens[1]==NULL){
		chdir(home);
	}else if(chdir(tokens[1])==-1){
		perror(tokens[1]);
	}
}

/* createHistory() takes an input and stores it in the array of structs */
void createHistory(char * input){
		strcpy(history[count].string, input);
		history[count].commandNumber = historycount+1;
       		historycount++;
		count=(count+1)%20;
}

/* printHistory() is function which contents of the array of structs */
void printHistory(){
	int small=history[0].commandNumber;
	for(int i=0;i<20;i++){
		if(history[i].commandNumber<small){
			small=history[i].commandNumber;
		}
	}
	for(int i = small-1; i < 20; i++){
		if (history[i].commandNumber != 0) {
			printf("%d %s", history[i].commandNumber, history[i].string);
			if(strchr(history[i].string, '\n') == NULL){
				printf("\n");	
			}
        	}
	}
	for (int i = 0;i < small-1; i++) {
        	if (history[i].commandNumber != 0) {
            		printf("%d %s", history[i].commandNumber, history[i].string);
			if(strchr(history[i].string, '\n') == NULL){
				printf("\n");	
			}
        	}
    	}
}

/* executeHistory() is a function that takes a commandNum and executes the commandNum from history*/
void executeHistory(int commandNum){
    for(int i = 0; i < 20; i++){
        if(history[i].commandNumber == commandNum){
            parseInput(history[i].string);
        }
    }
}

/* getNum() is a function to get extract a number from a char pointer from a given index */
int getNum(char *token, int a){
	int sum=0;
	for (int i = a; i < strlen(token); i++) {
		int temp = token[i] - '0';
		int power = getPow(10, strlen(token)-i-1);
		sum += temp * power;
		temp = 0;
		power = 0;
        }
	return sum;
}

/* getPow() is a function which takes two parameters int a and int b and returns a^b */
int getPow(int a, int b){
	int finalNum = 1;
	for(int i=0;i<b;i++){
		finalNum = finalNum * a;
	}
	return finalNum;
}

/* isDigit() checks whether the given char array is an array and returns 1 if is a digit and 0 if is not a digit */
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

/* addtoHistory() is a function that takes a char pointer and adds it to history */
void addtoHistory(char * input){
	if(input!=NULL){
		char * commandnum = strtok(input," ");
		int cn = getNum(commandnum,0);
		char * str = strtok(NULL," ");
		strcpy(history[count].string, str);
		history[count].commandNumber = cn;
		historycount=cn;
		count=(count+1)%20;
	}
	
}

/* saveHistory() is a function to save the history to a .hist_list file */
void saveHistory(){
	chdir(home);
	FILE *file = fopen(".hist_list","w");
	if(file==NULL){
		return;
	}
	for(int i = count; i<20; i++){
		if(history[i].string!=NULL){
			fprintf(file, "%s",history[i].string);
		}
	}
	for(int i = 0; i < count; i++){
		if(history[i].string!=NULL){
			fprintf(file, "%s",history[i].string);
		}
	}
	printf("Saving File\n");
	fclose(file);
}

/* loadHistory() is a function to load the history from the .hist_list file */
void loadHistory(){
	FILE *file ;
	char string[BUFFER_SIZE];
	if((file = fopen(".hist_list", "r")) == NULL){
		printf("File not present. Creating new file.\n");
	}
	else{
		while(1){
			if(fgets(string, BUFFER_SIZE , file)==NULL){
				break;
			}
			createHistory(string);	
		}
		fclose(file);
	}
}

/* commandExists() is a function which checks whether a given number is a valid command in history*/
int commandExists(int commandno){
	int ret=0;
	for(int i=0;i<20;i++){
		if(history[i].commandNumber==commandno){
			ret=1;
		}
	}
	return ret;
}

/* histerror() is a function that takes in an error number and displays the appopriate error message for errors relating to commands starting with !*/
void errorMessage(char * token, int eno){
	errno=EINVAL;
	perror(token);
	switch(eno){
		case 1:printf("Please use !<no> with a valid <no> and no parameters. Use: !<no>\n");
		break;
		case 2:printf("Please use !-<no> with a valid <no> and no parameters. Use: !-<no>\n");
		break;
		case 3:printf("Please use !! without any parameters. Use: !!\n");
		break;
		case 4:printf("No commands entered yet, Please enter atleast one command to use !!.\n");
		break;
		case 5:printf("Please use getpath without parameters. Use: getpath.\n");
		break;
		case 6:printf("Please use setpath with only one parameter. Use: setpath <path>.\n");
		break;
		case 7:printf("Please use cd with one parameter or no parameters. Use: cd or cd <directory>.\n");
		break;
		case 8:printf("Please use exit without any parameters. Use: exit.\n");
		break;
		case 9:printf("Please use history without any parameters. Use: history.\n");
		break;
		case 10:printf("Please use alias with two parameters. Use: alias <name> <command>.\n");
		break;
		case 11:printf("Only 20 aliases allowed.Please unalias a command to alias a new command\n");
		break;
		default: printf("Invalid error number.\n");
		break;
	}
}

/* startShell() is a function that sets up by setting the working directory to home and loading the history the shell before runShell() */
void startShell(){
	setHome();
	loadHistory();
}

/* endShell() is a function that saves the history and restores the path to the original path while starting the shell */
void endShell(){
	saveHistory();
	restorePath();
        exit(0);
}

void addAlias(char *tokens[]){
	if(aliasCount >= 20){
		errorMessage(tokens[0], 11);
	}
	else{
		strcpy(aliases[aliasCount].aliasName, tokens[1]);
		strcpy(aliases[aliasCount].aliasCommand, tokens[2]);
		aliasCount++;
	}
}

void printAliases(){
	for(int i = 0; i < 20; i++){
		if(aliases[i].aliasName != NULL){
			printf("%s %s \n", aliases[i].aliasName, aliases[i].aliasCommand);
		}
	}
}
