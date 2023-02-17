import pandas as pd
import numpy as np

from scipy.special import comb
from math import *

clustering_output = pd.read_csv(r'test.csv',sep='\t') #Replace test.csv with path of the input file, input must contain actual and predicted clusterid.

clusters = clustering_output.groupby('actual')['predicted'].apply(list)

cluster_entropy=[]
for cluster in clusters:
    cluster = pd.DataFrame(cluster)
    classes=cluster[0].value_counts()
    classes = classes/float(cluster.count())
    e = (classes * [log(x, 2) for x in classes]).sum()
    cluster_entropy += [-e]
cluster_size = np.array([len(c) for c in clusters])
cluster_fraction = cluster_size/float(cluster_size.sum())
entropy = (cluster_fraction * cluster_entropy).sum()
print ('entropy: ', entropy)

