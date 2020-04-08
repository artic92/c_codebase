#include <stdio.h>

#include "utils.h"

int main(int argc, char const *argv[])
{
    char str_to_lowerise[] = "STRING TO Lowerise";

    current_debug_level = DEBUG_DEBUG;
    log_file_p = stdout;

    debug(DEBUG_DEBUG, "#################### Library Test Suite ####################");
    debug(DEBUG_DEBUG, "#################### Test lowerise() #######################");
    debug(DEBUG_DEBUG, "string to convert: '%s'", str_to_lowerise);
    lowerise(str_to_lowerise);
    debug(DEBUG_DEBUG, "string converted: '%s'", str_to_lowerise);
    debug(DEBUG_DEBUG, "#############################################################");

    return 0;
}
