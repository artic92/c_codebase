/*ANSI C standard */
#include <ctype.h>
#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <time.h>

enum DEBUG_LEVEL_
{
    DEBUG_DISABLED,
    DEBUG_INFO,
    DEBUG_ERROR,
    DEBUG_DEBUG
} DEBUG_LEVEL;

extern unsigned int current_debug_level;
extern FILE* log_file_p;

static const unsigned int bufsize_byte = 1024;
static const unsigned int timestamp_bufsize = sizeof("YYYY-MM-DD HH:MM:SS");       /* Includes '\0' */
static const char timestampFmt[] = "%F %X";                                        /* = YYYY-MM-DD HH:MM:SS */

/*
*******************************************************************************
* debug
************************
* Prints message onto a stream if level > current_debug_level.
* Appends a '\n' character after each line
*******************************************************************************
*/
void debug(int level, char* message, ...)
{
    char buffer[bufsize_byte];
    char timestamp[timestamp_bufsize];
    struct tm* timeinfo = { 0 };
    va_list args = { 0 };
    time_t rawtime = 0;

    memset(buffer, 0, sizeof(buffer));
    memset(timestamp, 0, sizeof(timestamp));

    if (level <= current_debug_level)
    {
        time(&rawtime);
        timeinfo = localtime(&rawtime);

        if (strftime(timestamp, timestamp_bufsize, timestampFmt, timeinfo) != 0)
        {
            fprintf(log_file_p, "[%s]", timestamp);
        }

        va_start(args, message);
        vfprintf(log_file_p, message, args);
        va_end(args);

        fprintf(log_file_p, "\n");
    }
}

/******************************************************************************
* lowerise
* Transforms a string in lower case
* NOTE: the function operates on the input string!
******************************************************************************/
void lowerise(char input_string[])
{
    int i = 0;
    char c = -1;

    if(input_string != NULL)
    {
        while(input_string[i])
        {
            c = input_string[i];
            input_string[i] = tolower(c);
            i++;
        }
    }
}
