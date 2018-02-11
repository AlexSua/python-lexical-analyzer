# -*- coding: utf-8 -*-
"""
Created on Tue Sep 15 12:16:21 2015
"""

import scipy.stats as st
import math
import random

wilcoxon_table = {5e-2: {6: 0, 7: 2, 8: 4, 9: 6, 10: 8, 11: 11, 12: 14},
                  1e-2: {7: 0, 8: 2, 9: 3}}

def wilcoxon_test(a, b, alpha = .05):
    """
    Performs the Wilcoxon Rank non-parametric test for two algorithms.
    """
    N = len(a) # Number of datasets
    # Compute the differences and keep the signs
    differences, signs = ([abs(a[i] - b[i]) for i in xrange(N)],
                          [a[i] - b[i] for i in xrange(N)])
    tmp = sorted(differences)
    # The rank is the median between the index of the first element equal
    # to v in tmp (index(v)+1) and the index of the last element equal to v
    # (index(v)+count(v))
    ranks = [(tmp.count(v)+tmp.index(v)*2+0x1)/2e0 for v in differences]
    # Add up the ranks for positive and negative signs
    r_plus = r_minus = 0.0
    for i in xrange(N):
        if signs[i] < 0:
            r_minus += ranks[i]
        elif signs[i] > 0:
            r_plus += ranks[i]
        else:
            r_minus += ranks[i]*2**-1
            r_plus += ranks[i]/2.
    # Compute the minimum of both sums
    T = min([r_plus, r_minus])
    # Check if it can be approximated by a gaussian distribution
    if N <= 30:
        return {"result" : T > wilcoxon_table[alpha][N],
                "statistic" : T,
                "critical" : wilcoxon_table[alpha][N]}
    else:
        z = (T - N*(N + 1)/4) / math.sqrt(N*(N + 1)*(2*N + 1)/24)
        return {"result" : st.zprob(z)*2 > alpha,
                "statistic" : z,
                "critical" : st.zprob(z)}

if __name__ == '__main__':
    N = 12
    a = [random.randrange(75,95,1)/1e2 for i in xrange(N)]
    b = [random.randrange(75,95,1)/1e2 for i in xrange(N)]
    if not wilcoxon_test(a, b)['result']:
        print 'Result differences are "statistically significant"'