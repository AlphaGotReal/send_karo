import sys
import numpy as np
from matplotlib import pyplot as plt

def generate_gaussian(shape:tuple, mu:tuple=(0, 0), sigma:tuple=(1, 1), show:bool=False):
    h, w = shape
    x, y = np.meshgrid(np.arange(w), np.arange(h))
    
    x = (x - mu[1])/sigma[1]
    y = (y - mu[0])/sigma[0]

    g = 1/(sigma[1] * sigma[0] * 2 * np.pi) * np.exp(-0.5*x*x - 0.5*y*y)

    if (show):
        plt.imshow(g, cmap="gray")
        plt.show()

    return g


