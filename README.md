# Atividade de Matemática Discreta 2

**Universidade:** Universidade de Brasília (UnB)  
**Disciplina:** FGA0108 - MATEMÁTICA DISCRETA 2 - T01 (2025.2)  
**Professora:** CRISTIANE LOESCH DE SOUZA COSTA

---

## Descrição do Projeto

Este repositório contém as implementações desenvolvidas para a atividade avaliativa da disciplina de **Matemática Discreta II (FGA0108)**, da Universidade de Brasília. A atividade, com entrega para **18/10/2025**, consiste em quatro questões que aplicam conceitos de teoria dos números em problemas práticos, com implementação obrigatória em linguagem C ou C++.

O trabalho completo proposto pela disciplina abrange os seguintes desafios:

### Questão 01: Sistema de Criptografia RSA Completo

* **Objetivo:** Implementar um sistema de criptografia e descriptografia RSA em três etapas.
    1.  **Fatoração:** Utilizar o **método p de Pollard** para encontrar os fatores primos `p` e `q` de dois números compostos fornecidos pelo usuário. O cálculo do MDC, parte do método, deve ser feito com o Algoritmo de Euclides.
    2.  **Geração de Chaves:** A partir de `p` e `q`, calcular o módulo `n` e o totiente de Euler `z(n)`. Em seguida, encontrar os expoentes `E` (público) e `D` (privado), onde `D` é o inverso modular de `E` (mod z) e deve ser encontrado com o **Algoritmo Estendido de Euclides**.
    3.  **Criptografia/Descriptografia:** Cifrar e decifrar uma mensagem convertendo caracteres para um sistema numérico pré-definido (`A=11`, `B=12`, etc.). A exponenciação modular ($M^E \pmod{n}$ e $C^D \pmod{n}$) deve ser otimizada aplicando o **Pequeno Teorema de Fermat**, o **Teorema de Euler** ou a **Divisão Euclidiana**, com o programa indicando qual teorema foi usado.

### Questão 02: Chaves Periódicas

* **Objetivo:** Descobrir o primeiro ano, entre 1 e 50, em que um conjunto de chaves com diferentes ciclos de ativação se tornam ativas simultaneamente. O problema é inspirado no desafio "Cigarras Periódicas" da Maratona de Programação da SBC.
* **Solução:** O problema se resolve encontrando o **Mínimo Múltiplo Comum (MMC)** de todos os ciclos de chaves fornecidos. Se o resultado for maior que 50, deve-se informar a impossibilidade. O enunciado deixa a cargo do aluno a investigação e implementação do método.

### Questão 03: A Razão de Eficiência de um Número

* **Objetivo:** Calcular a "Razão de Eficiência" de um número N, definida como $\frac{\sigma(N)}{\tau(N)}$, onde $\sigma(N)$ é a soma dos divisores de N e $\tau(N)$ é a quantidade de divisores de N.
* **Implementação:** O programa deve primeiro decompor N em seus fatores primos usando o método **Trial Division**. A partir da fatoração prima, deve-se usar as fórmulas matemáticas para calcular $\tau(N)$ e $\sigma(N)$ e, por fim, a razão, exibindo todos os passos intermediários do cálculo.

### Questão 04 (Bônus): Análise e Conclusão de Código

* **Objetivo:** Esta questão possui duas partes:
    1.  **Completar o Código:** Preencher as linhas faltantes em um programa C fornecido que realiza divisão modular ($H \div G \pmod{Zn}$) e exponenciação modular ($a^x \pmod{n1}$).
    2.  **Análise Teórica:** Após preencher o código, determinar a saída para um conjunto de entradas específico e classificar uma série de afirmativas sobre os algoritmos do código como Verdadeiro (V) ou Falso (F).

## Equipe

| Aluno                            | Matrícula |
| -------------------------------- | --------- |
| Mário Vinícius Beleza Carneiro   | 202046265 |
| Mateus de Siqueira Silva         | 200024787 |

---

## Como Compilar e Executar

### Pré-requisitos
- Um compilador C (como GCC) instalado.

### Passos

1.  Clone o repositório:
    ```bash
    git clone [URL-DO-SEU-REPOSITORIO]
    ```

2.  Navegue até o diretório do projeto:
    ```bash
    cd [NOME-DO-SEU-DIRETORIO]
    ```

3.  Compile o código-fonte:
    ```bash
    gcc nome_arquivo.c -o nome_arquivo
    ```

4.  Execute o programa:
    ```bash
    ./nome_arquivo
    ```
