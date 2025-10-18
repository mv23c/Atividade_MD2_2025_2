# Análise de Código Criptográfico

Este documento resume a análise, execução e verificação de um conjunto de
funções em C destinadas a operações de aritmética modular, comuns em
criptografia.

## 1\. Resultado da Execução

O código foi simulado com as seguintes entradas para determinar seu resultado
final.

### Entradas:

- `H: 7`
- `G: 3`
- `Zn: 11`
- `x: 10`
- `n1: 13`

### Valor Final da Resposta

A execução do programa com as entradas acima produz o seguinte resultado final:

```
Valor final da congruência: 4
```

---

## 2\. Verificação das Afirmativas

A seguir estão as respostas e justificativas para as afirmativas propostas,
baseadas na análise do código.

- **Afirmativa:** O algoritmo de Euclides estendido é utilizado para calcular o
  inverso modular de um número.

  - ✅ **(V) Verdadeiro.** A função `inversoModular` implementa precisamente
    este algoritmo.

- **Afirmativa:** Se mdc(G, Zn) ≠ 1, o programa ainda consegue encontrar o
  inverso de G em Zn.

  - ❌ **(F) Falso.** O inverso modular só existe se o MDC entre os números for
    igual a 1.

- **Afirmativa:** A operação (H \* inverso) % Zn representa a divisão modular de
  H por G.

  - ✅ **(V) Verdadeiro.** Esta é a definição formal de divisão em aritmética
    modular (multiplicar pelo inverso).

- **Afirmativa:** Se n1 for primo, o código aplica o Pequeno Teorema de Fermat
  para simplificar o cálculo de a^x mod n1.

  - ❌ **(F) Falso.** O código usa o algoritmo de exponenciação por quadratura,
    que não depende do Teorema de Fermat para funcionar, embora ambos lidem com
    exponenciação modular.

- **Afirmativa:** A função powMod implementa o cálculo de potência modular
  utilizando multiplicações diretas sem otimização.

  - ❌ **(F) Falso.** A função utiliza um método altamente otimizado
    (exponenciação por quadratura), que é muito mais eficiente.

- **Afirmativa:** Quando o resultado do inverso é negativo, o código ajusta o
  valor somando o módulo m0.

  - ✅ **(V) Verdadeiro.** O código contém uma verificação (`if (x1 < 0)`) para
    ajustar resultados negativos, garantindo uma saída positiva.

- **Afirmativa:** O cálculo de fi(n1) (função totiente de Euler) é utilizado
  apenas quando n1 não é primo.

  - ❌ **(F) Falso.** A função totiente de Euler não é utilizada em nenhuma
    parte do código analisado.
