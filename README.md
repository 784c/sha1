# SHA-1 analysis

This repository is a reflection on the impossibility of reversing data from a SHA-1 hash without bruteforce.
## Menu
 - [Basic Knowledge](#basic-knowledge)
 - [The Equation of e](#the-equation-of-e)
 - [The Equation of w](#the-equation-of-w)
 - [The Equation with two unknowns](#the-equation-with-two-unknowns)

## Basic Knowledge

### SHA-1 Register Relations (Forward) :
```math
registers = 
\begin{cases}
a_i = \_left\_rotate(a_{i-1}, 5) + f_i + e_{i-1} + k_{i} + w_i \\
a_i = \left( (a_{i-1} \ll 5) \mid (a_{i-1} \gg 27) \right) + f_i + e_{i-1} + k_{i} + w_i \\
b_i = a_{i-1} \\
c_i = \_left\_rotate(b_{i-1}, 30) \\
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
b_{i-1} = \_left\_rotate(c_i, 2) \\
b_{i-1} = (c_i \ll 2) \mid (c_i \gg 30) \\
a_{i-1} = b_i
\end{cases}
```

if 0 ≤ *i* < 20 :
```math
\begin{align}
f_{i-1} &= e_i \oplus \left( \_left\_rotate(c_i, 2) \land (d_i \oplus e_i) \right) \\
f_{i-1} &= e_i \oplus \left( \left( (c_i \ll 2) \mid (c_i \gg 30) \right) \land (d_i \oplus e_i) \right)
\end{align}
```

if 20 ≤ *i* < 40 :
```math
\begin{align}
f_{i-1} &= \_left\_rotate(c_i, 2) \oplus d_i \oplus e_i \\
f_{i-1} &= \left( (c_i \ll 2) \mid (c_i \gg 30) \right) \oplus d_i \oplus e_i
\end{align}
```

if 40 ≤ *i* < 60 :
```math
\begin{align}
f_{i-1} &= (\_left\_rotate(c_i, 2) \land d_i) \lor (\_left\_rotate(c_i, 2) \land e_i) \lor (d_i \land e_i) \\
f_{i-1} &= (\left( (c_i \ll 2) \mid (c_i \gg 30) \right) \land d_i) \lor (\left( (c_i \ll 2) \mid (c_i \gg 30) \right) \land e_i) \lor (d_i \land e_i)
\end{align}
```

if 60 ≤ *i* < 80 :
```math
\begin{align}
f_{i-1} &= \_left\_rotate(c_i, 2) \oplus d_i \oplus e_i \\
f_{i-1} &= \left( (c_i \ll 2) \mid (c_i \gg 30) \right) \oplus d_i \oplus e_i
\end{align}
```

## The Equation of *e*

In SHA-1, some information is lost between rounds : the value of register *e* at round *i* is no longer accessible at round *i+1*.

However, we can use the equation of *a[i+1]* to recover *e[i]*, provided we have the initial data (*w*). While *e[i]* appears to be lost at round *i+1*, the definition of *a[i+1]* is:

```math
\begin{align}
a_{i+1} &= \_left\_rotate(a_i, 5) + f_{i+1} + e_i + k_{i+1} + w_{i+1} \\
a_{i+1} &= \left( (a_i \ll 5) \mid (a_i \gg 27) \right) + f_{i+1} + e_i + k_{i+1} + w_{i+1}
\end{align}
```

Which is equivalent to :

```math
\begin{align}
a_{i+1} &= \_left\_rotate(b_{i+1}, 5) + f_{i+1} + e_i + k_{i+1} + w_{i+1} \\
a_{i+1} &= \left( (b_{i+1} \ll 5) \mid (b_{i+1} \gg 27) \right) + f_{i+1} + e_i + k_{i+1} + w_{i+1}
\end{align}
```
<br>

And since we know the initial data, we also know *w[i]*, so the only unknown in this equation is *e[i]* :

```math
\begin{align}
e_i &= a_{i+1} - \left( \_left\_rotate(a_i, 5) + f_{i+1} + k_{i+1} + w_{i+1} \right) \\
e_{i} &= a_{i+1} - \left( \left( (a_i \ll 5) \mid (a_i \gg 27) \right) + f_{i+1} + k_{i+1} + w_{i+1} \right)
\end{align}
```

## The Equation of *w*

In SHA-1, *w[0]* through *w[15]* correspond to the input chunk, and the subsequent words *w[16]* to *w[79]* are computed from this initial chunk.
The operation from *w[16]* to *w[79]* is :

```math
\begin{align}
w_{i} &= \_left\_rotate\big( w_{i - 3} \oplus w_{i - 8} \oplus w_{i - 14} \oplus w_{i - 16},\, 1 \big) \\
w_{i} &= \left( \left( w_{i - 3} \oplus w_{i - 8} \oplus w_{i - 14} \oplus w_{i - 16} \right) \ll 1 \right) \mid \left( \left( w_{i - 3} \oplus w_{i - 8} \oplus w_{i - 14} \oplus w_{i - 16} \right) \gg 31 \right)
\end{align}
```
<br>
And the equation is :

```math
\begin{align}
w_{i} &= a_{i} - \big( \_left\_rotate(a_{i-1},\, 5) + f_{i} + e_{i-1} + k_{i} \big) \\
w_{i} &= a_{i} - \big( \big( (a_{i-1} \ll 5) \mid (a_{i-1} \gg 27) \big) + f_{i} + e_{i-1} + k_{i} \big)
\end{align}
```
<br>

But even if you know the result of the equation (*w[i]*), there are still too many possible values.  
For example, let's take *w[50]* = 1000 :

```math
\begin{align}
w_{50} = \_left\_rotate\big( w_{47} \oplus w_{42} \oplus w_{36} \oplus w_{34},\, 1 \big) = 1000
\end{align}
```

According to [possibilities.py](https://github.com/784c/sha1/blob/main/scripts/possibilities.py) (wich is a bit broken for the moment) :  
- There are 64 possible values for `((a_b_c ^ d) << 1) | ((a_b_c ^ d) >> 31) = 1000`
- Based on those, there are 32 768 possibilities for solving `(a_b ^ c) = a_b_c`
- And from those, there are 16 777 216 possibilities for solving `(a ^ b) = a_b`

## The Equation with two unknowns

As mentioned earlier, completing the equation for *e* requires knowing *w*, and computing *w* depends on the initial data or depends on *e*. The most challenging part is that brute-forcing offers no clear stopping point : if you try to brute-force *w* in order to solve the *e* equation, there's no way to verify whether the current value of *w* is actually correct.  

The real challenge in SHA-1 is finding *w* without knowing *e*, or *e* without knowing *w*. If you succeed, recovering the initial data becomes possible, since the remaining equation then has only one unknown.