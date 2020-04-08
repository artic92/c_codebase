#ifndef __UTILS__
#define __UTILS__

enum DEBUG_LEVEL_
{
    DEBUG_DISABLED,
    DEBUG_INFO,
    DEBUG_ERROR,
    DEBUG_DEBUG
} DEBUG_LEVEL;

unsigned int current_debug_level = DEBUG_DISABLED;
FILE *log_file_p = NULL;

void debug(int level, char* message, ...);
void lowerise(char input_string[]);

#endif /*__UTILS__*/
