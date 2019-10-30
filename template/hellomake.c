/**************************************************************************
*                  Copyright (C) Antonio Riccio, 2019				              *
* ------------------------------------------------------------------------*
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.        *
* ----------------------------------------------------------------------- *
* 																		                                    *
* Examples:																                                *
*	- Asynchronous I/O through the select() system call.				            *
*	- User-space Linux input subsystem						  			                  *
* 																		                                    *
* More info at:															                              *
*	- Asynchronous I/O: 'The Linux Programming Interface' chapter 63        *
*	- Linux input subsystem: Documentation/input/input_uapi.rst			        *
***************************************************************************/
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hellomake.h"

/* program configuration variables */
static unsigned int currentDebugLevel = debugDisabled;
static bool print_hello = false;
static char executable_name[] = "pippozzo";

/* getopt_long options */
static struct option longOptions[] = {
    { "help",       no_argument,            0,  'h' },
    { "vv",         no_argument,            0,   0  },
    { 0,            0,                      0,   0  }
};

/**
 * usage() - Prints usage information.
 */
static void usage()
{
	printf("Usage: %s ", executable_name);
  printf("[-p][-v debugLevel|--vv][-h|--help]");
  printf("\n");
}

int main(int argc, char* const argv[])
{
    int opt = 0;
    int optionIndex = 0;

    strcpy(executable_name, argv[0]);

    /* CLI parameters check */
    while ((opt = getopt_long(argc, argv, "phv:", longOptions, &optionIndex)) != -1)
    {
        switch (opt)
        {
        case 'p':
          print_hello = true;
          break;
        case 'v':
          currentDebugLevel = (debugDisabled < atoi(optarg) < debugLvlDebug ? atoi(optarg) : debugDisabled);
          break;
        case 0:
          currentDebugLevel++;
        case 'h':
          /* fall-through */
        default:
          usage(argv[0]);
          return EXIT_FAILURE;
        }
    }

  if (optind == 1){
    usage(argv[0]);
    return EXIT_FAILURE;
  }

  if(print_hello)
    myPrintHelloMake();

  return EXIT_SUCCESS;
}
