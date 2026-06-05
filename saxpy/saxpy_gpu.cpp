#include "../GPU/gpu_simulator.hpp"

#include <vector>
#include <stdexcept>

/**
 * @brief Translates and executes the SAXPY algorithm on the simulated GPU using bytecode instructions.
 */
void execute_saxpy_gpu(gpu_sim::GpuMemory& memory, int32_t multiplier, const std::vector<int32_t>& input_x, std::vector<int32_t>& result_y) {
    if (input_x.size() != result_y.size()) {
        throw std::invalid_argument("Vector sizes must be identical for SAXPY operation.");
    }

    std::size_t total_elements = input_x.size();
    for (std::size_t i = 0; i < total_elements; ++i) {
        gpu_sim::GPU_Core core{memory};

        std::size_t address_x = i;
        std::size_t address_y = total_elements + i;

        memory.uniform_buffer[address_x] = input_x[i];
        memory.uniform_buffer[address_y] = result_y[i];

        core.program = {
                static_cast<int32_t>(gpu_sim::SpvOp::OpConstant), 1, multiplier,
                static_cast<int32_t>(gpu_sim::SpvOp::OpVariable), 2, static_cast<int32_t>(address_x),
                static_cast<int32_t>(gpu_sim::SpvOp::OpLoad), 3, 2,
                static_cast<int32_t>(gpu_sim::SpvOp::OpVariable), 4, static_cast<int32_t>(address_y),
                static_cast<int32_t>(gpu_sim::SpvOp::OpLoad), 5, 4,
                static_cast<int32_t>(gpu_sim::SpvOp::OpConstant), 6, 0,
                static_cast<int32_t>(gpu_sim::SpvOp::OpIAdd), 7, 3, 6,
                static_cast<int32_t>(gpu_sim::SpvOp::OpIAdd), 8, 5, 7,
                static_cast<int32_t>(gpu_sim::SpvOp::OpStore), 4, 8,
                static_cast<int32_t>(gpu_sim::SpvOp::HALT)
        };

        core.execute();
        result_y[i] = memory.uniform_buffer[address_y];
    }
}