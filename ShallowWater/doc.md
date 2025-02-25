## 一维浅水方程（Saint-Venant方程）

### 控制方程

考虑计算域$[0,1] \times [0, T]$，一维浅水方程的控制方程为
$$
\frac{\partial h}{\partial t} + \frac{\partial (h u)}{\partial x} = 0, \\
\frac{\partial (h u)}{\partial t} + \frac{\partial (h u^2 + \frac{1}{2} g h^2)}{\partial x} = 0, \\
$$
其中$h$为水深，$u$为速度。

## 数值格式

$$
\begin{aligned}
\frac{\partial h}{\partial t} + h \frac{\partial u}{\partial x} + u \frac{\partial h}{\partial x} = 0 & & (1) \\
h \frac{\partial u}{\partial t} + u \frac{\partial h}{\partial t} + 2 u h \frac{\partial u}{\partial x} + u^2 \frac{\partial h}{\partial x} + g h \frac{\partial h}{\partial x} = 0 & & (2) \\
\end{aligned}
$$

$(1)$两端乘$u$并减去$(2)$再除以$h$，得到
$$
\begin{aligned}
\frac{\partial u}{\partial t} + u \frac{\partial u}{\partial x} + g \frac{\partial h}{\partial x} = 0 & & (3) \\
\end{aligned}
$$
与$(1)$联立，
$$
\begin{aligned}
\frac{\partial h}{\partial t} + h \frac{\partial u}{\partial x} + u \frac{\partial h}{\partial x} = 0 & & (1) \\
\frac{\partial u}{\partial t} + u \frac{\partial u}{\partial x} + g \frac{\partial h}{\partial x} = 0 & & (3) \\
\end{aligned}
$$
则有
$$
\frac{\partial}{\partial t} \begin{pmatrix} h \\ u \end{pmatrix} + \frac{\partial}{\partial x} \begin{pmatrix} h u \\ \frac{1}{2} u^2 + gh \end{pmatrix} = 0
$$
对于方程
$$
\frac{\partial u}{\partial t} + \frac{\partial f(x, t, u(x, t))}{\partial x} = 0,
$$
Richtmyer提出的两步格式为
$$
\begin{aligned}
u^{n+\frac{1}{2}}_{i+\frac{1}{2}} = \frac{1}{2} (u^n_i + u^n_{i+1}) - \frac{\Delta t}{2 \Delta x} (f(u^n_{i+1}) - f(u^n_{i})) \\
u^{n+1}_{i} = u^{n}_{i} - \frac{\Delta t}{\Delta x} \left( f\left(u^{n+\frac{1}{2}}_{i+\frac{1}{2}}\right) - f\left(u^{n+\frac{1}{2}}_{i-\frac{1}{2}}\right) \right)
\end{aligned}
$$

Lax-Friedrichs格式为
$$
u^{n+1}_i = \frac{1}{2} (u^{n}_{i+1} + u^{n}_{i-1}) - \frac{\Delta t}{2 \Delta x} (f(u^{n}_{i+1}) - f(u^{n}_{i-1}))
$$
Lax-Wendroff格式为
$$
u^{n+1}_i = u^{n}_i - \frac{\Delta t}{2 \Delta x} (f(u^{n}_{i+1}) - f(u^{n}_{i-1})) + \frac{\Delta t^2}{2 \Delta x^2} \left( a^n_{i+\frac{1}{2}} (f(u^n_{i+1}) - f(u^n_i)) - a^n_{i-\frac{1}{2}} (f(u^n_i) - f(u^n_{i-1})) \right) \\
a^n_{i \pm \frac{1}{2}} = \nabla f \left(\frac{u^n_i + u^n_{i \pm 1}}{2}\right) = \begin{pmatrix} \frac{\partial f_h}{\partial h} & \frac{\partial f_h}{\partial u} \\ \frac{\partial f_u}{\partial h} & \frac{\partial f_u}{\partial u} \end{pmatrix}
$$

## 数值样例

$$
\begin{aligned}
& h(x, 0) = 1 + \frac{2}{5} exp(−5 x^2), & & \forall x \in [0, 1] \\
& h(0, t) = h(1, t) = 1, & & \forall t \in [0, T], \\
& u(0, t) = u(1, t) = 0, & & \forall t \in [0, T].
\end{aligned}
$$

---

$$
\begin{aligned}
& h(x, 0) = \begin{cases} 2, & x \in [0, 0.5] \\ 1, & x \in (0.5, 1] \\ \end{cases} \\
& h(0, t) = 2, & & \forall t \in [0, T], \\
& h(1, t) = 1, & & \forall t \in [0, T], \\
& u(0, t) = u(1, t) = 0, & & \forall t \in [0, T].
\end{aligned}
$$

