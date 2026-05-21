#ifndef CPUVGPU_GPU_SIMULATOR_HPP
#define CPUVGPU_GPU_SIMULATOR_HPP

#include <vector>
#include <cstdint>
#include <stdexcept>

namespace gpu_sim {

    enum OpCode : int32_t {
        OP_IADD = 1,
        OP_STORE = 2,
        OP_RETURN = 0
    };

    /**
     * @brief Representation of a generic SIMT Graphics Processing Unit.
     * Simulates parallel execution of instructions across a grid of threads.
     */
    struct GPUEngine {
        std::size_t target_thread_count;

        inline GPUEngine(std::size_t thread_count) : target_thread_count(thread_count) {
            if (thread_count == 0) {
                throw std::invalid_argument("Thread count must be greater than zero.");
            }
        }

        /**
         * @brief Executes intermediate shader instructions across all allocated virtual threads.
         */
        inline void execute_shader(const std::vector<int32_t>& instruction_stream, std::vector<int32_t>& uniform_buffer) {
            if (instruction_stream.empty()) {
                throw std::invalid_argument("Instruction stream cannot be empty.");
            }

            for (std::size_t thread_index = 0; thread_index < target_thread_count; ++thread_index) {
                std::size_t program_counter = 0;
                int32_t local_registers[4] = {0, 0, 0, 0};

                while (true) {
                    int32_t current_opcode = instruction_stream[program_counter];
                    program_counter++;

                    if (current_opcode == OpCode::OP_RETURN) {
                        break;
                    }

                    if (current_opcode == OpCode::OP_IADD) {
                        std::size_t destination_register = static_cast<std::size_t>(instruction_stream[program_counter++]);
                        int32_t first_operand = instruction_stream[program_counter++];
                        int32_t second_operand = instruction_stream[program_counter++];
                        local_registers[destination_register] = first_operand + second_operand;
                    }
                    else if (current_opcode == OpCode::OP_STORE) {
                        std::size_t target_address = static_cast<std::size_t>(instruction_stream[program_counter++]);
                        std::size_t source_register = static_cast<std::size_t>(instruction_stream[program_counter++]);

                        if (target_address >= uniform_buffer.size()) {
                            throw std::out_of_range("Target memory address is out of bounds.");
                        }
                        uniform_buffer[target_address] = local_registers[source_register];
                    }
                    else {
                        throw std::runtime_error("Invalid or unsupported GPU opcode encountered.");
                    }
                }
            }
        }
    };

} // namespace gpu_sim

#endif
