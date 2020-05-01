/**
 * @file hellomake.c
 * @author Antonio Riccio <antonio.riccio.27@gmail.com>
 * @brief Driver file for the template project
 * @version 0.1
 * @date 2020-05-01
 *
 * @copyright (c) 2020
 * This program is free software; you can redistribute it and/or modify it under
 * the terms of the GNU General Public License as published by the Free Software
 * Foundation; either version 3 of the License, or any later version. This
 * program is distributed in the hope that it will be useful, but WITHOUT ANY
 * WARRANTY; without even the implied warranty of MERCHANTABILITY or FITNESS FOR
 * A PARTICULAR PURPOSE. See the GNU General Public License for more details.
 * You should have received a copy of the GNU General Public License along with
 * this program; if not, write to the Free Software Foundation, Inc., 51
 * Franklin Street, Fifth Floor, Boston, MA 02110-1301, USA.
 *
 */
#include <getopt.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

#include "hellomake.h"

static unsigned int currentDebugLevel = debugDisabled;
static bool print_hello = false;
static char executable_name[] = "pippozzo";

/**
 * @brief *long options for getopt_long()*
 *
 */
static struct option longOptions[] = {
    { "help",       no_argument,            0,  'h' },
    { "vv",         no_argument,            0,   0  },
    { 0,            0,                      0,   0  }
};

/**
 * @brief *prints usage information*
 *
 */
static void usage()
{
	printf("Usage: %s ", executable_name);
  printf("[-p][-v debugLevel|--vv][-h|--help]");
  printf("\n");
}

/**
 * @brief *main function*
 *
 * @details *argv* and *argc* are how command line arguments are passed to
 * *main()* in C and C++. The variables are named **argc** (*argument count*)
 * and **argv** (*argument vector*) by convention, but they can be given any
 * valid identifier, e.g.:
 *
 * ```c
 * int main(int num_args, char** arg_strings)
 * ```
 *
 * is equally valid. They can also be omitted entirely, yielding:
 *
 * ```c
 * int main()
 * ```
 *
 * if you do not intend to process command line arguments.
 *
 * @param argc [in] number of strings pointed to by argv
 * @details *argc* stands for *argument count*. This will (in practice) be 1
 * plus the number of arguments, as virtually all implementations will prepend
 * the name of the program to the array.
 * @param argv [in] array of strings contaning parameters plus executable name
 * @details *argv* stands for *argument count*. *argument vector*
 * @return int the function returns EXIT_SUCCESS on success, otherwise
 * EXIT_FAILURE
 */
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
