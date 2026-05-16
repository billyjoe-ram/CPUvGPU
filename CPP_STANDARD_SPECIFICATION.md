# Especificação de Desenvolvimento C++: Código Limpo e Pragmático

Este documento serve como um template técnico rigoroso para a geração de soluções em C++. Ele equilibra os princípios de Clean Code com a pragmática das filosofias KISS e YAGNI, especificamente adaptado para problemas de computação de alto desempenho ou álgebra linear.

## 1. Padrões Arquiteturais
- **Abordagem Header-Only:** Para minimizar a burocracia do projeto, prefira um único arquivo `.hpp` para a lógica.
- **Encapsulamento por Namespace:** Toda a lógica deve estar contida em um namespace relevante (ex: `namespace algebra`) para evitar poluição do escopo global.
- **Lógica Inline:** Todas as funções definidas em headers devem ser marcadas com a palavra-chave `inline` para evitar erros de "múltipla definição" durante a linkagem.

## 2. Princípios SOLID & Clean Code
- **SRP (Responsabilidade Única):** As funções devem realizar uma tarefa clara.
- **KISS & YAGNI:** Evite "over-engineering". Sem interfaces, tabelas virtuais ou hierarquias complexas de classes, a menos que seja estritamente necessário.
- **Fail Fast:** Use exceções padrão (ex: `std::invalid_argument`) para validar entradas no início da função.
- **Código Autodocumentado:** Não use comentários explicando *o que* o código faz. Nomes de funções e variáveis devem ser descritivos (ex: `apply_saxpy`, `multiplier`, `result_y`).

## 3. Regras Específicas de Implementação C++
- **Uso de STL Moderna:** Use `std::vector` para gerenciamento de memória (RAII). Sem ponteiros crus, `new` ou `delete`.
- **Passagem de Parâmetros:** - Entradas não modificadas: `const std::vector<T>&`.
    - Saídas/Buffers modificados: `std::vector<T>&`.
- **Const-Correctness:** Garanta que parâmetros sejam `const` sempre que não devam ser modificados.
- **Tipagem Explícita vs. Auto:**
    - Use `auto` **APENAS** quando o tipo estiver explicitamente declarado no lado direito da atribuição (ex: `auto vec = std::vector<float>{...}`).
    - Use tipos explícitos (ex: `float`, `int`, `std::size_t`) para parâmetros de função, tipos de retorno, variáveis de loop e expressões complexas.

## 4. Documentação e Idioma
- **Consistência de Idioma:** O código (variáveis, funções, classes) deve ser em Inglês.
- **Documentação de Métodos:** Forneça um breve comentário estilo Doxygen (em Inglês) apenas no header para explicar o propósito matemático da função.
- **Main Limpo:** O arquivo `main.cpp` deve conter zero comentários. Deve lidar estritamente com a preparação de dados, a chamada da função dentro de um bloco `try-catch` e a saída.

## 5. Estilo de Codificação
- **Indentação:** 4 espaços.
- **Loops:** Use `std::size_t` para indexação quando o acesso direto por índice for necessário para correção matemática entre múltiplos vetores.
