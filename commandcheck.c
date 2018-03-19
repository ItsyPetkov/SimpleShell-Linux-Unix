

/*
 * commandCheck()
 *
 * Parameters  : char * commands[] : Tokenized command array.
 *
 * Description : The function checks the first command with inbuilt commands, if it is an
 * 		 inbuit command, the command is executed, otherwise it is treated as an/
 * 		 external command.
 *				 
 * Return Type : void
 *
 */
void commandCheck(char * commands[]){

	/* Check for whether the the first command is getpath */
	if(strcmp(commands[0],"getpath")==0){
		
		/* If getpath is used with parameters, an error message is
		 * displayed otherwise getpath() is called.
		 */
		if(commands[1]!=NULL){
			errorMessage(commands[0],1);
			return;
		}
		getpath();
	}


	/* Check for whether the the first command is setpath */
	else if(strcmp(commands[0],"setpath")==0){

		/* If setpath is used with too many or too little parameters,
		 * an error message is displayed otherwise setpath() is called
		 */
		if (commands[1]!=NULL && commands[2]==NULL){
			setpath(commands[1]);
		}else{
			errorMessage(commands[0],2);
		}
	}
	

	/* Check for whether the the first command is cd */
	else if(strcmp(commands[0],"cd")==0){
		
		/* If cd is used with too many parameters, an error message is 
		 * displayed otherwise changeDirectory() is called. 
		 */
		if(commands[2]==NULL){
			changeDirectory(commands);
		}else{
			errorMessage(commands[0],3);
		}	
	}


	/* Check for whether the the first command is exit */
	else if(strcmp(commands[0],"exit")==0){

		/* If exit is used with parameters, an error message is 
		 * displayed otherwise endShell() is called. 
		 */
		if(commands[1]==NULL){
			endShell();
		}else{
			errorMessage(commands[0],4);
		}

	}
    
	/* Checks whether the first token is ! and if there are appropriate parameters */
    else if(strcspn(commands[0],"!")==0){
		
		/* Checks whether the invocation is !-<no> */
		if(commands[0][1]=='-'){
		
			/* If <no> in !-<no> is not a digit an error message is displayed */
			if(isDigit(commands)==false){
				errorMessage(commands[0],5);
				return;
			}
			
			/* If !-<no> is called with parameters an error message is displayed */
			if(commands[1]!=NULL){
				errorMessage(commands[0],5);
				return;
			}

			/* If there are no commands in the History Array, an error message is displayed */
			if(isHistoryEmpty()==true){
				errorMessage(commands[0],24);	
				return;
			}

			int getnumber = getNum(commands[0],2);
			int commandno = history_count-getnumber+1;
			
			/* If the <no> in !-<no> is not a commandNumber in the Aliases Array,
			 * an error message is displayed
			 */
			if(commandExists(commandno)==false){
				errorMessage(commands[0],6);
				return;
			}

			/* If there are no errors, history is successfully invoked */
			executeHistory(commandno);

		}
			
		/* Checks whether the invocation is !! */
		else if(commands[0][1]=='!'){

			/* If !! is called with parameters, an error message is displayed */
			if(commands[1]!=NULL){
				errorMessage(commands[0],7);
				return;
			}

			/* If there are no commands in the History Array, an error message is displayed */
			if(isHistoryEmpty()==true){
				errorMessage(commands[0],8);
			}

			/* If there are no errors, history is successfully invoked */
			executeHistory(history_count);
				
		}

		/* An error message is displayed if the invocation is ! */
		else if (commands[0][1]=='\0'){
			errorMessage(commands[0],9);
			return;
		}

		/* If the invocation is not !, !!, !-<no> it has to be !<no> */
		else{

			/* If <no> in !<no> is not a digit an error message is displayed */
			if(isDigit(commands)==false){
				errorMessage(commands[0],9);
				return;
			}

			/* If !<no> is called with parameters an error message is displayed */
			if(commands[1]!=NULL){
				errorMessage(commands[0],9);
				return;
			}
				
			/* If there are no commands in the History Array, an error message is displayed */
			if(isHistoryEmpty()==true){
				errorMessage(commands[0],23);	
				return;
			}

			int getnumber = getNum(commands[0],1);
			int commandno = getnumber;

			/* If the <no> in !<no> is not a commandNumber in the Aliases Array,
			 * an error message is displayed
			 */
			if(commandExists(commandno)==false){
				errorMessage(commands[0],10);
				return;
			}
			
			/* If there are no errors, history is successfully invoked */			
			executeHistory(commandno);
				
		}	

   	}

	/* Check for whether the the first command is history */
	else if(strcmp(commands[0], "history") == 0){

		/* If history is called with parameters, an error message is displayed. */
		if(commands[1]!=NULL){
			errorMessage(commands[0],11);
			return;
		}
		printHistory();
	}

	/* Check for whether the the first command is alias */
	else if(strcmp(commands[0], "alias") == 0){

		/* If alias was called with only one parameter, an error message is displayed */
		if(commands[1]!=NULL && commands[2] == NULL){
			errorMessage(commands[0], 12);
			return;
		} 

		/* Check for whether alias was called without any parameters */
		if(commands[1]==NULL){
	
			/* If the Alias Array is empty, an error message is displayed, otherwise
			 * all aliases in the Alias Array are printed. 
			 */
			if(isAliasEmpty()==true){
				errorMessage(commands[0],17);
				return;
			}
			printAliases();
		}

		/* Check for alias was called with two parameters. */
		else if(commands[1] != NULL && commands[2]!=NULL){

			/* If alias name is same as command, an error message is displayed, otherwise
			 * the alias is added to the Alias Array */
			if(strcmp(commands[1], commands[2]) == 0 && commands[3] == NULL){
				errorMessage(commands[0], 13);
				return;
			}

			addAlias(commands);

			/* Confirmation Message is displayed if Alias was successfully added to the array */
			if(wasAliasAdded(commands)==true){
				printf("Alias Added Successfully\n");
			}
			
		}

	}

	/* Check for whether the the first command is unalias */	
	else if (strcmp(commands[0], "unalias") == 0) {

		/* If unalias is called with too little or too many parameters,
		 * an error message is displayed otherwise the alias is removed
		 */
		if (commands[1] == NULL || commands[2] != NULL) {
			errorMessage(commands[0], 15);
			return;		
		}

		int index = aliasIndexCheck(commands[1]);
		removeAlias(commands);

		/* Confirmation Message is displayed if Alias was successfully removed from the array */
		if(wasAliasRemoved(commands, index)==true){
				printf("Alias Removed Successfully\n");
		}
		
	}


	/* If the command fails to be recognised as an inbuilt command, it is considered to be
	 * an external command. externalCommandexec() is called with the command to exectute the 
	 * external command.
	 */
	else{
		externalCommandexec(commands);	
	}
}
