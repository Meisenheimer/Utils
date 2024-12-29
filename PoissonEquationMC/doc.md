该程序的目标为通过直接模拟法求解$[0,1]$上Poisson方程。

求解目标为
$$
\begin{aligned}
& \Delta u(x) = f(x), & & x \in [0, 1] \\
& u(x) = g(x), & & x \in \{0, 1\}
\end{aligned}
$$
差分格式为
$$
\begin{aligned}
& \forall i \in \{ 1, \dots, n-1 \}, & & \frac{u_{i-1} - 2 u_i + u_{i+1}}{\Delta x^2} = f_i, \\
& u_0 = g_0, & & u_n = g_n
\end{aligned}
$$
整理后得到
$$
\frac{1}{2} u_{i-1} + \frac{1}{2} u_{i+1} - \frac{1}{2} f_i \Delta x^2 = u_i,
$$
因此考虑随机游走：当处于$0, n$时停止，对于任意点$i \in \{ 1, \dots, n-1 \}$，游走至$i+1$和$i-1$处的概率均为$\frac{1}{2}$。

若从$P$点开始的随机游走路径为$P \to P_1 \to \cdots \to P_{k-1} \to Q_k$，则取
$$
S = - \frac{1}{2} f_{P} \Delta x^2 - - \frac{1}{2} f_{P_1} \Delta x^2 - \cdots - - \frac{1}{2} f_{P_{k-1}} \Delta x^2 + g_{Q_k}
$$
从$P$点出发进行$m$次随机游走，可以得到$u_P$的估计值
$$
u_P = \frac{1}{m} \sum_{i=1}^m \theta_i.
$$
