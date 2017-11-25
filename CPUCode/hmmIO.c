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

void loadMatrix(char *fname, uint32_t **dest, int *rows, int *cols)
{
  FILE *inputMatrix=fopen(fname,"r");
  *dest = malloc((*cols) * (*rows) * sizeof(int));
  for(int i=0; i < *rows; ++i){
    for(int j=0;j < *cols; ++j){
      fscanf(inputMatrix,"%d",&dest[i][j]);
    }
  }
  fclose(inputMatrix);
  return;
}

void loadObservations(char *filename, uint32_t *dest, int *length)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL)
    {
      perror("Error opening observations file.\n");
    }

  dest = malloc((*length) * sizeof(int));
  char buf[256];
  int i=0;
  while (EOF != getline(&buf,sizeof(buf), file))
    {
      printf("\"%s\"\n", buf);
    }
  /*       exit(1); */
    /*   dest[i] = atoi(buf); */
    /*   ++i; */
    /* } */
    //(file, "%d\n", &dest[i]))
    
    fclose(file);
    return;
}
