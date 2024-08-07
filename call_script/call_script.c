#include <stdio.h>      /* printf */
#include <stdlib.h>     /* system, NULL, EXIT_FAILURE */


int main ()
{
  char shellcmd[20] = {0};
  int i;
  printf ("Checking if processor is available...");
  if (system(NULL)) puts ("Ok");
    else exit (EXIT_FAILURE);
  printf ("Executing script.sh...\n");
  sprintf(shellcmd, "./test.sh");
  i=system (shellcmd);
  printf ("The value returned was: %d.\n",i);
  return 0;
}
