#include "myshell.h"

void myps(char query[])
{
    char save[MAXLEN];
    getcwd(save, MAXLEN);
    int n = strlen(query);
    int a = -1, e = -1, f = -1, d = -1;
    int k = 0;
    int fdi = open("/proc/self/fd/0", O_RDONLY);
    char *self = ttyname(fdi);
    char self2[MAXLEN];
    memset(self2, '\0', MAXLEN);
    strcpy(self2, self);
    FILE *stat_file = fopen("/proc/stat", "r");
    if (stat_file == NULL)
    {
        printf("Unable to open file\n");
        return;
    }
    unsigned long long total_cpu_time;
    fscanf(stat_file, "cpu %llu", &total_cpu_time);
    fclose(stat_file);
    long up_time;
    for (int i = 0; i < n; i++)
    {
        if (query[i] == '-')
        {
            i++;
            for (; i < n; i++) // consider case of wrong option ie ls -allll
            {
                if (query[i] == 'a')
                    a = i;
                else if (query[i] == 'e')
                    e = i;
                else if (query[i] == 'f')
                    f = i;
                else if (query[i] == 'd')
                    d = i;
                else if (query[i] == ' ')
                    break;
                else
                {
                    printf("Invalid Option\n");
                    return;
                }
            }
        }
    }
    DIR *proc = opendir("/proc");
    struct dirent *fileptr;
    chdir("/proc");
    if (proc == NULL)
    {
        printf("ERROR: Could not open Proc Directory\n");
        return;
    }
    char new2[MAXLEN];
    memset(new2, '\0', MAXLEN);
    for (int i = 5; i < strlen(self2); i++)
    {
        new2[i - 5] = self2[i];
    }
    new2[strlen(self2) - 5] = '\0';
    strcpy(self2, new2);
    int x = 0;
    // printf("%s\n", self2);
    if (f == -1)
        printf("   PID    TTY     TIME CMD\n");
    else
        printf("UID            PID    PPID   C STIME  TTY          TIME CMD\n");
    while ((fileptr = readdir(proc)) != NULL)
    {
        // printf("%s\n", self2);
        bool flag = 1;
        for (int i = 0; i < strlen(fileptr->d_name); i++)
        {
            if (!isdigit(fileptr->d_name[i]))
            {
                flag = 0;
                break;
            }
        }
        if (flag)
        {
            process *procInfo = malloc(sizeof(process));
            chdir(fileptr->d_name);
            FILE *file = fopen("stat", "r");
            if (file == NULL)
            {
                printf("Unable to open file\n");
                break;
            }
            fscanf(file, "%lld %[^)]%*c %c %lld %*d %lld %*d %*d %*u %*d %*d %*d %*d %lld %lld %lld %lld %*d %*d %*d %*d %ld %*d %llu %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %*d %d %d",
                   &procInfo->pid, procInfo->cmd, &procInfo->status, &procInfo->ppid, &procInfo->sid, &procInfo->uTime, &procInfo->sTime, &procInfo->cuttime, &procInfo->cstime, &procInfo->STime, &procInfo->rss, &procInfo->psr, &procInfo->c);
            memset(procInfo->tty, '\0', MAXLEN);
            procInfo->c = procInfo->sTime / sysconf(_SC_CLK_TCK);
            fclose(file);
            FILE *up = fopen("/proc/uptime", "r");
            if (up == NULL)
            {
                printf("Unable to open file\n");
                break;
            }
            fscanf(up, "%ld", &up_time);
            fclose(up);
            file = fopen("status", "r");
            if (file == NULL)
            {
                printf("Unable to open file\n");
                break;
            }
            char line[MAXLEN];
            memset(line, '\0', MAXLEN);
            while (fgets(line, sizeof(line), file) != NULL)
            {
                // Search for the line that starts with "Uid:"
                if (strncmp(line, "Uid:", 4) == 0)
                {
                    // Extract the UID from the line
                    sscanf(line, "Uid:\t%lld", &procInfo->uid);
                    break;
                }
            }
            fclose(file);
            file = fopen("cmdline", "r");
            if (file == NULL)
            {
                printf("Unable to open file\n");
            }
            fscanf(file, "%s", procInfo->cmdFull);
            fclose(file);
            unsigned long long tim = (procInfo->uTime + procInfo->sTime) / sysconf(_SC_CLK_TCK);
            int hours = tim / 3600;          // Calculate hours
            int minutes = (tim % 3600) / 60; // Calculate minutes
            int seconds = tim % 60;          // Calculate seconds
            char temp[MAXLEN];
            memset(temp, '\0', MAXLEN);
            for (int i = 1; i < strlen(procInfo->cmd); i++)
            {
                temp[i - 1] = procInfo->cmd[i];
            }
            temp[strlen(procInfo->cmd)] = '\0';
            strcpy(procInfo->cmd, temp);
            chdir("fd");

            // Open the terminal file for reading
            char address[MAXLEN];
            memset(address, '\0', MAXLEN);
            sprintf(address, "/proc/%s/fd/0", fileptr->d_name);
            int fd = open(address, O_RDONLY, 0);
            if (ttyname(fd) == NULL)
            {
                // No terminal associated with the process, set tty to "N/A"
                strcpy(procInfo->tty, "?");
            }
            else
                strcpy(procInfo->tty, ttyname(fd));
            close(fd);
            if (strlen(procInfo->tty) > 5)
            {
                char new[MAXLEN];
                memset(new, '\0', MAXLEN);
                for (int i = 5; i < strlen(procInfo->tty); i++)
                {
                    new[i - 5] = procInfo->tty[i];
                }
                new[strlen(procInfo->tty) - 5] = '\0';
                strcpy(procInfo->tty, new);
                // printf("\n%s", procInfo->tty);
            }
            procInfo->STime = up_time - procInfo->STime / sysconf(_SC_CLK_TCK);
            procInfo->STime = (time_t)time(0) - procInfo->STime;
            printFunction(procInfo, tim, self2, a, e, f, d, total_cpu_time);
            chdir("/proc");
            // printf("%s %d\n", procInfo->tty, x);
        }
    }

    closedir(proc);
    chdir(save);
}