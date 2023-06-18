#include "myshell.h"

void myls(char query[])
{
    int index = 5;
    char save[MAXLEN];
    getcwd(save, MAXLEN);

    int n = strlen(query);
    int a = 0, l = 0, S = 0, t = 0, r = 0;
    int k = 0;
    for (int i = index - 1; i < n; i++)
    {
        if (i == '&')
            break;
        if (query[i] == '-')
        {
            i++;
            for (; i < n; i++) // consider case of wrong option ie ls -allll
            {
                if (query[i] == 'a')
                    a = i;
                else if (query[i] == 'l')
                    l = i;
                else if (query[i] == 'S')
                    S = i;
                else if (query[i] == 't')
                    t = i;
                else if (query[i] == 'r')
                    r = i;
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
    int ind = -1;
    for (int i = 0; i < n; i++)
    {
        if (query[i] == ' ' && query[i + 1] != '-')
            ind = i + 1;
    }
    char *path = (char *)malloc(sizeof(char) * MAXLEN); // change name later
    if (ind < strlen(query))
    {
        path = convertToString(query, &ind);
        if (chdir(path) == -1 && ind != -1)
        {
            printf("Invalid Path\n");
            return;
        }
    }
    struct dirent *fileptr;
    file *list = malloc(sizeof(file) * 1000);
    k = 0;
    char cwd[MAXLEN];
    getcwd(cwd, MAXLEN);
    DIR *current = opendir(cwd);

    if (current == NULL)
    {
        printf("ERROR: Could not open Current Directory\n");
        return;
    }
    // closedir(current);
    int total = 0;
    int total_hidden = 0;
    while ((fileptr = readdir(current)) != NULL)
    {
        strcpy(list[k].name, fileptr->d_name);
        struct stat myfile;
        stat(list[k].name, &myfile);
        list[k].mode = myfile.st_mode;
        list[k].link = myfile.st_nlink;
        list[k].user_id = myfile.st_uid;
        list[k].group_id = myfile.st_gid;
        list[k].size = myfile.st_size;
        list[k].time = myfile.st_mtime;
        if (fileptr->d_name[0] == '.')
            total_hidden += myfile.st_blocks;
        total += myfile.st_blocks;
        k++;
    }
    qsort(list, k, sizeof(file), nameCmp);
    if (S > t)
        qsort(list, k, sizeof(file), sizeCmp);
    else if (t > S)
        qsort(list, k, sizeof(file), timeCmp);
    if (!a && l)
        printf("total %d\n", (total - total_hidden) / 2);
    else if (l)
        printf("total %d\n", total / 2);
    if (r)
    {
        for (int i = k - 1; i >= 0; i--)
        {
            if (!a)
            {
                if (!l)
                {
                    if (list[i].name[0] != '.')
                        printf("%s\n", list[i].name);
                }
                else
                {
                    if (list[i].name[0] != '.')
                    {
                        list[i].time += 19800;
                        struct tm *a = gmtime(&list[i].time);
                        printPer(list[i].mode);
                        printf(" %3d", list[i].link);
                        printUser(list[i].user_id);
                        printGroup(list[i].group_id);
                        printf(" %10lld ", list[i].size);
                        printMonth(a->tm_mon + 1);
                        printf(" %2d ", a->tm_mday);

                        if (a->tm_hour < 10)
                            printf("0");
                        printf("%d:", a->tm_hour);
                        if (a->tm_min < 10)
                            printf("0");
                        printf("%d ", a->tm_min);
                        printf("%s", list[i].name);
                        printf("\n");
                    }
                }
            }
            else if (!l)
                printf("%s\n", list[i].name);
            else
            {
                list[i].time += 19800;
                struct tm *a = gmtime(&list[i].time);
                printPer(list[i].mode);
                printf(" %3d", list[i].link);
                printUser(list[i].user_id);
                printGroup(list[i].group_id);
                printf(" %10lld ", list[i].size);
                printMonth(a->tm_mon + 1);
                printf(" %2d ", a->tm_mday);

                if (a->tm_hour < 10)
                    printf("0");
                printf("%d:", a->tm_hour);
                if (a->tm_min < 10)
                    printf("0");
                printf("%d ", a->tm_min);
                printf("%s", list[i].name);
                printf("\n");
            }
        }
    }
    else
    {
        for (int i = 0; i < k; i++)
        {
            if (!a)
            {
                if (!l)
                {
                    if (list[i].name[0] != '.')
                        printf("%s\n", list[i].name);
                }
                else
                {
                    if (list[i].name[0] != '.')
                    {
                        list[i].time += 19800;
                        struct tm *a = gmtime(&list[i].time);
                        printPer(list[i].mode);
                        printf(" %3d", list[i].link);
                        printUser(list[i].user_id);
                        printGroup(list[i].group_id);
                        printf(" %10lld ", list[i].size);
                        printMonth(a->tm_mon + 1);
                        printf(" %2d ", a->tm_mday);

                        if (a->tm_hour < 10)
                            printf("0");
                        printf("%d:", a->tm_hour);
                        if (a->tm_min < 10)
                            printf("0");
                        printf("%d ", a->tm_min);
                        printf("%s", list[i].name);
                        printf("\n");
                    }
                }
            }
            else if (!l)
                printf("%s\n", list[i].name);
            else
            {
                list[i].time += 19800;
                struct tm *a = gmtime(&list[i].time);
                printPer(list[i].mode);
                printf(" %3d", list[i].link);
                printUser(list[i].user_id);
                printGroup(list[i].group_id);
                printf(" %10lld ", list[i].size);
                printMonth(a->tm_mon + 1);
                printf(" %2d ", a->tm_mday);

                if (a->tm_hour < 10)
                    printf("0");
                printf("%d:", a->tm_hour);
                if (a->tm_min < 10)
                    printf("0");
                printf("%d ", a->tm_min);
                printf("%s", list[i].name);
                printf("\n");
            }
        }
    }
    closedir(current);
    chdir(save);
}