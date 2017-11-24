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

  const int nStates = 2;
  const int nSymbols = 4;

/* void update( */
/* 	    int* y, */
/* 	    float* eta, */
/* 	    float* q, */
/* 	    float* a, */
/* 	    float* b, */
/* 	    float* phi, */
/* 	    float* gamma, */
/* 	    ); */

/* void initialize( */
/* 		float* q, */
/* 		float* a, */
/* 		float* b, */
/* 		float* phi, */
/* 		float* gamma, */
/* 		); */
 
int main()
{
  float q[nStates];// = { [1 ... nStates-1] = 1./nStates}; // initial prior
  float a[nStates][nStates];// = {1./nStates}; // initial transition prob matrix
  float b[nStates][nSymbols];// = {1./nSymbols}; // initial emission prob matrix
  float phi[nStates][nStates][nSymbols][nStates];// = {0};// suff stats phi[x_{t-1}][x_t][y_T][x_T]
  float gamma[nStates][nStates][nSymbols];// = {0}; // gamma[x_{T-1}][x_T][y_T]
  int n = 0;

  int length = 1000;
  float eta = 1.0;

  /* initialize( */
  /* 	    (int *)q, */
  /* 	    (int *)a, */
  /* 	    (int *)b, */
  /* 	    (int *)phi, */
  /* 	    (int *)gamma ); */
  int i,j,k,h,l;

  //initialize phi  
  for (i=0; i<nStates; ++i){
    for (j=0; j<nStates; ++j){
      for (k=0; k<nStates; ++k){
	for (h=0; h<nStates; ++h)
	  phi[i][j][k][h] = 0;
      }
    }
  }
  // TODO: to initialize phi, run alg for 10^ iters just changing phi

  //initialize q
  for (l=0; l<nStates; ++l)
    q[l] = 1./nStates;

  // initialize a
  for (i=0; i<nStates; ++i)
    for (j=0; j<nStates; ++j)
      a[i][j] = 1./nStates;

  // initialize b
  for (j=0; j<nStates; ++j)
    for (k=0; k<nSymbols; ++k)
      b[j][k] = 1./nSymbols;

  
  // normalization const
  for (int y; y<nSymbols;++y){
  float gamma_denom = 0.0;
  for (i=0; i<nStates; ++i)
    for (j=0; j<nStates; ++j)
      gamma_denom += a[i][j]*b[j][y]*q[i];
  
  
  //initialize gamma
  for (i=0; i<nStates; ++i)
    for (j=0; j<nStates; ++j)
      gamma[i][j][y] = a[i][j]*b[j][y]/gamma_denom;
  }
  
  uint32_t *observations;
  
  loadObservations(
		   "../data/sim.y.txt",
		   &observations,
		   &length);

  int y=-1;
    
  for (int n=0; n < length; ++n){
    // *************** update

      y = observations[n];
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
	  phi[i][j][k][h] = temp; // TODO: currently this greedily updates and uses the new vals for all later iters.  not like original Mongillo alg
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
  for (i=0; i<nStates; ++i)
    for (j=0; j<nStates; ++j)
      q[i] += gamma[i][j][y]*q[i];

  float b_denom, a_denom;
  
  // update a
  for (i=0; i<nStates; ++i){
    a_denom = 0.0;
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
    b_denom = 0.0;
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

    /* update( observations[n], */
    /* 	    eta, */
    /* 	    (int *)q, */
    /* 	    (int *)a, */
    /* 	    (int *)b, */
    /* 	    (int *)phi, */
    /* 	    (int *)gamma ); */
      eta *= (n-1) / n;
  }

  int check_results = 0;
  int  status = 0;
  if (check_results) {
    uint32_t * a_true,b_true;
    loadMatrix("../data/transition.mat", &a_true, &nStates, &nStates);
    loadMatrix("../data/emission.mat", &b_true, &nStates, &nSymbols);
    //    int status = check(length, a, b, a_true, b_true);
    
    if (status)
      printf("Test failed.\n");
    else
      printf("Test passed OK!\n");
  }
  return status;
}
