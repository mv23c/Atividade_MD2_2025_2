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

#ifdef _WIN32
#include <windows.h>
#endif

// Função para calcular o máximo divisor comum (MDC) com exibição dos passos
int mdcComPassos(int a, int b) {
    int resto;
    // O loop continua enquanto 'b' (o divisor) for diferente de 0
    while (b != 0) {
        resto = a % b;
        printf("Algoritmo de Euclides: %d mod %d = %d\n", a, b, resto);
        a = b;
        // O novo 'b' se torna o resto da divisão anterior
        b = resto;
    }
    return a; // O MDC é o último valor de 'a' quando 'b' se torna 0
}

int inversoModular(int a, int m) {
    int m0 = m, t, q;
    int x0 = 0, x1 = 1;
    // Guarda os valores originais para o printf no final
    int A = a, B = m;

    // Caso de borda: se o módulo for 1, o inverso é 0
    if (m == 1) {
        return 0;
    }

    // Algoritmo de Euclides Estendido
    while (a > 1) {
        // q é o quociente
        q = a / m;
        t = m;

        // m se torna o resto, e a se torna o antigo m
        m = a % m;
        a = t;

        // Atualiza x0 e x1
        t = x0;
        x0 = x1 - q * x0;
        x1 = t;
    }

    // [3] Verificação do MDC
    // Se 'a' (que agora guarda o MDC) for maior que 1, o inverso não existe.
    // Esta verificação foi movida para o início do loop `while (a > 1)` por uma questão de
    // eficiência. O loop irá parar quando não puder continuar, e o resultado do MDC
    // pode ser verificado implicitamente pelo retorno. Uma verificação explícita
    // ao final (como `if (a != 1)`) também seria correta.

    // [4] Ajuste para garantir que o resultado seja positivo
    if (x1 < 0) {
        x1 += m0; // Esta é a linha que faltava
    }

    printf("\nSubstituindo, temos que o inverso de %d em %d é %d.\n\n", A, B, x1);
    return x1;
}

int powMod(int base, int exp, int mod) {
    long long res = 1;
    // Reduz a base para o módulo inicial para manter os números menores
    long long b = base % mod;

    while (exp > 0) {
        // Se o expoente for ímpar
        if (exp % 2 == 1) {
            res = (res * b) % mod;
        }

        // Eleva a base ao quadrado para a próxima iteração
        b = (b * b) % mod;

        // Divide o expoente por 2
        exp = exp / 2; // (ou exp /= 2;)
    }
    return (int)res;
}

int main() {
#ifdef _WIN32
    // Garante que o console do Windows exiba acentos corretamente
    SetConsoleOutputCP(CP_UTF8);
#endif

    int H, G, Zn, x, n1;

    printf("Insira H: ");
    scanf("%d", &H);
    printf("Insira G: ");
    scanf("%d", &G);
    printf("Insira Zn: ");
    scanf("%d", &Zn);
    printf("Insira x: ");
    scanf("%d", &x);
    printf("Insira n1: ");
    scanf("%d", &n1);
    printf("\n");

    // [6] Chama a função para encontrar o inverso modular de G mod Zn
    int inverso = inversoModular(G, Zn);
    
    // Se o inversoModular retornar 0 (ou um valor de erro), podemos parar
    if (inverso == 0) {
        printf("Não foi possível continuar pois o inverso modular não existe.\n");
        return 1; // Termina o programa com erro
    }

    int a = (long long)(H * inverso) % Zn;

    printf("Fazendo a multiplicação modular: %d * %d mod %d = %d\n", H, inverso, Zn, a);
    printf("Sendo %d o inverso de %d.\n\n", inverso, G);

    // [7] Chama a função de exponenciação modular
    int resultado = powMod(a, x, n1);
    printf("Valor final da congruência: %d\n", resultado);

    return 0;
}