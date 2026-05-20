# Lista 03 - Victor Yaegashi Setti - RA 206362

## Questão 1
- Assumindo que o vetor `V` é sempre 1-indexado e possui tamanho `n`
- Assumindo que a função `Swap(V, i, j)` realiza a troca dos valores de `V[i]` com `V[j]`
```c++
  MinHeapify(V, n, i):
01  esquerdo = 2 * i
02  direito = 2 * i + 1
03  menor = i
04    
05  if (esquerdo <= n && V[esquerdo] < V[menor]):
06      menor = esquerdo
07  
08  if (direito <= n && V[direito] < V[menor]):
09      menor = direito
10  
11  if (menor != i):
12      Swap(V, i, menor)
13      MinHeapify(V, n, menor)
```

```c++
  BuildMinHeap(V, n):
01  for (i = floor(n / 2); i >= 1; i--):
02      MinHeapify(V, i, n)
```

```c++
  InvertHeapsort(V, n):
01  BuildMinHeap(V, n)
02  
03  for (i = n; i > 1; i--):
04      Swap(V, 1, i)
05      n -= 1
06      MinHeapify(V, 1, n)
```

## Questão 3
#### Demonstrando por Contradição:

Suponha, por absurdo, que exista um algoritmo de ordenação por comparação com complexidade temporal $O(n)$ para pelo menos metade das permutações de $n$ entradas.

$\Rightarrow$ Pelo menos $\frac{n!}{2}$ entradas levam à folhas com profundidade menor ou igual a $c.n$, sendo $c \in \R_{>0}$ e constante.

Sabe-se que em uma árvore binária, como a de decisão, com altura $k$ pode ter, no máximo, $2^k$ folhas.

$\Rightarrow$ O número máximo de folhas que podem existir até a profundidade $c.n$ é $2^{c.n}$.

Como cada uma das $\frac{n!}{2}$ entradas deve levar à uma folha diferente temos que:
$$
\frac{n!}{2} \le 2^{c.n}
$$

Aplicando $log_2$ dos dois lados da inequação:
$$
log_2(\frac{n!}{2}) \le log_2(2^{c.n})
$$

Aplicando algumas propriedades do $log_2$:
$$
log_2(n!) - 1 \le c.n
$$

Aplicando a aproximação de Stirling: $log_2(n!) \ge \frac{n}{2}log_2(\frac{n}{2})$:
$$
\frac{n}{2}log_2(\frac{n}{2}) - 1 \le c.n
$$
$$
\frac{n}{2}(log_2(n) - 1) - 1 \le c.n
$$

Dividindo ambos os lados da inequação por $n$:
$$
\frac{1}{2}log_2(n) - \frac{1}{2} - \frac{1}{n} \le c
$$

Como $log_2(n)$ cresce com relação a $n$, teríamos que a constante $c$ dependeria de $n$, o que gera uma contradição com a hipótese inicial.

## Questão 5
### a) Algoritmo de complexidade $O(nlog(k))$
- Sabendo que o heap possuirá uma tupla `(nota, index)` ao invés de simplesmente o campo `nota`
- Utilizando funções semelhantes às já ensinadas em aula
- Assumindo que o vetor `V` é 1-indexado
```c++
  TopCandidatos(V, n, k):
01  heap = CreateEmptyMinHeap()
02  
03  for (i = 1; i <= k; i++):
04      Insert(heap, (V[i], i))
05  
06  for (i = k+1; i <= n; i++):
07      (nota_raiz, index_raiz) = GetMin(heap)
08  
09      if (V[i] > nota_raiz):
10          RemoveMin(heap)
11          Insert(heap, (V[i], i))
12
13  resultados = CreateVector(k)
14  for (i = 1; i <= k; i++):
15      (nota, index) = GetMin(heap)
16      RemoveMin(heap)
17      resultados[i] = index
18
19  return resultados
```

### b) Algoritmo de complexidade $O(n + l)$
- Assumindo que o vetor `V` é 0-indexado
```c++
  TopCandidatosCountingSort(V, n, k, l):
01  contador = CreateVector(l + 1)
02  
03  for (i = 0; i < n; i++):
04      nota = V[i]
05      contador[nota] += 1
06  
07  posicao_atual = 0
08  for (i = l; i >= 0; i--):
09      frequencia = contador[i]
10      contador[i] = posicao_atual
11      posicao_atual += frequencia
12  
13  temp = CreateVector(n)
14  for (i = 0; i < n; i++):
15      nota = V[i]
16      posicao_destino = contador[nota]
17      temp[posicao_destino] = i
18      contador[nota] += 1
19  
20  resultados = CreateVector(k)
21  for (i = 0; i < k; i++):
22      resultados[i] = temp[i]
23  
24  return resultados
```

## Questão 7
```c++
  EncontraTamanhoVetor(A):
01  if (A[0] == false):
02      return 0
03
04  i = 1
05  while (true):
06      if (A[i] == false):
07          break
08      i *= 2
09  
10  esquerda = i / 2
11  direita = i
12  n = 0
13  
14  while (esquerda <= direita):
15      meio = floor((esquerda + direita) / 2)
16      if (A[meio] == false):
17          direita = meio - 1
18      else:
19          n = meio + 1
20          esquerda = meio + 1
21  return n
```

```c++
  BuscaBinariaSemTamanho(A, x):
01  n = EncontrarTamanhoVetor(A)
02  
03  if (n == 0):
04      return -1
05
06  esquerda = 0
07  direita = n - 1
08  
09  while (esquerda <= direita):
10      meio = floor((esquerda + direita) / 2)
11      if (A[meio] == x):
12          return meio
13      else if (A[meio] < x):
14          esquerda = meio + 1
15      else:
16          direita = meio - 1
17  return -1
```

Como o algoritmo `EncontrarTamanhoVetor(A)` possui complexidade $O(log(n))$, temos que o algoritmo `BuscaBinariaSemTamanho(A, x)` possui também complexidade $O(log(n))$.