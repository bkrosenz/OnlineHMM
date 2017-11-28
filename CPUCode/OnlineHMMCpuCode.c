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
const int STEP = 10; // print after STEP iters
const int length = 5000;

int main()
{
    float q[nStates];// = { [1 ... nStates-1] = 1./nStates}; // initial prior
    float a[nStates][nStates];// = {1./nStates}; // initial transition prob matrix
    float b[nStates][nSymbols];// = {1./nSymbols}; // initial emission prob matrix
    float phi[nStates][nStates][nSymbols][nStates];// = {0};// suff stats phi[x_{t-1}][x_t][y_T][x_T]
    float gamma[nStates][nStates][nSymbols];// = {0}; // gamma[x_{T-1}][x_T][y_T]
    int n = 0;

    float eta0 = 1.0;
    float eta = 0.01;
    float temp;
    int i,j,k,h,l,y;

    //initialize phi  
    for (i=0; i<nStates; ++i){
	for (j=0; j<nStates; ++j){
	    for (k=0; k<nSymbols; ++k){
		for (h=0; h<nStates; ++h)
		    phi[i][j][k][h] = 0.001;
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
  
    for (y=0; y<nSymbols;++y){

	// normalization const
	float gamma_denom = 0.0;
	for (i=0; i<nStates; ++i)
	    for (j=0; j<nStates; ++j)
		gamma_denom += a[i][j]*b[j][y]*q[i];
  
  
	//initialize gamma
	for (i=0; i<nStates; ++i)
	    for (j=0; j<nStates; ++j)
		gamma[i][j][y] = a[i][j]*b[j][y]/gamma_denom;
    }

    /* for (int s=0; s<1000; ++s){ */
    /* 	y = s%nSymbols; */
    /* 	for (i=0; i<nStates; ++i){ */
    /* 	    for (j=0; j<nStates; ++j){ */
    /* 		for (k=0; k<nSymbols; ++k){ */
    /* 		    int match = (y==k); */
    /* 		    for (h=0; h<nStates; ++h){ */
    /* 			temp = 0.0; */
    /* 			for (l=0; l<nStates; ++l){ */
    /* 			    int g = (i==l) * (j==h); */
    /* 			    temp += gamma[l][h][y] * ( phi[i][j][k][l] + eta * ( match*g*q[l] - phi[i][j][k][l] ) ); */
    /* 			} */
    /* 			phi[i][j][k][h] = temp; // TODO: currently this greedily updates and uses the new vals for all later iters.  not like original Mongillo alg */

    /* 		    } */
    /* 		} */
    /* 	    } */
    /* 	} */
    /* } */

    /* ***** end of initialization **** */

    /* uint32_t * observations = malloc(length * sizeof(uint32_t)); */  
    /* loadObservations( */
    /* 		   "../data/sim.y.txt", */
    /* 		   &observations, */
    /* 		   &length); */

  
    /* ****************** main loop *************** */
    FILE *file = fopen("../data/sim.y.txt", "r");

    //  for (int n=0; n < length; ++n){
  
    /* y = observations[n]; */
    char* line = NULL;
    size_t len = 0;
    i = 0;
    n = -1;
    while ( (n<length) && getline(&line, &len, file) != -1)
    {
	/* *** check vals *** */
	if (0 == n%STEP) {
	    printf("y: %d, n: %d, eta: %.5f\na:\n",y,n,eta);
	    printf("\na:\n");
	    for(int i=0; i < nStates; ++i){
		for(int j=0; j < nStates; ++j)
		    printf("%f ",a[i][j]);
		printf("\n");
	    }
	    printf("\n****\n");
	    printf("\nb:\n");
	    for(int i=0; i < nStates; ++i){
		for(int j=0; j < nSymbols; ++j)
		    printf("%f ",b[i][j]);
		printf("\n");
	    }
	    printf("\n****\n");
	}

	y = atoi(line);
	++n;
	//update phi  
	for (i=0; i<nStates; ++i){
	    for (j=0; j<nStates; ++j){
		for (k=0; k<nSymbols; ++k){
		    int match = (y==k);
		    for (h=0; h<nStates; ++h){
			temp = 0.0;
			for (l=0; l<nStates; ++l){
			    int g = (i==l) * (j==h);
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
	for (i=0; i<nStates; ++i){
	    temp = 0.0;
	    for (j=0; j<nStates; ++j){
		temp += gamma[j][i][y]*q[j];
	    }
	    q[i] = temp;
	}

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
	/* eta = eta0 / (n+1); */
    }

    
    fclose(file);

/* printMatrix(&a, &nStates, &nStates); */
/* printMatrix(&b, &nStates, &nSymbols); */
    
    int check_results = 0;
    int  status = 0;
    if (check_results) {
	uint32_t * a_true;
	uint32_t * b_true;
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
