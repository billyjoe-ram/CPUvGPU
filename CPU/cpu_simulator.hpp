#ifndef CPUVGPU_CPU_SIMULATOR_HPP
#define CPUVGPU_CPU_SIMULATOR_HPP

#include <vector>
#include <stdexcept>
#include <cstdint>

/**
 * @brief Simple Von Neumann CPU architecture simulation.
 * Core logic for fetching, decoding, and executing basic instructions.
 */
namespace cpu_sim {

    enum OpCode : int32_t {
        MOV_IMM = 1, // Move immediate to register
        ADD_REG = 2, // Add register to register
        HALT    = 0  // Stop execution
    };

    struct CPU {
        int32_t pc = 0;                  // Program Counter
        int32_t ir = 0;                  // Instruction Register
        int32_t registers[4] = {0, 0, 0, 0}; // R0, R1, R2, R3
        bool zero_flag = false;          // Flags
        std::vector<int32_t> memory;     // Unified Memory

        inline CPU(std::size_t mem_size) : memory(mem_size, 0) {}

        /**
         * @brief Executes the Fetch-Decode-Execute cycle.
         */
        inline void run() {
            while (true) {
                // Fetch
                ir = memory[pc];
                pc++;

                // Decode & Execute
                if (ir == OpCode::HALT) {
                    break;
                }

                if (ir == OpCode::MOV_IMM) {
                    int32_t reg_idx = memory[pc++];
                    int32_t value = memory[pc++];
                    registers[reg_idx] = value;
                }
                else if (ir == OpCode::ADD_REG) {
                    int32_t dest_idx = memory[pc++];
                    int32_t src_idx = memory[pc++];

                    registers[dest_idx] += registers[src_idx];
                    zero_flag = (registers[dest_idx] == 0); // Update Flags
                }
                else {
                    throw std::runtime_error("Unknown Instruction");
                }
            }
        }
    };

} // namespace cpu_sim

#endif //CPUVGPU_CPU_SIMULATOR_HPP