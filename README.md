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

The main challenge in reversing SHA-1 stems from the circular dependency between *w* and *e* : each depends on the other to be computed (or on the initial data for *w*). Since brute-forcing provides no way to validate partial results, breaking this dependency is essential. If you manage to recover either *w* or *e*, the rest of the data can be deduced, as the equation then has only one unknown.  

## Notes

```
i=70 : a = 861089886, b = 3779606719, c = 1356075940, d = 4152800796, e = 1955098220, k = 3395469782, w = 0
i=69 : a = ?, b = ?, c = ?, d = ?, e = ?, w = ?

d69 = e70 = 1955098220
c69 = d70 = 4152800796
b69 = _right_rotate(c70, 30) = 1129336465
a69 = b70 = 1475121602

f70 = b69 ^ c69 ^ d69 = 1129336465 ^ 4152800796 ^ 1955098220 = 3227424481

e69 = (a70 - (_left_rotate(a69, 5) + f70 + k70 + w70)) & 0xffffffff
e69 = (861089886 - (_left_rotate(3779606719, 5) + 3227424481 + 3395469782 + 0)) & 0xffffffff
e69 = (861089886 - (((3779606719 << 5) | (3779606719 >> 27)) + 3227424481 + 3395469782 + 0)) & 0xffffffff = 2139799467

f69 = b68 ^ c68 ^ d68 = _right_rotate(c69, 30) ^ d69 ^ e69 = _right_rotate(4152800796, 30) ^ 1955098220 ^ 2139799467 = 3575122356

w69 = (a69 - (_left_rotate(a69, 5) + f69 + e68 + k69)) & 0xffffffff
w69 = (3779606719 - (_left_rotate(1129336465, 5) + 3575122356 + e68 + 3395469782)) & 0xffffffff
w69 = (-4970013939 + e68) & 0xffffffff = (-4970013939 + ?) & 0xffffffff = ?

i=69 : a = 3779606719, b = 1129336465, c = 4152800796, d = 1955098220, e = 2139799467, w = ?

or

a69 = (_left_rotate(a68, 5) + f69 + e68 + k69 + w69) & 0xffffffff = 3779606719
a69 = (_left_rotate(b69, 5) + f69 + e68 + k69 + w69) & 0xffffffff = 3779606719
a69 = (_left_rotate(1129336465, 5) + 3575122356 + e68 + 3395469782 + w69) & 0xffffffff = 3779606719
a69 = (8749620658 + e68 + w69) & 0xffffffff = 3779606719

8749620658 & 0xffffffff = 159686066
(3779606719 - 159686066) & 0xffffffff = 3619920653
(e68 + w69) & 0xffffffff = 3619920653

but we need to verify this result :

if e68 = 0 and w69 = 3619920653:
    f68 = 3525425585 ^ 2139799467 ^ 0 = 2913666586 because f68 = b67 ^ c67 ^ d67 and d67 = e68

    a68 = 1129336465
    a68 = (_left_rotate(a67, 5) + f68 + e67 + k68 + w68) & 0xffffffff = 1129336465
    a68 = (_left_rotate(b68, 5) + f68 + e67 + k68 + w68) & 0xffffffff = 1129336465
    a68 = (_left_rotate(3726301299, 5) + 2913666586 + e67 + 3395469782 + w68) & 0xffffffff = 1129336465
    a68 = (9586660971 + e67 + w68) & 0xffffffff = 1129336465
    
    9586660971 & 0xffffffff = 996726379 --> size of what we know : _left_rotate(a67, 5) + f68 + k68
    1129336465 - 996726379 = 132610086 --> size of unknowns : e67 + w68
    e67 + w68 = 132610086

if e68 = 3619920653 and w69 = 0:
    f68 = 3525425585 ^ 2139799467 ^ 3619920653 = 2053676311 because f68 = b67 ^ c67 ^ d67 and d67 = e68

    a68 = 1129336465
    a68 = (_left_rotate(a67, 5) + f68 + e67 + k68 + w68) & 0xffffffff = 1129336465
    a68 = (_left_rotate(b68, 5) + f68 + e67 + k68 + w68) & 0xffffffff = 1129336465
    a68 = (_left_rotate(3726301299, 5) + 2053676311 + e67 + 3395469782 + w68) & 0xffffffff = 1129336465
    a68 = (8726670696 + e67 + w68) & 0xffffffff = 1129336465

    8726670696 & 0xffffffff = 136736104 --> size of what we know : _left_rotate(a67, 5) + f68 + k68
    1129336465 - 136736104 = 992600361 --> size of unknowns : e67 + w68
    e67 + w68 = 992600361
```

