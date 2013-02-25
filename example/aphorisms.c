#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char *strings[] = {
"here today, gone tomorrow",
"a word to the wise is sufficient",
"no matter where you go, there your are",
NULL};

char *saying () {
	int n;
	for (n = 0; strings[n]; n++);
	int i;
	srand (time (NULL));
	i = rand() % n;
	return strings[i];
}

void main (int argc, char *argv[]) {
	printf (saying ());
	printf ("\n");
}

