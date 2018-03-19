
/* Define tokens to remove constants. */
#define BUFFER_SIZE 512
#define TOKEN_SIZE 50
#define HISTORY_SIZE 20
#define ALIAS_SIZE 10
#define COMMAND_DETECT_SIZE 20

/* Stores the system path on entry to the shell. */
char* path;

/* history_count points to the first empty index in the history array. */
int history_count = 0;

/* alias_count points to the first empty index in the aliases array. */
int alias_count = 0;

/* command_detect_count points to the first empty index in the command_detect array. */
int command_detect_count = 0;

/* Struct for history array storing commandNumber and command. */
typedef struct {
	int commandNumber;
	char string[BUFFER_SIZE];
}Hist;

/* Struct for aliases array storing aliasName and aliasCommand. */
typedef struct {
	char aliasName[BUFFER_SIZE];
	char aliasCommand[BUFFER_SIZE];
}Alias;

/* Declaring history array of struct type. */
Hist history[HISTORY_SIZE];

/* Declaring aliases array of struct type. */
Alias aliases[ALIAS_SIZE];

/* Declaring command_detect array of char * type. */
char * command_detect[COMMAND_DETECT_SIZE];


void parseInput(char *line);
void runShell();
void readInput(char * line);
void externalCommandexec(char * commands[]);
void startShell();
void endShell();
bool checkifLineIsOnlySpace(char * line);
void addAlias(char * commands[]);
void printAliases();
bool checkEmptyAlias(int index);
void removeAlias(char *commands[]);
int aliasIndexCheck(char * name);
void adjustAliasArray(int index);
bool isAliasFull();
bool isAliasEmpty();
void clearAliasesArray();
bool wasAliasAdded(char * commands[]);
bool wasAliasRemoved(char * commands[], int index);
void replaceAlias(char * commands[]);
void commandCheck(char * commands[]);
void addToCommandDetect(char * command);
void clearCommandDetect();
bool checkCircular(char * command);
void printCommandDetect();
bool isCommandDetectFull();
void errorMessage(char * command, int eno);
void getpath();
void setpath(char *path);
void changeDirectory(char *commands[]);
void restorePath();
char * getenvHome();
void getenvPath();
bool isDirectoryHome();
void setHome(char * home);
void addToHistoryArray(char * command);
void adjustHistoryArray(char * command);
void printHistory();
void executeHistory(int commandNum);
bool isHistoryFull();
bool commandExists(int commandno);
int getNum(char *command, int index);
int getPow(int number, int exponent);
bool isDigit(char * commands[]);
bool isHistoryEmpty();
void saveAlias();
void loadAlias();
void saveHistory();
void loadHistory();
void addToAliasArray(char * line);
bool checkAliasesContent(char * line);










