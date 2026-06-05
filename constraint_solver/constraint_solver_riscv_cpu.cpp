#include "../CPU/cpu_simulator.hpp"
#include <vector>
#include <stdexcept>
#include <cstring>

/**
 * @brief Translates and executes the Graph Coloring Constraint Solver on the simulated RISC-V CPU.
 */
void constraint_solver_riscv_cpu(cpu_sim::RISCV_CPU& cpu, int total_colors, const std::vector<int>& graph_bitmasks, std::vector<int>& color_assignments) {
    if (graph_bitmasks.size() != color_assignments.size()) {
        throw std::invalid_argument("Graph bitmasks and color assignments vector sizes must be identical.");
    }
    if (graph_bitmasks.empty()) {
        return;
    }
    if (total_colors <= 0 || total_colors > 31) {
        throw std::invalid_argument("Total colors must be between 1 and 31 due to bitmask limitations.");
    }

    std::size_t vector_size = graph_bitmasks.size();
    int32_t code_segment_size = 400;
    int32_t data_size = static_cast<int32_t>(vector_size * sizeof(int32_t));

    int32_t address_graph = code_segment_size;
    int32_t address_colors = address_graph + data_size;

    if (cpu.memory.size() < static_cast<std::size_t>(address_colors + data_size)) {
        throw std::invalid_argument("CPU memory size is insufficient for the requested operation vectors.");
    }

    std::memcpy(&cpu.memory[address_graph], graph_bitmasks.data(), static_cast<std::size_t>(data_size));
    std::vector<int32_t> initial_colors(vector_size, 0);
    std::memcpy(&cpu.memory[address_colors], initial_colors.data(), static_cast<std::size_t>(data_size));

    int32_t buffer_pointer = 0;

    int32_t op_addi = static_cast<int32_t>(cpu_sim::OpCode::ADDI);
    int32_t op_add = static_cast<int32_t>(cpu_sim::OpCode::ADD);
    int32_t op_slli = static_cast<int32_t>(cpu_sim::OpCode::SLLI);
    int32_t op_and = static_cast<int32_t>(cpu_sim::OpCode::AND);
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
    int32_t reg_x6 = 6;
    int32_t reg_x7 = 7;
    int32_t reg_x10 = 10;
    int32_t reg_x11 = 11;
    int32_t reg_x12 = 12;
    int32_t reg_x13 = 13;
    int32_t reg_x14 = 14;

    int32_t constant_two = 2;
    int32_t constant_one = 1;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x10, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &address_graph, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x11, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &address_colors, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x12, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    int32_t signed_vector_size = static_cast<int32_t>(vector_size);
    std::memcpy(&cpu.memory[buffer_pointer], &signed_vector_size, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x13, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &total_colors, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;

    int32_t solver_loop_start = buffer_pointer;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x12, sizeof(int32_t)); buffer_pointer += 4;
    int32_t patch_target_success = buffer_pointer;
    buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_blt, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    int32_t patch_target_failure = buffer_pointer;
    buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_slli, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x1, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
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
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x4, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x2, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_flw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x3, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    int32_t patch_target_first_color = buffer_pointer;
    buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_slli, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &constant_one, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    int32_t patch_target_check_constraint = buffer_pointer;
    buffer_pointer += 4;

    int32_t set_initial_color_bit = buffer_pointer;
    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &constant_one, sizeof(int32_t)); buffer_pointer += 4;

    int32_t evaluate_color = buffer_pointer;

    std::memcpy(&cpu.memory[buffer_pointer], &op_slli, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x6, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &constant_one, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x13, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_blt, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x6, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    int32_t patch_target_backtrack = buffer_pointer;
    buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_and, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x7, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x4, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x7, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    int32_t patch_target_valid_color = buffer_pointer;
    buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_slli, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &constant_one, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &evaluate_color, sizeof(int32_t)); buffer_pointer += 4;

    int32_t register_assignment = buffer_pointer;
    std::memcpy(&cpu.memory[buffer_pointer], &op_fsw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x5, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x3, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &constant_one, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &solver_loop_start, sizeof(int32_t)); buffer_pointer += 4;

    int32_t execute_backtrack = buffer_pointer;
    std::memcpy(&cpu.memory[buffer_pointer], &op_fsw, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x3, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    int32_t step_back = -1;
    std::memcpy(&cpu.memory[buffer_pointer], &step_back, sizeof(int32_t)); buffer_pointer += 4;

    std::memcpy(&cpu.memory[buffer_pointer], &op_beq, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &solver_loop_start, sizeof(int32_t)); buffer_pointer += 4;

    int32_t termination_success = buffer_pointer;
    std::memcpy(&cpu.memory[buffer_pointer], &op_halt, sizeof(int32_t)); buffer_pointer += 4;

    int32_t termination_failure = buffer_pointer;
    std::memcpy(&cpu.memory[buffer_pointer], &op_addi, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x14, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &reg_x0, sizeof(int32_t)); buffer_pointer += 4;
    int32_t error_flag = -1;
    std::memcpy(&cpu.memory[buffer_pointer], &error_flag, sizeof(int32_t)); buffer_pointer += 4;
    std::memcpy(&cpu.memory[buffer_pointer], &op_halt, sizeof(int32_t));

    std::memcpy(&cpu.memory[patch_target_success], &termination_success, sizeof(int32_t));
    std::memcpy(&cpu.memory[patch_target_failure], &termination_failure, sizeof(int32_t));
    std::memcpy(&cpu.memory[patch_target_first_color], &set_initial_color_bit, sizeof(int32_t));
    std::memcpy(&cpu.memory[patch_target_check_constraint], &evaluate_color, sizeof(int32_t));
    std::memcpy(&cpu.memory[patch_target_backtrack], &execute_backtrack, sizeof(int32_t));
    std::memcpy(&cpu.memory[patch_target_valid_color], &register_assignment, sizeof(int32_t));

    cpu.pc = 0;
    cpu.run();

    if (cpu.x_regs[14] == -1) {
        throw std::runtime_error("No valid solution found satisfying the given constraints.");
    }

    std::memcpy(color_assignments.data(), &cpu.memory[address_colors], static_cast<std::size_t>(data_size));
}
