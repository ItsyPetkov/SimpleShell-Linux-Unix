

/* 
 * saveAlias() 
 *
 * Description : The function saves the Aliases Array in the file .aliases located
 *		 in the users home.
 *		 The format of the .aliases file is as follows:
 *		 <aliasName> <aliasCommand>
 *
 * Return Type : void
 *
 */
void saveAlias(){

	/* The file .aliases is opened */
	FILE *file = fopen(".aliases","w+");

	if(file==NULL){
		return;
	}

	/* aliasName and aliasCommand are copied from the Alias Array to the .aliases file */
	int i;
	for(i = 0; i<ALIAS_SIZE; i++){
		if(checkEmptyAlias(i)==false){
			fprintf(file, "%s %s\n", aliases[i].aliasName, aliases[i].aliasCommand);
		}
	}

	printf("Saving aliases to .aliases file\n");
	fclose(file);
}


/* 
 * loadAliases()
 *
 * Description : The function loads the contents of the .aliases file to the Alias Array.
 *		 Furthermore, appropriate checks are made so that there are no errors in
 *		 the loading process
 *				 
 * Return Type : void
 *
 */
void loadAlias(){
	FILE *file ;
	char line[BUFFER_SIZE];

	/* If a .aliases file is not found, it is created so that aliases can be saved into it */
	if((file = fopen(".aliases", "r")) == NULL){
		printf("No .aliases found. New .aliases file being created.\n");
	}

	else{
		while(1){
			
			/* Each line is taken one by one and stored into char line[] */
			if(fgets(line, BUFFER_SIZE , file)==NULL){
				break;
			}

			/* If the line is formatted correctly, the alias is loaded into the array,
			 * otherwise the alias array is cleared and no aliases are loaded.
			 */
			if(checkAliasesContent(line)==true){
				addToAliasArray(line);
			}else{
				clearAliasesArray();
				errorMessage(".aliases",19);
			}
		}
		fclose(file);
	}
}


/* 
 * saveHistory() 
 *
 * Description : The function saves the History Array in the file .hist_list located
 *		 in the users home.
 *		 The format of the .hist_list file is as follows:
 *		 <command>
 *				 
 * Return Type : void
 *
 */
void saveHistory(){
	
	/* The file .hist_list is opened */
	FILE *file = fopen(".hist_list","w+");

	if(file==NULL){
		return;
	}

	/* Commands are copied from the Alias Array to the .aliases file */
	int i;
	for(i = 0; i<HISTORY_SIZE; i++){
		if(history[i].string!=NULL){
			fprintf(file, "%s",history[i].string);
		}
	}

	printf("Saving history to .hist_list file\n");
	fclose(file);
}


/* 
 * loadHistory()
 *
 *
 * Description : The function loads the contents of the .hist_list file to the History Array.
 *		 Furthermore, appropriate checks are made so that there are no errors in
 *		 the loading process.
 *				 
 * Return Type : void
 *
 */
void loadHistory(){
	FILE *file ;
	char line[BUFFER_SIZE];

	/* If a .hist_list file is not found, it is created so that aliases can be saved into it */
	if((file = fopen(".hist_list", "r")) == NULL){
		printf("No .hist_list found. New .hist_list file being created.\n");
	}

	else{
		while(1){
			/* Each line is taken one by one and stored into char line[] */
			if(fgets(line, BUFFER_SIZE , file)==NULL){
				break;
			}
			addToHistoryArray(line);	
		}
		fclose(file);
	}
}


/* 
 * addToAliasArray() 
 *
 * Parameters  : char * line : Line read from .aliases file 
 *
 * Description : The function tokenizes the line from the first whitespace and
 *               adds the contents to the Alias Array.
 *				 
 * Return Type : void
 *
 */
void addToAliasArray(char * line){

	/* The line is tokenised from the first whitespace */
	char * tokenName = strtok(line, " ");
	char * tokenCommand= strtok(NULL, "\n");

	/* Appropriate checks are made to make sure the contents of the file is correct */
	if(tokenName == NULL || tokenCommand == NULL){
		errorMessage(".aliases",19);
		clearAliasesArray();
	}else{
		strcpy(aliases[alias_count].aliasName,tokenName);
		strcpy(aliases[alias_count].aliasCommand,tokenCommand);
		alias_count++;
	}
}

/* 
 * checkAliasesContent()
 *
 * Parameters  : char * line : Line read from .aliases file 
 *
 * Description : The function checks whether there is a white space in the line
 *               read from the .aliases file and returns a bool.
 *				 
 * Return Type : bool
 *	       > true  : fucntion returns true when there is a whitespace in the line.
 *	       > false : function returns false when there is no whitespace in the line.
 *               
 *
 */
bool checkAliasesContent(char * line){
	char * space = strchr(line, ' ');
	if(space==NULL){
		return false;
	}else{
		return true;
	}
}
