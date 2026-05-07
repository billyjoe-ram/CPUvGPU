# 🧠 Simulação da CPU (Arquitetura Von Neumann Simplificada)

A modelagem da CPU em software baseia-se em uma arquitetura do tipo **Von Neumann simplificada**, onde **instruções e dados compartilham a mesma memória**.

Buscou-se reproduzir o funcionamento interno da CPU, incluindo seus principais componentes e o ciclo de execução de instruções.

---

## ⚙️ Componentes da CPU

A CPU simulada é composta pelos seguintes elementos:

- **PC (Program Counter)**
  Armazena o endereço da próxima instrução a ser executada.

- **IR (Instruction Register)**
  Guarda a instrução atualmente em execução.

- **Registradores (R0, R1, R2, ...)**
  Usados para armazenar dados temporários durante a execução.

- **FLAGS (ex: Zero Flag)**
  Indicam o resultado de operações (por exemplo, se o resultado foi zero).

- **Memória**
  Única estrutura que contém tanto **instruções quanto dados**.

- **ALU (Unidade Lógica e Aritmética)**
  Responsável por operações como soma e subtração.

- **CU (Unidade de Controle)**
  Responsável por interpretar instruções e controlar o fluxo de execução.

---

## 🔁 Ciclo de Execução

O modelo da CPU opera em um ciclo contínuo composto por três etapas principais:

### 1. Fetch (Busca)
O modelo lê a próxima instrução da memória usando o valor do PC:
- `IR ← memória[PC]`
- `PC ← PC + 1`

---

### 2. Decode (Decodificação)
A instrução armazenada no IR é interpretada:
- Identifica-se o **opcode** (tipo de instrução)
- Identificam-se os **operandos**
- Determina-se o **modo de endereçamento**

---

### 3. Execute (Execução)
A instrução é executada conforme seu tipo:

Exemplos:
- **MOV** → move valores para registradores
- **ADD** → soma valores usando a ALU
- **SUB** → subtração
- **JUMP** → altera o fluxo do programa
- **JZ** → desvio condicional baseado na flag zero
- **HALT** → encerra a execução

Após a execução:
- As **flags** são atualizadas (ex: Zero Flag)
- O ciclo reinicia (voltando ao Fetch), a menos que seja `HALT`

---

## 🧩 Modos de Endereçamento

A modelagem foi feita com modos simples de acesso a dados:

- **Imediato**
  O valor está diretamente na instrução
  Ex: `MOV R1, #5`

- **Registrador**
  O operando é outro registrador
  Ex: `ADD R1, R2`

- **Memória (direto)**
  O operando está em um endereço da memória
  Ex: `LOAD R1, [10]`

---

## 🔀 Controle de Fluxo

A execução não precisa ser linear. A modelagem da CPU também suporta:

- **JUMP** → altera diretamente o PC (desvio incondicional)
- **JZ (Jump if Zero)** → desvia se o resultado anterior foi zero

Isso permite implementar:
- condicionais (`if`)
- loops (`while`, `for`)

---

## 🧠 Características do Modelo

- Arquitetura de memória unificada (Von Neumann)
- Execução sequencial baseada em PC
- Separação clara entre:
  - dados
  - instruções
  - controle
- Totalmente determinístico

---

## ⚠️ Limitações (intencionais)

Este modelo é simplificado e não inclui:

- Pipeline de execução
- Cache
- Interrupções
- Execução paralela
- Modos avançados de endereçamento

Essas omissões são intencionais para manter o foco na compreensão do funcionamento básico de uma CPU.

---
