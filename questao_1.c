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
 *
 * @file rsa_completo_final.c
 * @brief Implementação completa do sistema de criptografia RSA em três etapas.
 * @details Este código foi escrito para máxima portabilidade, utilizando o padrão C89/ANSI C,
 * garantindo que compile sem flags especiais na maioria dos compiladores GCC.
 * @version 2.4 - Adicionada funcao mdcDetalhado para cumprir requisito de "Passo a Passo Visivel".
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <ctype.h>

// --- Protótipos das Funções ---
long multiplicacao_modular(long a, long b, long mod);
int preCodificarMensagem(const char* mensagem, long blocos[], char* ignorados);
int ehPrimo(long n);
long mdc(long a, long b);
// === PROTÓTIPO ADICIONADO (FUNCAO DIDATICA) ===
long mdcDetalhado(long a, long b, const char* label_a, const char* label_b);


// -----------------------------------------------------------------------------
// SEÇÃO 1: FUNÇÕES DE FATORAÇÃO (POLLARD'S RHO)
// -----------------------------------------------------------------------------

/**
 * @brief Calcula o Máximo Divisor Comum (MDC) entre dois números.
 * @details Utiliza o Algoritmo de Euclides com divisões sucessivas.
 * Esta e a versao "silenciosa" da funcao, usada em loops
 * (como dentro do pollardRho) onde a impressao de cada passo
 * poluiria a saida.
 * @param a O primeiro número.
 * @param b O segundo número.
 * @return O MDC de a e b.
 */
long mdc(long a, long b) {
    long resto;
    while (b != 0) {
        resto = a % b;
        a = b;
        b = resto;
    }
    return a;
}

// ===========================================================================
// === FUNÇÃO 'mdcDetalhado' ADICIONADA AQUI ===
// ===========================================================================
/**
 * @brief (FUNCAO DIDATICA) Calcula o MDC e imprime a chamada e o resultado.
 * @details Esta e uma versao "verbosa" do mdc, usada em pontos-chave
 * do programa (Passo 5 e verificacao de Euler) para cumprir o
 * requisito de "Passo a Passo Visivel", mostrando o calculo que
 * esta sendo feito e seu resultado.
 * @param a O primeiro número.
 * @param b O segundo número.
 * @param label_a Nome da variavel 'a' para impressao (ex: "e").
 * @param label_b Nome da variavel 'b' para impressao (ex: "z").
 * @return O MDC de a e b.
 */
long mdcDetalhado(long a, long b, const char* label_a, const char* label_b) {
    // Imprime o cálculo que será feito
    printf("     - [MDC] Calculando mdc(%s, %s) = mdc(%ld, %ld)\n", label_a, label_b, a, b);
    
    long original_a = a;
    long original_b = b;
    long resto;

    // Algoritmo de Euclides (logica identica ao mdc silencioso)
    while (b != 0) {
        resto = a % b;
        a = b;
        b = resto;
    }
    
    // Imprime o resultado final do cálculo
    printf("     - [MDC] Resultado: mdc(%ld, %ld) = %ld\n", original_a, original_b, a);
    return a; // 'a' e o resultado
}


/**
 * @brief Retorna o valor absoluto de um número.
 * @param x O número de entrada.
 * @return O valor absoluto de x.
 */
long valor_absoluto(long x) {
    return (x < 0) ? -x : x;
}

/**
 * @brief Função de iteração para o método p de Pollard: g(x) = (x² + 1) mod N.
 * @details Conforme especificado no enunciado, esta função gera a sequência pseudo-aleatória
 * necessária para o algoritmo encontrar um fator.
 * @param x O valor atual.
 * @param N O módulo.
 * @return O próximo valor da sequência.
 */
long g(long x, long N) {
    // Usa uma função de multiplicação modular segura para evitar overflow (estouro de capacidade)
    // que ocorreria com uma simples multiplicação `(x * x)`.
    return (multiplicacao_modular(x, x, N) + 1) % N;
}

