/*
 * Document: MaxCompiler State Machine Tutorial (maxcompiler-sm-tutorial.pdf)
 * Example: 5
 * Summary:
 *      Headers for functions to load and save images from the file system.
 */
void loadMatrix(char *fname, uint32_t **dest, const int *rows, const int *cols);

void loadObservations(char *filename, uint32_t *dest, const int *length);

void printMatrix(uint32_t **mat,const int *rows, const int *cols);
