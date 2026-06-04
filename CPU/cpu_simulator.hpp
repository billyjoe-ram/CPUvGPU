#ifndef CPUVGPU_SAXPY_CPU_HPP
#define CPUVGPU_SAXPY_CPU_HPP

#include <vector>
#include <stdexcept>
#include <cstdint>
#include <cstring>

/**
 * @brief Simple Von Neumann CPU architecture simulation.
 * Core logic for fetching, decoding, and executing basic instructions.
 */
namespace cpu_sim {
    enum OpCode : int32_t {
        HALT    = 0,
        ADDI    = 1,  // Somar imediato a um registrador inteiro (ex: i++)
        BEQ     = 2,  // Desviar se dois registradores inteiros forem iguais (controle do loop)
        SLLI    = 3,  // Shift Left Logical (usado para multiplicar o índice por 4 bytes do float)
        ADD     = 4,  // Somar dois registradores inteiros (calcular endereço de memória)
        FLW     = 5,  // Floating-point Load Word (ler float da memória)
        FSW     = 6,  // Floating-point Store Word (escrever float na memória)
        FMADD_S = 7,   // Fused Multiply-Add (fa2 = fa0 * fa1 + fa2)
        BLT     = 8,  // Para o Bubble Sort ordenar
        RAND_F  = 9,  // Para o Monte Carlo sortear
        AND     = 10  // Para o Constraint Solver filtrar
    };

    struct RISCV_CPU {
        int32_t pc = 0;
        int32_t ir = 0;

        int32_t x_regs[32] = {0};
        float   f_regs[32]  = {0.0f};

        std::vector<uint8_t> memory;

        RISCV_CPU(std::size_t mem_size) : memory(mem_size, 0) {}

        inline int32_t fetch_int32() {
            int32_t val;
            std::memcpy(&val, &memory[pc], sizeof(int32_t));
            pc += sizeof(int32_t);
            return val;
        }

        inline float read_mem_float(int32_t address) {
            float val;
            std::memcpy(&val, &memory[address], sizeof(float));
            return val;
        }

        inline void write_mem_float(int32_t address, float val) {
            std::memcpy(&memory[address], &val, sizeof(float));
        }

        /**
         * @brief Ciclo de Execução RISC-V traduzido em C++
         */
        void run() {
            while (true) {
                ir = fetch_int32();

                switch (static_cast<OpCode>(ir)) {
                    case OpCode::HALT:
                        return;

                    case OpCode::BEQ: {
                        int32_t r1 = fetch_int32();
                        int32_t r2 = fetch_int32();
                        int32_t offset = fetch_int32(); // Para onde pular se igual
                        if (x_regs[r1] == x_regs[r2]) {
                            pc = offset;
                        }
                        break;
                    }

                    case OpCode::SLLI: {
                        int32_t rd = fetch_int32();
                        int32_t rs1 = fetch_int32();
                        int32_t shamt = fetch_int32(); // Quantidade de bits para deslocar
                        x_regs[rd] = x_regs[rs1] << shamt;
                        break;
                    }

                    case OpCode::ADD: {
                        int32_t rd = fetch_int32();
                        int32_t rs1 = fetch_int32();
                        int32_t rs2 = fetch_int32();
                        x_regs[rd] = x_regs[rs1] + x_regs[rs2];
                        break;
                    }

                    case OpCode::FLW: {
                        int32_t fd = fetch_int32();   // Registrador float destino
                        int32_t rs1 = fetch_int32();  // Registrador inteiro com o endereço base
                        f_regs[fd] = read_mem_float(x_regs[rs1]);
                        break;
                    }

                    case OpCode::FSW: {
                        int32_t fs = fetch_int32();   // Registrador float contendo o dado
                        int32_t rs1 = fetch_int32();  // Registrador inteiro com o endereço base
                        write_mem_float(x_regs[rs1], f_regs[fs]);
                        break;
                    }

                    case OpCode::FMADD_S: {
                        int32_t fd = fetch_int32();
                        int32_t fs1 = fetch_int32();
                        int32_t fs2 = fetch_int32();
                        int32_t fs3 = fetch_int32();
                        // fd = fs1 * fs2 + fs3
                        f_regs[fd] = (f_regs[fs1] * f_regs[fs2]) + f_regs[fs3];
                        break;
                    }

                    case OpCode::ADDI: {
                        int32_t rd = fetch_int32();
                        int32_t rs1 = fetch_int32();
                        int32_t imm = fetch_int32();
                        x_regs[rd] = x_regs[rs1] + imm;
                        break;
                    }

                    case OpCode::BLT: {
                        int32_t r1 = fetch_int32();
                        int32_t r2 = fetch_int32();
                        int32_t offset = fetch_int32();
                        if (x_regs[r1] < x_regs[r2]) {
                            pc = offset;
                        }
                        break;
                    }

                    case OpCode::RAND_F: {
                        int32_t fd = fetch_int32();
                        f_regs[fd] = static_cast<float>(std::rand()) / static_cast<float>(RAND_MAX);
                        break;
                    }

                    case OpCode::AND: {
                        int32_t rd = fetch_int32();
                        int32_t rs1 = fetch_int32();
                        int32_t rs2 = fetch_int32();
                        x_regs[rd] = x_regs[rs1] & x_regs[rs2];
                        break;
                    }

                    default:
                        throw std::runtime_error("Instrução RISC-V desconhecida.");
                }
            }
        }
    };
}

#endif //CPUVGPU_SAXPY_CPU_HPP