```
i=68 : a = ?, b = ?, c = ?, d = ?, e = ?, w = ?
i=67 : a = 3726301299, b = 3525425585, c = 2139799467, d = 3619920653, e = 992600361, w = 0

e68 = d67 = 3619920653
d68 = c67 = 2139799467
c68 = _left_rotate(b67, 30) = 1955098220
b68 = a67 = 3726301299

f68 = b67 ^ c67 ^ d67 = 3525425585 ^ 2139799467 ^ 3619920653 = 2053676311

a68 = (_left_rotate(a67, 5) + f68 + e67 + k68 + w68) & 0xffffffff
a68 = (_left_rotate(3726301299, 5) + 2053676311 + 992600361 + 3395469782 + w68) & 0xffffffff
a68 = (((3726301299 << 5) | (3726301299 >> 27)) + 2053676311 + 992600361 + 3395469782 + w68) & 0xffffffff
a68 = (125683388049 + w68) & 0xffffffff = (125683388049 + ?) & 0xffffffff = ?

i=68 : a = ?, b = 3726301299, c = 1955098220, d = 2139799467, e = 3619920653, w = ?
```

```
Resolution example :

i=79 : a = 2652194797, b = 1475121602, c = 821726152, d = 3346553428, e = 2817500643, w = 0
i=78 : a = ?, b = ?, c = ?, d = ?, e = ?, w = ?

a78 = b79 = 1475121602
b78 = _right_rotate(c79, 30) = 3286904608
c78 = d79 = 3346553428
d78 = e79 = 2817500643

f79 = b78 ^ c78 ^ d78 = 3286904608 ^ 3346553428 ^ 2817500643 = 2742931607

e78 = (a79 - (_left_rotate(a78, 5) + f79 + k79 + w79)) & 0xffffffff
e78 = (2652194797 - (_left_rotate(1475121602, 5) + 2742931607 + 3395469782 + 0)) & 0xffffffff
e78 = (2652194797 - (((1475121602 << 5) | (1475121602 >> 27)) + 2742931607 + 3395469782 + 0)) & 0xffffffff = 849509686

f78 = b77 ^ c77 ^ d77 = _right_rotate(c78, 30) ^ d78 ^ e78 = _right_rotate(3346553428, 30) ^ 2817500643 ^ 849509686 = 2293019014

can't solve this equation because e77 and w78 are unknwowns :
w78 = (a78 - (_left_rotate(a77, 5) + f78 + e77 + k78)) & 0xffffffff

i=78 : a = 1475121602, b = 3286904608, c = 3346553428, d = 2817500643, e = 849509686, w = ?

so :

a78 = (_left_rotate(a77, 5) + f78 + e77 + k78 + w78) & 0xffffffff = 1475121602
a78 = (_left_rotate(b78, 5) + f78 + e77 + k78 + w78) & 0xffffffff = 1475121602
a78 = (_left_rotate(3286904608, 5) + 2293019014 + e77 + 3395469782 + w78) & 0xffffffff = 1475121602
a78 = (7790221172 + e77 + w78) & 0xffffffff = 1475121602

7790221172 & 0xffffffff = 3495253876 --> size of what we know
(1475121602 - 3495253876) & 0xffffffff = 2274835022 --> size of unknowns
(e77 + w78) & 0xffffffff = 2274835022

e77 = d76 = c75 = b74 = a73, (b74 = _right_rotate(b75, 30), a73 = b74)
d76 --> f77 --> a77 --> b78 --> [...]

f77 = b76 ^ c76 ^ d76 = _right_rotate(c77, 30) ^ d77 ^ e77 :
c77 = d78 = 2817500643
d77 = e78 = 849509686
f77 = _right_rotate(2817500643, 30) ^ 849509686 ^ e77 = 2904353464 ^ e77

Let's take e77 = 0, w78 = 2274835022 for example :
    f77 = 2904353464 ^ 0 = 2904353464

    a76 = b77 = _right_rotate(c78, 30) = _right_rotate(3346553428, 30) = 501311827

    a77 = (_left_rotate(a76, 5) + f77 + e76 + k77 + w77) & 0xffffffff = b78 = 3286904608
    a77 = (_left_rotate(501311827, 5) + 2904353464 + e76 + 3395469782 + w77) & 0xffffffff = b78 = 3286904608
    a77 = (9456899825 + e76 + w77) & 0xffffffff = b78 = 3286904608

    9456899825 & 0xffffffff = 866965233 --> size of what we know
    (3286904608 - 866965233) & 0xffffffff = 2419939375 --> size of unknowns

    a77 = (_left_rotate(a76, 5) + f77 + e76 + k77 + w77) & 0xffffffff = b78
    a77 = (_left_rotate(501311827, 5) + f77 + e76 + 3395469782 + w77) & 0xffffffff = b78 = 3286904608
    a77 = (6552546361 + f77 + e76 + w77) & 0xffffffff = b78 = 3286904608

    The idea was to compare the sum of what we know in the equation, but it's impossible because f77 depends on e77 (d76 = e77) --> another unknown.
    But there is still a thing that looks interessant : if we choose a value for e[i], d[i-1] is impacted and f[i] in a[i] too.
    Maybe an idea to verify the value that we choosed.
```