/**
 * @brief Encontra um fator não trivial de um número composto N usando o método p de Pollard.
 * @details
 * JUSTIFICATIVA DE MÉTODO (Item 3 das instruções):
 * O método p (Rho) de Pollard foi escolhido por sua eficiência em encontrar
 * fatores pequenos de números compostos. É um algoritmo probabilístico
 * geralmente mais rápido que a tentativa de divisão por força bruta, sendo ideal para
 * a fatoração exigida na disciplina de MD2, onde a eficiência é relevante.
 * @param N O número composto a ser fatorado.
 * @return Um fator não trivial de N, ou -1 em caso de falha.
 */
long pollardRho(long N) {
    long x = 2, y = 2, d = 1;
    int iter = 0;

    printf("\n2.1. Tabela de Iteracoes para N = %ld:\n", N);
    printf("------------------------------------------------------\n");
    printf("Iter\t x\t y\t |x - y|\t mdc(|x - y|, N)\n");
    printf("------------------------------------------------------\n");

    if (N % 2 == 0) return 2; 

    while (d == 1) {
        x = g(x, N);        
        y = g(g(y, N), N);  
        
        // === JUSTIFICATIVA DE IMPLEMENTACAO ===
        // A 'mdc' silenciosa e usada aqui, pois esta dentro de um loop.
        // Imprimir os passos do MDC a cada iteracao do Pollard Rho
        // tornaria a saida excessivamente poluida e ilegivel.
        d = mdc(valor_absoluto(x - y), N);
        iter++;
        
        printf("%3d\t %-7ld\t %-7ld\t %-10ld\t %ld\n", iter, x, y, valor_absoluto(x - y), d);
        
        if (d == N) {
            return -1;
        }
    }
    printf("------------------------------------------------------\n");
    return d;
}


// -----------------------------------------------------------------------------
// SEÇÃO 2: FUNÇÕES DE GERAÇÃO DE CHAVES (EUCLIDES ESTENDIDO)
// -----------------------------------------------------------------------------

/**
 * @brief Calcula o inverso modular usando o Algoritmo Estendido de Euclides.
 * @details
 * JUSTIFICATIVA DE MÉTODO (Item 3 das instruções):
 * O Algoritmo Estendido de Euclides é o método padrão e mais eficiente para
 * encontrar o inverso modular 'd' tal que (d * e) ≡ 1 (mod z). Ele é um pilar
 * do RSA para o cálculo da chave privada 'd' a partir da chave pública.
 * Esta funcao TAMBEM cumpre o requisito de mostrar o "Calculo do mdc",
 * pois as colunas r1, r2 e resto sao a execucao do Alg. de Euclides.
 * @param e O número do qual se deseja encontrar o inverso (expoente público).
 * @param z O módulo (função totiente de Euler).
 * @return O inverso modular de 'e' em relação a 'z'.
 */
long algoritmoEuclidesEstendido(long e, long z) {
    long r1 = z, r2 = e, t1 = 0, t2 = 1;
    long quociente, resto, t;

    printf("\n  -> Tabela de iteracoes do Algoritmo Estendido de Euclides para encontrar d:\n");
    printf("  ----------------------------------------------------------------\n");
    printf("  r1\t\t r2\t\t resto\t\t quociente\t t1\t\t t2\t\t t\n");
    printf("  ----------------------------------------------------------------\n");

    while (r2 > 0) {
        quociente = r1 / r2;
        resto = r1 % r2;
        
        t = t1 - quociente * t2;
        
        printf("  %-15ld %-15ld %-15ld %-15ld %-15ld %-15ld %-15ld\n", r1, r2, resto, quociente, t1, t2, t);
        
        r1 = r2; r2 = resto;
        t1 = t2; t2 = t;
    }
    printf("  ----------------------------------------------------------------\n");
    
    if (t1 < 0) t1 = t1 + z;
    
    return t1;
}


// -----------------------------------------------------------------------------
// SEÇÃO 3: FUNÇÕES DE CODIFICAÇÃO E DECODIFICAÇÃO
// -----------------------------------------------------------------------------

/**
 * @brief Calcula (a * b) % mod de forma segura para evitar overflow.
 * @details Usa o método de adição e duplicação para calcular o produto sem
 * exceder a capacidade de um 'long', tornando o código mais portável.
 * @param a O primeiro número.
 * @param b O segundo número.
 * @param mod O módulo.
 * @return O resultado de (a * b) % mod.
 */
