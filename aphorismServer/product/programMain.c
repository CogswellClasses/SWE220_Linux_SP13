#include <stdio.h>
#include <stdlib.h>
#include <time.h>


char *strings[] = {
#include "Read me"
  NULL
};

static int nStrings = sizeof (strings) / sizeof (strings[0]);

char *saying () {
  int i;

  srand (time (NULL));
  i = rand() % nStrings;
  return strings[i];
}

void main (int argc, char *argv[]) {
  printf ("%s\n", saying ());
}
