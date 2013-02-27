#include <stdio.h>
#include <stdlib.h>
#include <time.h>

char stringArray[100][80]; /*100 lines by 80 columns*/
FILE *aphorismsFile;


void main (int argc, char *argv[])
{
	int n = 0;

	aphorismsFile = fopen("aphorismsFile.txt", "r");

	while (!feof(aphorismsFile))
	{
		fgets(stringArray[n], 80, aphorismsFile);
		printf("String read: %s\n", stringArray[n]);
		n++;
	}

	fclose(aphorismsFile);
	printf("\n\n");

	int i = 0;

	srand (time (NULL));
	i = rand() % n;

	printf("RANDOM STRING: %s", stringArray[i]);
	printf ("\n\n");
}
