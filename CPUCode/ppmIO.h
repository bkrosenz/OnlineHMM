/*
 * Document: MaxCompiler State Machine Tutorial (maxcompiler-sm-tutorial.pdf)
 * Example: 5
 * Summary:
 *      Headers for functions to load and save images from the file system.
 */
void loadImage(char *filename, uint32_t **dest, int *width, int *height, int grayscale);
void writeImage(char *filename, uint32_t *data, int width, int height, int grayscale);
