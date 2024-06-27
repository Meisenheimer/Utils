该程序的目标为实现求解矩形区域上的Poisson方程的多重网格法。

求解目标为
$$
\begin{aligned}
& \Delta u(x) = f(x), & & x \in \Omega \\
& \alpha(x) u(x) + \beta(x) \frac{\partial u}{\partial \mathbf{n}}(x) = g(x), & & x \in \partial \Omega
\end{aligned}
$$
其中$\Omega$为给定的矩形区域。

使用的迭代法格式为weighted Jacobi，记松弛因子为$\omega$，$k = \frac{2}{\Delta x^2}+\frac{2}{\Delta y^2}$：

对于内部点，其二阶格式为
$$
f = \frac{u_{i-1,j} + u_{i+1,j} }{\Delta x^2} + \frac{u_{i,j-1} + u_{i,j+1}}{\Delta y^2} - k u_{i,j}, \\
u^*_{i,j} = (1 - \omega) u_{i,j} + \frac{\omega}{k} \left( \left( \frac{u_{i-1,j} + u_{i+1,j} }{\Delta x^2} + \frac{u_{i,j-1} + u_{i,j+1}}{\Delta y^2} \right) - f \right). \\
$$


对于边界点，记$u$为边界上的点，$u_i$为垂直边界方向上的内部点，$d_1$为$u, u_i$方向的网格大小，则一阶格式为
$$
\alpha u + \beta \frac{u - u_i}{d_1} = g, \\
u^* = (1 - \omega) u + \frac{\omega}{\alpha + \frac{\beta}{d_1}} \left( \frac{\beta}{d_1} u_i + g \right).
$$
