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
#include <stdbool.h> // Para usar o tipo 'bool' e os valores 'true'/'false'

/**
 * @brief Calcula o Maximo Divisor Comum (MDC) entre dois numeros usando o Algoritmo de Euclides.
 *
 * @param a O primeiro numero.
 * @param b O segundo numero.
 * @param verbose Se true, imprime cada passo do algoritmo.
 * @return O MDC de a e b.
 */
int mdc_verboso(int a, int b, bool verbose) {
    if (verbose) {
        printf("      2.1.1. Iniciando calculo de MDC(%d, %d) via Algoritmo de Euclides:\n", a, b);
    }
    int original_a = a;
    int original_b = b;

    while (b != 0) {
        int r = a % b;
        if (verbose) {
            printf("         - %d = %d * %d + %d\n", a, b, (a / b), r);
        }
        a = b;
        b = r;
    }

    if (verbose) {
        printf("      2.1.2. O MDC(%d, %d) e %d.\n", original_a, original_b, a);
    }
    return a;
}

// Funcao para limpar o buffer de entrada em caso de erro do scanf
void limpar_buffer_entrada() {
    int c;
    while ((c = getchar()) != '\n' && c != EOF);
}

int main() {
    // =================================================================
    // JUSTIFICATIVA DO METODO (Comentario para o trabalho)
    // =================================================================
    // O problema "Chaves Periodicas" busca o primeiro ano em que todos os
    // eventos (ativacao de chaves) ocorrem simultaneamente. Isso corresponde
    // ao menor inteiro positivo que e um multiplo comum de todos os ciclos C_i.
    // Matematicamente, este valor e o Minimo Multiplo Comum (MMC).
    // A implementacao utiliza o Algoritmo de Euclides para calcular o Maximo
    // Divisor Comum (MDC), que e entao usado na formula MMC(a, b) = (a / MDC(a, b)) * b
    // de forma iterativa para todos os ciclos.
    // =================================================================

    int n;
    int ciclos[10]; // Em C, usamos um array de tamanho fixo. O limite e 10.
    const int LIMITE_ANO = 50;

    printf("========================================\n");
    printf("  Solucao para o Problema: Chaves Periodicas (Versao C)\n");
    printf("========================================\n\n");

    // --- PASSO 0: ENTRADA E VALIDACAO ---
    printf("--- PASSO 0: ENTRADA DE DADOS ---\n");
    
    // Validacao do numero de chaves N
    while (true) {
        printf("Digite o numero de chaves N (entre 1 e 10): ");
        if (scanf("%d", &n) != 1 || n < 1 || n > 10) {
            printf("Entrada invalida. Por favor, tente novamente.\n");
            limpar_buffer_entrada();
        } else {
            limpar_buffer_entrada(); // Limpa o '\n' que sobrou
            break;
        }
    }

    // Validacao dos ciclos C_i
    printf("Digite os %d ciclos C_i (cada um entre 2 e 20), separados por espaco: ", n);
    for (int i = 0; i < n; ++i) {
        while (true) {
            if (scanf("%d", &ciclos[i]) != 1 || ciclos[i] < 2 || ciclos[i] > 20) {
                printf("\nCiclo invalido. Digite um valor entre 2 e 20 para o ciclo %d: ", i + 1);
                limpar_buffer_entrada();
            } else {
                break;
            }
        }
    }
    limpar_buffer_entrada();
    printf("Entrada recebida com sucesso.\n\n");


    // --- PASSO 1: INICIO DA RESOLUCAO ---
    printf("--- PASSO 1: JUSTIFICATIVA DA ESTRATEGIA ---\n");
    printf("O problema requer o primeiro ano em que todas as chaves se ativam.\n");
    printf("Isso e equivalente a encontrar o Minimo Multiplo Comum (MMC) dos ciclos.\n");
    printf("Vamos calcular o MMC de forma iterativa.\n\n");

    // --- PASSO 2: CALCULO ITERATIVO DO MMC ---
    printf("--- PASSO 2: CALCULO ITERATIVO DO MMC ---\n");

    int mmc_parcial = ciclos[0];
    printf("   2.0. Valor inicial do MMC parcial = %d (ciclo da primeira chave).\n", mmc_parcial);

    for (int i = 1; i < n; ++i) {
        int proximo_ciclo = ciclos[i];
        printf("\n   2.%d. Calculando o MMC entre o MMC parcial (%d) e o proximo ciclo (%d):\n", i, mmc_parcial, proximo_ciclo);
        
        printf("      2.%d.1. Primeiro, precisamos do MDC( %d, %d ).\n", i, mmc_parcial, proximo_ciclo);
        int mdc = mdc_verboso(mmc_parcial, proximo_ciclo, true);

        printf("      2.%d.2. Agora, usamos a formula: MMC(a, b) = (a / MDC(a, b)) * b\n", i);
        
        // --- SUGESTAO IMPLEMENTADA ---
        // A ordem (a / mdc) * b e matematicamente equivalente a (a * b) / mdc,
        // mas e computacionalmente mais segura, pois evita o risco de um
        // estouro de memoria (overflow) na multiplicacao (a * b) caso os
        // numeros sejam muito grandes.
        int novo_mmc = (mmc_parcial / mdc) * proximo_ciclo;
        printf("         - MMC(%d, %d) = (%d / %d) * %d = %d\n", mmc_parcial, proximo_ciclo, mmc_parcial, mdc, proximo_ciclo, novo_mmc);
        
        mmc_parcial = novo_mmc;
        printf("      2.%d.3. O novo MMC parcial e: %d\n", i, mmc_parcial);
    }

    int resultado_final = mmc_parcial;
    printf("\nO MMC final de todos os ciclos e: %d.\n\n", resultado_final);


    // --- PASSO 3: RESULTADO E VERIFICACAO ---
    printf("--- PASSO 3: ANALISE DO RESULTADO FINAL ---\n");
    printf("O primeiro ano em que todas as chaves se ativam simultaneamente e %d.\n", resultado_final);
    printf("O limite de ano para a solucao e %d.\n", LIMITE_ANO);

    if (resultado_final > LIMITE_ANO) {
        printf("\n[RESULTADO]\n");
        printf("IMPOSSIVEL. O primeiro ano sincronizado (%d) esta fora do limite de %d anos.\n", resultado_final, LIMITE_ANO);
    } else {
        printf("\n[RESULTADO]\n");
        printf("O primeiro ano em que todas as chaves podem ser usadas simultaneamente e: %d\n\n", resultado_final);

        // --- PASSO 4: CONFIRMACAO DO RESULTADO ---
        printf("--- PASSO 4: CONFIRMACAO DE CONSISTENCIA ---\n");
        printf("Verificando se %d e multiplo de todos os ciclos inseridos:\n", resultado_final);
        bool todos_ok = true;
        for (int i = 0; i < n; i++) {
            if (resultado_final % ciclos[i] == 0) {
                printf("   - %d %% %d = 0 (OK)\n", resultado_final, ciclos[i]);
            } else {
                printf("   - %d %% %d != 0 (ERRO NA LOGICA)\n", resultado_final, ciclos[i]);
                todos_ok = false;
            }
        }
        if (todos_ok) {
            printf("Confirmacao bem-sucedida. O resultado esta correto.\n");
        }
    }
    
    printf("\n========================================\n");

    return 0;
}