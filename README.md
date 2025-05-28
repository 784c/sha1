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
```