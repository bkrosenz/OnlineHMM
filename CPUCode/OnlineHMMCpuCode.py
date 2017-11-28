from sys import argv
import argparse, os
import numpy as np

STEP = 10000
MAX = None # max number of training points to use

def file_reader(fn):
    with open(fn,'r') as f:
        for n,line in enumerate(f):
            if MAX and n>MAX:
                raise StopIteration
            else:
                yield int(line)

class Learner():
    def __init__(self,N,M,eta=0.01):
        self.eta = eta
        self.N = N
        self.M = M
        init = 0.001
        self.phi = np.reshape(
            np.repeat(init,N**3*M),(N,N,M,N)
        )
        alpha = 5
        self.gamma = np.random.dirichlet([alpha]*M, (N,N))
        self.q = np.random.dirichlet([alpha]*N, 1)[0]
        self.a = np.random.dirichlet([alpha]*N, N)
        self.b = np.random.dirichlet([alpha]*M, N)
        print self.phi.shape
        print 'a',self.a.shape,'q',self.q.shape,'b',self.b[:,0]
        
    def initialize(self):
        # self.a /= self.N
        # self.b /= self.M
        # self.q /= self.N
        
        self.n = 0
        
    def update_phi(self,y):
        for i in range(self.N):
            for j in range(self.N):
                for k in range(self.M):
                    match_ky = k==y
                    for h in range(self.N):
                        match_jh = j==h
                        self.phi[i,j,k,h] = sum(
                            self.gamma[l,h,y] * (self.phi[i,j,k,l] +
                                                 self.eta * (
                                                     match_ky * match_jh * (i==l) * self.q[l] - self.phi[i,j,k,l]
                                                 )
                            )
                            for l in range(self.N)
                        )
                            
                        
    def update(self,y):
            
        if not self.n%STEP:
            print 'gam',self.gamma, 'y',y,'n',self.n,'q',self.q,'a',self.a,'b',self.b
            print self.phi

        self.update_phi(y)
        
        self.n+=1

        if self.n<100:
            return
        
        gamma_denom = np.einsum('m,mn,n->',self.q,self.a,self.b[:,y])
        self.gamma[:,:,y] = np.einsum('lh,h->lh',self.a,self.b[:,y]) / gamma_denom
        
        self.q = np.einsum('m,ml->m',self.q,self.gamma[:,:,y])
        a_denom = 1. / np.einsum('ijkh->i',self.phi)
        self.a = np.einsum('ijkh,i->ij',self.phi,a_denom) 

        b_denom = 1. / np.einsum('ijkh->j',self.phi)
        self.b = np.einsum('ijkh,j->jk',self.phi,b_denom) 

def main(args):
    hmm = Learner(2,4)
    hmm.initialize()
    for y in file_reader(args[0]):
        hmm.update(y)    

if __name__ == "__main__":
    main(argv[1:])
