#include <stdio.h>
#include <stdlib.h>
#ifdef __WXMSW__
	#include <conio.h>
	#define wait_key getch
#else
	#define wait_key getchar
#endif
#include <string.h>

int main(int argc, char** argv)
{
	if (argc < 2)
    {
        printf("Usage: console_runner <filename> <args ...>\n");
        return 1;
    }

    char cmdline[4096] = {0};

    // 1st arg (executable) enclosed in quotes to support filenames with spaces
    strcat(cmdline, "\"");
    strcat(cmdline, argv[1]);
    strcat(cmdline, "\" ");

    for (int i = 2; i < argc; ++i)
    {
        strcat(cmdline, argv[i]);
        strcat(cmdline, " ");
    }
    
    //printf("Would run '%s'\n", cmdline);
    system(cmdline);
    printf("\nPress any key to continue.\n");
    wait_key();
    return 0;
}
