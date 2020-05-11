import numpy as np
from scipy.linalg import expm
import matplotlib.pyplot as plt

def transform(mat, dim, scale=1.0):
    for row in range(dim):
        for col in range(1, dim-1):
            sum = np.abs(mat[row][col-1] + mat[row][col+1])
            mat[row][col] *= (1.0 - sum) * scale

    return mat


dim = 12
dcy = 2

A = np.zeros((dim, dim))
for ind1 in range(dim):
    for ind2 in range(ind1, dim):
        A[ind1][ind2] = (-1 + 2 * np.random.uniform()) * (ind2 - ind1)**dcy
        A[ind2][ind1] = -A[ind1][ind2]

U = expm(A)

plt.figure()
plt.pcolor(U, cmap='inferno')
plt.colorbar()

for _ in range(1000):
    ID = np.transpose(U) @ U

    oldU = np.copy(U)
    U = transform(U, dim, 1.0)

    diff = np.sum(np.abs(U) - np.abs(oldU))
    # print(diff)

plt.figure()
plt.pcolor(U, cmap='inferno')
plt.colorbar()
plt.show()
