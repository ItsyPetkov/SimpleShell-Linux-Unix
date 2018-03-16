#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <sys/types.h>
#include <unistd.h>
#include <errno.h>
#include <sys/wait.h>

#define BUFFER_SIZE 512
#define TOKEN_SIZE 50
#define HISTORY_SIZE 20
#define ALIAS_SIZE 10
#define true 1
#define false 0

char* path;
const char* home;

int history_count = 0;
int alias_count = 0;

typedef struct {
	int commandNumber;
	char string[BUFFER_SIZE];
}hist;

typedef struct {
	char aliasName[BUFFER_SIZE];
	char aliasCommand[BUFFER_SIZE];
}alias;

typedef int bool;

hist history[HISTORY_SIZE];
alias aliases[ALIAS_SIZE];

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
bool isDigit(char *tokens[]);
void saveHistory();
void loadHistory();
int getNum(char *token, int a);
bool commandExists(int commandno);
void errorMessage(char * token, int eno);
void startShell();
void endShell();
void addAlias(char * tokens[]);
void printAliases();
void removeAlias(char *tokens[]);
int aliasIndexCheck(char * token);
bool checkEmptyAlias(int index);
void adjustAliasArray(int index);
void adjustHistoryArray(char * input);
bool isHistoryFull();
bool isAliasFull();
bool isHistoryEmpty();
bool isAliasEmpty();
void getenvHome();
void getenvPath();
void loadAlias();
void saveAlias();
void addToAliasArray(char * input);
bool isDirectoryHome();
bool checkAliasesContent(char * input);
void clearAliasesArray();

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
	char * tokenarray[TOKEN_SIZE];
	token=strtok(input," |><&\t\n;");

	while(token != NULL){
		tokenarray[position]=token;
		token = strtok(NULL," |><&\t\n;");
		position++;
	}

	for(int i=position;i<50;i++){
		tokenarray[i]=NULL;
	}

	int alias_index = aliasIndexCheck(tokenarray[0]);

		if(alias_index != -1){
			char new_input[BUFFER_SIZE];
			strcpy(new_input, aliases[alias_index].aliasCommand);
			int index=1;
			while(tokenarray[index]!=NULL){
				strcat(new_input, tokenarray[index]);
				strcat(new_input, " ");
				index++;
			} 
			
			parseInput(new_input);
		} 
    

    else if (tokenarray[0] != NULL) {
	
        commandCheck(tokenarray);
    }
}

