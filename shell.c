#include <stdio.h>
#include <string.h>
#include <stdlib.h>

void displayPrompt();
void readInput();
void terminateCheck();
void parseInput();

int terminate = 0;

int main(void){
	displayPrompt();
	return 0;
}

void displayPrompt(){
	while (terminate == 0){
		printf(">");
		readInput();
	}
	
			
}

void readInput(){
	char input[512];
    if (fgets(input,512,stdin)==NULL){
        exit(0);
    }
	terminateCheck(input);
	if(terminate == 1){
		exit(0);
	}
	parseInput(input);

}

void terminateCheck(char *input){
    if(strncmp(input, "exit", 4)==0){
        terminate=1;
    }else{
        terminate=0;
    }
}

void parseInput(char *input){
	char del[2]=" ";
	char *token;
	token=strtok(input,del);
	printf("Tokens\n");
	while(token != NULL){
		printf("%s, " ,token);
		token = strtok(NULL,del);
	}
}