long multiplicacao_modular(long a, long b, long mod) {
    long res = 0;
    a %= mod;
    while (b > 0) {
        if (b % 2 == 1) res = (res + a) % mod;
        a = (a * 2) % mod;
        b /= 2;
    }
    return res;
}

/**
 * @brief Converte uma mensagem de texto em blocos numericos e registra caracteres invalidos.
 * @details A função mapeia A-Z para 11-36 e espaço para 00. Caracteres que não se
 * encaixam nessas categorias são registrados em uma string separada e ignorados.
 * @param mensagem A string de texto original.
 * @param blocos Um array para armazenar os blocos numericos resultantes.
 * @param ignorados Um buffer de char para armazenar os caracteres que foram ignorados.
 * @return O número de blocos gerados.
 */
int preCodificarMensagem(const char* mensagem, long blocos[], char* ignorados) {
    int len = strlen(mensagem);
    int count = 0;
    int ignorados_count = 0;
    int i; 

    for (i = 0; i < len; i++) {
        char c_upper = toupper(mensagem[i]);
        if (c_upper == ' ') {
            blocos[count++] = 0;
        } else if (c_upper >= 'A' && c_upper <= 'Z') {
            blocos[count++] = c_upper - 'A' + 11;
        } else {
            ignorados[ignorados_count++] = mensagem[i];
        }
    }
    ignorados[ignorados_count] = '\0'; 
    return count;
}


/**
 * @brief Converte blocos numéricos de volta para texto.
 * @param blocos Array de blocos numéricos.
 * @param num_blocos O número de blocos no array.
 * @param mensagem Buffer para armazenar a string de texto resultante.
 */
void reconverterParaTexto(const long blocos[], int num_blocos, char* mensagem) {
    int i; 
    for (i = 0; i < num_blocos; i++) {
        if (blocos[i] == 0) {
            mensagem[i] = ' ';
        } else if (blocos[i] >= 11 && blocos[i] <= 36) {
            mensagem[i] = (char)(blocos[i] - 11 + 'A');
        } else {
            mensagem[i] = '?'; 
        }
    }
    mensagem[num_blocos] = '\0'; 
}

/**
 * @brief Verifica (de forma simples) se um numero e primo.
 * @details Esta e uma funcao auxiliar simples apenas para a verificacao
 * hipotetica do Teorema de Fermat.
 * @param n O numero a ser testado.
 * @return 1 se for primo, 0 se for composto.
 */
int ehPrimo(long n) {
    long i;
    if (n <= 1) return 0; 
    if (n <= 3) return 1; 
    
    if (n % 2 == 0 || n % 3 == 0) return 0; 
    
    for (i = 5; i * i <= n; i = i + 6) {
        if (n % i == 0 || n % (i + 2) == 0)
            return 0;
    }
    return 1;
}

/**
 * @brief Calcula a exponenciação modular (base^exp mod mod) de forma detalhada.
 * @details
 * JUSTIFICATIVA DE MÉTODO (Item 3 das instruções):
 * Utiliza o método de "Exponenciação por Quadrados" (ou "Binary Exponentiation")
 * por ser altamente eficiente para calcular grandes potências em aritmética modular.
 * Adicionalmente, aplica o Teorema de Euler (ou Fermat) para reduzir o expoente
 * quando possível, otimizando ainda mais o cálculo, conforme solicitado.
 * @param base A base da potência.
 * @param exp O expoente.
 * @param mod O módulo.
 * @param z O valor do totiente de Euler (usado para a otimização).
 * @return O resultado de (base^exp) mod mod.
 */
