

/* 
 * errorMessage()
 *
 * Parameters  : char * command : Command that gave the error
 *	       : int eno	: Error Number
 *
 * Description : The function takes a command and an error number and displays the error message.
 *		 List of Errors with Error Number (eno)
 *		 1.  getpath used with parameters
 *		 2.  setpath used with no or more than one parameters
 *		 3.  cd used with more than one parameter
 *		 4.  exit used with parameters
 *		 5.  !-<no> used with parameters
 *		 6.  !-<no> used with a <no> that doesnt exist in the history array
 *		 7.  !! used with parameters
 *		 8.  !! used when the history array is empty
 *		 9.  !<no> used with parameters
 *		 10. !<no> used with a <no> that doesnt exist in the history array
 *		 11. history used with parameters
 *		 12. alias used with either one parameter or more than two parameters
 *		 13. alias name and alias command are indentical
 *		 14. alias array is full
 *		 15. unalias used with more than one or no parameters
 *		 16. unalias used on an alias name that does not exist
 *		 17. alias used with alias array is empty.
 *		 18. alias name exists - overriding message
 *		 19. .aliases file has incorrect format
 *		 20. .hist_list file has incorrect format
 *		 21. circular dependency has been detected
 *		 22. maximum substitutions have been performed for aliasing
 *		 23. !<no> used when the history array is empty
 *		 24. !-<no> used when the history array is empty
 *		 25. The current directory is not Home
 *				 
 * Return Type : void
 *
 */
void errorMessage(char * command, int eno){
	switch(eno){
			
		case 1: fprintf(stderr, "%s : %s", command , "Invalid Parameters\n");
			printf("Please use getpath without parameters. Use: getpath.\n");
			break;

		case 2: fprintf(stderr, "%s : %s", command , "Invalid Parameters\n");
			printf("Please use setpath with only one parameter. Use: setpath <path>.\n");
			break;

		case 3: fprintf(stderr, "%s : %s", command , "Invalid Parameters\n");
			printf("Please use cd with one parameter or no parameters. Use: cd or cd <directory>.\n");
			break;

		case 4: fprintf(stderr, "%s : %s", command , "Invalid Parameters\n");
			printf("Please use exit without any parameters. Use: exit.\n");
			break;
	
		case 5: fprintf(stderr, "%s : %s", command , "Invalid Parameters\n");
			printf("Please use !-<no> without any parameters. Use: !-<no>.\n");
			break;

		case 6: fprintf(stderr, "%s : %s", command , "Number does not exist in History\n");
			printf("Please use !-<no> with a <no> that exists in history. Use: !-<no>.\n");
			break;

		case 7: fprintf(stderr, "%s : %s", command , "Invalid Parameters\n");
			printf("Please use !! without any parameters. Use: !!\n");
			break;

		case 8: fprintf(stderr, "%s : %s", command , "Empty History\n");
			printf("Please enter at least one command to use !!. Use: !!\n");
			break;

		case 9: fprintf(stderr, "%s : %s", command , "Invalid Parameters\n");
			printf("Please use !<no> without any parameters. Use: !<no>.\n");
			break;
		
		case 10: fprintf(stderr, "%s : %s", command , "Number does not exist in History\n");
			printf("Please use !<no> with a <no> that exists in history. Use: !<no>.\n");
			break;

		case 11: fprintf(stderr, "%s : %s", command , "Invalid Parameters\n");
			printf("Please use history without any parameters. Use: history.\n");
		  	break;

		case 12: fprintf(stderr, "%s : %s", command , "Invalid Parameters\n");
			printf("Please use alias with two parameters or use alias without any parameters to print aliases. Use: alias <name> <command> or alias.\n");
		  	break;

		case 13: fprintf(stderr, "%s : %s", command , "Invalid Alias\n");
			printf("Alias name must not be identical to command.\n");
			break;

		case 14:fprintf(stderr, "%s : %s", command, "Aliases are full\n");
			printf("Please remove an alias before adding a new alias.\n");
			break;
			
		case 15: fprintf(stderr, "%s : %s", command , "Invalid Parameters\n");
			printf("Please use unalias with one parameter. Use: unalias <name>.\n");
			break;

		case 16: fprintf(stderr, "%s : %s", command , "Alias does not exist\n");
			printf("Please use unalias on an alias name that exists. Use: unalias <name>.\n");
			break;

		case 17: fprintf(stderr, "%s : %s", command , "No Alias created yet\n");
			printf("Please add an alias before using alias. Use: alias.\n");
			break;

		case 18: fprintf(stderr, "%s : %s", command , "Alias name already exists\n");
			printf("Alias being overridden with existing alias.\n");
			break;

		case 19: fprintf(stderr, "%s : %s", command , "Incorrect Format\n");
			printf(".aliases file has incorrect format. No aliases loaded.\n");
			break;

		case 20: fprintf(stderr, "%s : %s", command , "Incorrect Format\n");
			printf(".hist_list file has incorrect format. No history loaded.\n");
			break;

		case 21: fprintf(stderr, "%s : %s", command , "Circular Dependency\n");
			printf("Circular Dependency Detected. Command not executed.\n");
			break;

		case 22: fprintf(stderr, "%s : %s", command , "Maximum Substitutions\n");
			printf("Maximum Substitutions Performed. Command not executed.\n");
			break;

		case 23: fprintf(stderr, "%s : %s", command , "Empty History\n");
			printf("Please enter at least one command to use !<no>. Use: !<no>\n");
			break;

		case 24: fprintf(stderr, "%s : %s", command , "Empty History\n");
			printf("Please enter at least one command to use !-<no>. Use: !-<no>\n");
			break;

		case 25: fprintf(stderr, "%s : %s", command , "Current Directory is not Home\n");
			printf("Current Directory is not Home. The directory is being changed to home to save/load .hist_list and .aliases file in right location.\n");
			break;

		default: printf("Invalid error number\n");
			break;

	
	}
}
