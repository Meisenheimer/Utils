该程序使用数值方法计算不可压Navier-Stokes方程和对流扩散方程。

# 不可压Navier-Stokes方程

粘性不可压流动方程形式为
$$
\begin{aligned}
\frac{\partial \mathbf{u}}{\partial t} + \mathbf{u} \cdot \nabla \mathbf{u} =& \mathbf{f} - \frac{1}{\rho} \nabla p + \nu \Delta \mathbf{u}, \\
\nabla \cdot \mathbf{u} =& 0. 
\end{aligned}
$$

考虑计算域
$$
\Omega = \{ (x, y): x \in [0, s],\ y \in [0, s] \},
$$

初始条件为

$$
\mathbf{u}(x, y) = \mathbf{0},
$$

边界条件为

$$
\begin{aligned}
	\mathbf{u}(0, y) =& \mathbf{u}(s, y) = \mathbf{0}, & & \forall y \in [0, s], \\
	\mathbf{u}(x, 0) =& \mathbf{0}, \mathbf{u}(x, s) = (u_h, 0)^T, & & \forall x \in [0, s], \\
\end{aligned}
$$

其中$\rho, \mu$​为常数。

---

# 数值算法

## 算法流程

1. **预算速度**：忽略压力梯度项，显式地使用动量方程来计算中间速度$\mathbf{u}^*$：

$$
\frac{\mathbf{u}^* - \mathbf{u}^n}{\Delta t} = \mathbf{f} - (\mathbf{u}^n \cdot \nabla) \mathbf{u}^n + \nu \Delta \mathbf{u}^n =: \mathbf{F}(\mathbf{u}^n), \\
u^* = u^n + \Delta t \left(f_x + \nu \left( \frac{\partial^2 u^n}{\partial x^2} + \frac{\partial^2 u^n}{\partial y^2} \right) - \left( u^n \frac{\partial u^n}{\partial x} + v^n \frac{\partial u^n}{\partial y} \right) \right), \\
v^* = v^n + \Delta t \left(f_y + \nu \left( \frac{\partial^2 v^n}{\partial x^2} + \frac{\partial^2 v^n}{\partial y^2} \right) - \left( u^n \frac{\partial v^n}{\partial x} + v^n \frac{\partial v^n}{\partial y} \right) \right). \\
$$

2. **计算压强**：通过中间速度$\mathbf{u}^*$使用迭代法求解Poisson方程计算压强：

$$
\Delta p^{n+1} = \frac{\rho}{\Delta t} \nabla \cdot \mathbf{u}^*, \left. \frac{\partial p^{n+1}}{\partial \mathbf{n}} \right|_{\partial \Omega} = 0, \\
\nabla \cdot \mathbf{u}^* = \frac{\partial u}{\partial x} + \frac{\partial v}{\partial y}
$$

3. **矫正速度**：校正中间速度$\mathbf{u}^*$以获得时间步的最终解：

$$
\frac{\mathbf{u}^{n+1} - \mathbf{u}^*}{\Delta t} =  - \frac{1}{\rho} \nabla p^{n+1}.
$$

## 数值格式

对于$x,y$方向上格点数一致的正方形计算域，有$\Delta x = \Delta y = \Delta s$。

---

对$u \frac{\partial u}{\partial x}$，令$f(u) = u^2$，Richtmyer提出的两步格式为

$$
\begin{aligned}
u^{n+\frac{1}{2}}_{i+\frac{1}{2}} = \frac{1}{2} (u^n_i + u^n_{i+1}) - \frac{\Delta t}{2 \Delta x} (f(u^n_{i+1}) - f(u^n_{i})) \\
\frac{1}{2} \frac{\partial u^2}{\partial x} = u \frac{\partial u}{\partial x} = \frac{1}{2 \Delta x} \left( f\left(u^{n+\frac{1}{2}}_{i+\frac{1}{2}}\right) - f\left(u^{n+\frac{1}{2}}_{i-\frac{1}{2}}\right) \right)
\end{aligned}
$$

