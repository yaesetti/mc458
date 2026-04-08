# Lista 02 - Victor Yaegashi Setti - RA 206362

## Questão 1
### a) Propondo o algoritmo:

Supondo que as operações de inserção, remoção e subvetorização de vetor `inserir(V, u)`, `remover(V, index)` e `subvetor(V, i, f)` são todas $O(1)$:

```c++
  bruxaria(A, k):
01    S = []
02    if (!Magico(A, k)):
03        return S
04    
05    soma = k
06    for (i = 0; i < tamanho(A) and soma != 0; i++):
07        temp = A[i]
08        if ((i + 1 < n and !Magico(subvetor(A, i+1, n), soma)) or Magico(temp, soma)):
09            adiciona(S, temp)
10            soma = soma - temp
11    return S
```

### b) Provando a corretude do algoritmo:
Seja a invariante do laço na linha 06 tal que:<br>

$P(i) \Leftrightarrow \text{o vetor}$ `S` $\text{é tal que, até a posição i em A,} \sum_{j=0}^{|S|-1}{S[j]} + \text{soma = k e}$ `Magico(subvetor(A, i, n), soma)` $\text{é verdade}$.

Provando a invariante:

Antes de entrar no laço, o algoritmo termina caso `Magico(A, k)` retorne falso, indicando que não há subconjunto que satisfaça o requisito.<br>

Caso `Magico(A, k)` retorna vazio teremos que:<br>

`S = []` $\Rightarrow \sum_{j=0}^{|S|-1}{S[j]} = 0$<br>
`soma = k`

$\Rightarrow \sum_{j=0}^{|S|-1}{S[j]} + soma = 0 + k = k$<br>

Além disso, como já vimos que `Magico(A, k)` retorna verdadeiro para este caso $\Rightarrow$ A invariante vale.

Supondo uma iteração qualquer do laço tal que $P(c)$, na iteração seguinte teremos `c' = c + 1` e `temp = A[c']`, divindo em dois casos:

- Caso `Magico(subvetor(A, c + 1, n), soma - temp)` retorna verdadeiro $\Rightarrow$ `soma' = soma - temp` e `adiciona(S, x)` $\Rightarrow$ $(\sum_{j=0}^{|S|-1}{S[j]})' = \sum_{j=0}^{|S|-1}{S[j]} + temp \Rightarrow (\sum_{j=0}^{|S|-1}{S[j]})' + soma' = \sum_{j=0}^{|S|-1}{S[j]} + temp + soma - temp = k$ (utilizando $P(c)$).<br>
  Além disso, como temos também que `Magico(subvetor(A, c + 1, n), soma - temp)` = `Magico(subvetor(A, c', n), soma')` = verdadeiro.<br>
  $\Rightarrow P(c + 1)$ vale.

- No caso contrário teremos que `Magico(subvetor(A, c + 1, n), soma - temp)` retorna falso $\Rightarrow$ `temp` não pertence a solução $\Rightarrow$ tanto `S` quanto `soma` permanencem inalterados e valendo.<br>
  Além disso, teremos que `Magico(subvetor(A, c + 1, n), soma)` = `Magico(subvetor(A, c', n), soma)` = verdadeiro $\Rightarrow$ $P(c + 1)$ vale.<br>

Assim, a invariante vale $\forall{i} \in [0, |A|]$ no laço da linha 06.<br>

Sabendo que a condição de parada do laço é `i = |A|`, e dada a invariante, teremos que para $P(|A|)$ `subvetor(A, |A|, |A|)` é vazio e `Magico(subvetor(A, |A|, |A|), soma)` retorna verdadeiro $\Rightarrow$ soma = 0 $\Rightarrow$ $k = \sum_{j=0}^{|S|-1}{S[j]}$.

$\therefore \text{Como o algoritmo sempre retorna S em P(n), é provada a corretude. } \blacksquare$





### c) Provando a complexidade temporal:
Antes do laço são realizadas apenas operaçõs de complexidade temporal constante.<br>
Como o único laço do algoritmo `bruxaria` é um `for` que itera de 0 até n, sendo `n = tamanho(A)`, e dentro deste laço todas as operações são $O(1)$, teremos uma complexidade $T(n) = n \cdot O(1) \Rightarrow T(n) \in O(n)$.<br>
$\therefore$ O algoritmo `bruxaria` possui complexidade $\text{O(n) } \blacksquare$

## Questão 2
Usando o algoritmo dado em sala `intercala` podemos construir o algoritmo:
```c++
  merge_sort3(V, i, f):
01    if (i <= f):
02        return
03    
04    x = i + (f-1)/3
05    y = i + 2(f-1)/3
06
07    merge_sort3(V, i, x)
08    merge_sort3(V, x+1, y)
09    merge_sort3(V, y+1, f)
10    intercala(V, i, x, y)
11    intercala(V, i, y, f)
12
13    return
```

Agora provando a complexidade de `merge_sort3`:<br>
Temos pelo enunciado que x - i + 1 $\approx$ y - x $\approx$ f - y $\approx$ $\frac{n}{3}$, sendo $n$ o tamanho do vetor. Temos também que, conforme visto em aua, a complexidade temporal de `intercala` é $\Theta(n)$.