long exponenciacaoModularDetalhada(long base, long exp, long mod, long z) {
    long res = 1;
    long exp_original = exp;
    long base_original = base;

    printf("\n  -> Calculando: %ld ^ %ld (mod %ld)\n", base, exp, mod);

    if (base_original == 0) {
        printf("     - JUSTIFICATIVA: A base e 0. Para qualquer expoente positivo, o resultado e 0.\n");
        printf("     - Resultado Final: 0 ^ %ld (mod %ld) = 0\n", exp_original, mod);
        return 0;
    }

    printf("     - Verificando condicoes para aplicar teoremas de reducao de expoente...\n");
    
    
    // 1. Verificação do Pequeno Teorema de Fermat (caso hipotético)
    if (ehPrimo(mod)) {
        printf("     - JUSTIFICATIVA: O modulo %ld E PRIMO. O Pequeno Teorema de Fermat e aplicavel.\n", mod);
        
        long p_menos_1 = mod - 1;
        long novo_exp = (p_menos_1 > 0) ? (exp % p_menos_1) : 0;
        
        if (exp_original > 0 && novo_exp == 0 && mdc(base, mod) == 1) { // mdc silencioso ok aqui
            novo_exp = p_menos_1; 
        }
        
        printf("     - Reduzindo expoente (Fermat): %ld (mod %ld) = %ld\n", exp, p_menos_1, novo_exp);
        exp = novo_exp;
    } 
    // 2. Se nao for primo, verifica Euler
    else {
        printf("     - JUSTIFICATIVA [Fermat]: O Pequeno Teorema de Fermat nao foi aplicado, pois o modulo %ld E COMPOSTO.\n", mod);
        
        // ======================================================================
        // === MODIFICACAO (CUMPRIR REQ. "Passo a Passo Visivel" para o MDC) ===
        // ======================================================================
        // Em vez de chamar mdc() silenciosamente dentro do 'if', chamamos
        // 'mdcDetalhado()' aqui para imprimir o calculo e armazenar o resultado.
        
        long mdc_resultado = mdcDetalhado(base, mod, "base", "mod");

        // 2a. Condicao de Euler E satisfeita
        if (mdc_resultado == 1) {
            printf("     - JUSTIFICATIVA [Euler]: O Teorema de Euler e aplicavel, pois mdc(base, mod) = 1.\n");
            
            long novo_exp = exp % z;
            if (exp_original > 0 && novo_exp == 0) novo_exp = z;

            printf("     - Reduzindo expoente (Euler): %ld (mod %ld) = %ld\n", exp, z, novo_exp);
            exp = novo_exp;
        } 
        // 2b. Condicao de Euler NAO E satisfeita
        else {
            // A justificativa de falha do mdc ja foi impressa pela 'mdcDetalhado'
            printf("     - JUSTIFICATIVA [Euler]: O Teorema de Euler nao foi aplicado, pois mdc(base, mod) = %ld (e != 1).\n", mdc_resultado);
            // Nenhuma reducao de expoente e feita
        }
    }
    
    printf("\n     - Tabela de exponenciacao por quadrados (base^%ld mod %ld):\n", exp, mod);
    printf("     --------------------------------------------------\n");
    printf("     exp\t\t base\t\t resultado\n");
    printf("     --------------------------------------------------\n");
    printf("     %-15ld %-15ld %-15ld (Inicial)\n", exp, base, res);

    base %= mod;
    
    if (exp == 0 && exp_original == 0) {
         res = 1;
    }

    while (exp > 0) {
        if (exp % 2 == 1) {
            res = multiplicacao_modular(res, base, mod);
            printf("     %-15ld %-15ld %-15ld (exp e impar, res = res*base)\n", exp, base, res);
        }
        exp /= 2;
        base = multiplicacao_modular(base, base, mod);
        if (exp > 0) {
             printf("     %-15ld %-15ld %-15ld (exp/=2, base=base^2)\n", exp, base, res);
        }
    }
    if (exp_original == 0) res = 1;
    
    printf("     --------------------------------------------------\n");
    printf("     - Resultado Final: %ld ^ %ld (mod %ld) = %ld\n", base_original, exp_original, mod, res);
    return res;
}


/**
 * @brief Função auxiliar para obter e validar a entrada de um número do usuário.
 * @details Solução robusta para evitar bugs de buffer ao misturar scanf e fgets.
 * Garante que o número está no intervalo [100, 9999] e é distinto
 * de N1 (se aplicável). Cumpre o requisito de "Validação e Interatividade".
 * @param prompt A mensagem a ser exibida para o usuário.
 * @param numero Ponteiro para a variável que armazenará o número lido.
 * @param N1_para_comparacao O valor de N1, para garantir que N2 seja diferente. Passar 0 se não for aplicável.
 */
