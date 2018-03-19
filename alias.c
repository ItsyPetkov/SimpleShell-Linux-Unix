

/*
 * addAlias()
 *
 * Parameters  : char * commands[] : Tokenized command array.
 *
 * Description : The function adds a given alias to the Alias Array. 
 *				 
 * Return Type : void
 *
 */
void addAlias(char * commands[]){

	/* If the same aliasName exists, an error message for overridding 
	 * is displayed and alias is overriden.
     */
	if(aliasIndexCheck(commands[1])!=-1){
		errorMessage(commands[0],18);
		replaceAlias(commands);
		return;
	}

	/* If the Alias Array is already full, an error message is displayed */
	if(isAliasFull()==true){
		errorMessage(commands[0],14);
		return;		
	}

	/* If Alias Array is not full and aliasName dosent exist, the Alias is added
	 * normally. The aliasName is copied to the Alias Array and all commands
	 * after aliasName are concatinated with a space after each command to
	 * aliasCommand in the Alias Array.
	 */
	strcpy(aliases[alias_count].aliasName,commands[1]);
	int commandIndex = 2;
	while(commands[commandIndex]!=NULL){
		strcat(aliases[alias_count].aliasCommand,commands[commandIndex]);
		strcat(aliases[alias_count].aliasCommand," ");
		commandIndex++;
	}

	alias_count++;
}


/*
 * printAliases()
 *
 * Description : The function prints all the aliases in the Alias Array. 
 *				 
 * Return Type : void
 *
 */
void printAliases(){
	int i;
	for(i=0;i<ALIAS_SIZE;i++){
		if(checkEmptyAlias(i)==false){
			printf("%s : %s\n",aliases[i].aliasName, aliases[i].aliasCommand);
		}
	}
}


/* 
 * checkEmptyAlias()
 *
 * Parameters  : int index : index of the alias to be checked.
 *
 * Description : The function checks whether a given alias is empty and returns
 *				 a bool.
 *				 
 * Return Type : bool
 *             > true  : function returns true if the alias at the given index is empty.
 *	       > false : function returns false if the alias at the given index is not empty.
 *
 */
bool checkEmptyAlias(int index){
	if(strcmp(aliases[index].aliasName,"\0")==0 && strcmp(aliases[index].aliasCommand,"\0")==0 ){
		return true;
	}
	return false;
}


/*
 * aliasRemove()
 *
 * Parameters  : char * commands[] : Tokenized command array.
 *
 * Description : The function checks whether the given alias exists in the Alias 
 *		 Array and removes it if it exists.
 *				 
 * Return Type : void
 *
 */
void removeAlias(char *commands[]) {
	/* Check whether alias Exists */
	int index = aliasIndexCheck(commands[1]);
	
	/* If alias exists, remove it otherwise display an error message */
	if(index>=0){

		/* Alias at index is removed */
		strcpy(aliases[index].aliasName,"");
		strcpy(aliases[index].aliasCommand,"");
		
		/* Alias array is adjusted to remove the empty space inbetween and alias_count is corrected */
		adjustAliasArray(index);
		alias_count--;
	} else {
		errorMessage(commands[0], 16);
	}
}


/*
 * aliasIndexCheck()
 *
 * Parameters  : char * name : aliasName of the alias whose index is to be retrieved.
 *
 * Description : The function checks all aliasNames in the Alias Array against name
 *		 and returns an int.
 *				 
 * Return Type : int
 * 	       > index : function returns the index of the given alias if found.
 *	       > -1    : function returns -1 if the alias is not found.
 *
 */
int aliasIndexCheck(char * name){
	int index;
	for(index =0;index<ALIAS_SIZE;index++){
		if(strcmp(aliases[index].aliasName,name)==0){
			return index;
		}
	}
	return -1;
}


/*
 * adjustAliasArray()
 *
 * Parameters : int index : Index of the empty position in the Alias Array.
 *
 * Description : The function fixes the array after an alias is removed by shifting
 *		 all aliases an index backwords.
 *				 
 * Return Type : void
 *
 */
void adjustAliasArray(int index){

	/* Data is moved one index backwords */
	int i;
	for(i=index;i<alias_count-1; i++){
		strcpy(aliases[i].aliasName,aliases[i+1].aliasName);
		strcpy(aliases[i].aliasCommand,aliases[i+1].aliasCommand);
	}

	/* The last index is cleared again to make place for another alias */
	strcpy(aliases[alias_count-1].aliasName,"");
	strcpy(aliases[alias_count-1].aliasCommand,"");

}


/*
 * isAliasFull()
 *
 * Description : The function checks whether the Alias Array is full and returns a bool.
 *				 
 * Return Type : bool
 *	       > true  : function returns true when the Alias Array is full.
 *	       > false : function returns false when the Alias Array is not full.
 *
 */
bool isAliasFull(){
	if(alias_count<ALIAS_SIZE){
		return false;
	}else{
		return true;
	}
}


/*
 * isAliasEmpty()
 *
 * Description : The function checks whether the Alias Array is empty and returns a bool.
 *				 
 * Return Type : bool
 *             > true  : function returns true when the Alias Array is empty.
 *	       > false : function returns false when the Alias Array is not empty.
 *
 */
bool isAliasEmpty(){
	if(alias_count > 0){
		return false;
	}else{
		return true;
	}
}


/*
 * clearAliasesArray()
 *
 * Description : The function clear all elements of the alias array and sets alias_count to 0.
 *				 
 * Return Type : void
 *
 */
void clearAliasesArray(){
	int i;
	for(i = 0; i<ALIAS_SIZE; i++){
		strcpy(aliases[i].aliasName,"");
		strcpy(aliases[i].aliasCommand,"");
	}
	alias_count = 0;
}


/*
 * wasAliasAdded()
 *
 * Parameters  : char * commands[] : Tokenized array of Commands
 *
 * Description : The function checks whether a given Alias was added to the Alias array and
 * 				 returns a bool.
 * 
 * Return Type : bool
 *	       > true  : function returns true when an Alias was sucessfully added.
 *	       > false : function returns false when an Alias was not successfully added.
 *
 */
bool wasAliasAdded(char * commands[]){
	if(strcmp(commands[1], aliases[alias_count-1].aliasName)==0){
		return true;
	}else{
		return false;
	}
}


/*
 * wasAliasRemoved()
 * 
 * Parameters  : char * commands[] : Tokenized array of Commands
 *			   : int index         : Index of the Alias that was to be removed
 *
 * Description : The function checks whether a given Alias was successfully removed from
 *				 the Alias array.
 *
 * Return Type : bool
 *	       > true  : function returns true when an Alias was successfully removed.
 *	       > false : function returns false when an Alias was not successfully removed.
 *
 */
bool wasAliasRemoved(char * commands[], int index){
	if(index>=0){
		if(strcmp(aliases[index].aliasName,commands[1])!=0){
			return true;
		}
	}
	return false;
}


/*
 * replaceAlias()
 *
 * Parameters  : char * commands[] : Tokenized array of Commands
 *
 * Description : The function finds the appropriate alias to be overriden, and overrides the
 *		 aliasCommand.
 *
 * Return Type : void
 * 
 */
void replaceAlias(char * commands[]){
	int index = aliasIndexCheck(commands[1]);
	strcpy(aliases[index].aliasCommand, "");
	int commandIndex = 2;	
	while(commands[commandIndex]!=NULL){
		strcat(aliases[index].aliasCommand,commands[commandIndex]);
		strcat(aliases[index].aliasCommand," ");
		commandIndex++;
	}
}


