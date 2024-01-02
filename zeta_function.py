import math
import numpy as np

def zeta(s, till=100):

    sum_ = 0

    for r in range(1, till+1):
        sum_ += (1/r**s)

    return sum_


