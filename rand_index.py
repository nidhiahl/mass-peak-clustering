import pandas as pd
import numpy as np

from scipy.special import comb


clustering_output = pd.read_csv(r'test.csv',sep=' ') #Replace test.csv with path of the input file, input must contain actual and predicted clusterid.

def rand_index(actual, pred):    #first argument: actual clusterid list of all points and second argument: predicted clusterid list

    tp_plus_fp = comb(np.bincount(actual), 2).sum()
    tp_plus_fn = comb(np.bincount(pred), 2).sum()
    A = np.c_[(actual, pred)]
    tp = sum(comb(np.bincount(A[A[:, 0] == i, 1]), 2).sum()
             for i in set(actual))
    fp = tp_plus_fp - tp
    fn = tp_plus_fn - tp
    tn = comb(len(A), 2) - tp - fp - fn
    return (tp + tn) / (tp + fp + fn + tn)

rand_index(clustering_output['actual'], clustering_output['predicted'])  
