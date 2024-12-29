# 问题描述

计算$\Omega = [0, 1] \times [0, 1]$上的极小曲面方程
$$
\frac{\partial}{\partial x} \left( \frac{\frac{\partial u}{\partial x}}{\sqrt{1 + \left( \frac{\partial u}{\partial x} \right)^2 + \left( \frac{\partial u}{\partial y} \right)^2}} \right) + \frac{\partial}{\partial y} \left( \frac{\frac{\partial u}{\partial y}}{\sqrt{1 + \left( \frac{\partial u}{\partial x} \right)^2 + \left( \frac{\partial u}{\partial y} \right)^2}} \right) = 0, \\
u \big|_{\Gamma_1} = \tilde u, \\
\frac{\partial u}{\partial \mathbf{n}} \big|_{\Gamma_2} = 0, \\
$$

其中$\Gamma_1 \cap \Gamma_2 = \emptyset$，$\Gamma_1 \cup \Gamma_2 = \partial \Omega$。

---

# 数值算法

## 变分

设$f = \big( 1 + \vert \nabla u \vert^2 \big)^{-\frac{1}{2}}$，代入原方程得到
$$
\nabla \cdot (f \nabla u) = 0,
$$

在等式两边乘$v \in \left\{ v: v \in L^2(\Omega), \nabla v \in L^2(\Omega), v\big|_{\Gamma_1} = 0 \right\}$，并在$\Omega$上积分，由于边界上$v = 0$或$\nabla u = 0$，因此得到

$$
\begin{aligned}
& \int_\Omega v \nabla \cdot (f \nabla u) \mathrm{d} x = 0, \\
\Rightarrow & \int_\Omega \nabla \cdot (f v \nabla u) \mathrm{d} x - \int_\Omega \nabla v f \nabla u \mathrm{d} x = 0, \\
\Rightarrow & \int_{\partial\Omega} f v \nabla u \mathbf{n} \mathrm{d} s - \int_\Omega f \nabla v \nabla u \mathrm{d} x = 0, \\
\Rightarrow & \int_\Omega f \nabla v \nabla u \mathrm{d} x = 0, \\
\Rightarrow & \int_\Omega \frac{\nabla v \nabla u}{\sqrt{1 + \vert \nabla u \vert^2}} \mathrm{d} x = 0, \\
\end{aligned}
$$

---

## 多项式插值

考虑均匀正方形网格，设格点数为$n+1$，$h = \Delta x = \Delta y = \frac{1}{n}$，则下标为$(i,j)$的点坐标为$(ih, jh) = (\frac{i}{n}, \frac{j}{n})$。

由于函数平移不变，只需考虑$[0, h]\times[0,h]$，令$u_1 = u(0,0), u_2 = u(h,0), u_3 = (0,h), u_4 = (h,h)$，则其上的插值基函数为
$$
\begin{aligned}
f_1(x, y) =& \frac{1}{h^2} (h - x) (h - y), & \nabla f_1 (x, y) =& \frac{1}{h^2} \begin{pmatrix} y - h \\ x - h \end{pmatrix}, \\
f_2(x, y) =& \frac{1}{h^2} x (h - y), & \nabla f_2 (x, y) =& \frac{1}{h^2} \begin{pmatrix} h - y \\ - x \end{pmatrix}, \\
f_3(x, y) =& \frac{1}{h^2} (h - x) y, & \nabla f_3 (x, y) =& \frac{1}{h^2} \begin{pmatrix} -y \\ h - x \end{pmatrix}, \\
f_4(x, y) =& \frac{1}{h^2} x y, & \nabla f_4 (x, y) =& \frac{1}{h^2} \begin{pmatrix} y \\ x \end{pmatrix}, \\
\end{aligned}
$$
其上的双线性元为
$$
u(x, y) = u_1 f_1 + u_2 f_2 + u_3 f_3 + u_4 f_4, \\
\nabla u(x, y) = u_1 \nabla f_1 + u_2 \nabla f_2 + u_3 \nabla f_3 + u_4 \nabla f_4,
$$

---

## 数值积分

记
$$
F_{v}(x, y) = \frac{\nabla v(x, y) \nabla u(x, y)}{\sqrt{1 + \vert \nabla u(x, y) \vert^2}} = \frac{\nabla v (u_1 \nabla f_1 + u_2 \nabla f_2 + u_3 \nabla f_3 + u_4 \nabla f_4)}{\sqrt{1 + \vert u_1 \nabla f_1 + u_2 \nabla f_2 + u_3 \nabla f_3 + u_4 \nabla f_4 \vert^2}}, \\
$$
$[0, h]\times[0,h]$上的几类数值积分公式为
$$
\begin{aligned}
\int_0^1 \int_0^1 F(x, y) \mathrm{d}x \ \mathrm{d}y =& \frac{h^2}{4} (F(0, 0) + F(h, 0) + F(0, h) + F(h, h)) + \frac{h^4}{12} (f_{xx} + f_{yy}) + O(h^6) \\

\int_0^1 \int_0^1 F(x, y) \mathrm{d}x \ \mathrm{d}y =& \frac{h^2}{12} (F(0, 0) + F(h, 0) + F(0, h) + F(h, h) + 8 F(\frac{h}{2}, \frac{h}{2})) + \frac{h^6}{2880} (f_{xxxx} + 10 f_{xxyy} + f_{yyyy}) + O(h^6) \\

\int_0^1 \int_0^1 F(x, y) \mathrm{d}x \ \mathrm{d}y =& \frac{h^2}{36} (F(0, 0) + F(h, 0) + F(0, h) + F(h, h) + 16 F\left(\frac{h}{2}, \frac{h}{2}\right) \\
& \left. + 4 F\left(0, \frac{h}{2}\right) + 4 F\left(\frac{h}{2}, 0\right) + 4 F\left(h, \frac{h}{2}\right) + 4 F\left(\frac{h}{2}, h\right)\right) + \frac{h^6}{2880} (f_{xxxx} + f_{yyyy}) + O(h^8).
\end{aligned}
$$

---

## 算法流程

记
$$
F_{v}(x, y) = \frac{\nabla v(x, y) \nabla u(x, y)}{\sqrt{1 + \vert \nabla u(x, y) \vert^2}} = \frac{\nabla v (u_1 \nabla f_1 + u_2 \nabla f_2 + u_3 \nabla f_3 + u_4 \nabla f_4)}{\sqrt{1 + \vert u_1 \nabla f_1 + u_2 \nabla f_2 + u_3 \nabla f_3 + u_4 \nabla f_4 \vert^2}}, \\
$$
问题为在$[0,h] \times[0,h]$区域上求解$u_1, u_2, u_3, u_4$，使得对任意$v \in \{f_1(x,y), f_2(x,y), f_3(x,y), f_4(x,y)\}$，均有
$$
\int_0^1 \int_0^1 F_v(x, y) \mathrm{d}x \ \mathrm{d}y = 0,
$$
因此考虑优化目标为
$$
\min_{u_1, u_2, u_3, u_4 \in \mathbb{R}} (\int_0^1 \int_0^1 F_v(x, y) \mathrm{d}x \ \mathrm{d}y)^2.
$$
在程序实现过程中，计算导数只需逐个区域计算角点处的导数，再累加即可。
