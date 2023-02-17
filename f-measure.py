import pandas as pd
import numpy as np

from scipy.special import comb
from math import *

clustering_output = pd.read_csv(r'/intermediatefiles/cId.csv',sep=' ') #Replace test.csv with path of the input file, input must contain actual and predicted clusterid.

clusters = clustering_output.groupby('actual')['predicted'].apply(list)

classes = clustering_output.groupby('predicted')['actual'].apply(list)

precision= []
recall = []
for class_i in classes:
    class_i = pd.DataFrame(class_i)
    class_i = class_i[0].value_counts()
    precision_i = class_i.max() / class_i.sum()
    recall_i = class_i.max() / len(clusters[class_i.idxmax()])
    precision += [precision_i]
    recall += [recall_i]
precision = np.array(precision)
recall = np.array(recall)
f_measure  = np.average((2 * precision*recall)/(precision+recall))
print('f-measure: ', f_measure)

