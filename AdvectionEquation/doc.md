考虑$\Omega = [0, l] \times [0, T]$上的方程
$$
\begin{aligned}
& u_t = -a u_x, && (x, t) \in \Omega, \\
& u(0, t) = u(l, t), && t \in [0, T], \\
& u(x, 0) = \eta(x), && x \in [0, l].
\end{aligned}
$$
设格点数为$n+1$，空间步长为$\Delta x =\frac{l}{n}$，$\mathbf{u}(t)$为$t$时刻的数值解，对$u_x$使用中心差分格式，则有
$$
\mathbf{u}^\prime (t) = A \mathbf{u} (t),
$$
其中
$$
A = -\frac{a}{2 \Delta x} \begin{pmatrix}
	0 & 1 & & & & -1 \\
	-1 & 0 & 1 & & & \\
	& -1 & 0 & 1 & & \\
	& & \ddots & \ddots & \ddots & \\
	& & & 1 & 0 & -1 \\
	1 & & & & -1 & 0 \\
\end{pmatrix}
$$
其为反对称矩阵，因此特征值全部分布于虚轴上。

现考虑使用不同的IVP方法进行时间推进，程序应当根据算法的RAS是否包含虚轴部分有不同表现。
$$
\begin{aligned}
    \mathbf{y}_1 & = \mathbf{u}_n + \Delta t \mathbf{f}(\mathbf{u}_n, t_n) \\ 
    \mathbf{y}_2 & = \frac{3}{4} \mathbf{u}_n + \frac{1}{4} \mathbf{y}_1 + \frac{1}{4} \Delta t \mathbf{f}(\mathbf{y}_1, t_n + k) \\
    \mathbf{u}_{n+1} & = \frac{1}{3} \mathbf{u}_n + \frac{2}{3} \mathbf{y}_2 + \frac{2}{3} \Delta t \mathbf{f}\left(\mathbf{y}_2, t_n + \frac{k}{2}\right) \\
\end{aligned}
$$
