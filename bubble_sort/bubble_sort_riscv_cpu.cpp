#include "../CPU/cpu_simulator.hpp"
#include <vector>
#include <vector>
#include <stdexcept>
#include <cstring>

/**
 * @brief Translates and executes the Monte Carlo asset price prediction algorithm on the simulated RISC-V CPU.
 */
void bubble_sort_riscv_cpu(cpu_sim::RISCV_CPU& cpu, const std::vector<float>& input_elements, std::vector<float>& sorted_elements) {
    if (input_elements.size() != sorted_elements.size()) {
        throw std::invalid_argument("Input and sorted element vector sizes must be identical.");
    }
    if (input_elements.empty()) {
        return;
    }

    std::size_t vector_size = input_elements.size();
    int32_t code_segment_size = 400;
    int32_t data_size = static_cast<int32_t>(vector_size * sizeof(float));

    int32_t address_sorted = code_segment_size;

    if (cpu.memory.size() < static_cast<std::size_t>(address_sorted + data_size)) {
        throw std::invalid_argument("CPU memory size is insufficient for the requested operation vectors.");
    }

    std::memcpy(&cpu.memory[address_sorted], input_elements.data(), static_cast<std::size_t>(data_size));

    int32_t buffer_pointer = 0;

    int32_t op_addi = static_cast<int32_t>(cpu_sim::OpCode::ADDI);
    int32_t op_add = static_cast<int32_t>(cpu_sim::OpCode::ADD);
    int32_t op_slli = static_cast<int32_t>(cpu_sim::OpCode::SLLI);
    int32_t op_flw = static_cast<int32_t>(cpu_sim::OpCode::FLW);
    int32_t op_fsw = static_cast<int32_t>(cpu_sim::OpCode::FSW);
    int32_t op_beq = static_cast<int32_t>(cpu_sim::OpCode::BEQ);
    int32_t op_blt = static_cast<int32_t>(cpu_sim::OpCode::BLT);
    int32_t op_halt = static_cast<int32_t>(cpu_sim::OpCode::HALT);

    int32_t reg_x0 = 0;
    int32_t reg_x1 = 1;
    int32_t reg_x2 = 2;
    int32_t reg_x3 = 3;
    int32_t reg_x4 = 4;
    int32_t reg_x5 = 5;
    int32_t reg_x10 = 10;
    int32_t reg_x11 = 11;
    int32_t reg_x12 = 12;
    int32_t reg_x13 = 13;
    int32_t reg_x14 = 14;

    int32_t reg_f1 = 1;
    int32_t reg_f2 = 2;

    int32_t constant_two = 2;
    int32_t constant_one = 1;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x10, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &address_sorted, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x11, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    int32_t signed_vector_size = static_cast<int32_t>(vector_size);
    std::memcpy(&cpu.memory[buffer_pointer], &signed_vector_size, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x12, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x11, sizeof(int32_t)); buffer_pointer += 4;
    int32_t minus_one = -1;
    std::memcpy(&cpu.memory[buffer_pointer], &minus_one, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x13, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;

    int32_t outer_loop_start = buffer_pointer;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x13, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x12, sizeof(int32_t)); buffer_pointer += 4;
    int32_t patch_target_halt = buffer_pointer;
    buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;

    int32_t inner_loop_start = buffer_pointer;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x12, sizeof(int32_t)); buffer_pointer += 4;
    int32_t patch_target_inner_exit = buffer_pointer;
    buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_slli, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x1, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &constant_two, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_add, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x2, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x10, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x1, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x3, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x2, sizeof(int32_t)); buffer_pointer += 4;
    int32_t sizeof_float = 4;
    std::memcpy(&cpu.memory[buffer_pointer], &sizeof_float, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_flw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f1, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x2, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_flw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f2, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x3, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_fsw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f1, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_fsw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f2, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x1, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_flw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f1, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_flw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f2, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x1, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_blt, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x1, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    int32_t patch_target_no_swap = buffer_pointer;
    buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_fsw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f1, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x3, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_fsw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f2, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x2, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &constant_one, sizeof(int32_t)); buffer_pointer += 4;

    int32_t no_swap_address = buffer_pointer;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &constant_one, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &inner_loop_start, sizeof(int32_t)); buffer_pointer += 4;

    int32_t inner_loop_exit_address = buffer_pointer;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    int32_t patch_target_early_exit = buffer_pointer;
    buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x13, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x13, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &constant_one, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &outer_loop_start, sizeof(int32_t)); buffer_pointer += 4;

    int32_t halt_address = buffer_pointer;
    std::memcpy(&cpu.memory[buffer_pointer], &op_halt, sizeof(int32_t));

    std::memcpy(&cpu.memory[patch_target_halt], &halt_address, sizeof(int32_t));
    std::memcpy(&cpu.memory[patch_target_inner_exit], &inner_loop_exit_address, sizeof(int32_t));
    std::memcpy(&cpu.memory[patch_target_no_swap], &no_swap_address, sizeof(int32_t));
    std::memcpy(&cpu.memory[patch_target_early_exit], &halt_address, sizeof(int32_t));

    cpu.pc = 0;
    cpu.run();

    std::memcpy(sorted_elements.data(), &cpu.memory[address_sorted], static_cast<std::size_t>(data_size));
}