#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>
#include <stdbool.h>
#include <pwd.h>
#include <time.h>
#include <grp.h>
#include <sys/stat.h>
#include <readline/readline.h>
#include <readline/history.h>
#include <fcntl.h>
#include <ctype.h>
#define MAXLEN 10000
#define red "\001\e[01;31m\002"
#define cyan "\001\e[01;36m\002"
#define reset "\001\e[0m\002"

typedef struct file
{
    char name[MAXLEN];
    unsigned int index;
    unsigned int mode;
    unsigned int link;
    unsigned int user_id;
    unsigned int group_id;
    unsigned long long size;
    time_t time;
} file;

typedef struct process
{
    long long pid, ppid, uid, sid;
    unsigned long long rss;
    long int sz;
    char tty[MAXLEN], cmd[MAXLEN], cmdFull[MAXLEN];
    int psr;
    time_t STime;
    long long int uTime, sTime, cuttime, cstime;
    char forTime[MAXLEN], forSTime[MAXLEN];
    int c;
    char status;
} process;

typedef struct pairStr
{
    char path[MAXLEN];
    char name[MAXLEN];
} pairStr;

bool isValidString(char query[]);
char *convertToString(char query[], int* index);
bool checkHome(char cwd[], char userName[]);
void setDir(char dir[], char userName[]);
char *setArg(char userName[], char hostName[], char dir[]);
void cd(char query[], int validityQuery, char userName[]);
void myls(char query[]);
void printPer(unsigned int per);
void printUser(unsigned int user);
int nameCmp(const void *a, const void *b);
int sizeCmp(const void *a, const void *b);
int timeCmp(const void *a, const void *b);
void printPer(unsigned int per);
void printUser(unsigned int user);
void printGroup(unsigned int user);
void printMonth(int t);
void myps(char query[]);
void mycp(char query[]);
char *trim(char a[]);
void setStr(pairStr *pair, char arg[], char cwd[]);
void mycp(char query[]);
void mymv(char query[]);
void mygrep(char query[]);
void printFunction(process *p, long long time, char self[], int a, int e, int f, int F, unsigned long long total_cpu_time);
void help(char*);