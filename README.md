# SHA-1 analysis

This repository is a reflection on the impossibility of reversing data from a SHA-1 hash without bruteforce.
## Menu
 - [Basic Knowledge](#basic-knowledge)
 - [The Equation of e](#the-equation-of-e)

## Basic Knowledge

SHA-1 Register Relations (Forward):
```math
registers = 
\begin{cases}
a_i = \left( (a_{i-1} \ll 5) \mid (a_{i-1} \gg 27) \right) + \left( d_{i-1} \oplus \left( b_{i-1} \land (c_{i-1} \oplus d_{i-1}) \right) \right) + e_{i-1} + k + w_i \\
b_i = a_{i-1} \\
c_i = \left( (b_{i-1} \ll 30) \mid (b_{i-1} \gg 2) \right) \\
d_i = c_{i-1} \\
e_i = d_{i-1}
\end{cases}
```
<br>

SHA-1 Register Relations (Backward):
```math
registers =
\begin{cases}
d_{i-1} = e_i \\
c_{i-1} = d_i \\
b_{i-1} = \left( (c_i \ll 2) \mid (c_i \gg 30) \right) \\
a_{i-1} = b_i
\end{cases}
```
## The Equation of *e*

In SHA-1, some information is lost between rounds : the value of register *e* at round *i* is no longer accessible at round *i+1*.

However, we can use the equation of *a[i+1]* to recover *e[i]*, provided we have the initial data (the chunk). While *e[i]* appears to be lost at round *i+1*, the definition of *a[i+1]* is:

```math
a_{i+1} = \left( (a_i \ll 5) \mid (a_i \gg 27) \right) + \left( d_i \oplus \left( b_i \land (c_i \oplus d_i) \right) \right) + e_i + k + w_{i+1}
```

Which is equivalent to :

```math
a_{i+1} = \left( (b_{i+1} \ll 5) \mid (b_{i+1} \gg 27) \right) + \left( e_{i+1} \oplus \left( \left( (c_{i+1} \ll 2) \mid (c_{i+1} \gg 30) \right) \land (d_{i+1} \oplus e_{i+1}) \right) \right) + e_i + k + w_{i+1}
```
<br>

And since we know the initial data, we also know *w[i]*, so the only unknown in this equation is *e[i]* :

```math
e_{i} = a_{i+1} - \left( \left( a_{i} \ll 5 \mid a_{i} \gg 27 \right) + \left( d_{i} \oplus \left( b_{i} \land (c_{i} \oplus d_{i}) \right) \right) + k + w_{i+1} \right)
```