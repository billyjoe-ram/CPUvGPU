#include <iostream>
#include <vector>
#include "CPU/cpu_simulator.hpp"
#include "GPU/gpu_simulator.hpp"

void saxpy_riscv_cpu(cpu_sim::RISCV_CPU& cpu, float multiplier, const std::vector<float>& input_x, std::vector<float>& result_y);

int main() {
    std::cout << "Hello, World!" << std::endl;

    std::cout << "CPU" << std::endl;

    std::cout << "..." << std::endl;

    std::cout << "SAXPY" << std::endl;

    try {
        cpu_sim::RISCV_CPU cpu{2048};
        float multiplier = 2.5f;
        std::vector<float> input_x = {1.0f, 2.0f, 3.0f, 4.0f};
        std::vector<float> result_y = {10.0f, 20.0f, 30.0f, 40.0f};

        saxpy_riscv_cpu(cpu, multiplier, input_x, result_y);

        for (std::size_t i = 0; i < result_y.size(); ++i) {
            std::cout << result_y[i] << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
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
