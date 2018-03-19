

/* 
 * addToCommandDetect()
 *
 * Parameters  : char * command : Command that has to be added to the command_detect array.
 *
 * Description : The function copies the command into the first empty index in the command
 *		 detect array.
 *				 
 * Return Type : bool
 *             > true  : function returns true if the alias at the given index is empty.
 *	       > false : function returns false if the alias at the given index is not empty.
 *
 */
void addToCommandDetect(char * command){
	command_detect[command_detect_count]=command;
	command_detect_count++;
}


/* 
 * clearCommandDetect()
 *
 * Description : The function clears the command_detect array.
 *				 
 * Return Type : void
 *
 */
void clearCommandDetect(){
	char *p = NULL;
	int i;
	for(i = 0 ; i < command_detect_count; i++){
		command_detect[i]=p;
	}
	command_detect_count = 0;
}


/* 
 * checkCircular()
 *
 * Parameters  : char * command : Command that has to be checked against the command_detect array.
 *
 * Description : The function checks if adding the command to the command_detect will cauuse a
 *		 circular dependency in the command_detect array.
 *				 
 * Return Type : bool
 *	       > true  : function returns true when a circular dependency will occur if the command
 *			 is added to the command_detect array.
 *	       > false : function returns false when a circular dependency will not occur if the
 *			 command is added to the command_detect array.
 *
 */
bool checkCircular(char * command){
	int i;
	for(i = 0 ; i < command_detect_count; i++){
		if(strcmp(command_detect[i], command)==0){
			return true;
		}	
	}
	return false;
}


/* 
 * printCommandDetect()
 *
 * Description : The function prints all elements in the command_detect array
 *				 
 * Return Type : void
 *
 */
void printCommandDetect(){
	int i;
	for(i = 0 ; i < command_detect_count; i++){
		printf("%s \n" , command_detect[i]);	
	}
}


/* 
 * checkCircular()
 *
 * Description : The function checks if the command_detect array is full and returns a bool.
 *				 
 * Return Type : bool
 *	       > true  : function returns true when the command_detect array is full.
 *	       > false : function returns false when the command_detect array is not full.
 *
 */
bool isCommandDetectFull(){
	if(command_detect_count < COMMAND_DETECT_SIZE ){
		return false;
	}else{
		return true;
	}
}