/* runShell() displays >, takes user input, calls createHistory() and parseInput() */
void runShell(){
	char input[BUFFER_SIZE];
	while (1){
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
	char directory[BUFFER_SIZE];
	if (chdir(home) == 0) {
		printf("HOME : %s\n", getcwd(directory, sizeof(directory)));
    	} else {
		// Pass to errorMessage
        	perror(home);
		printf("Could not change working directory to home.");
    	}
}

/*getenvHome() is a function which gets the environment Home */
void getenvHome(){
	home = getenv("HOME");
}

/*getenvPath() is a function which gets the environment Path */
void getenvPath(){
    path = getenv("PATH");
}

/* commandCheck() is a function to check what command the user has input */
void commandCheck(char * tokens[]){

	/* Calls the function getPath() if correct arguments are provided else displays error */
	if(strcmp(tokens[0],"getpath")==0){
		if (tokens[1]==NULL){
			getPath();
		}else{
			errorMessage(tokens[0],1);
		}
	}

	/* Calls the function setPath() if correct arguments are provided else displays error */
	else if(strcmp(tokens[0],"setpath")==0){
		if (tokens[1]!=NULL && tokens[2]==NULL){
			setPath(tokens[1]);
		}else{
			errorMessage(tokens[0],2);
		}
	}
	
	/* Calls the function changeDirectory() with the token array */
	else if(strcmp(tokens[0],"cd")==0){
		if(tokens[2]==NULL){
			changeDirectory(tokens);
		}else{
			errorMessage(tokens[0],3);
		}	
	}

	/* Checks whether first command is exit and if there are appropriate parameter */
	else if(strcmp(tokens[0],"exit")==0){
		if(tokens[1]==NULL){
			endShell();
		}else{
			errorMessage(tokens[0],4);
		}

	}
    
	/* Checks whether the first token is ! and if there are appropriate parameters */
    	else if(strcspn(tokens[0],"!")==0){
		
		/* checks if the second token is - */
		if(tokens[0][1]=='-'){
			if(isDigit(tokens)==true){
				if(tokens[1]==NULL){
					int sum = getNum(tokens[0],2);
					if(commandExists(history_count-sum+1)==true){
						executeHistory(history_count-sum+1);
					}else{
						errorMessage(tokens[0],6);
					}
				}else{
					errorMessage(tokens[0],5);
				}
			}else{
				errorMessage(tokens[0],5);
			}
		}

		/* checks if the second token is ! */
		else if(tokens[0][1]=='!'){
			if(tokens[1]==NULL){
				if(isHistoryEmpty()==false){
					executeHistory(history_count);
				}else{
					errorMessage(tokens[0],8);
				}
			}else{
				errorMessage(tokens[0],7);
			}
				
		}

		/* checks if the second token is NULL */
		else if (tokens[0][1]=='\0'){
			errorMessage(tokens[0],9);
		}

		else{
			if(isDigit(tokens)==true){
				if(tokens[1]==NULL){
					int sum = getNum(tokens[0],1);
					if(commandExists(sum)==true){
						executeHistory(sum);
					}else{
						errorMessage(tokens[0],10);
					}
				}else{
					errorMessage(tokens[0],9);
				}
			}else{
				errorMessage(tokens[0],9);
			}
		}	

    	}

	/* Checks whether first command is history and if there are appropriate parameters */
	else if(strcmp(tokens[0], "history") == 0){
		if(tokens[1]==NULL){
			printHistory();
		}else{
			errorMessage(tokens[0],11);
		}
		
	}

	/* Checks whether the first command is alias and if there are appropriate parameters */
	else if(strcmp(tokens[0], "alias") == 0){

		if(tokens[1]!=NULL && tokens[2] == NULL){
			errorMessage(tokens[0], 12);
		} 

		else if(tokens[1]==NULL){
			if(isAliasEmpty()==true){
				errorMessage(tokens[0],17);
			}else{
				printAliases();
			}
		}


		else if(tokens[1] != NULL && tokens[2]!=NULL){
			if(strcmp(tokens[1], tokens[2]) == 0 && tokens[3] == NULL){
				errorMessage(tokens[0], 13);
			}else{
				addAlias(tokens);
				printf("Alias added Successfully\n");
			}
		}

	}

	/* Checks whether the first command is unalias and if there are appropriate parameters */	
	else if (strcmp(tokens[0], "unalias") == 0) {
		if (tokens[1] == NULL || tokens[2] != NULL) {
			errorMessage(tokens[0], 15);
		
		} else {
			removeAlias(tokens);	
			printf("Alias removed Successfully\n");
		}
	

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
		if(isHistoryFull()==true){
			adjustHistoryArray(input);
		}else{
			strcpy(history[history_count].string, input);
			history[history_count].commandNumber = history_count+1;
			history_count++;
		}

}

/* adjustHistoryArray() is a function which ajusts the history array when it is full */
void adjustHistoryArray(char * input){
	for(int i = 0; i < HISTORY_SIZE-1; i++){
		strcpy(history[i].string, history[i+1].string);
	}
	strcpy(history[HISTORY_SIZE-1].string,input);
}

/* printHistory() is function which contents of the array of structs */
void printHistory(){
	
	for (int i = 0;i < HISTORY_SIZE; i++) {
        	if (history[i].commandNumber != 0) {
            		printf("%d %s", history[i].commandNumber, history[i].string);
        	}
    	}

}

/* executeHistory() is a function that takes a commandNum and executes the commandNum from history*/
void executeHistory(int commandNum){
    for(int i = 0; i < HISTORY_SIZE; i++){
        if(history[i].commandNumber == commandNum){
		char input_copy[BUFFER_SIZE];
		strcpy(input_copy, history[i].string);
		parseInput(input_copy);
        }
    }
}

/* isHistoryFull() is a function which returns true when the history array is full and false when the history array is not full */
bool isHistoryFull(){
	if(history_count<HISTORY_SIZE){
		return false;
	}else{
		return true;
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
bool isDigit(char * tokens[]){
	bool isdigit=false;
	for(int i=1;i<strlen(tokens[0]);i++){
		if(tokens[0][i]>='0' && tokens[0][i]<='9'){
			isdigit=true;
		}else{
			isdigit=false;
		}
	}
	return isdigit;
}

/* saveHistory() is a function to save the history to a .hist_list file */
void saveHistory(){
	FILE *file = fopen(".hist_list","w+");
	if(file==NULL){
		return;
	}

	for(int i = 0; i<HISTORY_SIZE; i++){
		if(history[i].string!=NULL){
			fprintf(file, "%s",history[i].string);
		}
	}

	printf("Saving history to .hist_list file\n");
	fclose(file);
}

/* loadHistory() is a function to load the history from the .hist_list file */
void loadHistory(){
	FILE *file ;
	char string[BUFFER_SIZE];
	if((file = fopen(".hist_list", "r")) == NULL){
		printf("No .hist_list found. New .hist_list file being created.\n");
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
bool commandExists(int commandno){

	if(commandno <= 0) {
		return false;
	}

	if(isHistoryFull()==true && commandno<HISTORY_SIZE){
		return true;
	}

	else{

		for(int i=0;i<HISTORY_SIZE;i++){
			if(history[i].commandNumber==commandno){
				return true;
			}
		}

	}
	
	return false;
}

/* errorMessage() is a function that takes in an error number and displays the appopriate error message for errors relating to commands starting with !*/
void errorMessage(char * token, int eno){
	switch(eno){
			
		case 1: fprintf(stderr, "%s : %s", token , "Invalid Parameters\n");
			printf("Please use getpath without parameters. Use: getpath.\n");
			break;

		case 2: fprintf(stderr, "%s : %s", token , "Invalid Parameters\n");
			printf("Please use setpath with only one parameter. Use: setpath <path>.\n");
			break;

		case 3: fprintf(stderr, "%s : %s", token , "Invalid Parameters\n");
			printf("Please use cd with one parameter or no parameters. Use: cd or cd <directory>.\n");
			break;

		case 4: fprintf(stderr, "%s : %s", token , "Invalid Parameters\n");
			printf("Please use exit without any parameters. Use: exit.\n");
			break;
	
		case 5: fprintf(stderr, "%s : %s", token , "Invalid Parameters\n");
			printf("Please use !-<no> without any parameters. Use: !-<no>.\n");
			break;

		case 6: fprintf(stderr, "%s : %s", token , "Invalid Number\n");
			printf("Please use !-<no> with a valid <no>. Use: !-<no>.\n");
			break;

		case 7: fprintf(stderr, "%s : %s", token , "Invalid Parameters\n");
			printf("Please use !! without any parameters. Use: !!\n");
			break;

		case 8: fprintf(stderr, "%s : %s", token , "Empty History\n");
			printf("Please enter at least one command to use !!. Use: !!\n");
			break;

		case 9: fprintf(stderr, "%s : %s", token , "Invalid Parameters\n");
			printf("Please use !<no> without any parameters. Use: !<no>.\n");
			break;
		
		case 10: fprintf(stderr, "%s : %s", token , "Invalid Number\n");
			printf("Please use !<no> with a valid <no>. Use: !<no>.\n");
			break;

		case 11: fprintf(stderr, "%s : %s", token , "Invalid Parameters\n");
			printf("Please use history without any parameters. Use: history.\n");
		  	break;

		case 12: fprintf(stderr, "%s : %s", token , "Invalid Parameters\n");
			printf("Please use alias with two parameters or use alias without any parameters to print aliases. Use: alias <name> <command> or alias.\n");
		  	break;

		case 13: fprintf(stderr, "%s : %s", token , "Invalid Alias\n");
			printf("Alias name must not be identical to command.\n");
			break;

		case 14:fprintf(stderr, "%s : %s", token, "Aliases are full\n");
			printf("Please remove an alias before adding a new alias.\n");
			break;
			
		case 15: fprintf(stderr, "%s : %s", token , "Invalid Parameters\n");
			printf("Please use unalias with one parameter. Use: unalias <name>.\n");
			break;

		case 16: fprintf(stderr, "%s : %s", token , "Alias does not exist\n");
			printf("Please use unalias on an alias name that exists. Use: unalias <name>.\n");
			break;

		case 17: fprintf(stderr, "%s : %s", token , "No Alias created yet\n");
			printf("Please add an alias before using alias. Use: alias.\n");
			break;

		case 18: fprintf(stderr, "%s : %s", token , "Alias name already exists\n");
			printf("Alias being overridden with existing alias.\n");
			break;

		case 19: fprintf(stderr, "%s : %s", token , "Incorrect Format\n");
			printf(".aliases file has incorrect format. No aliases loaded.\n");
			break;

		case 20: fprintf(stderr, "%s : %s", token , "Incorrect Format\n");
			printf(".hist_list file has incorrect format. No history loaded.\n");
			break;

		default: printf("Invalid error number\n");
			break;

	
	}
}

/* startShell() is a function that sets up by setting the working directory to home and loading the history the shell before runShell() */
void startShell(){
	getenvPath();
	getenvHome();
	setHome();
	if(isDirectoryHome()==false){
		printf("Current Directory is not Home. The directory is being changed to home to load .hist_list and .aliases file from the right location.\n");
		setHome();
	}
	loadHistory();
	loadAlias();
}

/* endShell() is a function that saves the history and restores the path to the original path while starting the shell */
void endShell(){

	if(isDirectoryHome()==false){
		printf("Current Directory is not Home. The directory is being changed to home to save .hist_list and .aliases file in right location.\n");
		setHome();
	}
	saveHistory();
	saveAlias();
	restorePath();
	exit(0);
}

/* addAlias() is a function which add an alias*/
void addAlias(char * tokens[]){
	if(isAliasFull()==true){
		errorMessage(tokens[0],14);
	}else{
		if(aliasIndexCheck(tokens[1])!=-1){
			errorMessage(tokens[0],18);
			removeAlias(tokens);
		}
		strcpy(aliases[alias_count].aliasName,tokens[1]);
		int commandIndex = 2;	
		while(tokens[commandIndex]!=NULL){
			strcat(aliases[alias_count].aliasCommand,tokens[commandIndex]);
			strcat(aliases[alias_count].aliasCommand," ");
			commandIndex++;
		}
		alias_count++;
	}

}

/* printAiases() is a function which prints all the aliases*/
void printAliases(){
	for(int i=0;i<ALIAS_SIZE;i++){
		if(checkEmptyAlias(i)==false){
			printf("%s : %s\n",aliases[i].aliasName, aliases[i].aliasCommand);
		}
	}
}

/* checkEmptyAlias() is a function which checks if the current alias is empty */
bool checkEmptyAlias(int index){
	if(strcmp(aliases[index].aliasName,"\0")==0 && strcmp(aliases[index].aliasCommand,"\0")==0 ){
		return true;
	}
	return false;
}

/* removeAlias() is a function which removes the chosen alias */
void removeAlias(char *tokens[]) {
	int index = aliasIndexCheck(tokens[1]);
	if(index>=0){
		strcpy(aliases[index].aliasName,"");
		strcpy(aliases[index].aliasCommand,"");
		adjustAliasArray(index);
		alias_count--;
	} else {
		errorMessage(tokens[0], 16);
	}
}

/* aliasIndexCheck */
int aliasIndexCheck(char * token){
	for(int i =0;i<ALIAS_SIZE;i++){
		if(strcmp(aliases[i].aliasName,token)==0){
			return i;
		}
	}
	return -1;
}


/* adjustAliasArray() is function which fixes the array after an alias is deleted by shifting everything after by on position backwards*/
void adjustAliasArray(int index){

	for(int i=index;i<alias_count-1; i++){
		strcpy(aliases[i].aliasName,aliases[i+1].aliasName);
		strcpy(aliases[i].aliasCommand,aliases[i+1].aliasCommand);
	}

	strcpy(aliases[alias_count-1].aliasName,"");
	strcpy(aliases[alias_count-1].aliasCommand,"");

}

/* isAliasFull() is a function which returns true when the alias array is full and false when the alias array is not full */
bool isAliasFull(){
	if(alias_count<ALIAS_SIZE){
		return false;
	}else{
		return true;
	}
}

/* isHistoryEmpty() is a function which returns true when the history array is empty and false when the history array is not empty */
bool isHistoryEmpty(){
	if(history_count > 0){
		return false;
	}else{
		return true;
	}
}

/* isAliasEmpty() is a function which returns true when the alias array is empty and false when the alias array is not empty */
bool isAliasEmpty(){
	if(alias_count > 0){
		return false;
	}else{
		return true;
	}
}

/* saveAlias() is a function which saves the aliases in the .aliases file */
void saveAlias(){
	FILE *file = fopen(".aliases","w+");

	if(file==NULL){
		return;
	}

	for(int i = 0; i<ALIAS_SIZE; i++){
		if(checkEmptyAlias(i)==false){
			fprintf(file, "%s %s\n",aliases[i].aliasName, aliases[i].aliasCommand);
		}
	}

	printf("Saving aliases to .aliases file\n");
	fclose(file);
}

/* saveAlias() is a function which loads the aliases from the .aliases file */
void loadAlias(){
	FILE *file ;
	char string[BUFFER_SIZE];
	if((file = fopen(".aliases", "r")) == NULL){
		printf("No .aliases found. New .aliases file being created.\n");
	}
	else{
		while(1){
			if(fgets(string, BUFFER_SIZE , file)==NULL){
				break;
			}
			if(checkAliasesContent(string)==true){
				addToAliasArray(string);
			}else{
				clearAliasesArray();
				errorMessage(".aliases",19);
			}
		}
		fclose(file);
	}
}

/* addToAliasArray() takes a char pointer as a parameter, splits the pointer from the first whitespace and adds the first token to AliasName and everything after to aliasCommand */
void addToAliasArray(char * input){
	char * tokenName = strtok(input, " ");
	char * tokenCommand= strtok(NULL, "\n");
	
	strcpy(aliases[alias_count].aliasName,tokenName);
	strcpy(aliases[alias_count].aliasCommand,tokenCommand);
	alias_count++;
}

/* isDirectoryHome() is a function which checks whether the current directory is home and returns true if it is home and false if it is not home */
bool isDirectoryHome(){
	char directory[BUFFER_SIZE];
	getcwd(directory, sizeof(directory));
	if (strcmp(directory,home)==0) {
		return true;
    }else{
		return false;
	}
}

/* */
bool checkAliasesContent(char * input){
	char * p = strchr(input, ' ');
	if(p==NULL){
		return false;
	}else{
		return true;
	}
}

/* */
void clearAliasesArray(){
	for(int i = 0; i<ALIAS_SIZE; i++){
		strcpy(aliases[i].aliasName,"");
		strcpy(aliases[i].aliasCommand,"");
	}
	alias_count = 0;
}



/* meaningfull comments */


