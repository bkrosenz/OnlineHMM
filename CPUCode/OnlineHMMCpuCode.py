from sys import argv
import argparse, os
import numpy as np

class learner():
    def __init__(self,N,M,eta=0.01):
        self.eta = 0.01
        self.N = N
        self.M = M
        self.phi = np.ndarray([N,N,M,N])
        self.gamma = np.ndarray([N,N,M])
        self.q = np.ndarray([N])
        self.a = np.ndarray([N,N])
        self.b = np.ndarray([N,M])

    def initialize(self):
        self.a = 1./self.N
        self.b = 1./self.M

    def update(y):
        for i in range(self.N):
            for j in range(self.N):
                for k in range(self.M):
                    for h in range(self.N):
                        self.phi[i,j,k,h] = sum( self.gamma[l,h,y] * (phi[i,j,k,l] + eta * g * match * q[l] - phi[i,j,k,l]) for l in range(self.N) )

        gamma_denom = np.einsum()
        np.dot(q,np.dot(a,b))
        for l in range(self.N):
            for h in range(self.N):
                gamma[l,h,y] = a[l,h]*b[h,y] / gamma_denom

        q = np.einsum('m,ml->m',q,gamma)

        self.a = np.einsum('ijkh->ij',self.phi)

        a_denom = np.einsum('ijkh->i',self.phi)

        self.a

        self.b = np.einsum('ijkh->jk',self.phi)

        b_denom = np.einsum('ijkh->j',self.phi)
