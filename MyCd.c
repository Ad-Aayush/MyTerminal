#include "myshell.h"

void cd(char query[], int validityQuery, char userName[])
{
    if (!isValidString(query))
        return;
    if (strncmp(query, "cd", 2))
        return;
    int index = 3;
    char HOME[MAXLEN];
    memset(HOME, '\0', MAXLEN);
    strcpy(HOME, "/home/");
    strcat(HOME, userName);
    // Implementation of cd command
    if (index != -1 && validityQuery == 0)
    {

        if (index >= strlen(query) || query[index] == '&') // Presence of '&' implies start of a new query
        {
            chdir(HOME);
        }
        else
        {

            char *path = (char *)malloc(MAXLEN * sizeof(char));
            for (int i = 0; i < MAXLEN; i++)
                path[i] = '\0';
            path = convertToString(query, &index);
            chdir(path);
        }
    }
}