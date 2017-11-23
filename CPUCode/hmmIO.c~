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

void loadImage(char *filename, uint32_t **dest, int *width, int *height, int grayscale)
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

	*dest = malloc((*width) * (*height) * sizeof(int));

	int pixels_read = 0;
	int pixel = 0;
	int curr_component = 0;

	for (int i = 0; i < ((*width) * (*height) * 3); i++) {
		int v;
		int got = fscanf(file, "%d", &v);
		if (got == 0) {
			fprintf(
				stderr,
				"Unexpected end of file after reading %d color values",
				i);
			exit(1);
		}
		switch (curr_component)
		{
			case 0:
				pixel = v;
				curr_component++;
				break;
			case 1:
				pixel = (pixel << 8) | (v & 0xFF);
				curr_component++;
				break;
			case 2:
				pixel = (pixel << 8) | (v & 0xFF);
				curr_component = 0;
				if (grayscale)
					pixel = ((pixel >> 16) & 0xff) * 3 / 10 + ((pixel >> 8)
						& 0xff) * 59 / 100 + ((pixel) & 0xff) * 11 / 100;
				(*dest)[pixels_read] = pixel;
				pixels_read++;
				break;
		}
	}
	fclose(file);
}

void writeImage(char *filename, uint32_t *data, int width, int height, int grayscale)
{
	FILE *file = fopen(filename, "w");

	fprintf(file, "P3\n");
	fprintf(file, "%d %d\n", width, height);
	fprintf(file, "255\n");

	for (int i = 0; i < width * height; i++) {
		if (grayscale) {
			data[i] = data[i] > 255 ? 255 : data[i];

			for (int j = 0; j < 3; j++)
				fprintf(file, "%d\n", (int) data[i]);
		} else {
			fprintf(file, "%d\n", ((int) data[i]) >> 16);
			fprintf(file, "%d\n", (((int) data[i]) >> 8) & 0xff);
			fprintf(file, "%d\n", (((int) data[i])) & 0xff);
		}
	}
	fclose(file);
}
