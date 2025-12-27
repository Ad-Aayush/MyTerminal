#include "myshell.h"

void mygrep(char query[])
{
    // printf("%s",query);
    int index = -1;
    int n = strlen(query);
    char *pattern;
    char *first;

    int w = -1, I = -1, N = -1, C = -1; // for options
    for (int i = 0; i < n; i++)
    {
        if (query[i] == ' ' && query[i + 1] != '-')
        {
            index = i + 1;
            // printf("\n%s\n%c\n", query, query[index]);
            pattern = convertToString(query, &index);
            break;
        }
    }
    if (index == -1)
    {
        printf("Invalid Syntax\n");
        return;
    }
    for (int i = index; i < n; i++)
    {
        if (query[i] == ' ' && query[i + 1] != '-')
        {
            index = i + 1;
            first = convertToString(query, &index);
            break;
        }
    }
    for (int i = 0; i < n; i++)
    {
        if (query[i] == '-')
        {
            i++;
            for (; i < n; i++) // consider case of wrong option ie ls -allll
            {
                if (query[i] == 'i')
                    I = i;
                else if (query[i] == 'w')
                    w = i;
                else if (query[i] == 'n')
                    N = i;
                else if (query[i] == 'c')
                    C = i;
                else if (query[i] == ' ')
                    break;
                else
                {
                    printf("Invalid options\n");
                    return;
                }
            }
        }
    }
    if (I != -1)
    {
        for (int i = 0; pattern[i] != '\0'; i++)
            pattern[i] = tolower(pattern[i]);
    }
    pairStr fir;
    char cwd[MAXLEN];
    getcwd(cwd, MAXLEN);
    strcpy(fir.path, cwd);
    setStr(&fir, first, cwd);
    if (strlen(fir.name) == 0 || strlen(fir.path) == 0)
    {
        return;
    }
    // printf("%s\n%s\n", first, pattern);
    chdir(fir.path);
    FILE *fp = fopen(fir.name, "r");
    if (fp == NULL)
    {

        chdir(cwd);
        printf("Can't open file\n");
        return;
    }
    char str[MAXLEN];
    for (int i = 0; i < MAXLEN; i++)
        str[i] = '\0';
    int cnt = 0;
    int line[MAXLEN] = {0};
    int start[MAXLEN] = {0};
    int startCnt = 0;
    int cntLine = 0;
    // printf("\n%d\n", C);
    int printable = 0;
    int non = 0;
    while (fgets(str, MAXLEN, fp) != NULL)
    {

        for (int i = 0; str[i] != '\0'; i++)
        {
            if (isprint(str[i]) || str[i] == '\n' || str[i] == '\t')
                printable++;
            else
            {
                non++;
            }
        }
    }
    fclose(fp);

    // printf("\n%d\n", I);

    fp = fopen(fir.name, "r");
    if (fp == NULL)
    {
        printf("Can't open file\n");
        chdir(cwd);
        return;
    }
    if (non != 0 && printable / non <= 10)
    {
        printf("Can't read file\n");
        chdir(cwd);
        return;
    }
    while (fgets(str, MAXLEN, fp) != NULL)
    {
        cnt++;
        if (isValidString(str) == 0 || str[0] == '\n')
            continue;
        // printf("%s\n", str);
        int m = strlen(str);
        if (I != -1)
        {
            for (int i = 0; str[i] != '\0'; i++)
                str[i] = tolower(str[i]);
        }
        for (int i = 0; str[i] != '\0'; i++)
        {
            int j = 0;
            if (w == -1)
            {
                if (str[i] == pattern[0] || pattern[0] == '.')
                {
                    bool isPat = 1;

                    // printf("\n%s\n", str);

                    for (j = 0; j < strlen(pattern); j++)
                    {
                        if (pattern[j] == '.' && i + j < strlen(str))
                            continue;
                        if (str[i + j] != pattern[j])
                        {
                            isPat = 0;
                            break;
                        }
                    }
                    if (isPat)
                    {
                        line[cntLine++] = cnt;
                        start[startCnt++] = i;
                        i = i + j - 1;
                    }
                }
            }
            else
            {
                if (!((isalnum(str[i - 1])) || (isalnum(str[i + strlen(pattern)]))))
                {
                    bool isPat = 1;

                    // printf("\n%s\n", str);

                    for (j = 0; j < strlen(pattern); j++)
                    {

                        if (str[i + j] != pattern[j])
                        {
                            if (pattern[j] == '.' && i + j < strlen(str))
                                continue;
                            isPat = 0;
                            break;
                        }
                    }
                    if (isPat)
                    {
                        line[cntLine++] = cnt;
                        start[startCnt++] = i;
                        i = i + j - 1;
                    }
                }
            }
        }
    }
    fclose(fp);

    // printf("\n%d\n", I);

    fp = fopen(fir.name, "r");
    if (fp == NULL)
    {
        printf("Can't open file\n");
        chdir(cwd);
        return;
    }
    // printf("\n%d\n", line[0]);
    int fLine = 0;
    cnt = 0;
    //  printf("%d\n\n", C);
    int answer = 0;
    // printf("aaaaa");
    if (cntLine != 0)
    {
        while (fgets(str, MAXLEN, fp) != NULL)
        {
            cnt++;
            if (isValidString(str) == 0 || str[0] == '\n')
                continue;
            if (cnt == line[fLine])
            {
                answer++;
                if (N != -1 && C == -1)
                    printf("%d:", cnt);
                int i = 0;
                while (cnt == line[fLine])
                {
                    // printf("%d: ", cnt);
                    for (; i < start[fLine] + strlen(pattern); i++)
                    {
                        if (i == start[fLine])
                        {
                            if (C == -1)
                                printf(red);
                        }
                        if (C == -1)
                            printf("%c", str[i]);
                    }
                    if (C == -1)
                        printf(reset);

                    fLine++;
                }
                for (; str[i] != '\0'; i++)
                {
                    if (C == -1)
                        if (str[i] != '\n')
                            printf("%c", str[i]);
                }
                if (C == -1)
                    printf("\n");
            }
        }
    }
    else
        printf("No Matches\n");
    if (C != -1)
        printf("%d\n", answer);
    fclose(fp);
    free(first);
    free(pattern);
    chdir(cwd);
}