---

对$a \frac{\partial u}{\partial x}$采用Lax-Wendroff格式，即
$$
a \frac{\partial u}{\partial x} = \frac{a}{2 \Delta x} (u_{i+1,j} - u_{i-1,j}) - \frac{a^2 \Delta t}{2 \Delta x^2} (u_{i+1,j} - 2 u_{i,j} + u_{i-1,j}).
$$

---

二阶项采用中心差分格式，即
$$
\Delta u = \frac{u_{i,j} + u_{i,j} + u_{i,j} + u_{i,j} - 4 u_{i,j}}{\Delta s^2}.
$$
迭代法格式为
$$
p_{i,j} = \frac{p_{i-1,j} + p_{i+1,j} + p_{i,j-1} + p_{i,j+1} - \frac{\Delta s^2 \rho}{\Delta t} \nabla \cdot \mathbf{u}^*}{4}.
$$

---

时间推进采用三阶强稳定龙格库塔方法
$$
\begin{aligned}
    \mathbf{y}_1 & = \mathbf{u}_n + \Delta t \mathbf{f}(\mathbf{u}_n, t_n) \\ 
    \mathbf{y}_2 & = \frac{3}{4} \mathbf{u}_n + \frac{1}{4} \mathbf{y}_1 + \frac{1}{4} \Delta t \mathbf{f}(\mathbf{y}_1, t_n + k) \\
    \mathbf{u}_{n+1} & = \frac{1}{3} \mathbf{u}_n + \frac{2}{3} \mathbf{y}_2 + \frac{2}{3} \Delta t \mathbf{f}\left(\mathbf{y}_2, t_n + \frac{k}{2}\right) \\
\end{aligned}
$$

# 结果对照

[Lid-driven cavity problem -- CFD-Wiki, the free CFD reference](https://www.cfd-online.com/Wiki/Lid-driven_cavity_problem)

# 对流扩散方程

设体积分数为$F$, 对流扩散方程形式为
$$
\frac{\partial F}{\partial t} + u \frac{\partial F}{\partial x} + v \frac{\partial F}{\partial y} = \mu \left(\frac{\partial^2 F}{\partial x^2} + \frac{\partial^2 F}{\partial y^2}\right) + r.
$$

考虑计算域
$$
\Omega = \{ (x, y): x \in [0, s],\ y \in [0, s] \},
$$

初始条件为

$$
F(x, y) = 0,
$$

边界条件为

$$
\begin{aligned}
	F(0, y) =& F(s, y) = 0, & & \forall y \in [0, s], \\
	F(x, 0) =& 0, F(x, s) = F_h, & & \forall x \in [0, s]. \\
\end{aligned}
$$

方程两边同时对控制体$\hat\Omega = [x_i, x_{i+1}] \times [y_i, y_{i+1}]$积分，得到
$$
\int_\hat\Omega \frac{\partial F}{\partial t} \mathrm{d} x + \int_\hat\Omega \mathbf{u} \cdot \nabla F \mathrm{d} x = \mu \int_\hat\Omega \Delta F \mathrm{d} x + \int_\hat\Omega r \mathrm{d} x,
$$
由散度定理，
$$
\Delta s^2 \frac{\partial \bar F}{\partial t} = \mu \int_{\partial\hat\Omega} \frac{\partial F}{\partial \mathbf{n}} \mathrm{d} s -  \int_{\partial\hat\Omega} F \mathbf{u} \cdot \mathbf{n} \mathrm{d} s + \hat r,
$$
数值格式为
$$
\bar F^*_{i,j} = \bar F_{i,j} + \Delta t \left( \frac{\mu(\bar F_{i+1,j} + \bar F_{i-1,j} + \bar F_{i,j+1} + \bar F_{i,j-1} - 4 \bar F_{i,j})}{\Delta s^2} - \frac{1}{\Delta s^2} \int_{\partial\hat\Omega} F \mathbf{u} \cdot \mathbf{n} \mathrm{d} s + \hat r \right),
$$
