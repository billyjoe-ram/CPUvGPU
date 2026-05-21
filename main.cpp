#include <iostream>
#include <vector>
#include "CPU/cpu_simulator.hpp"
#include "GPU/gpu_simulator.hpp"

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::cout << "CPU" << std::endl;

    try {
        int32_t x = 10;
        int32_t y = 20;

        cpu_sim::CPU cpu(64);

        // Program to sum x and y:
        // 0: MOV R0, x
        // 3: MOV R1, y
        // 6: ADD R0, R1
        // 9: HALT
        cpu.memory[0] = cpu_sim::OpCode::MOV_IMM;
        cpu.memory[1] = 0;
        cpu.memory[2] = x;

        cpu.memory[3] = cpu_sim::OpCode::MOV_IMM;
        cpu.memory[4] = 1;
        cpu.memory[5] = y;

        cpu.memory[6] = cpu_sim::OpCode::ADD_REG;
        cpu.memory[7] = 0;
        cpu.memory[8] = 1;

        cpu.memory[9] = cpu_sim::OpCode::HALT;

        cpu.run();

        std::cout << "Result in R0: " << cpu.registers[0] << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    std::cout << "GPU" << std::endl;

    gpu_sim::GpuMemory vram(1024);

    vram.uniform_buffer[100] = 10; // Endereço 100 contém o valor 10
    vram.uniform_buffer[101] = 20; // Endereço 101 contém o valor 20
    vram.uniform_buffer[102] = 0;  // Endereço 102 guardará o resultado

    gpu_sim::GPU_Core core(vram);

    /** * Representação binária do SPIR-V montado anteriormente:
     * No SPIR-V real, os IDs são gerados sequencialmente.
     */
    core.program = {
            (int32_t)gpu_sim::SpvOp::OpVariable, 1, 100,
            (int32_t)gpu_sim::SpvOp::OpVariable, 2, 101,
            (int32_t)gpu_sim::SpvOp::OpVariable, 3, 102,

            (int32_t)gpu_sim::SpvOp::OpLoad, 4, 1,
            (int32_t)gpu_sim::SpvOp::OpLoad, 5, 2,

            (int32_t)gpu_sim::SpvOp::OpIAdd, 6, 4, 5,

            (int32_t)gpu_sim::SpvOp::OpStore, 3, 6,

            (int32_t)gpu_sim::SpvOp::OpReturn
    };

    std::cout << "Iniciando Kernel SPIR-V na GPU..." << std::endl;

    try {
        core.execute();

        std::cout << "Execução finalizada." << std::endl;
        std::cout << "Resultado na VRAM (addr 102): " << vram.uniform_buffer[102] << std::endl;

        std::cout << "ID %6 (Resultado da ALU): " << core.id_table[6] << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "GPU Exception: " << e.what() << std::endl;
    }

    return 0;
}
