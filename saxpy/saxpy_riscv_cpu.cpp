#include "../CPU/cpu_simulator.hpp"

#include <vector>
#include <stdexcept>
#include <cstring>

/**
 * @brief Translates and executes the SAXPY algorithm on the simulated RISC-V CPU.
 */
void saxpy_riscv_cpu(cpu_sim::RISCV_CPU& cpu, float multiplier, const std::vector<float>& input_x, std::vector<float>& result_y) {
    if (input_x.size() != result_y.size()) {
        throw std::invalid_argument("Vector sizes must be identical for SAXPY operation.");
    }

    std::size_t vector_size = input_x.size();
    int32_t code_segment_size = 120;
    int32_t data_size = static_cast<int32_t>(vector_size * sizeof(float));

    int32_t address_x = code_segment_size;
    int32_t address_y = address_x + data_size;

    if (cpu.memory.size() < static_cast<std::size_t>(address_y + data_size)) {
        throw std::invalid_argument("CPU memory size is insufficient for the requested operation vectors.");
    }

    std::memcpy(&cpu.memory[address_x], input_x.data(), static_cast<std::size_t>(data_size));
    std::memcpy(&cpu.memory[address_y], result_y.data(), static_cast<std::size_t>(data_size));

    int32_t buffer_pointer = 0;

    int32_t op_addi = static_cast<int32_t>(cpu_sim::OpCode::ADDI);
    int32_t op_add = static_cast<int32_t>(cpu_sim::OpCode::ADD);
    int32_t op_slli = static_cast<int32_t>(cpu_sim::OpCode::SLLI);
    int32_t op_flw = static_cast<int32_t>(cpu_sim::OpCode::FLW);
    int32_t op_fsw = static_cast<int32_t>(cpu_sim::OpCode::FSW);
    int32_t op_fmadd = static_cast<int32_t>(cpu_sim::OpCode::FMADD_S);
    int32_t op_beq = static_cast<int32_t>(cpu_sim::OpCode::BEQ);
    int32_t op_halt = static_cast<int32_t>(cpu_sim::OpCode::HALT);

    int32_t reg_x0 = 0;
    int32_t reg_x1 = 1;
    int32_t reg_x2 = 2;
    int32_t reg_x3 = 3;
    int32_t reg_x10 = 10;
    int32_t reg_x11 = 11;
    int32_t reg_x12 = 12;
    int32_t reg_x13 = 13;

    int32_t reg_f0 = 0;
    int32_t reg_f1 = 1;
    int32_t reg_f2 = 2;

    int32_t constant_two = 2;
    int32_t constant_one = 1;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x10, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &address_x, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x11, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &address_y, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x12, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    int32_t signed_size = static_cast<int32_t>(vector_size);
    std::memcpy(&cpu.memory[buffer_pointer], &signed_size, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x13, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;

    cpu.f_regs[0] = multiplier;

    int32_t loop_start_address = buffer_pointer;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x13, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x12, sizeof(int32_t)); buffer_pointer += 4;
    int32_t patch_target_halt = buffer_pointer;
    buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_slli, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x1, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x13, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &constant_two, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_add, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x2, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x10, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x1, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_add, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x3, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x11, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x1, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_flw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f1, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x2, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_flw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f2, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x3, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_fmadd, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f2, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f1, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f2, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_fsw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_f2, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x3, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x13, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x13, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &constant_one, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &loop_start_address, sizeof(int32_t)); buffer_pointer += 4;

    int32_t halt_address = buffer_pointer;
    std::memcpy(&cpu.memory[buffer_pointer], &op_halt, sizeof(int32_t));

    std::memcpy(&cpu.memory[patch_target_halt], &halt_address, sizeof(int32_t));

    cpu.pc = 0;
    cpu.run();

    std::memcpy(result_y.data(), &cpu.memory[address_y], static_cast<std::size_t>(data_size));
}