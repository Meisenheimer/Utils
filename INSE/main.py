import math
import numpy as np
from matplotlib import pyplot as plt, cm

# Global settings
ns = 33
s_min = 0
s_max = 2

ds = (s_max - s_min) / (ns - 1)

rho = 1
nu = 0.1
dt = 0.001

u = np.zeros((ns, ns))
v = np.zeros((ns, ns))
p = np.zeros((ns, ns))

# Boundary conditions
# U TOP
u[ns - 1, :] = 1
# U BOTTOM
u[0, :] = 0
# U RIGHT
u[:, ns - 1] = 0
# U LEFT
u[:, 0] = 0

# V TOP
v[ns - 1, :] = 0
# V BOTTOM
v[0, :] = 0
# V RIGHT
v[:, ns - 1] = 0
# V LEFT
v[:, 0] = 0


def set_pressure_boundary(p):
    # Pressure boundary conditions (Neumann or "second-type")
    '''
    Trick question: Would the approach below break depending on
    whether you use forward, backward, or centered differences?
    Why or why not?
    '''
    p[:, ns - 1] = p[:, ns - 2]  # dp/dx = 0 at x = 2
    p[0, :] = p[1, :]   # dp/dy = 0 at y = 0
    p[:, 0] = p[:, 1]   # dp/dx = 0 at x = 0
    # Pressure boundary conditions (Dirichlet or "fixed")
    p[ns - 1, :] = 0        # p = 0 at y = 2

    return p


def diff_1st_x(f):
    return (f[1:-1, 2:] - f[1:-1, 0:-2])/(2*ds)


def diff_1st_y(f):
    return (f[2:, 1:-1] - f[0:-2, 1:-1])/(2*ds)

# Discretization


def forward_diff_x(f):
    return (f[1:-1, 2:] - f[1:-1, 1:-1])/ds


def forward_diff_y(f):
    return (f[2:, 1:-1] - f[1:-1, 1:-1])/ds


def diff_2nd_x(f):
    # WARNING: Probably not more computationally efficient,
    # but it should work, and it's pedagogically interesting.
    # return (forward_diff_x(f) - backward_diff_x(f))/dx
    # f(i-1,j) + 2*f(i,j) + f(i+1,j)
    return (f[1:-1, 0:-2] - 2*f[1:-1, 1:-1] + f[1:-1, 2:]) / ds ** 2


def diff_2nd_y(f):
    # return (forward_diff_y(f) - backward_diff_y(f))/dy
    # f(i,j-1) + 2*f(i,j) + f(i,j+1)
    return (f[0:-2, 1:-1] - 2*f[1:-1, 1:-1] + f[2:, 1:-1]) / ds ** 2


def laplacian(f):
    return diff_2nd_x(f) + diff_2nd_y(f)


# VELOCITY WITHOUT THE INFLUENCE OF THE PRESSURE...
# ...BECAUSE I DONT KNOW THAT YET :'(

def vel_without_pressure(un, vn):
    u_without_pressure = un.copy()
    v_without_pressure = vn.copy()
    u_without_pressure[1:ns - 1, 1:ns - 1] = (-(un[1:ns - 1, 1:ns - 1] * forward_diff_x(un)
                                                + vn[1:ns - 1, 1:ns - 1] * forward_diff_y(un))
                                              + nu * laplacian(un)) * dt + un[1:ns - 1, 1:ns - 1]
    v_without_pressure[1:ns - 1, 1:ns - 1] = (-(un[1:ns - 1, 1:ns - 1] * forward_diff_x(vn)
                                                + vn[1:ns - 1, 1:ns - 1] * forward_diff_y(vn))
                                              + nu * laplacian(vn)) * dt + vn[1:ns - 1, 1:ns - 1]
    return u_without_pressure, v_without_pressure


def get_R(u_without_pressure, v_without_pressure):
    R = np.zeros((ns, ns))
    R[1:-1, 1:-1] = (rho/dt) * (forward_diff_x(u) + forward_diff_y(v))
    return R


def get_pressure(p, R):
    # Δ𝑦2(𝑝𝑛+1(𝑖−1,𝑗)+𝑝𝑛+1(𝑖+1,𝑗))
    term1 = (p[1:ns - 1, 0:ns - 2] + p[1:ns - 1, 2:]) / 4
    # Δ𝑥2(𝑝𝑛+1(𝑖−1,𝑗)+𝑝𝑛+1(𝑖+1,𝑗))
    term2 = (p[0:ns - 2, 1:ns - 1] + p[2:, 1:ns - 1]) / 4
    # −R𝑛(𝑖,𝑗)Δ𝑥2Δ𝑦2
    term3 = -1 * R[1:ns - 1, 1:ns - 1] * (ds ** 2) / 4

    p[1:ns - 1, 1:ns - 1] = (term1 + term2 + term3)
    set_pressure_boundary(p)  # (Don't forget this.)
    return p


def update_velocity(u, v, u_without_pressure, v_without_pressure, p):
    u[1:-1, 1:-1] = (-dt/rho) * diff_1st_x(p) + u_without_pressure[1:-1, 1:-1]
    v[1:-1, 1:-1] = (-dt/rho) * diff_1st_y(p) + v_without_pressure[1:-1, 1:-1]
    return u, v


def simulate_cavity_flow(nt, u, v, p):
    for n in range(nt):
        u_without_pressure, v_without_pressure = vel_without_pressure(u, v)
        R = get_R(u_without_pressure, v_without_pressure)
        p = get_pressure(p, R)
        u, v = update_velocity(u, v, u_without_pressure, v_without_pressure, p)
    return u, v, p


simulate_cavity_flow(nt=1000, u=u, v=v, p=p)

x = np.linspace(s_min, s_max, ns)
y = np.linspace(s_min, s_max, ns)

X, Y = np.meshgrid(x, y)

# for i in range(ns):
#     for j in range(ns):
#         p[i, j] = math.sqrt(u[i, j] * u[i, j] + v[i, j] * v[i, j])

fig = plt.figure(figsize=(11, 7), dpi=100)
plt.contourf(X, Y, p, alpha=0.5, cmap=cm.viridis)
plt.colorbar()
plt.contour(X, Y, p, cmap=cm.viridis)
plt.quiver(X[::2, ::2], Y[::2, ::2], u[::2, ::2], v[::2, ::2])
plt.xlabel('X')
plt.ylabel('Y')
plt.show()
