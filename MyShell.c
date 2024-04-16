#include "myshell.h"

int main()
{
    char *userName = (char *)malloc(MAXLEN * sizeof(char));
    userName = getlogin();

    if (userName == NULL) {
        userName = "aayush_ad";
    }

    char *hostname = (char *)malloc(MAXLEN * sizeof(char));
    gethostname(hostname, MAXLEN);

    char *query = (char *)malloc(MAXLEN * sizeof(char));
    // char query[MAXLEN];
    memset(query, '\0', MAXLEN);
    char dir[MAXLEN];
    setDir(dir, userName);
    char *arg = setArg(userName, hostname, dir);
    char last[MAXLEN];
    memset(last, '\0', MAXLEN);
    while (((query = readline(arg))) != NULL)
    {
        if (strcmp(query, "exit") == 0)
        {
            printf("Session Over\n");
            return 0;
        }
        if (strlen(query) > 0 && isValidString(query) && strcmp(last, query))
            add_history(query);
        int validityQuery = -1;
        query = trim(query);
        if (strncmp(query, "help", 4) == 0)
        {
            help(query);
        }
        else if (strncmp(query, "myls", 4) == 0)
        {
            myls(query);
        }
        else if (strncmp(query, "mycp", 4) == 0)
        {
            mycp(query);
        }
        else if (strncmp(query, "mymv", 4) == 0)
        {
            mymv(query);
        }
        else if (strncmp(query, "myps", 4) == 0)
        {
            myps(query);
        }
        else if (strncmp(query, "mygrep", 6) == 0)
        {
            mygrep(query);
        }
        else
            validityQuery = system(query); // Stores the return value of system function(that is 0 if command is valid)
        cd(query, validityQuery, userName);

        setDir(dir, userName);
        arg = setArg(userName, hostname, dir);
        strcpy(last, query);
    }
}