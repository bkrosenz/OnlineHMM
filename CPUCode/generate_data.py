from sys import argv
import argparse, os
import numpy as np

def generate_argparser():
    parser = argparse.ArgumentParser(
        prog="generate_data.py",
        description=__doc__,
        formatter_class=argparse.ArgumentDefaultsHelpFormatter
    )
    parser.add_argument("-n",type=int,
                        help="number of samples.", default=1000)
    parser.add_argument("-e", "--emission", type=os.path.abspath,
                        help="emission matrix file", default='../data/emission.mat')
    parser.add_argument("-t", "--transition", type=os.path.abspath,
                        help="transition matrix file", default='../data/transition.mat')
    parser.add_argument("-p", "--prior", type=os.path.abspath,
                        help="prior matrix file",  default='../data/prior.mat')
    parser.add_argument("-o", "--outfile", type=os.path.abspath,
                        help="output file prefix",  default='../data/sim')
    return parser

############ main
arguments = argv[1:]
parser = generate_argparser()
args = parser.parse_args(args=arguments)
    
N = args.n                # length of sequence
A = np.genfromtxt(args.transition)      # transition matrix file
B = np.genfromtxt(args.emission)      # emission matrix file
q = np.genfromtxt(args.prior)      # priors
outfile = args.outfile               # output prefix for .x.txt and .y.txt files
print A,B,q
# categorical random variable
rv_cat = lambda dist: np.argmax(np.random.multinomial(1,dist))

x = np.zeros(N,dtype=int)
y = np.zeros(N,dtype=int)

x[0] = rv_cat(q)
y[0] = rv_cat( B[ x[0] ] )
for i in range(1,N):
    x[i] = rv_cat( A[ x[i-1] ] )
    y[i] = rv_cat( B[ x[i] ] )

np.savetxt(outfile+'.x.txt',x, fmt = '%d')
np.savetxt(outfile+'.y.txt',y, fmt = '%d')
