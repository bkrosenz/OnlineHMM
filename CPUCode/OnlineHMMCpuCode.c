/**
 * Document: MaxCompiler State Machine Tutorial (maxcompiler-sm-tutorial.pdf)
 * Example: 5      Name: Histogram
 * MaxFile name: HistogramSM
 * Summary:
 *     Stream an image to the DFE and read back a histogram from mapped memory.
 */
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>
/* #include <MaxSLiCInterface.h> */
/* #include "Maxfiles.h" */
#include "hmmIO.h"

int nStates = 2;
int nSymbols = 4;
float q[nStates] = {1./nStates}; // initial prior
float a[nStates][nStates] = {1./nStates}; // initial transition prob matrix
float b[nStates][nSymbols] = {1./nSymbols}; // initial emission prob matrix
float phi[nStates][nStates][nSymbols][nStates] = {0};// suff stats phi[x_{t-1}][x_t][y_T][x_T]
float gamma[nStates][nStates][nSymbols] = {0}; // gamma[x_{T-1}][x_T][y_T]
float eta_0 = 1.0; // discount factor.
int n = 0;


void update(
	int y,
	float* q,
	float* a,
	float* b,
	float* phi,
	float* gamma,
	    ) 
{
  
  int i,j,k,h,l;

  //update phi  
  for (i=0; i<nStates; ++i){
    for (j=0; j<nStates; ++j){
      for (k=0; k<nStates; ++k){
	for (h=0; h<nStates; ++h){
	  float temp = 0;
	  for (l=0; l<nStates; ++l){
	    int match = (int) (y==k);
	    int g = (int)(i==l) * (int)(j==h);
	    temp += gamma[l][h][y] * ( phi[i][j][k][l] + eta * ( match*g*q[l] - phi[i][j][k][l] ) );
	  }
	}
      }
    }
  }

  // normalization const
  float gamma_denom = 0.0;
  for (i=0; i<nStates; ++i)
    for (j=0; j<nStates; ++j)
      gamma_denom += a[i][j]*b[j][y]*q[i];
  
  //update gamma
  for (i=0; i<nStates; ++i)
    for (j=0; j<nStates; ++j)
      gamma[i][j][y] = a[i][j]*b[j][y]/gamma_denom;

  //update q
  for (l=0; l<nStates; ++l)
    for (m=0; m<nStates; ++m)
      q[l] += gamma[l][m][y]*q[l];

  // update a
  for (i=0; i<nStates; ++i){
    float a_denom = 0.0;
    for (j=0; j<nStates; ++j){
      a[i][j] = 0;
      for (k=0; k<nSymbols; ++k){
	for (h=0; h<nStates; ++h)
	  a[i][j] += phi[i][j][k][h];
      }
      a_denom += a[i][j];
    }
    for (j=0; j<nStates; ++j)
      a[i][j] /= a_denom;
  }

  // update b
  for (j=0; j<nStates; ++j){
    float b_denom = 0.0;
    for (k=0; k<nSymbols; ++k){
      b[j][k] = 0.0;
      for (i=0; i<nStates; ++i){
	for (h=0; h<nStates; ++h)
	  b[j][k] += phi[i][j][k][h];
      }
      b_denom += b[j][k];
    }
    for (k=0; k<nSymbols; ++k)
      b[j][k] /= b_denom;
  }

  ++n;
  
}

void HistogramSMCPU(
	int dataSize,
	uint32_t* inImage,
	uint32_t* outImage,
	uint64_t* ramIn,
	uint64_t* outHistogram)
{
	ramIn = ramIn; /*unused*/
	for (int i = 0; i < HistogramSM_histogramSize; i++) {
		outHistogram[i] = 0;
	}

	for (int i = 0; i < dataSize; i++) {
		int index = ((int) inImage[i]) >> 3;
		outHistogram[index]++;
		outImage[i] = outImage[i];
	}

	int sum = 0;
	printf("Expected histogram: ");
	for (int i = 0; i < HistogramSM_histogramSize; i++) {
		printf("%ld ", outHistogram[i]);
		sum += outHistogram[i];
	}
	printf("sum = %d\n", sum);
}

int check(
	int histogramSize,
	uint64_t *expectedOut,
	uint64_t *dataOut)
{
	int status = 0;

	int sum = 0;
	printf("Received histogram: ");
	for (int i = 0; i < histogramSize; i++) {
		printf("%ld ", dataOut[i]);
		sum += dataOut[i];
	}
	printf("sum = %d\n", sum);

	for (int i = 0; i < histogramSize; i++) {
		if (dataOut[i] != expectedOut[i]) {
			fprintf(stderr, "Error: data @ %d = %ld (expected %ld)\n",
				i, dataOut[i], expectedOut[i]);
			status = 1;
		}
	}

	return status;
}

int main()
{
  int N = 1000;
  float eta = 1.0;
  
  eta *= (n-1) / n;
  
	uint32_t *observations;

	if (histogramSize < 2 || !((histogramSize & (histogramSize - 1)) == 0)) {
		fprintf(stderr, "histogramSize must be a power of two and >1!");
		return 1;
	}

	loadObservations(
		"../EngineCode/src/histogramsm/test2.ppm",
		&observations,
		&length);
	
	uint64_t* expectedHistogram = malloc(histogramSize * sizeof(uint64_t));
	HistogramSMCPU(width * height, inImage, NULL, NULL, expectedHistogram);

	printf("Running DFE.\n");
	HistogramSM(width * height, inImage, outImage, ramIn, outHistogram);

	writeImage("test_output.ppm", outImage, width, height, 1);

	int status = check(histogramSize, expectedHistogram, outHistogram);

	if (status)
		printf("Test failed.\n");
	else
		printf("Test passed OK!\n");

	return status;
}
