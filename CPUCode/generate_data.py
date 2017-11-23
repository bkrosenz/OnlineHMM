from sys import argv
import numpy as np

N = int(argv[1])                # length of sequence
A = np.genfromtxt(argv[2])      # transition matrix file
B = np.genfromtxt(argv[3])      # emission matrix file
q = np.genfromtxt(argv[4])      # priors
outfile = argv[5]               # output prefix for .x.txt and .y.txt files

# categorical random variable
rv_cat = lambda dist: np.argmax(np.random.multinomial(1,dist))

x = np.zeros(N,dtype=int)
y = np.zeros(N,dtype=int)

x[0] = rv_cat(q)
y[0] = rv_cat( B[ x[0] ] )
for i in range(1,N):
    x[i] = rv_cat( A[ x[i-1] ] )
    y[i] = rv_cat( B[ x[i] ] )
    
np.savetxt(outfile+'.x.txt',x)
np.savetxt(outfile+'.y.txt',x)