```
a[i]   = _left_rotate(a[i-1], 5) + f[i] + e[i-1] + k[i] + w[i]
a[i-1] = b[i]

b[i]   = a[i-1]
b[i-1] = _left_rotate(c[i], 2)

c[i]   = _left_rotate(b[i-1], 30)
c[i-1] = d[i]

d[i]   = c[i-1]
d[i-1] = e[i]

e[i]   = d[i-1]
e[i-1] = a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])

w[i]   = a[i] - (_left_rotate(a[i-1], 5) + f[i] + e[i-1] + k[i])
w[i-1] = b[i] - (_left_rotate(_left_rotate(c[i], 2), 5) + f[i-1] + e[i-2] + k[i-1])

if 0 =< i < 20 :
    f[i]   = d[i-1] ^ (b[i-1] & (c[i-1] ^ d[i-1]))

    f[i-1] = d[i-2] ^ (b[i-2] & (c[i-2] ^ d[i-2]))
    f[i-1] = e[i-1] ^ (_left_rotate(c[i-1], 2) & (d[i-1] ^ e[i-1]))
    f[i-1] = (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])) ^ (_left_rotate(d[i], 2) & (e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))))
    
    w[i-1] = b[i] - (_left_rotate(_left_rotate(c[i], 2), 5) + ((a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])) ^ (_left_rotate(d[i], 2) & (e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))))) + e[i-2] + k[i-1])
    e[i-2] = b[i] - (_left_rotate( _left_rotate(c[i], 2), 5) + ((a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])) ^ (_left_rotate(d[i], 2) & (e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))))) + k[i-1] + w[i-1])

    w[i-1] = b[i] - (_left_rotate(_left_rotate(c[i], 2), 5) + ((a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])) ^ (_left_rotate(d[i], 2) & (e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))))) + (b[i] - (_left_rotate( _left_rotate(c[i], 2), 5) + ((a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])) ^ (_left_rotate(d[i], 2) & (e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))))) + k[i-1] + w[i-1])) + k[i-1])

if 20 =< i < 40 :
    f[i]   = b[i-1] ^ c[i-1] ^ d[i-1]

    f[i-1] = b[i-2] ^ c[i-2] ^ d[i-2]
    f[i-1] = _left_rotate(c[i-1], 2) ^ d[i-1] ^ e[i-1]
    f[i-1] = _left_rotate(d[i], 2) ^ e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))

    w[i-1] = b[i] - (_left_rotate(_left_rotate(c[i], 2), 5) + (_left_rotate(d[i], 2) ^ e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) + e[i-2] + k[i-1])
    e[i-2] = b[i] - (_left_rotate( _left_rotate(c[i], 2), 5) + (_left_rotate(d[i], 2) ^ e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) + k[i-1] + w[i-1])

    w[i-1] = b[i] - (_left_rotate(_left_rotate(c[i], 2), 5) + (_left_rotate(d[i], 2) ^ e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) + (b[i] - (_left_rotate( _left_rotate(c[i], 2), 5) + (_left_rotate(d[i], 2) ^ e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) + k[i-1] + w[i-1])) + k[i-1])

if 40 =< i < 60 :
    f[i]   = (b[i-1] & c[i-1]) | (b[i-1] & d[i-1]) | (c[i-1] & d[i-1])

    f[i-1] = (b[i-2] & c[i-2]) | (b[i-2] & d[i-2]) | (c[i-2] & d[i-2])
    f[i-1] = (_left_rotate(c[i-1], 2) & d[i-1]) | (_left_rotate(c[i-1], 2) & e[i-1]) | (d[i-1] & e[i-1])
    f[i-1] = (_left_rotate(d[i], 2) & e[i]) | (_left_rotate(d[i], 2) & (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) | (e[i] & (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])))

    w[i-1] = b[i] - (_left_rotate(_left_rotate(c[i], 2), 5) + ((_left_rotate(d[i], 2) & e[i]) | (_left_rotate(d[i], 2) & (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) | (e[i] & (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])))) + e[i-2] + k[i-1])
    e[i-2] = b[i] - (_left_rotate( _left_rotate(c[i], 2), 5) + ((_left_rotate(d[i], 2) & e[i]) | (_left_rotate(d[i], 2) & (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) | (e[i] & (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])))) + k[i-1] + w[i-1])

    w[i-1] = b[i] - (_left_rotate(_left_rotate(c[i], 2), 5) + ((_left_rotate(d[i], 2) & e[i]) | (_left_rotate(d[i], 2) & (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) | (e[i] & (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])))) + (b[i] - (_left_rotate( _left_rotate(c[i], 2), 5) + ((_left_rotate(d[i], 2) & e[i]) | (_left_rotate(d[i], 2) & (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) | (e[i] & (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])))) + k[i-1] + w[i-1])) + k[i-1])

if 60 =< i < 80 :
    f[i]   = b[i-1] ^ c[i-1] ^ d[i-1]

    f[i-1] = b[i-2] ^ c[i-2] ^ d[i-2]
    f[i-1] = _left_rotate(c[i-1], 2) ^ d[i-1] ^ e[i-1]
    f[i-1] = _left_rotate(d[i], 2) ^ e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))
    
    w[i-1] = b[i] - (_left_rotate(_left_rotate(c[i], 2), 5) + (_left_rotate(d[i], 2) ^ e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) + e[i-2] + k[i-1])
    e[i-2] = b[i] - (_left_rotate( _left_rotate(c[i], 2), 5) + (_left_rotate(d[i], 2) ^ e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) + k[i-1] + w[i-1])

    w[i-1] = b[i] - (_left_rotate(_left_rotate(c[i], 2), 5) + (_left_rotate(d[i], 2) ^ e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) + (b[i] - (_left_rotate( _left_rotate(c[i], 2), 5) + (_left_rotate(d[i], 2) ^ e[i] ^ (a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i]))) + k[i-1] + w[i-1])) + k[i-1])

The real problem is here, the circular dependency : we need w[i-1] to solve w[i-1].
Because we need w[i] to solve e[i-1] and e[i-1] to solve w[i].
We need a way to get e[i-1] without w[i] or w[i] without e[i-1].

e[i-1] = a[i] - (_left_rotate(b[i], 5) + f[i] + k[i] + w[i])
f[i-1] = b[i-2] ^ c[i-2] ^ d[i-2] = _left_rotate(c[i-1], 2) ^ d[i-1] ^ e[i-1]

to get e[77] we need f[77], let's take for example f[77] = 1000 :
f[77] = _left_rotate(c[i-1], 2) ^ d[i-1] ^ e[i-1] = _left_rotate(d[i], 2) ^ e[i] ^ e[i-1] = 1000
f[77] = _left_rotate(d[77], 2) ^ e[77] ^ e[i-1] = 1000

But how to get f[77] ?
f[77] = a[77] - (_left_rotate(a[76], 5) + e[76] + k[77] + w[77])
But we don't have e[76] and w[77].
```

