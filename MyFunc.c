#include "myshell.h"

// Checks whether a string is just empty spaces(returns false in that case), returns true otherwise
bool isValidString(char query[])
{
    for (int i = 0; i < strlen(query); i++)
        if (query[i] != ' ')
            return true;

    return false;
}

char *trim(char a[])
{
    int n = strlen(a);
    char *new = malloc(MAXLEN * sizeof(char));
    memset(new, '\0', MAXLEN);
    int k = 0;
    bool first = 0;
    for (int i = 0; i < n; i++)
    {
        if (a[i] == '"' || a[i] == '\'')
        {
            new[k++] = a[i];
            char breakChar = a[i++];
            for (; i < n; i++)
            {
                new[k++] = a[i];
                if (a[i] == breakChar)
                    break;
            }
        }
        else if (first == 0 && a[i] != ' ')
        {
            first = 1;
            new[k++] = a[i];
        }
        else if (first == 1 && (a[i] != ' ' || (a[i + 1] != ' ' || i + 1 == n)))
        {
            new[k++] = a[i];
        }
    }
    if (new[k - 1] == ' ')
        new[k - 1] = '\0';
    else
        new[k] = '\0';
    return new;
}

// for cd command, outputs a string containing path(content followed by cd), covers cases like
// cd 'Folder 1', cd folder1, cd "Folder 2"
char *convertToString(char query[], int *index)
{
    char *address = (char *)malloc(MAXLEN * sizeof(char));
    int k = 0;
    char breakChar = ' ';
    if (*index > strlen(query))
        return "";
    if (query[*index] == '"') //
        breakChar = query[(*index)++];
    else if (query[*index] == '\'')
        breakChar = query[(*index)++];

    for (; *index < strlen(query); (*index)++)
        if (query[*index] != breakChar)
            address[k++] = query[*index];
        else
            break;
    // printf("inFuc:%d\n",*index);
    address[k++] = '\0';
    return address;
}

// Checks the initial path of current directory,returns true
// if starting is /home/username or the home folder a '~'
// will be used to represent the home folder
bool checkHome(char cwd[], char userName[])
{
    char *homePath = (char *)malloc(MAXLEN * sizeof(char));
    strcpy(homePath, "/home/");
    int n = strlen(homePath);

    for (int i = 0; i < strlen(userName); i++)
        homePath[n++] = userName[i];

    homePath[n] = '\0';

    if (strlen(cwd) < strlen(homePath))
        return false;

    for (int i = 0; i < strlen(homePath); i++)
        if (homePath[i] != cwd[i])
            return false;

    return true;
}

// Checks the presence of home folder in path, assign's the home folder(if present) '~'
void setDir(char dir[], char userName[])
{
    char cwd[MAXLEN];
    getcwd(cwd, MAXLEN);
    if (checkHome(cwd, userName))
    {
        int lenUptoHome = 6 + strlen(userName);
        int lenDir = 1;
        dir[0] = '~';

        for (int i = lenUptoHome; i < strlen(cwd); i++)
            dir[lenDir++] = cwd[i];

        dir[lenDir++] = '$';
        dir[lenDir++] = ' ';
        dir[lenDir] = '\0';
    }
    else
    {
        strcpy(dir, cwd);
        int lenDir = strlen(cwd);
        dir[lenDir++] = '$';
        dir[lenDir++] = ' ';
        dir[lenDir] = '\0';
    }
}

// Assign's arg the concatinated string to be printed each time that is (red)username@hostname:(cyan)~/(white ahed)$
char *setArg(char userName[], char hostName[], char dir[])
{
    char *arg = (char *)malloc(sizeof(char) * MAXLEN);
    int k = 0;
    char r[] = red;
    char c[] = cyan;
    char re[] = reset;
    for (int i = 0; i < strlen(r); i++)
        arg[k++] = r[i];
    for (int i = 0; i < strlen(userName); i++)
        arg[k++] = userName[i];
    arg[k++] = '@';
    for (int i = 0; i < strlen(hostName); i++)
        arg[k++] = hostName[i];
    arg[k++] = ':';
    for (int i = 0; i < strlen(c); i++)
        arg[k++] = c[i];
    for (int i = 0; i < strlen(dir) - 2; i++)
        arg[k++] = dir[i];
    for (int i = 0; i < strlen(re); i++)
        arg[k++] = re[i];
    arg[k++] = '$';
    arg[k++] = ' ';
    arg[k] = '\0';

    return arg;
}

