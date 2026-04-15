#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

// Multiplica polinomios utilizando metodo trivial
uint32_t *multiplica_simples(uint32_t grau, const uint32_t *coef1, const uint32_t *coef2) {
    uint32_t *resultado = (uint32_t *)calloc(2 * grau + 1, sizeof(uint32_t));
    for (uint32_t i = 0; i <= grau; i++) {
        for (uint32_t j = 0; j <= grau; j++) {
            resultado[i + j] += coef1[i] * coef2[j];
        }
    }
    return resultado;
}

uint32_t *soma_polinomio (uint32_t grau1, const uint32_t *coef1, uint32_t grau2, const uint32_t *coef2) {
    uint32_t maior_grau = (grau1 > grau2) ? grau1 : grau2;
    uint32_t menor_grau = (grau1 > grau2) ? grau2 : grau1;
    const uint32_t *maior_p = (grau1 > grau2) ? coef1 : coef2;
    const uint32_t *menor_p = (grau1 > grau2) ? coef2 : coef1;

    uint32_t *resultado = calloc(maior_grau + 1, sizeof(uint32_t));
    uint32_t diff = maior_grau - menor_grau;

    for (uint32_t i = 0; i < diff; i++) {
        resultado[i] = maior_p[i];
    }
    for (uint32_t i = diff; i <= maior_grau; i++) {
        resultado[i] = maior_p[i] + menor_p[i - diff];
    }
    return resultado;
}

uint32_t *subtrai_polinomio (uint32_t grau1, const uint32_t *coef1, uint32_t grau2, const uint32_t *coef2) {
    uint32_t *resultado = calloc(grau1 + 1, sizeof(uint32_t));
    uint32_t diff = grau1 - grau2;

    for (uint32_t i = 0; i < diff; i++) {
        resultado[i] = coef1[i];
    }
    for (uint32_t i = diff; i <= grau1; i++) {
        resultado[i] = coef1[i] - coef2[i - diff];
    }
    return resultado;
}

uint32_t *multiplica_polinomio(uint32_t grau, const uint32_t *coef1, const uint32_t *coef2)
{
    if (grau < 16) {
        return multiplica_simples(grau, coef1, coef2);
    }

    uint32_t grau_alta = grau / 2;
    uint32_t grau_baixa = grau - (grau_alta + 1);

    const uint32_t *u_alta = coef1;
    const uint32_t *u_baixa = coef1 + (grau_alta + 1);
    const uint32_t *v_alta = coef2;
    const uint32_t *v_baixa = coef2 + (grau_alta + 1);

    uint32_t *y0 = soma_polinomio(grau_alta, u_alta, grau_baixa, u_baixa);
    uint32_t *y1 = soma_polinomio(grau_alta, v_alta, grau_baixa, v_baixa);

    uint32_t *y  = multiplica_polinomio(grau_alta, y0, y1);
    uint32_t *x0 = multiplica_polinomio(grau_alta, u_alta, v_alta);
    uint32_t *x2 = multiplica_polinomio(grau_baixa, u_baixa, v_baixa);

    uint32_t *temp = subtrai_polinomio(2 * grau_alta, y, 2 * grau_alta, x0);
    uint32_t *x1 = subtrai_polinomio(2 * grau_alta, temp, 2 * grau_baixa, x2);

    uint32_t *resultado = calloc(2 * grau + 1, sizeof(uint32_t));

    for (uint32_t i = 0; i <= 2 * grau_alta; i++)
        resultado[i] += x0[i];

    for (uint32_t i = 0; i <= 2 * grau_alta; i++)
        resultado[i + (grau - grau_alta)] += x1[i];

    for (uint32_t i = 0; i <= 2 * grau_baixa; i++)
        resultado[i + 2 * (grau_alta + 1)] += x2[i];

    free(y0);
    free(y1);
    free(y);
    free(x0);
    free(x1);
    free(x2);
    free(temp);

    return resultado;
}

// Verifica erros
int32_t avalia_polinomio(int32_t x, uint32_t grau, const uint32_t *coef)
{
    uint32_t resultado = 0;

    for (uint32_t i = 0; i <= grau; i++) {
        resultado = resultado * x + coef[i];
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