```
i=79 : a = 2652194797, b = 1475121602, c = 821726152, d = 3346553428, e = 2817500643, f = 2742931607, w = 0
i=78 : a = ?, b = ?, c = ?, d = ?, e = ?, f = ?, w = ?
i=77 : a = ?, b = ?, c = ?, d = ?, e = 2274835022, f = ?, w = ?
i=76 : a = ?, b = ?, c = ?, d = ?, e = ?, f = ?, w = ?
i=75 : a = ?, b = ?, c = ?, d = ?, e = 4225431107, f = ?, w = ?

a78 = b79 = 1475121602
b78 = _left_rotate(c79, 2) = _left_rotate(821726152, 2) = 3286904608
c78 = d79 = 3346553428
d78 = e79 = 2817500643
f79 = b78 ^ c78 ^ d78 = 3286904608 ^ 3346553428 ^ 2817500643 = 2742931607
e78 = (a79 - (_left_rotate(a78, 5) + f79 + k79 + w79)) & 0xffffffff = (2652194797 - (_left_rotate(1475121602, 5) + 2742931607 + 3395469782 + 0)) & 0xffffffff = 849509686
f78 = b77 ^ c77 ^ d77 = _left_rotate(c78, 2) ^ d78 ^ e78 = _left_rotate(3346553428, 2) ^ 2817500643 ^ 849509686 = 2293019014
a77 = b78 = 3286904608
e77 = 2274835022
w78 = (a78 - (_left_rotate(a77, 5) + e77 + k78 + f78)) & 0xffffffff = (1475121602 - (_left_rotate(3286904608, 5) + 2274835022 + 3395469782 + 2293019014)) & 0xffffffff = 0

i=79 : a = 2652194797, b = 1475121602, c = 821726152, d = 3346553428, e = 2817500643, f = 2742931607, w = 0
i=78 : a = 1475121602, b = 3286904608, c = 3346553428, d = 2817500643, e = 849509686, f = 2293019014, w = 0
i=77 : a = ?, b = ?, c = ?, d = ?, e = 2274835022, f = ?, w = ?
i=76 : a = ?, b = ?, c = ?, d = ?, e = ?, f = ?, w = ?
i=75 : a = ?, b = ?, c = ?, d = ?, e = 4225431107, f = ?, w = ?

a77 = b78 = 3286904608
b77 = _left_rotate(c78, 2) = _left_rotate(3346553428, 2) = 501311827
c77 = d78 = 2817500643
d77 = e78 = 849509686
e77 = 2274835022
f77 = b76 ^ c76 ^ d76 = _left_rotate(c77, 2) ^ d77 ^ e77 = _left_rotate(2817500643, 2) ^ 849509686 ^ 2274835022 = 713806070

w77 = (a77 - (_left_rotate(a76, 5) + e76 + k77 + f77)) & 0xffffffff = (3286904608 - (_left_rotate(501311827, 5) + e76 + 3395469782 + 713806070)) & 0xffffffff
w77 = (-3979447823 + e76) & 0xffffffff

i=79 : a = 2652194797, b = 1475121602, c = 821726152, d = 3346553428, e = 2817500643, f = 2742931607, w = 0
i=78 : a = 1475121602, b = 3286904608, c = 3346553428, d = 2817500643, e = 849509686, f = 2293019014, w = 0
i=77 : a = 3286904608, b = 501311827, c = 2817500643, d = 849509686, e = 2274835022, f = 713806070, w = ?
i=76 : a = 501311827, b = 2680067982, c = 849509686, d = 2274835022, e = ?, f = ?, w = ?
i=75 : a = 2680067982, b = 3398038744, c = 2274835022, d = ?, e = 4225431107, f = ?, w = ?
```