void printMonth(int t)
{
    switch (t)
    {
    case 1:
        printf("Jan");
        break;
    case 2:
        printf("Feb");
        break;
    case 3:
        printf("Mar");
        break;
    case 4:
        printf("Apr");
        break;
    case 5:
        printf("May");
        break;
    case 6:
        printf("Jun");
        break;
    case 7:
        printf("Jul");
        break;
    case 8:
        printf("Aug");
        break;
    case 9:
        printf("Sep");
        break;
    case 10:
        printf("Oct");
        break;
    case 11:
        printf("Nov");
        break;
    case 12:
        printf("Dec");
        break;
    }
}
int nameCmp(const void *a, const void *b)
{
    return strcmp(((file *)a)->name, ((file *)b)->name);
}

int sizeCmp(const void *a, const void *b)
{
    if (((file *)a)->size < ((file *)b)->size)
        return 1;
    else if (((file *)a)->size > ((file *)b)->size)
        return -1;
    else
        return 0;
}

int timeCmp(const void *a, const void *b)
{
    if (((file *)a)->time < ((file *)b)->time)
        return 1;
    else if (((file *)a)->time > ((file *)b)->time)
        return -1;
    else
        return 0;
}
void printPer(unsigned int per)
{
    printf((S_ISDIR(per)) ? "d" : "-");
    printf((per & S_IRUSR) ? "r" : "-");
    printf((per & S_IWUSR) ? "w" : "-");
    printf((per & S_IXUSR) ? "x" : "-");
    printf((per & S_IRGRP) ? "r" : "-");
    printf((per & S_IWGRP) ? "w" : "-");
    printf((per & S_IXGRP) ? "x" : "-");
    printf((per & S_IROTH) ? "r" : "-");
    printf((per & S_IWOTH) ? "w" : "-");
    printf((per & S_IXOTH) ? "x" : "-");
}
void printUser(unsigned int user)
{
    struct passwd *pws;
    pws = getpwuid(user);
    printf(" %15s", pws->pw_name);
}

void printGroup(unsigned int user)
{
    struct group *pws;
    pws = getgrgid(user);
    printf(" %15s", pws->gr_name);
}

