

/* 
 * getPath() 
 *
 * Description : The function prints the current PATH of the system.
 *
 * Return Type : void
 *
 */
void getpath() {
    char *currpath = getenv("PATH");
    printf("PATH : %s\n", currpath); 
}


/* 
 * setPath() 
 *
 * Parameters  : char *path : path which will be set as system PATH
 *
 * Description : The function sets the system path to char *path.
 *
 * Return Type : void
 *
 */
void setpath(char *path) {
    if (setenv("PATH", path, 1) == 0) {
        printf("PATH : %s\n", getenv("PATH"));
    } else {
	perror(path);
    }
}


/* 
 * changeDirectory() 
 *
 * Parameters: char * commands[] : Tokenized Command Array
 *
 * Description : The function changes the current directory of the system to
 * 		 a given directory.
 *
 * Return Type : void
 *
 */
void changeDirectory(char *commands[]){

	/* If no parameters are given, directory is set to home otherwise the directory
	 * is set to whatever the user selects
	 */
	if(commands[1]==NULL){
		chdir(getenvHome());
	}else if(chdir(commands[1])==-1){
		perror(commands[1]);
	}
}


/* 
 * restorePath() 
 *
 * Description : The function restores the original system path back.
 *
 * Return Type : void
 *
 */
void restorePath() {
    setpath(path);
}


/* 
 * setHome() 
 *
 * Parameters: char * home : home directory of the user
 *
 * Description : The function changes the current working directory to home.
 *
 * Return Type : void
 *
 */
void setHome(char * home) {
	char directory[BUFFER_SIZE];
	if (chdir(getenvHome()) == 0) {
		printf("HOME : %s\n", getcwd(directory, sizeof(directory)));
    	} else {
       		perror(home);
    	}
}


/* 
 * getenvHome() 
 *
 * Description : The function gets the enironment home.
 *
 * Return Type : char * : function returns the user Home.
 *
 */
char * getenvHome(){
	return getenv("HOME");
}


/* 
 * getenvPath()  
 *
 * Description : The function sets the global variable path as environment path.
 *
 * Return Type : void
 *
 */
void getenvPath(){
    path = getenv("PATH");
}


/* 
 * isDirectoryHome() 
 *
 * Description : The function checks whether the current working directory is
 *		 home and returns a bool. 
 *
 * Return Type : bool
 *             > true  : function returns true if the current directory is home
 *             > false : function returns false if the current directory is not home
 *
 */
bool isDirectoryHome(){
	char directory[BUFFER_SIZE];
	getcwd(directory, sizeof(directory));
	if (strcmp(directory,getenvHome())==0) {
		return true;
    	}else{
		return false;
	}
}
