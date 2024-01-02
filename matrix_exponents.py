import numpy as np
from math import e

def matrix_from_complex(c:complex) -> np.ndarray:

    r, i = c.real, c.imag

    return r * np.array([
                [1, 0],
                [0, 1]
            ]) + i * np.array([
                    [0, -1],
                    [1, 0]
                ])

def exponent_from_matrix(A:np.ndarray) -> np.ndarray:

    exp = np.array([
            [1, 0],
            [0, 1]    
        ], dtype=np.float64)

    a_pow_n = np.array([
            [1, 0],
            [0, 1]
        ], dtype=np.float64)

    n_fac = 1

    for n in range(1, 100):
    
        a_pow_n = a_pow_n.dot(A)
        n_fac = n_fac * n
        exp = exp + (a_pow_n / n_fac)

    return exp

def display(A:np.ndarray) -> None:

    print()
    for row in A:
        for r in row:
            print(round(r, 3), end='\t')
        print()
    print()

T = -3

A = matrix_from_complex(T)
B = exponent_from_matrix(A)
display(B)

C = matrix_from_complex(e**(T))
display(C)

