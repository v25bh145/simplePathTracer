# How to calculate dpdx & dpdy?

```
scene.cpp:
unsigned Scene::intersect(Interaction *interaction)
```

![image-20220222173132930](https://s2.loli.net/2022/02/22/cToUjnpumy3L1Ki.png)

## inference

#### Algebraic Representation of CD

WARNING: can't use the vector ray.direction to represent CD, because denominator may be 0

So should move C along the direction of the vector ray.direction, get E, then use two-point form.
$$
e=c+ray.direction
$$

$$
\frac{x-c_x}{x-e_x}=\frac{y-c_y}{y-e_y}=\frac{z-c_z}{z-e_z}
$$
Then transform the equation to linear from.
$$
\frac{e_x-c_x}{x-e_x}=\frac{e_y-c_y}{y-e_y}=\frac{e_z-c_z}{z-e_z}
$$

#### Algebraic Representation of triangle(where BD is inside it)

$$
n_xx+n_yy+n_zz=n_xb_x+n_yb_y+n_zb_z
$$

#### calculate

$$
\begin{aligned}
\begin{cases}n_xx+n_yy+n_zz=n_xb_x+n_yb_y+n_zb_z\\\frac{e_x-c_x}{x-e_x}=\frac{e_y-c_y}{y-e_y}=\frac{e_z-c_z}{z-e_z}\end{cases}
&=\begin{cases}n_xx+n_yy+n_zz=n_xb_x+n_yb_y+n_zb_z\\\frac{e_x-c_x}{x-e_x}=\frac{e_y-c_y}{y-e_y}\\\frac{e_x-c_x}{x-e_x}=\frac{e_z-c_z}{z-e_z}\end{cases}
\\&=\begin{cases}n_xx+n_yy+n_zz=n_xb_x+n_yb_y+n_zb_z\\(e_y-c_y)x-(e_x-c_x)y=e_x(e_y-c_y)-e_y(e_x-c_x)\\(e_z-c_z)x-(e_x-c_x)z=e_x(e_z-c_z)-e_z(e_x-c_x)\end{cases}
\end{aligned}
$$

$$
AX=B\\
\begin{pmatrix}n_x&n_y&n_z\\e_y-c_y&-e_x+c_x&0\\e_z-c_z&0&-e_x+c_x\end{pmatrix}
\begin{pmatrix}x\\y\\z\end{pmatrix}=
\begin{pmatrix}n_xb_x+n_yb_y+n_zb_z\\e_x(e_y-c_y)-e_y(e_x-c_x)\\e_x(e_z-c_z)-e_z(e_x-c_x)\end{pmatrix}
$$