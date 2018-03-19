
/*
 * CS 210 : Simple Shell
 *
 * Tuesday 11 : Group 7
 * 
 * Group Members:
 * > Shivam Khattar        : vib16216
 * > Khaled S A A Alqallaf : yqb17104
 * > Hristo Emilov Petkov  : ypb16168
 * > Saeed Ahmed Khan	   : hsb16145
 *
 */

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/wait.h>
#include <stdbool.h>
#include <sys/types.h>

#include "shell.h"

#include "commanddetect.c"
#include "error.c"
#include "functions.c"
#include "history.c"
#include "persistance.c"
#include "alias.c"
#include "commandcheck.c"

/* 
 * main()
 *
 * Description : main calls other functions in the program so the shell runs.
 *				 
 * Return Type : int
 *
 */
int main(void){
	startShell();
	runShell();
	return 0;
}

/* 
 * parseInput()
 *
 * Parameters  : char *line : line read from the console.
 *
 * Description : The function tokenizes the line read from the console using given delimeters.
 *               The function also checks whether the first token is an alias and swaps it with
 *               the appropriate command from the alias array.
 *				 
 * Return Type : void           
 *
 */
void parseInput(char *line){
	
	/* Extracting the first token */
	int position=0;
	char *token;
	char * tokenarray[TOKEN_SIZE];
	token=strtok(line," |><&\t\n;");
	
	/* Check if the first token is NULL, if it is NULL further extraction 
         * isn't done to prevent segmentation faults.
         */
	if(token == NULL){
		return;
	}
	
	/* Further extraction of tokens into tokenarray[] */
	while(token != NULL){
		tokenarray[position]=token;
		token = strtok(NULL," |><&\t\n;");
		position++;
	}
	
	int i;
	for(i=position;i<50;i++){
		tokenarray[i]=NULL;
	}

	/* Check whether the command is an alias */
	int alias_index = aliasIndexCheck(tokenarray[0]);
	if(alias_index != -1){
		
		/* If the command is an alias it is replaced with the alias command
		 * and all other tokens are  concatenated to the new string.
		 */
		char new_input[BUFFER_SIZE];
		strcpy(new_input, aliases[alias_index].aliasCommand);
		int index=1;

		while(tokenarray[index]!=NULL){
			strcat(new_input, tokenarray[index]);
			strcat(new_input, " ");
			index++;
		}

		/* If circular dependency is detected, an error message is displayed */
		if(checkCircular(tokenarray[0]) == true){
			errorMessage(tokenarray[0], 21);
			return;
		}

		/* If command detect is full, an error message is displayed */
		if(isCommandDetectFull() == true){
			errorMessage(tokenarray[0], 22);
			return;
		}

		/* Command is added to command_detect array, if the replaced command is 
		 * a history invocation it is also added to the command array to prevent
		 * segmentation faults.
		 */
		addToCommandDetect(tokenarray[0]);
		if(strcspn(new_input,"!")==0){
			char *tokenlist;
			tokenlist=strtok(line," |><&\t\n;");
			addToCommandDetect(tokenlist);
		}

		/* The new input is sent back to parseInput() for retokenization */
		parseInput(new_input);
	}
			
	/* If command is not an alias it is executed normally. */		
	else if (tokenarray[0] != NULL) {
        	commandCheck(tokenarray);
	}
			
}


/* 
 * runShell()
 *
 * Description : The function runs the shell.
 *				 
 * Return Type : void           
 *
 */
void runShell(){
	char line[BUFFER_SIZE];
	while (1){

		printf(">");

		/* line is set to an empty char array */
		memset(line,'\0',BUFFER_SIZE);

		readInput(line);

		/* If the line is not a history invocation it is added to the history array */
		if(strcspn(line,"!")!=0 && (line[0] != '\n')){
			if(checkifLineIsOnlySpace(line)==false){
				addToHistoryArray(line);
			}
		}
		
		/* If the line is not empty, it is parsed. */
		if(line[0] != '\n') {
			parseInput(line);
		}

		/* Command detect is cleared for the next command */
		clearCommandDetect();
		
	}
		
}

/* 
 * readInput()
 *
 * Parameters  : char * line : pointer to where the input has to be stored.
 *
 * Description : The function reads the user input from the console.
 *				 
 * Return Type : void           
 *
 */
void readInput(char * line){
	/* Check for CTRL+D */
	if (fgets(line,BUFFER_SIZE,stdin)==NULL){
			printf("\n");
			endShell();
    }
}


/* 
 * externalCommandexec()
 *
 * Parameters  : char * commands[] : tokenized array of commands.
 *
 * Description : Runs an external command by creating a child process using fork().
 *				 
 * Return Type : void           
 *
 */
void externalCommandexec(char * commands[]){
	
	/* Forking a child process */
	pid_t  pid;
 	pid = fork();

	/* Error in Fork */
	if (pid < 0){
		perror("ERROR!! Could not fork Child.");
	}

	/* If child Process*/
 	if (pid == 0){
		
		/* Detecting error in execvp */
		if(execvp(commands[0],commands)==-1){
			perror(commands[0]);
			kill(getpid(),SIGKILL);
		}

	}
	
	/* If it is the parent process, it has to wait till child complete */
	else{
		wait(NULL);
		
	}
    		
}

/* 
 * startShell()
 *
 * Description : The function sets up the shell before the first user input is taken. 
 *		 The system path is stored in char * path, the current working directory
 *		 is changed to HOME, the .aliases file and the .hist_list file are loaded.
 *				 
 * Return Type : void           
 *
 */
void startShell(){
	getenvPath();
	getpath();
	setHome(getenvHome());
	if(isDirectoryHome()==false){
		errorMessage("ERROR",25);
		setHome(getenvHome());
	}
	loadHistory();
	loadAlias();
}

/* 
 * endShell()
 *
 * Description : The function restores the shell to its original state and 
 *		 saves .hist_list and .aliases files.
 *				 
 * Return Type : void           
 *
 */
void endShell(){
	if(isDirectoryHome()==false){
		errorMessage("ERROR",25);
		setHome(getenvHome());
	}
	saveHistory();
	saveAlias();
	restorePath();
	exit(0);
}

/* 
 * checkifLineIsOnlySpace()
 *
 * Parameters : char * line : Line entered by the user.
 *
 * Description : The function checks whether the given line is only spaces.
 *				 
 * Return Type : bool
 *             > true  : function returns true when the line is only spaces.
 *             > false : function returns false when the line has other characters other than spaces.           
 *
 */
bool checkifLineIsOnlySpace(char * line){
	char c;
	int i;
	for(i = 0; i < strlen(line) - 1; i++){
		c = line[i];
		if(c != ' '){
			return false;
		}
	}
	return true;
}


