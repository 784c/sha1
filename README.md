# SHA-1 analysis

This repository is a reflection on the impossibility of reversing data from a SHA-1 hash without bruteforce.
## Menu
 - [Basic Knowledge](#basic-knowledge)
 - [The Equation of e](#the-equation-of-e)
 - [The Equation of w](#the-equation-of-w)

## Basic Knowledge

### SHA-1 Register Relations (Forward) :
```math
registers = 
\begin{cases}
a_i = \left( (a_{i-1} \ll 5) \mid (a_{i-1} \gg 27) \right) + f_i + e_{i-1} + k + w_i \\
b_i = a_{i-1} \\
c_i = (b_{i-1} \ll 30) \mid (b_{i-1} \gg 2) \\
d_i = c_{i-1} \\
e_i = d_{i-1}
\end{cases}
```
<br>

if 0 ≤ *i* < 20 :
```math
f_i = d_{i-1} \oplus \left( b_{i-1} \land (c_{i-1} \oplus d_{i-1}) \right)
```

if 20 ≤ *i* < 40 :
```math
f_i = b_{i-1} \oplus c_{i-1} \oplus d_{i-1}
```

if 40 ≤ *i* < 60 :
```math
f_i = (b_{i-1} \land c_{i-1}) \lor (b_{i-1} \land d_{i-1}) \lor (c_{i-1} \land d_{i-1})
```

if 60 ≤ *i* < 80 :
```math
f_i = b_{i-1} \oplus c_{i-1} \oplus d_{i-1}
```

### SHA-1 Register Relations (Backward) :
```math
registers =
\begin{cases}
d_{i-1} = e_i \\
c_{i-1} = d_i \\
b_{i-1} = (c_i \ll 2) \mid (c_i \gg 30) \\
a_{i-1} = b_i
\end{cases}
```

if 0 ≤ *i* < 20 :
```math
f_{i-1} = e_i \oplus \left( \left( (c_i \ll 2) \mid (c_i \gg 30) \right) \land (d_i \oplus e_i) \right)
```

if 20 ≤ *i* < 40 :
```math
f_{i-1} = \left( (c_i \ll 2) \mid (c_i \gg 30) \right) \oplus d_i \oplus e_i
```

if 40 ≤ *i* < 60 :
```math
f_{i-1} = (\left( (c_i \ll 2) \mid (c_i \gg 30) \right) \land d_i) \lor (\left( (c_i \ll 2) \mid (c_i \gg 30) \right) \land e_i) \lor (d_i \land e_i)
```

if 60 ≤ *i* < 80 :
```math
f_{i-1} = \left( (c_i \ll 2) \mid (c_i \gg 30) \right) \oplus d_i \oplus e_i
```

## The Equation of *e*

In SHA-1, some information is lost between rounds : the value of register *e* at round *i* is no longer accessible at round *i+1*.

However, we can use the equation of *a[i+1]* to recover *e[i]*, provided we have the initial data (*w*). While *e[i]* appears to be lost at round *i+1*, the definition of *a[i+1]* is:

```math
a_{i+1} = \left( (a_i \ll 5) \mid (a_i \gg 27) \right) + f_{i+1} + e_i + k + w_{i+1}
```

Which is equivalent to :

```math
a_{i+1} = \left( (b_{i+1} \ll 5) \mid (b_{i+1} \gg 27) \right) + f_{i+1} + e_i + k + w_{i+1}
```
<br>

And since we know the initial data, we also know *w[i]*, so the only unknown in this equation is *e[i]* :

```math
\begin{align}
e_i &= a_{i+1} - \left( \_left\_rotate(a_i, 5) + f_{i+1} + k + w_{i+1} \right) \\
e_{i} &= a_{i+1} - \left( \left( (a_i \ll 5) \mid (a_i \gg 27) \right) + f_{i+1} + k + w_{i+1} \right)
\end{align}
```

## The Equation of *w*

In SHA-1, *w[0]* through *w[15]* correspond to the input chunk, and the subsequent words *w[16]* to *w[79]* are computed from this initial chunk.
The operation from *w[16]* to *w[79]* is :

```math
\begin{align}
w_{i} &= \_left\_rotate\big( w_{i - 3} \oplus w_{i - 8} \oplus w_{i - 14} \oplus w_{i - 16},\, 1 \big) \\
w_{i} &= \left( \left( w_{i - 3} \oplus w{i - 8} \oplus w_{i - 14} \oplus w_{i - 16} \right) \ll 1 \right) \mid \left( \left( w_{i - 3} \oplus w_{i - 8} \oplus w_{i - 14} \oplus w_{i - 16} \right) \gg 31 \right)
\end{align}
```