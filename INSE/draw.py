import math
import numpy as np
from matplotlib import pyplot

fp = open("res.dat", "r")
s = fp.readlines()
n = round(math.sqrt(len(s)))
fp.close()

u = np.zeros((n, n))
v = np.zeros((n, n))
uv = np.zeros((n, n))
x, y = 0, 0
for line in s:
    tmp = [item for item in line.split(' ')]
    u[y, x] = float(tmp[2])
    v[y, x] = float(tmp[3])
    uv[y, x] = math.sqrt(u[y, x] * u[y, x] + v[y, x] * v[y, x])
    x += 1
    if (x == n):
        x = 0
        y += 1

x = np.arange(0, 2.0 + 1.0 / (n - 1), 2.0 / (n - 1))
y = np.arange(0, 2.0 + 1.0 / (n - 1), 2.0 / (n - 1))
x, y = np.meshgrid(x, y)

fig = pyplot.figure(figsize=(8, 6))
pyplot.contourf(x, y, uv, alpha=0.5, cmap="rainbow")
pyplot.colorbar()
pyplot.contour(x, y, uv, cmap="rainbow")
pyplot.quiver(x, y, u, v)
pyplot.savefig("res.png")
