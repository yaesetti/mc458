#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Multiplica polinonios utilizando metodo trivial
uint32_t *multiplica_simples(uint32_t n, const uint32_t *coef1, const uint32_t *coef2) {
    uint32_t *resultado = (uint32_t *)calloc(2 * n - 1, sizeof(uint32_t));

    for (int i = 0; i < n; i++) {
        for (int j = 0; j < n; j++) {
            resultado[i + j] = coef1[i] * coef2[j];
        }
    }
    return resultado;
}

// Multiplica polinomions utilizando Karatsuba
uint32_t *multiplica_polinomio(uint32_t grau, const uint32_t *coef1, const uint32_t *coef2){
    uint32_t n = grau + 1;

    if (grau <= 16) {
        return multiplica_simples(n, coef1, coef2);
    }

    uint32_t m = n / 2;
    uint32_t grau_dir = m - 1;
    uint32_t grau_esq = (n - m) - 1;

    uint32_t *P0 = multiplica_polinomio(grau_dir, coef1, coef2);

    uint32_t *P2 = multiplica_polinomio(grau_esq, coef1 + m, coef2 + m);

    uint32_t max_n_S = (n - m);

    uint32_t *S0 = (uint32_t *)calloc(max_n_S, sizeof(uint32_t));
    uint32_t *S1 = (uint32_t *)calloc(max_n_S, sizeof(uint32_t));

    for (int i = 0; i < max_n_S; i++) {
        if (i < m) {
            S0[i] = coef1[i] + coef1[i + m];
            S1[i] = coef2[i] + coef2[i + m];
        }
        else {
            S0[i] = coef1[i + m];
            S1[i] = coef2[i + m];
        }
    }

    uint32_t *P1 = multiplica_polinomio(max_n_S - 1, S0, S1);

    uint32_t *resultado = (uint32_t *)calloc(2 * n - 1, sizeof(uint32_t));

    for (int i = 0; i < 2 * (grau_dir + 1) - 1; i++) {
        resultado[i] += P0[i];
    }

    for (int i = 0; i < 2 * (grau_esq + 1) - 1; i++) {
        resultado[i + 2 * m] += P2[i];
    }

    uint32_t tam_P1 = 2 * max_n_S - 1;
    for (int i = 0; i < tam_P1; i++) {
        uint32_t val_p0 = (i < (2 * (grau_dir + 1) - 1)) ? P0[i] : 0;
        uint32_t val_p2 = (i < (2 * (grau_esq + 1) - 1)) ? P2[i] : 0;
    
        resultado[i + m] += (P1[i] - val_p0 - val_p2);
    }

    free(P0);
    free(P1);
    free(P2);
    free(S0);
    free(S1);

    return resultado;
}

// Verifica erros
int32_t avalia_polinomio(int32_t x, uint32_t grau, const uint32_t *coef)
{
    uint32_t resultado = (uint32_t)coef[grau];

    for (int i = (int)grau - 1; i >= 0; i--) {
        resultado = (resultado * x) + (int32_t)coef[i];
    }

    return resultado;
}

// Decodifica
uint32_t *divide_por_binomio(uint32_t grau, uint32_t *coef, int32_t raiz)
{
    // Divide o polinomio de grau 'grau' por um binomio do tipo (x - raiz), onde raiz é uma raiz do polinomio, ou seja, P(raiz) == 0
    // Dividir por um binomio vai diminuir um grau
    uint32_t *resultado = (uint32_t *)calloc(grau, sizeof(uint32_t));
    resultado[0] = coef[0]; // O coeficiente de maior grau do resultado é igual ao coeficiente de maior grau do polinômio original
    for(uint32_t i = 1; i < grau; i++)
    {
        resultado[i] = coef[i] + raiz * resultado[i - 1];
    }
    return resultado;
}

uint32_t *decodifica_mensagem(uint32_t grau, uint32_t *coef, uint32_t n_raizes, int32_t *raizes)
{
    // Decodifica a mensagem original M(X) a partir do polinômio codificado P(X) e das raízes fornecidas
    uint32_t *coeficientes = (uint32_t *)calloc(grau + 1, sizeof(uint32_t));
    // Copia P
    for (uint32_t i = 0; i <= grau; i++)
    {
        coeficientes[i] = coef[i];
    }
    for (uint32_t i = 0; i < n_raizes; i++)
    {
        uint32_t *temp = coeficientes;
        coeficientes = divide_por_binomio(grau - i, temp, raizes[i]);
        free(temp); // Libera a memória da iteração anterior para evitar memory leak
    }
    return coeficientes;
}

_Bool verifica_erros(uint32_t grau, const uint32_t *coef, uint32_t n_raizes, const int32_t *raizes)
{
    for (uint32_t i = 0; i < n_raizes; i++)
    {
        if (avalia_polinomio(raizes[i], grau, coef) != 0)
        {
            return 1; // erro detectado
        }
    }
    return 0;
}

// Adiciona ruido
void adiciona_ruido(uint32_t grau, uint32_t *coef, uint32_t m)
{
    // Adiciona ruído em m coeficientes aleatórios do polinômio
    for (uint32_t i = 0; i < m; i++)
    {
        uint32_t indice = rand() % (grau + 1);
        uint32_t ruido = (rand() % 10) + 1; // Ruído entre 1 e 10
        coef[indice] += ruido;
    }
}