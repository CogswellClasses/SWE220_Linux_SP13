#include <stdio.h>
#include <stdlib.h>
#include <time.h>


char *strings[] = {
#include "Read me"
  NULL
};

char *saying () {
  int n;
  for (n = 0; strings[n]; n++);
  int i;
  srand (time (NULL));
  i = rand() % n;
  return strings[i];
}

void main (int argc, char *argv[]) {
  printf ("%s\n", saying ());
}
