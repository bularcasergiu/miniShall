// C Program to design a shell in Linux 
#include<stdio.h> 
#include<string.h> 
#include<stdlib.h> 
#include<unistd.h> 
#include<sys/types.h> 
#include<sys/wait.h> 
#include <readline/readline.h>
#include <readline/history.h>

#include <fcntl.h>

#define MAXCOM 1000 // max number of letters to be supported 
#define MAXLIST 100 // max number of commands to be supported 
  
// Clearing the shell using escape sequences 
#define clear() printf("\033[H\033[J") 
  
// Greeting shell during startup 
void init_shell() 
{ 
    clear(); 
    printf("\n\n\n\n******************"
        "************************"); 
    printf("\n\n\n\t****MY SHELL****"); 
    printf("\n\n\t-USE AT YOUR OWN RISK-"); 
    printf("\n\n\n\n*******************"
        "***********************"); 
    char* username = getenv("USER"); 
    printf("\n\n\nUSER is: @%s", username); 
    printf("\n"); 
    sleep(1); 
    clear(); 
} 
  
// Function to take input 
int takeInput(char* str) 
{ 
    char* buf; 
  
    buf = readline("\n>>> "); 
    if (strlen(buf) != 0) { 
        add_history(buf); 
        strcpy(str, buf); 
        return 0; 
    } else { 
        return 1; 
    } 
} 
  
// Function to print Current Directory. 
void printDir() 
{ 
    char cwd[1024]; 
    getcwd(cwd, sizeof(cwd)); 
    printf("\nDir: %s", cwd); 
} 

  
// Help command builtin 
void openHelp() 
{ 
    puts("\n***WELCOME TO MY SHELL HELP***"
        "\n-Use the shell at your own risk..."
        "\nList of Commands supported:"
        "\n>cd"
        "\n>pwd"
        "\n>exit"
); 
  
    return; 
} 
  
// Function to execute builtin commands 
int ownCmdHandler(char** parsed) 
{ 
    int NoOfOwnCmds = 5, i, switchOwnArg = 0; 
    char* ListOfOwnCmds[NoOfOwnCmds]; 
    char* username; 
  
    ListOfOwnCmds[0] = "exit"; 
    ListOfOwnCmds[1] = "cd"; 
    ListOfOwnCmds[2] = "pwd"; 
	ListOfOwnCmds[3] = "help"; 
	ListOfOwnCmds[4] = "hello"; 
  
    for (i = 0; i < NoOfOwnCmds; i++) { 
        if (strcmp(parsed[0], ListOfOwnCmds[i]) == 0) { 
            switchOwnArg = i + 1; 
            break; 
        } 
    } 
  
    switch (switchOwnArg) { 
    case 1: 
        printf("\nGoodbye\n"); 
        exit(0); 
    case 2: 
        chdir(parsed[1]); 
        return 1; 
    case 3: 
        printDir();
        return 1; 
    case 4: 
        openHelp(); 
        return 1; 
    default: 
        break; 
    } 
  
    return 0; 
} 

// function for finding ">"
int parseOutRedirect(char* str, char** strpiped) 
{ 
    int i; 
    for (i = 0; i < 2; i++) { 
        strpiped[i] = strsep(&str, ">"); 
        if (strpiped[i] == NULL) 
            break; 
    } 
  
    if (strpiped[1] == NULL) 
        return 0; // returns zero if no ">" is found. 
    else { 
        return 1; 
    } 
} 
  
  
// function for parsing command words 
void parseSpace(char* str, char** parsed) 
{ 
    int i; 
  
    for (i = 0; i < MAXLIST; i++) { 
        parsed[i] = strsep(&str, " "); 
  
        if (parsed[i] == NULL) 
            break; 
        if (strlen(parsed[i]) == 0) 
            i--; 
    } 
} 
  
int writeInFile(char** parsed, char** fileNmae){
	    int out = open(fileNmae[0], O_RDWR|O_CREAT|O_APPEND, 0600);
	    if (-1 == out) { perror("opening file"); return 255; }

	    int save_out = dup(fileno(stdout));

	    if (-1 == dup2(out, fileno(stdout))) { perror("cannot redirect stdout"); return 255; }

	    ownCmdHandler(parsed);

	    fflush(stdout); close(out);

	    dup2(save_out, fileno(stdout));

	    close(save_out);
  return 0;

}

int processString(char* str, char** parsed, char** parsedpipe) 
{ 
  
    char* strpiped[2]; 
    int piped = 0; 
    int outRed = 0;
  
    outRed = parseOutRedirect(str, strpiped);
  
    if (outRed) { 
        parseSpace(strpiped[0], parsed); 
        parseSpace(strpiped[1], parsedpipe); 
    } else { 
  
        parseSpace(str, parsed); 
    } 
  
    if (!outRed) 
        return 1; 
    else
        return 1 + outRed; 
} 
  
int main() 
{ 
    char inputString[MAXCOM], *parsedArgs[MAXLIST]; 
    char* parsedArgsPiped[MAXLIST]; 
    int execFlag = 0; 
    init_shell(); 
  
    while (1) { 
        // print shell line 
        printDir(); 
        // take input 
        if (takeInput(inputString)) 
            continue; 
        // process 
        execFlag = processString(inputString, 
        parsedArgs, parsedArgsPiped); 
        // execflag returns zero if there is no command 
        // or it is a builtin command, 
        // 1 if it is a simple command 
        // 2 if it is including a pipe. 
  
        // execute 
        if (execFlag == 1) 
 		 ownCmdHandler(parsedArgs);
        if (execFlag == 2) 
		writeInFile(parsedArgs, parsedArgsPiped);
    } 
    return 0; 
} 