```
Semi reversed SHA-1 :

Odd round (row in which e and w are already known) :
    a[i] = b[i+1]
    b[i] = _left_rotate(c[i+1], 2)
    c[i] = d[i+1]
    d[i] = e[i+1]

    if 0 <= i < 20:
        f[i] = d[i-1] ^ (b[i-1] & (c[i-1] ^ d[i-1])) = e[i] ^ (_left_rotate(c[i], 2) & (d[i] ^ e[i]))
    if 20 <= i < 40:
        f[i] = b[i-1] ^ c[i-1] ^ d[i-1] = _left_rotate(c[i], 2) ^ d[i] ^ e[i]
    if 40 <= i < 60:
        f[i] = (b[i-1] & c[i-1]) | (b[i-1] & d[i-1]) | (c[i-1] & d[i-1]) = (_left_rotate(c[i], 2) & d[i]) | (_left_rotate(c[i], 2) & e[i]) | (d[i] & e[i])
    if 60 <= i < 80:
        f[i] = b[i-1] ^ c[i-1] ^ d[i-1] = _left_rotate(c[i], 2) ^ d[i] ^ e[i]

Even round (blank row)
    a[i] = b[i+1]
    b[i] = _left_rotate(c[i+1], 2)
    c[i] = d[i+1]
    d[i] = e[i+1]

    if 0 <= i < 20:
        f[i+1] = d[i] ^ (b[i] & (c[i] ^ d[i]))
    if 20 <= i < 40:
        f[i+1] = b[i] ^ c[i] ^ d[i]
    if 40 <= i < 60:
        f[i+1] = (b[i] & c[i]) | (b[i] & d[i]) | (c[i] & d[i])
    if 60 <= i < 80:
        f[i+1] = b[i] ^ c[i] ^ d[i]

    e[i] = a[i+1] - (_left_rotate(a[i], 5) + f[i+1] + k[i+1] + w[i+1])

    if 0 <= i < 20:
        f[i] = d[i-1] ^ (b[i-1] & (c[i-1] ^ d[i-1])) = e[i] ^ (_left_rotate(c[i], 2) & (d[i] ^ e[i]))
    if 20 <= i < 40:
        f[i] = b[i-1] ^ c[i-1] ^ d[i-1] = _left_rotate(c[i], 2) ^ d[i] ^ e[i]
    if 40 <= i < 60:
        f[i] = (b[i-1] & c[i-1]) | (b[i-1] & d[i-1]) | (c[i-1] & d[i-1]) = (_left_rotate(c[i], 2) & d[i]) | (_left_rotate(c[i], 2) & e[i]) | (d[i] & e[i])
    if 60 <= i < 80:
        f[i] = b[i-1] ^ c[i-1] ^ d[i-1] = _left_rotate(c[i], 2) ^ d[i] ^ e[i]

    a[i-1] = b[i]
    w[i] = (a[i] - (_left_rotate(a[i-1], 5) + e[i-1] + k[i] + f[i]))

Worked example :

i=39 : a = 2078076151, b = 2352833088, c = 3144677536, d = 2465103991, e = 746733994, f = 2780568215, w = 0
i=38 : a = ?, b = ?, c = ?, d = ?, e = ?, f = ?, w = ?
i=37 : a = ?, b = ?, c = ?, d = ?, e = 246108336, f = ?, w = 0
i=36 : a = ?, b = ?, c = ?, d = ?, e = ?, f = ?, w = ?

a38 = b39 = 2352833088
b38 = _left_rotate(c39, 2) = _left_rotate(3144677536, 2) = 3988775554
c38 = d39 = 2465103991
d38 = e39 = 746733994
f39 = b38 ^ c38 ^ d38 = 3988775554 ^ 2465103991 ^ 746733994 = 1406346079
k39 = 1859775393
w39 = 0
e38 = a39 - (_left_rotate(a38, 5) + f39 + k39 + w39) = (2078076151 - (_left_rotate(2352833088, 5) + 1406346079 + 1859775393 + 0)) & 0xffffffff = 830707174
f38 = _left_rotate(c38, 2) ^ d38 ^ e38 = _left_rotate(2465103991, 2) ^ 746733994 ^ 830707174 = 1455150482
k38 = 1859775393
e37 = 246108336
w37 = 0
w38 = (a38 - (_left_rotate(a37, 5) + e37 + k38 + f38)) = (2352833088 - (_left_rotate(3988775554, 5) + 246108336 + 1859775393 + 1455150482)) & 0xffffffff = 0

i=39 : a = 2078076151, b = 2352833088, c = 3144677536, d = 2465103991, e = 746733994, f = 2780568215, w = 0
i=38 : a = 2352833088, b = 3988775554, c = 2465103991, d = 746733994, e = 830707174, f = 1455150482, w = 0
i=37 : a = ?, b = ?, c = ?, d = ?, e = 246108336, f = ?, w = 0
i=36 : a = ?, b = ?, c = ?, d = ?, e = ?, f = ?, w = ?

a37 = b38 = 3988775554
b37 = _left_rotate(2465103991, 2) = 1270481374
c37 = 746733994
d37 = 830707174
f37 = _left_rotate(746733994, 2) ^ 830707174 ^ 246108336 = 2367800318

i=39 : a = 2078076151, b = 2352833088, c = 3144677536, d = 2465103991, e = 746733994, f = 2780568215, w = 0
i=38 : a = 2352833088, b = 3988775554, c = 2465103991, d = 746733994, e = 830707174, f = 1455150482, w = 0
i=37 : a = 3988775554, b = 1270481374, c = 746733994, d = 830707174, e = 246108336, f = 2367800318, w = 0
i=36 : a = ?, b = ?, c = ?, d = ?, e = ?, f = ?, w = ?
```

