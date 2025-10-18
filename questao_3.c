/**
 * ===================================================================================
 * Universidade: Universidade de Brasília (UnB)
 * Disciplina:   FGA0108 - MATEMÁTICA DISCRETA 2 - T01 (2025.2)
 * Professora:   CRISTIANE LOESCH DE SOUZA COSTA
 *
 * Alunos:
 * - Mário Vinícius Beleza Carneiro (Matrícula: 202046265)
 * - Mateus de Siqueira Silva      (Matrícula: 200024787)
 *
 * ===================================================================================
 */

#include <stdio.h>

/**
 * @file razao_eficiencia_v4.c
 * @brief Calcula a "Razao de Eficiencia" de um numero N (sigma(N)/tau(N)).
 *
 * Versao final com comentarios de justificativa de metodos, conforme
 * as diretrizes da atividade.
 */

void calcularRazaoEficiencia(int n);

int main() {
    int n;

    printf("Digite um numero inteiro N (entre 1 e 105): ");
    scanf("%d", &n);

    if (n < 1 || n > 105) {
        printf("\nErro: O numero deve estar no intervalo de 1 a 105.\n");
        return 1;
    }

    calcularRazaoEficiencia(n);

    return 0;
}

void calcularRazaoEficiencia(int n) {
    printf("\n--- Analise da Razao de Eficiencia para N = %d ---\n", n);

    if (n == 1) {
        printf("\nPara N=1, o unico divisor e 1.\n");
        printf("   - tau(1) = 1\n");
        printf("   - sigma(1) = 1\n");
        printf("   - Razao de Eficiencia = 1.00 / 1.00 = 1.00\n");
        return;
    }

    printf("\n1. Fatoracao Prima de %d (Metodo: Trial Division)\n", n);
    printf("----------------------------------------------------\n");
    /*
     * JUSTIFICATIVA DE METODO (Instrucao 3):
     * A estrategia de decompor o numero em fatores primos foi escolhida
     * por ser o metodo fundamental e mais eficiente para calcular as funcoes
     * tau e sigma, conforme a Teoria dos Numeros. Uma abordagem alternativa,
     * como testar todos os numeros de 1 a N para achar os divisores,
     * seria muito mais lenta para numeros maiores.
     */

    int temp_n = n;
    long long tau_n = 1;
    long long sigma_n = 1;

    /*
     * JUSTIFICATIVA DE METODO (Instrucao 3):
     * O algoritmo "Trial Division" (divisao por tentativa) foi escolhido para
     * a fatoracao. Ele testa a divisao de N por candidatos a primos (2, 3, 5...).
     * Essa escolha e justificada pelo limite baixo da entrada (N <= 105),
     * para o qual este metodo e simples de implementar e suficientemente rapido.
     * Para numeros muito grandes, algoritmos mais avancados como Pollard's Rho
     * ou a Peneira Quadratica seriam necessarios, mas aqui seriam um exagero.
     */
    for (int d = 2; d * d <= temp_n; d++) {
        if (temp_n % d == 0) {
            int expoente = 0;
            while (temp_n % d == 0) {
                expoente++;
                temp_n /= d;
            }
            printf("   - Fator primo encontrado: %d. Expoente: %d\n", d, expoente);

            // Usa as formulas baseadas nos expoentes da fatoracao
            tau_n *= (expoente + 1);

            /*
             * JUSTIFICATIVA DE METODO (Instrucao 3):
             * A potencia necessaria para o calculo de sigma(N) e feita
             * manualmente com um loop 'for' em vez de usar a funcao pow()
             * da biblioteca <math.h>. A escolha foi feita para evitar a
             * dependencia de uma biblioteca externa, simplificar a compilacao
             * (nao precisa de -lm) e operar exclusivamente com inteiros,
             * o que e mais seguro e apropriado para este problema.
             */
            long long potencia = 1;
            for (int i = 0; i < expoente + 1; i++) {
                potencia *= d;
            }
            sigma_n *= (potencia - 1) / (d - 1);
        }
    }

    if (temp_n > 1) {
        int expoente = 1;
        printf("   - Fator primo encontrado: %d. Expoente: %d\n", temp_n, expoente);
        tau_n *= (expoente + 1);
        sigma_n *= (temp_n + 1);
    }

    printf("\n2. Calculo de tau(N) e sigma(N) a partir dos fatores\n");
    printf("----------------------------------------------------\n");
    printf("   - tau(%d) [Quantidade de Divisores] = %lld\n", n, tau_n);
    printf("   - sigma(%d) [Soma dos Divisores]      = %lld\n", n, sigma_n);

    printf("\n3. Calculo Final da Razao de Eficiencia\n");
    printf("----------------------------------------------------\n");

    double razao = (double)sigma_n / tau_n;

    printf("   - Formula: Razao = sigma(%d) / tau(%d)\n", n, n);
    printf("   - Calculo: Razao = %lld / %lld\n", sigma_n, tau_n);
    printf("   - RESULTADO FINAL (com 2 casas decimais): %.2f\n", razao);
}