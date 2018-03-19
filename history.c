

/* 
 * addToHistory()
 *
 * Parameters  : char * command : Command to be added to the History Array
 *
 * Description : The function adds a given command to the history array
 *				 
 * Return Type : void
 *
 */
void addToHistoryArray(char * command){

	/* If the History Array is full, an error message is displayed */
	if(isHistoryFull()==true){
		adjustHistoryArray(command);
		return;
	}
	
	/* Command is copied to the first empty position in the History Array */
	strcpy(history[history_count].string, command);
	history[history_count].commandNumber = history_count+1;
	history_count++;
		

}


/* 
 * adjustHistoryArray()
 *
 * Parameters  : char * command : Command to be added to the History Array
 *
 * Description : The function shifts every element one postition backwords
 *		 to make place for a new command in the history array.
 *				 
 * Return Type : void
 *
 */
void adjustHistoryArray(char * command){
	int i;
	for(i = 0; i < HISTORY_SIZE-1; i++){
		strcpy(history[i].string, history[i+1].string);
	}
	strcpy(history[HISTORY_SIZE-1].string,command);
}


/* 
 * printHistory()
 *
 * Description : The function prints all the elements in the history array
 *				 
 * Return Type : void
 *
 */
void printHistory(){
	int i;
	for (i = 0;i < HISTORY_SIZE; i++) {
		
		/* Check for empty History spaces */
        	if (history[i].commandNumber != 0) {
            		printf("%d %s", history[i].commandNumber, history[i].string);
        	}
    	}
}


/* 
 * executeHistory()
 *
 * Parameters  : int commandNum : Command Number that has to be executed
 *
 * Description : The function executes a command from history.
 *				 
 * Return Type : void
 *
 */
void executeHistory(int commandNum){
	
	/* The function goes through all commands in the History array to find
	 * the command that has to be executed and executes the command again
         */
	int i;
	for(i = 0; i < HISTORY_SIZE; i++){
		if(history[i].commandNumber == commandNum){
			char input_copy[BUFFER_SIZE];
			strcpy(input_copy, history[i].string);
			parseInput(input_copy);
        	}
    	}
}


/* 
 * isHistoryFull()
 *
 * Description : The function checks whether the history array is full and returns a bool.
 *				 
 * Return Type : bool
 *	       > true  : function returns true when the history array is full
 *	       > false : function returns false when the history array is not full
 *
 */
bool isHistoryFull(){
	if(history_count<HISTORY_SIZE){
		return false;
	}else{
		return true;
	}
}


/* 
 * executeHistory()
 *
 * Parameters  : int commandNum : Command Number that has to be checked
 *
 * Description : The function checks whether a command number is present in the
 *		 history array and returns a bool.
 *				 
 * Return Type : bool
 *	       > true  : function returns true when the command exists in the history array
 *	       > false : function returns false when the command does not exist in the history array
 *
 */
bool commandExists(int commandno){

	if(commandno <= 0) {
		return false;
	}

	if(isHistoryFull()==true && commandno<HISTORY_SIZE){
		return true;
	}

	else{
		int i;
		for(i=0;i<HISTORY_SIZE;i++){
			if(history[i].commandNumber==commandno){
				return true;
			}
		}

	}
	
	return false;
}


/* 
 * getNum()
 *
 * Parameters  : char *command : command from which the number has to be extracted
 *	       : int index     : index from which number has to be extracted
 *
 * Description : The function extracts the number from a given command and index.		 
 *		
 * Return Type : int : number extracted from the command.
 *
 */
int getNum(char *command, int index){
	int sum=0;
	int i;
	for (i = index; i < strlen(command); i++) {
		int temp = command[i] - '0';
		int power = getPow(10, strlen(command)-i-1);
		sum += temp * power;
		temp = 0;
		power = 0;
        }
	return sum;
}


/* 
 * getPow()
 *
 * Parameters  : int number   : number 
 *	       : int exponent : power
 *
 * Description : The function is an auxiliary function to help getNum() work properly. 
 *               The function takes in a number and a power and returns an int.		 
 *		
 * Return Type : int : number^power.
 *
 */
int getPow(int number, int exponent){
	int finalNum = 1;
	int i;
	for(i=0;i<exponent;i++){
		finalNum = finalNum * number;
	}
	return finalNum;
}


/* 
 * isDigit()
 *
 * Parameters  : char * commands[] : command which has to be checked for a digit.
 *
 * Description : The function checks whether a given command is a digit.		 
 *		
 * Return Type : bool
 *	       > true  : function returns true when the given command is a digit.
 *	       > false : function returns false when the given command is not a digit.
 *
 */
bool isDigit(char * commands[]){
	bool isdigit=false;
	int i;
	for(i=1;i<strlen(commands[0]);i++){
		if(commands[0][i]>='0' && commands[0][i]<='9'){
			isdigit=true;
		}else{
			isdigit=false;
		}
	}
	return isdigit;
}


/* 
 * isHistoryEmpty()
 *
 * Description : The function checks whether the history array is empty and returns a bool.
 *				 
 * Return Type : bool
 *	       > true  : function returns true when the history array is empty
 *	       > false : function returns false when the history array is not empty
 *
 */
bool isHistoryEmpty(){
	if(history_count > 0){
		return false;
	}else{
		return true;
	}
}
