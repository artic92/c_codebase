/**
 * @file hellofunc.c
 * @author Antonio Riccio <antonio.riccio.27@gmail.com>
 * @brief Implementation file for hellomake.h
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
#include <stdio.h>

#include "hellomake.h"

/**
 * @details This function prints the string *Hello makefiles!* to the standard
 * output. It uses a *printf()*.
 */
void myPrintHelloMake(void) {

  printf("Hello makefiles!\n");

  return;
}
