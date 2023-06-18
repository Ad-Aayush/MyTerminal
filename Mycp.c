#include "myshell.h"

void mycp(char query[])
{
    char *cwd = malloc(sizeof(char) * MAXLEN);
    cwd = getcwd(cwd, MAXLEN);
    int index = 5;
    int n = strlen(query);
    char y_n = 'y';
    bool firstFound = 0;
    char first[MAXLEN], second[MAXLEN];
    int k = 0, l = 0;
    int indI = -1, indN = -1;
    for (int i = index - 1; i < n; i++)
    {
        if (query[i] == ' ' && query[i + 1] != '-' && firstFound == 0)
        {
            firstFound = 1;
            i++;
            for (; i < n; i++)
            {
                if (query[i] != ' ')
                    first[k++] = query[i];
                else
                {
                    first[k] = '\0';
                    break;
                }
            }
        }
        if (query[i] == ' ' && query[i + 1] != '-' && firstFound == 1)
        {
            i++;
            for (; i < n; i++)
            {
                if (query[i] != ' ')
                    second[l++] = query[i];
                else
                {
                    second[l] = '\0';
                    break;
                }
            }
        }
    }
    int indU = -1, indV = -1;
    for (int i = index; i < n; i++)
    {
        if (query[i] == '-')
        {
            for (; i < n; i++) // consider case of wrong option ie ls -allll
            {
                if (query[i] == 'i')
                    indI = i;
                else if (query[i] == 'n')
                    indN = i;
                else if (query[i] == 'u')
                    indU = i;
                else if (query[i] == 'v')
                    indV = i;
                else if (query[i] == ' ')
                    break;
            }
        }
    }
    first[k] = '\0';
    second[l] = '\0';
    char *userName = (char *)malloc(MAXLEN * sizeof(char));
    userName = getlogin();
    char HOME[MAXLEN];
    memset(HOME, '\0', MAXLEN);
    strcpy(HOME, "/home/");
    strcat(HOME, userName);
    if (second[0] == '/')
    {
        if (strncmp(second, HOME, strlen(HOME)))
        {
            printf("Cannot create regular file, Permission denied\n");
            return;
        }
    }

    pairStr fir, sec;
    strcpy(fir.path, cwd);
    strcpy(sec.path, cwd);
    setStr(&fir, first, cwd);
    if (strlen(fir.name) == 0)
    {
        printf("Invalid Argument\n");
        chdir(cwd);
        return;
    }
    strcpy(sec.name, fir.name);
    setStr(&sec, second, cwd);
    if ((((strlen(fir.name) == 0 && strlen(fir.path) == 0)) || ((strlen(sec.name) == 0) && (strlen(sec.path) == 0))))
    {
        chdir(cwd);
        return;
    }
    // printf("\np%s\nn%s\np%s\nn%s\n", fir.path, fir.name, sec.path, sec.name);
    FILE *fp, *fl;
    chdir(fir.path);
    char fcwd[MAXLEN];
    getcwd(fcwd, MAXLEN);
    if (fcwd[0] == '/')
    {
        if (strncmp(fcwd, HOME, strlen(HOME)))
        {
            chdir(cwd);
            printf("Cannot create regular file, Permission denied\n");
            return;
        }
    }
    bool isPre = 0;
    time_t firstTime;
    struct dirent *filptr;

    DIR *current = opendir(".");
    while ((filptr = readdir(current)) != NULL)
    {
        if (!strcmp(filptr->d_name, fir.name))
        {
            struct stat my;
            stat(filptr->d_name, &my);
            firstTime = my.st_mtime;
            isPre = 1;
        }
    }
    if (isPre == 0)
    {
        chdir(cwd);
        printf("No such file\n");
        return;
    }
    fp = fopen(fir.name, "rb");
    if (fp == NULL)
    {
        printf("Can't open file\n");
        chdir(cwd);
        return;
    }
    char chunks[1];
    struct stat mystat;
    stat(fir.name, &mystat);
    unsigned int per = mystat.st_mode;

    chdir(sec.path);
    char scwd[MAXLEN];
    getcwd(scwd, MAXLEN);
    if (scwd[0] == '/')
    {
        if (strncmp(scwd, HOME, strlen(HOME)))
        {
            chdir(cwd);
            printf("Cannot create regular file, Permission denied\n");
            return;
        }
    }
    bool already = 0;

    struct dirent *fileptr;
    current = opendir(".");

    if (current == NULL)
    {
        printf("ERROR: Could not open Current Directory\n");
        return;
    }
    time_t secondTime = __INT_MAX__;
    while ((fileptr = readdir(current)) != NULL)
    {
        if (strcmp(fileptr->d_name, sec.name) == 0)
        {
            struct stat my;
            stat(sec.name, &my);
            secondTime = my.st_mtime;
            already = 1;
        }
    }
    if (already && indI != -1 && indN < indI && indI > indU)
    {
        printf("File already exists do you want to overwrite?(Y/N)\n");
        char INP[MAXLEN];
        scanf("%[^\n]%*c", INP);
        strcpy(INP, trim(INP));
        y_n = INP[0];
        y_n = tolower(y_n);
    }
    else if (already && indN > indI && indN > indU)
    {
        chdir(cwd);
        return;
    }
    else if (already && firstTime <= secondTime && indU != -1)
    {
        chdir(cwd);
        return;
    }
    if (y_n != 'y')
    {
        chdir(cwd);
        return;
    }
    chdir(cwd);
    if (indV != -1)
    {
        if (chdir(second) != -1)
        {
            if (sec.path[strlen(sec.path) - 1] == '/')
                sec.path[strlen(sec.path) - 1] = '\0';
            printf("'%s' -> '%s/%s'\n", first, sec.path, sec.name);
            chdir(cwd);
        }
        else
        {
            printf("'%s' -> '%s'\n", first, second);
        }
    }
    chdir(cwd);
    chdir(sec.path);
    fl = fopen(sec.name, "wb");
    if (fl == NULL)
    {
        printf("Can't open file\n");
        chdir(cwd);
        return;
    }
    while (fread(chunks, 1, 1, fp) == 1)
    {
        fwrite(chunks, 1, 1, fl);
    }
    chmod(sec.name, per);
    chdir(cwd);
    fclose(fp);
    fclose(fl);
}