void setStr(pairStr *pair, char arg[], char cwd[])
{
    int n = strlen(arg);
    if (chdir(arg) != -1)
    {
        strcpy(pair->path, arg);
        chdir(cwd);
        return;
    }
    int indAddress = -1;
    for (int i = 0; i < n; i++)
    {
        if (arg[i] == '/')
        {
            indAddress = i;
        }
    }

    if (indAddress == -1)
    {
        int k = 0;
        if (chdir(arg) == -1)
        {
            strcpy(pair->name, arg);
        }
        else
        {
            strcpy(pair->path, arg);
            chdir(cwd);
        }
    }
    else
    {
        int k = 0;
        int l = 0;
        for (int i = 0; i < indAddress; i++)
        {
            pair->path[k++] = arg[i];
        }
        pair->path[k] = '\0';
        for (int i = indAddress + 1; i < n; i++)
        {
            pair->name[l++] = arg[i];
        }
        pair->name[l] = '\0';
    }
    if (chdir(pair->path) != -1)
    {
        chdir(cwd);
        return;
    }
    else
    {
        printf("Invalid Argument\n");
        pair->path[0] = '\0';
        pair->name[0] = '\0';
        return;
    }
}
void printFunction(process *p, long long time, char self[], int a, int e, int f, int d, unsigned long long total_cpu_time)
{
    int hours = time / 3600;          // Calculate hours
    int minutes = (time % 3600) / 60; // Calculate minutes
    int seconds = time % 60;          // Calculate seconds
    if (a == -1 && e == -1 && d == -1)
    {
        if (strcmp(self, p->tty) == 0)
        {
            if (f == -1)
            {

                printf("%6lld %6s", p->pid, p->tty);
                printf(" %02d:%02d:%02d", hours, minutes, seconds);
                printf(" %s\n", p->cmd);
            }
            else
            {
                struct passwd *pws;
                pws = getpwuid(p->uid);
                if (pws == NULL)
                    return;
                if (strlen(pws->pw_name) > 7)
                {
                    pws->pw_name[7] = '+';
                    pws->pw_name[8] = '\0';
                }
                if (strlen(p->cmdFull) > 50)
                {
                    p->cmdFull[50] = '\0';
                }
                int C = ((double)p->uTime / total_cpu_time) * 100;
                struct tm *tim = localtime(&p->STime);
                if (strlen(p->cmdFull) > 0)
                    printf("%-9s   %6lld  %6lld   %1d %02d:%02d%7s    %02d:%02d:%02d %s\n", pws->pw_name, p->pid, p->ppid, C, tim->tm_hour, tim->tm_min, p->tty, hours, minutes, seconds, p->cmdFull);
                else
                    printf("%-9s   %6lld  %6lld   %1d %02d:%02d%7s    %02d:%02d:%02d %s\n", pws->pw_name, p->pid, p->ppid, C, tim->tm_hour, tim->tm_min, p->tty, hours, minutes, seconds, p->cmd);
            }
        }
    }
    else if (e != -1)
    {
        if (f == -1)
        {
            printf("%6lld %6s", p->pid, p->tty);
            printf(" %02d:%02d:%02d", hours, minutes, seconds);
            printf(" %s \n", p->cmd);
        }
        else
        {
            struct passwd *pws;
            pws = getpwuid(p->uid);
            if (pws == NULL)
                return;
            if (strlen(pws->pw_name) > 7)
            {
                pws->pw_name[7] = '+';
                pws->pw_name[8] = '\0';
            }
            if (strlen(p->cmdFull) > 50)
            {
                p->cmdFull[50] = '\0';
            }
            int C = ((double)(p->uTime / sysconf(_SC_CLK_TCK)) / total_cpu_time) * 100;
            struct tm *tim = localtime(&p->STime);
            if (strlen(p->cmdFull) > 0)
                printf("%-9s   %6lld  %6lld   %1d %02d:%02d%7s    %02d:%02d:%02d %s\n", pws->pw_name, p->pid, p->ppid, C, tim->tm_hour, tim->tm_min, p->tty, hours, minutes, seconds, p->cmdFull);
            else
                printf("%-9s   %6lld  %6lld   %1d %02d:%02d%7s    %02d:%02d:%02d %s\n", pws->pw_name, p->pid, p->ppid, C, tim->tm_hour, tim->tm_min, p->tty, hours, minutes, seconds, p->cmd);
        }
    }
    else if (d != -1)
    {
        if (p->sid != p->pid)
        {
            if (f == -1)
            {
                printf("%6lld %6s", p->pid, p->tty);
                printf(" %02d:%02d:%02d", hours, minutes, seconds);
                printf(" %s\n", p->cmd);
            }
            else
            {
                struct passwd *pws;
                pws = getpwuid(p->uid);
                if (pws == NULL)
                    return;
                if (strlen(pws->pw_name) > 7)
                {
                    pws->pw_name[7] = '+';
                    pws->pw_name[8] = '\0';
                }
                int C = ((double)(p->uTime / sysconf(_SC_CLK_TCK)) / total_cpu_time) * 100;
                struct tm *tim = localtime(&p->STime);
                if (strlen(p->cmdFull) > 50)
                {
                    p->cmdFull[50] = '\0';
                }
                if (strlen(p->cmdFull) > 0)
                    printf("%-9s   %6lld  %6lld   %1d %02d:%02d%7s    %02d:%02d:%02d %s\n", pws->pw_name, p->pid, p->ppid, C, tim->tm_hour, tim->tm_min, p->tty, hours, minutes, seconds, p->cmdFull);
                else
                    printf("%-9s   %6lld  %6lld   %1d %02d:%02d%7s    %02d:%02d:%02d %s\n", pws->pw_name, p->pid, p->ppid, C, tim->tm_hour, tim->tm_min, p->tty, hours, minutes, seconds, p->cmd);
            }
        }
    }
    else if (a != -1)
    {
        if (p->sid != p->pid && strcmp(p->tty, "?") != 0)
        {
            if (f == -1)
            {
                printf("%6lld %6s", p->pid, p->tty);
                printf(" %02d:%02d:%02d", hours, minutes, seconds);
                printf(" %s\n", p->cmd);
            }
            else
            {
                struct passwd *pws;
                pws = getpwuid(p->uid);
                if (pws == NULL)
                    return;
                if (strlen(pws->pw_name) > 7)
                {
                    pws->pw_name[7] = '+';
                    pws->pw_name[8] = '\0';
                }
                if (strlen(p->cmdFull) > 50)
                {
                    p->cmdFull[50] = '\0';
                }
                int C = ((double)(p->uTime / sysconf(_SC_CLK_TCK)) / total_cpu_time) * 100;
                struct tm *tim = localtime(&p->STime);
                if (strlen(p->cmdFull) > 0)
                    printf("%-9s   %6lld  %6lld   %1d %02d:%02d%7s    %02d:%02d:%02d %s\n", pws->pw_name, p->pid, p->ppid, C, tim->tm_hour, tim->tm_min, p->tty, hours, minutes, seconds, p->cmdFull);
                else
                    printf("%-9s   %6lld  %6lld   %1d %02d:%02d%7s    %02d:%02d:%02d %s\n", pws->pw_name, p->pid, p->ppid, C, tim->tm_hour, tim->tm_min, p->tty, hours, minutes, seconds, p->cmd);
            }
        }
    }
}