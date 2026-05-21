#include <iostream>
#include <vector>
#include "CPU/cpu_simulator.hpp"
#include "CPU/gpu_simulator.hpp"

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

    try {
        auto shader_program = std::vector<int32_t>{1, 0, 10, 20, 2, 0, 0, 0};
        auto global_uniform_buffer = std::vector<int32_t>{0};

        gpu_sim::GPUEngine virtual_gpu(64);
        virtual_gpu.execute_shader(shader_program, global_uniform_buffer);

        std::cout << global_uniform_buffer[0] << std::endl;
    }
    catch (const std::exception& error) {
        std::cerr << error.what() << std::endl;
        return 1;
    }

    return 0;
}
