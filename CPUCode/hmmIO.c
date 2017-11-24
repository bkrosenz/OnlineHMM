/*
 * Document: MaxCompiler State Machine Tutorial (maxcompiler-sm-tutorial.pdf)
 * Example: 5
 * Summary:
 *      Functions to load and save images from the file system.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>

#include "ppmIO.h"

void loadMatrix(char *fname, uint32_t **dest, int rows, int cols)
{
  FILE *inputMatrix=fopen(fname,"r");
  *dest = malloc((*cols) * (*rows) * sizeof(int));
  for(i=0;i<rows;++i){
    for(j=0;j<cols+1;++j){
      fscanf(inputMatrix,"%f",&dest[i][j]);
    }
  }
  fclose(inputMatrix);
  return;
}

void loadObservations(char *filename, uint32_t **dest, int *length)
{
	// hack to make the buffer big enough
	char buffer[200];
	FILE *file = fopen(filename, "r");

	if (file == NULL)
	{
		printf("Error opening file %s.", filename);
		exit(1);
	}
	fgets(buffer, 200, file); // Type
	// assume no comment for now
	fscanf(file, "%d %d\n", width, height);
	fgets(buffer, 200, file); // Max intensity

	*dest = malloc((*length) * sizeof(int));
	for (int i = 0; i < length; i++) {
		int got = fscanf(file, "%d", &v);
		if (got == 0) {
			fprintf(
				stderr,
				"Unexpected end of file after reading %d obs",
				i);
			exit(1);
		}
		(*dest)[i] = v;
	fclose(file);
	return;
}