Com isso, temos que a complexidade temporal das duas chamadas de `intercala` será, com $c$ e $c_0$ constantes:
$$
c \cdot 2 \cdot \frac{n}{3} + cn = c_0n \in \Theta(n)
$$
Assim, a complexidade temporal de `merge_sort3` será representada pela expressão, com $c_1 \in \N$:
$$
\begin{cases}
T(n) = 3T(\frac{n}{2}) + nc_1, n \ge 3 \\
T(n) = c_1, n \lt 2
\end{cases}
$$
Portanto, utilizando o Teorema Mestre dado em aula com $a = 3$ e $b = 3$ teremos que:
$$
n^{\log_b{a}} = n^{\log_3{3}} = n \in \Theta(n) \\
\Rightarrow T(n) \in \Theta(n \cdot logn)
$$
$\therefore \text{O algoritmo}$ `merge_sort3` $\text{possui complexidade } \Theta(n \cdot logn) \blacksquare$

## Questão 3
```c++
  majoritario(V, i, f):
01   if (i > f):
02       return null
03   if (i == f):
04       return V[i]
05
06    n = f - i + 1
07    maj_e = majoritario(V, i, n/2)
08    maj_d = majoritario(V, n/2 + 1, f)
09    cont_e = 0
10    cont_d = 0
11
12    for (k = i; k <= f; k++)
13        if (maj_e = V[k]):
14            count_e++
15        if (maj_d = V[k]):
16            count_d++
17        
18    if (cont_e >= n/2 + 1):
19        return maj_e
20    if (cont_d >= n/2 + 1):
21        return maj_d
22    else:
23        return null
```

## Questão 4
Seja:
$$
X = 10a_2 + a_1 \\
Y = 10b_2 + b_1
$$
$$
A = 10X + a_0 \\
B = 10Y + b_0
$$

Calculando $X \cdot Y$:<br>
$$
X \cdot Y = (10a_2 + a_1) \cdot (10b_2 + b_1) \\
X \cdot Y = (100a_2b_2) + 10(a_2b_1 + a_1b_2) + a_1b_1
$$

Sabendo que:
$$
AB = XY \cdot 100 + (Xb_0 + Ya_0) \cdot 10 + a_0b_0 
$$

Podemos definir:
$$
M_0 = a_0b_0 \\
M_1 = XY \\
M_2 = (X + a_0)(Y + b_0)
$$

E com isso podemos calcular o termo $(Xb_0 + Ya_0)$, pois:
$$
(Xb_0 + Ya_0) = M_2 - M_1 - M_0
$$

Assim, teremos todos os termos necessários para calcular $AB$ com apenas 7 multiplicações:<br>

$X\text{: 1 multiplicação}$<br>
$Y\text{: 1 multiplicação}$<br>
$Xb_0\text{: 1 multiplicação }$<br>
$Ya_0\text{: 1 multiplicação }$<br>
$a_0b_0\text{: 1 multiplicação } (M_0)$<br>
$XY\text{: 1 multiplicação} (M_1)$<br>
$(X + a_0)(Y + b_0)\text{: 1 multiplicação} (M_2)$<br>

$\therefore \text{7 multiplicações } \blacksquare$

## Questao 5
```c++
  inverte(V, i, f):
1     if (i >= f):
2         return
3     inverte(V, i+1, f-1)
4     V[i] <-> V[f]
5     return
```

Provando o algoritmo por indução:<br>
Seja P(n) <=> Dado um vetor V de tamanho n = f - i + 1, `inverte(V, i, f)` inverte V

**Caso Base:**
- Para n = 0:<br>
  0 = f - i + 1<br>
  i = f + 1<br>
  => i $\ge$ f => retorna na linha 2<br>
  Num vetor vazio a sua inversão é ele mesmo => P(0)<br>

- Para n = 1:<br>
  1 = f - i + 1<br>
  i = f<br>
  => i $\ge$ f => retorna na linha 2<br>
  Num vetor com um único elemento a sua inversão é ele mesmo => P(1)<br>

**Hipótese Indutiva:**<br>
$\text{P(a) vale } \forall{a} \in \N | 0 \le a \le k, k \in \N_{\ge1}$

**Passo Indutivo:**<br>
Pela HI $k \ge 1 \Rightarrow k+1 \ge 2$, então:<br>
k + 1 = f - i + 1<br>
f = i + k $\ge$ i + 1<br>
=> Pula pela linha 2, sem retornar<br>

Seja n' o tamanho do vetor que será passado para `inverte(V, i+1, f-1)` teremos que:<br>
n' = (f-1) - (i+1) + 1<br>
n' = f - i - 1<br>
Como k = f - i temos:<br>
n' = k - 1<br>

Como k+1 $\ge$ 2 => k-1 $\ge$ 0. Não só isso, mas k-1 $\le$ k. => Pela HI, P(k-1) vale.

Com isso, temos que o vetor estará invertido corretamente de i+1 a f-1 e, uma vez que o centro deste subvetor é o mesmo que o do vetor de i a f, teremos que para terminar a inversão basta trocar V[i] e V[f], o que é feito na linha 4 do pseudocódigo.

=> P(k+1)

$\therefore \text{Pelo PIC,} \Rightarrow P(n) \forall{n} \in \N$

Além disso, como a cada chamada o tamanho do subvetor diminiu, sabemos que o algoritmo acaba. Assim, `inverte(V, i, f)` termina e inverte o vetor corretamente. $\blacksquare$