#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

// Multiplica polinonios utilizando metodo trivial
uint32_t *multiplica_simples(uint32_t n, const uint32_t *coef1, const uint32_t *coef2) {
    uint32_t *resultado = (uint32_t *)calloc(2 * n - 1, sizeof(uint32_t));

    for (uint32_t i = 0; i < n; i++) {
        for (uint32_t j = 0; j < n; j++) {
            resultado[i + j] += coef1[i] * coef2[j];
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
    uint32_t n_alta = n - m;
    uint32_t n_baixa = m;

    uint32_t *P_alta = multiplica_polinomio(n_alta - 1, coef1, coef2);

    uint32_t *P_baixa = multiplica_polinomio(n_baixa, coef1 + m, coef2 + m);

    uint32_t max_n_S = n_alta;

    uint32_t *S0 = (uint32_t *)calloc(max_n_S, sizeof(uint32_t));
    uint32_t *S1 = (uint32_t *)calloc(max_n_S, sizeof(uint32_t));

    for (uint32_t i = 0; i < max_n_S; i++) {
        S0[i] = coef1[i];
        S1[i] = coef2[i];

        uint32_t index_baixa = i - (n_alta - n_baixa);

        if (index_baixa >= 0) {
            S0[i] += coef1[n_alta + index_baixa];
            S1[i] += coef2[n_alta + index_baixa];
        }
    }

    uint32_t *P_misto = multiplica_polinomio(max_n_S - 1, S0, S1);

    uint32_t *resultado = (uint32_t *)calloc(2 * n - 1, sizeof(uint32_t));

    for (uint32_t i = 0; i < 2 * n_alta - 1; i++) {
        resultado[i] += P_alta[i];
    }

    for (uint32_t i = 0; i < 2 * n_baixa - 1; i++) {
        resultado[i + 2 * n_alta] += P_baixa[i];
    }

    for (uint32_t i = 0; i < 2 * max_n_S - 1; i++) {
        uint32_t val_alta = (i < (2 * n_alta - 1)) ? P_alta[i] : 0;
        uint32_t val_baixa = (i < (2 * n_baixa - 1)) ? P_baixa[i] : 0;
    
        resultado[i + n_baixa] += (P_misto[i] - val_alta - val_baixa);
    }

    free(P_alta);
    free(P_misto);
    free(P_baixa);
    free(S0);
    free(S1);

    return resultado;
}

// Verifica erros
int32_t avalia_polinomio(int32_t x, uint32_t grau, const uint32_t *coef)
{
    uint32_t resultado = (uint32_t)coef[0];

    for (uint32_t i = 1; i <= grau; i++) {
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