void obterNumeroDoUsuario(const char* prompt, long* numero, long N1_para_comparacao) {
    char buffer[256];
    int valido = 0;
    while (!valido) {
        printf("%s", prompt);
        fgets(buffer, sizeof(buffer), stdin);
        if (sscanf(buffer, "%ld", numero) == 1) {
            if (*numero >= 100 && *numero <= 9999) {
                 if (N1_para_comparacao == 0 || *numero != N1_para_comparacao) {
                     valido = 1; 
                 } else {
                     printf("Erro: N2 deve ser distinto de N1. Tente novamente.\n");
                 }
            } else {
                printf("Erro: O numero deve estar entre 100 e 9999. Tente novamente.\n");
            }
        } else {
            printf("Erro: Entrada invalida. Por favor, insira um numero.\n");
        }
    }
}


// -----------------------------------------------------------------------------
// PROGRAMA PRINCIPAL
// -----------------------------------------------------------------------------
int main() {
    long N1, N2, p, q, n, z, e, d;
    char mensagem_original[256];
    char mensagem_limpa[256];
    long blocos_originais[256], blocos_cifrados[256], blocos_decifrados[256];
    char mensagem_final[256];
    char caracteres_ignorados[256];
    int num_blocos;
    int i;

    printf("====================================================\n");
    printf("===        SISTEMA RSA COMPLETO (3 ETAPAS)         ===\n");
    printf("====================================================\n\n");
    
    // =========================================================================
    // ETAPA 1: FATORAÇÃO INTERATIVA
    // =========================================================================
    printf("----------------------------------------------------\n");
    printf("### ETAPA 1: FATORACAO INTERATIVA ###\n");
    printf("----------------------------------------------------\n");
    
    printf("\nPasso 1: Entrada de Dados\n");
    obterNumeroDoUsuario("N1 = ", &N1, 0);
    obterNumeroDoUsuario("N2 = ", &N2, N1);

    printf("\nPasso 2: Fatorando N1 = %ld\n", N1);
    p = pollardRho(N1);
    if (p == -1 || p == N1 || p == 1) {
        printf("Erro: N1 = %ld e primo ou a fatoracao falhou. Abortando.\n", N1);
        return 1;
    }
    printf("2.2. Fator de N1 encontrado: p = %ld\n", p);

    printf("\nPasso 3: Fatorando N2 = %ld\n", N2);
    q = pollardRho(N2);
    if (q == -1 || q == N2 || q == 1) {
        printf("Erro: N2 = %ld e primo ou a fatoracao falhou. Abortando.\n", N2);
        return 1;
    }
    printf("3.1. Fator de N2 encontrado: q = %ld\n", q);

    if (p == q) {
        printf("\nErro: Os fatores encontrados (p e q) sao iguais (%ld). O RSA requer primos distintos. Abortando.\n", p);
        return 1;
    }
    
    // =========================================================================
    // ETAPA 2: GERAÇÃO DAS CHAVES RSA
    // =========================================================================
    printf("\n\n----------------------------------------------------\n");
    printf("### ETAPA 2: GERACAO DAS CHAVES RSA ###\n");
    printf("----------------------------------------------------\n");
    printf("\nPasso 4: Calculo de n e z\n");
    n = p * q;
    z = (p - 1) * (q - 1);
    printf("n = p * q = %ld\n", n);
    printf("z = (p-1)*(q-1) = %ld\n", z);

    printf("\nPasso 5: Escolha do Expoente Publico (e)\n");
    printf("     - Procurando menor 'e' > 1 tal que mdc(e, z) = 1 (com z = %ld)\n", z);
    e = 2;
    
    // ======================================================================
    // === MODIFICACAO (CUMPRIR REQ. "Passo a Passo Visivel" para o MDC) ===
    // ======================================================================
    // A funcao 'mdc' silenciosa foi substituida por 'mdcDetalhado'
    // para imprimir o passo a passo da busca pelo expoente 'e'.
    
    while (mdcDetalhado(e, z, "e", "z") != 1) {
        e++;
        if(e >= z) { // Adicionada uma seguranca caso algo de errado
             printf("Erro: Nao foi possivel encontrar um expoente 'e' coprimo a 'z'.\n");
             return 1;
        }
        printf("     - Tentando proximo 'e' = %ld\n", e);
    }
    
    printf("  -> Expoente publico 'e' encontrado: %ld\n", e);

    printf("\nPasso 6: Calculo do Expoente Privado (d)\n");
    d = algoritmoEuclidesEstendido(e, z);
    printf("\n  -> Expoente privado 'd' encontrado: %ld\n", d);

    printf("\nPasso 7: Exibicao das Chaves\n");
    printf("  =======================================\n");
    printf("  Chave Publica (n, e) = (%ld, %ld)\n", n, e);
    printf("  Chave Privada (n, d) = (%ld, %ld)\n", n, d);
    printf("  =======================================\n");
    
    // =========================================================================
    // ETAPA 3: CODIFICAÇÃO E DECODIFICAÇÃO
    // =========================================================================
    printf("\n\n----------------------------------------------------\n");
    printf("### ETAPA 3: CODIFICACAO E DECODIFICACAO ###\n");
    printf("----------------------------------------------------\n");
    
    printf("\nPasso 8: Entrada da Mensagem\n");
    printf("Digite a mensagem a ser criptografada (A-Z, espaco): ");
    fgets(mensagem_original, sizeof(mensagem_original), stdin);
    mensagem_original[strcspn(mensagem_original, "\n")] = 0; 

    num_blocos = preCodificarMensagem(mensagem_original, blocos_originais, caracteres_ignorados);
    
    if (strlen(caracteres_ignorados) > 0) {
        printf("  -> Aviso: Os seguintes caracteres invalidos foram ignorados: %s\n", caracteres_ignorados);
    }

    reconverterParaTexto(blocos_originais, num_blocos, mensagem_limpa); 

    printf("  -> Mensagem pre-codificada em blocos de 2 digitos:\n    ");
    for (i = 0; i < num_blocos; i++) printf("%02ld ", blocos_originais[i]);
    printf("\n");

    printf("\nPasso 9: Processo de Codificacao (Criptografia)\n");
    printf("C = M^e (mod n)  |  C = M^%ld (mod %ld)\n", e, n);
    for (i = 0; i < num_blocos; i++) {
        blocos_cifrados[i] = exponenciacaoModularDetalhada(blocos_originais[i], e, n, z);
    }
    printf("\n  -> Mensagem Cifrada (blocos numericos):\n    ");
    for (i = 0; i < num_blocos; i++) printf("%ld ", blocos_cifrados[i]);
    printf("\n");

    printf("\nPasso 10: Processo de Decodificacao (Descriptografia)\n");
    printf("M = C^d (mod n)  |  M = C^%ld (mod %ld)\n", d, n);
    for (i = 0; i < num_blocos; i++) {
        blocos_decifrados[i] = exponenciacaoModularDetalhada(blocos_cifrados[i], d, n, z);
    }
    printf("\n  -> Blocos Numericos Decifrados:\n    ");
    for (i = 0; i < num_blocos; i++) printf("%02ld ", blocos_decifrados[i]);
    printf("\n");

    reconverterParaTexto(blocos_decifrados, num_blocos, mensagem_final);
    printf("  -> Mensagem Reconvertida para Texto: %s\n", mensagem_final);
    
    printf("\nPasso 11: Confirmacao Final do Resultado\n");
    if (strcmp(mensagem_limpa, mensagem_final) == 0) {
        printf("  ==============================================================\n");
        printf("  >>> SUCESSO: A mensagem decifrada e identica a original. <<<\n");
        printf("  ==============================================================\n");
    } else {
        printf("  ==============================================================\n");
        printf("  >>> FALHA: A mensagem decifrada e diferente da original. <<<\n");
        printf("  ==============================================================\n");
    }

    return 0;
}