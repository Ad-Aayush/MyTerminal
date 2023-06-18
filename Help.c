#include "myshell.h"

void help(char query[])
{
    if (strcmp(query, "help myls") == 0 || strcmp(query, "help ls") == 0)
    {
        printf("myls [OPTIONS]... [FILE]\n\n"

               "List information about the FILEs (the current directory by default)\n"
               "The entries are sorted alphebetically by default\n"
               "\nFLAGS Implemented\n"
               "-a: Also lists the hidden files and folders\n"
               "-l: Lists the files along with details such as permissions, owner, time of modification etc\n"
               "-S: Lists the files sorted in descending order by size\n"
               "-t: Lists the files sorted by the time of modefication\n"
               "-r: Lists the files in the reverse order\n\n"
               "The flags can be used in any order\nThe position of options is flexible\n");
    }
    else if (strcmp(query, "help mycp") == 0 || strcmp(query, "help cp") == 0)
    {
        printf("mycp [OPTION]... SOURCE DIRECTORY\n\n"

               "Copies SOURCE file to DEST along with the permissions\n"
               "\nFLAGS Implemented\n"
               "-i: Promts the user in case the DEST file already exists\n"
               "-n: Ensures DEST file is not overwritten if already prest\n"
               "-v: Explain what is being done\n"
               "-u: Copies only when the SOURCE file is newer or when the destination file is missing \n\n"

               "The flags can be used in any order\nThe position of options is flexible\n");
    }
    else if ((strcmp(query, "help mymv") == 0 || strcmp(query, "help mv") == 0))
    {
        printf("mymv [OPTION]... SOURCE DIRECTORY\n\n"

               "Copies SOURCE file to DEST along with the permissions, deletes the source later\n"
               "\nFLAGS Implemented\n"
               "-i: Promts the user in case the DEST file already exists\n"
               "-n: Ensures DEST file is not overwritten if already prest\n"
               "-v: Explain what is being done\n"
               "-u: Copies only when the SOURCE file is newer or when the destination file is missing \n\n"

               "The flags can be used in any order\nThe position of options is flexible\n");
    }
    else if (strcmp(query, "help myps") == 0 || strcmp(query, "help ps") == 0)
    {
        printf("myps [OPTION]...\n\n"

               "Copies SOURCE file to DEST along with the permissions, deletes the source later\n"
               "\nFLAGS Implemented\n"
               "-a: Displays all processes except both session leaders and processes not associated with a terminal\n"
               "-e: Displays all processes\n"
               "-f: List the processes along with more details about them\n"
               "-d: Displays all processes except session leaders\n\n"

               "The flags can be used in any order\nThe position of options is flexible\n");
    }
    else if (strcmp(query, "help mygrep") == 0 || strcmp(query, "help grep") == 0)
    {
        printf("mygrep [OPTION...] PATTERN [FILE]\n\n"

               "Searches  for  PATTERN in a given FILE.\n"
               "\nFLAGS Implemented\n"
               "-w: Prints only the lines containing matches that form whole words\n"
               "-i: Ignores case distinctions in patterns and input data\n"
               "-n: Prefixes each line of output with the line number within its input file.\n"
               "-c: Suppresses normal output; instead prints a count of matching lines for the input file\n\n"

               "The flags can be used in any order\nThe position of options is flexible\n");
    }
    else
    {

        printf("My Shell\n\n"

               "Try \"help myls\" to get more info on myls command\n"
               "Try \"help mycp\" to get more info on mycp command\n"
               "Try \"help mymv\" to get more info on mymv command\n"
               "Try \"help myps\" to get more info on myps command\n"
               "Try \"help mygrep\" to get more info on mygrep command\n\n"

               "For any other command directly refer to the man pages using \"man [COMMAND]\"\n");
        }
}