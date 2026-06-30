# Lista 03 - Victor Yaegashi Setti - RA 206362

## Questão 1
### a) Relação:

Seja $i$ o número de meses e $T(i)$ o custo ótimo para este $i$:

$$
T(i) = \begin{cases} 
0, & \text{se } i \le 0 \\ 
\min (T(i-1) + r_n \cdot m_n, ~ T(i-6) + 6 \cdot c), & \text{se } i > 0 
\end{cases}
$$

### b) Algoritmo:

```c++
  CustoMinimo(c, n, M, R):
01  DP = empty_vector(n+1)
02
03  for (i = 1; i <= n; i++):
04      if (i < 6):
05          temp = 6 * c
06      else:
07          temp = DP[i - 6] + (6 * c)
08      DP[i] = min(temp, DP[i - 1] + (R[i] * M[i]))
09  return DP[n]
```

## Questão 2

- Supondo que a função `sort(A, m)` ordena o vetor `A` de tamanho `m` em $O(m.logm)$ operações como, por exemplo, o Merge Sort:

```c++
  Elemento6(V, n):
01  S = V[0:10]
02  sort(S, 11)
03  return S[5]
```

## Questão 3