```
40 rounds where we have to find two numbers between 0 and 4294967295 inclusive.
Or 80 rounds where we have to find one number between 0 and 4294967295 inclusive.

If we search for two unknowns over 40 rounds :
2^32 x 2^32 = 2^64 for 1 round
(2^64)^40 = 2^(64x40) = 2^2560 total possibilities.

If we search for one unknown over 80 rounds :
2^32 for 1 round
(2^32)^80 = 2^(32x80) = 2^2560 total possibilities.

So, the number of possibilities hasn't actually been reduced — what changes is the structure and the solving approach.
What's beneficial is that, if for example we find a weakness in the relationship involving e, then the number of possibilities could be halved :
2^32 for 1 round
(2^32)^40 = 2^(32x40) = 2^1280 total possibilities.

But :
a2 = (3031962158 + w2) & 0xffffffff

We know a2 is between 0 and 4294967295 inclusive, and :
4294967295 - 3031962158 = 1263005137, so at first glance, it might seem like w2 must be between 0 and 1263005137. But there's a problem: the 0xffffffff mask.
Even if w2 exceeds 1263005137, it can still produce a valid a2 due to the wraparound effect.
Therefore, the range of possible w2 values remains [0; 4294967295], even after this.
And without knowing the actual value of a2, we can't rule out even a single possible value for w2 in that range.

But there's something else : reducing the number of valid pairs.
At the very start of the analysis, I came across this kind of equation :

(e + w) & 0xffffffff = 3619920653
Here’s what we know :
- e and w are both in [0; 4294967295]
- The sum of e + w must be 3619920653 (with & 0xffffffff)

Individually, we can’t rule out a single value in [0; 4294967295] for e or w.
But when we consider the sum, it’s different.
For instance: the pair e = 0 and w = 0 is invalid because (0 + 0) & 0xffffffff != 3619920653.

So we can restrict the search space for pairs — for example, the pair (0, 0) is guaranteed to be useless.

(e + w) & 0xffffffff = 3619920653
Let’s call e_w = e + w :
e_w & 11111111111111111111111111111111 = 11010111110000111001101100001101

There are 2^32 possibilities for e and 2^32 for w, which makes 2^64 theoretical combinations per round, 18446744073709551616 total possibilities.

There are only 3619920653 + 1 = 3619920654 valid (e + w) values that satisfy :
(e + w) & 0xffffffff = 3619920653

That means we've eliminated :
18446744073709551616 - 3619920654 = 18446744070089630962 impossible combinations.

Another example, (e + w) & 0xffffffff = 992600361 : possibilities = 992600361 + 1 = 992600362, 18446744073709551616 - 992600362 = 18446744072716951254 impossible combinations.
```