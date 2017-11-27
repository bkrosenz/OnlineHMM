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

void printMatrix(uint32_t **mat,const int *rows, const int *cols){
  for(int i=0; i < *rows; ++i)
    for(int j=0;j < *cols; ++j)
      printf("%d ",mat[i][j]);
  printf("\n");
  return;
}

void saveMatrix(char *fname, uint32_t **dest, const int *rows, const int *cols)
{
  FILE *outputMatrix=fopen(fname,"w");
  *dest = malloc((*cols) * (*rows) * sizeof(int));
  for(int i=0; i < *rows; ++i){
    for(int j=0;j < *cols; ++j){
      fscanf(outputMatrix,"%d",&dest[i][j]);
    }
  }
  fclose(outputMatrix);
  return;
}

void loadMatrix(char *fname, uint32_t **dest, const int *rows, const int *cols)
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

void loadObservations(char *filename, uint32_t *dest, const int *length)
{
  FILE *file = fopen(filename, "r");
  if (file == NULL)
    {
      perror("Error opening observations file.\n");
    }

  dest = malloc((*length) * sizeof(int));
  char* line = NULL;
  size_t len = 0;
  int i = 0;
  while ( getline(&line, &len, file) != -1)
    {
      //  printf("%d %s\n", i, line);
      dest[i++] = atoi(line);
    }
  /*       exit(1); */
    /*   dest[i] = atoi(buf); */
    /*   ++i; */
    /* } */
    //(file, "%d\n", &dest[i]))
    
    fclose(file);
    return;
}
