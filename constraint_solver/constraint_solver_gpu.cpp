#include "../GPU/gpu_simulator.hpp"

void constraint_solver_gpu(gpu_sim::GpuMemory& memory, const std::vector<int32_t>& graph_constraints, std::vector<int32_t>& color_assignments) {
    if (graph_constraints.size() != color_assignments.size()) {
        throw std::invalid_argument("Graph constraints and color assignments vector sizes must be identical.");
    }

    std::size_t total_nodes = graph_constraints.size();
    for (std::size_t i = 0; i < total_nodes; ++i) {
        memory.uniform_buffer[i] = color_assignments[i];
    }

    for (std::size_t node = 0; node < total_nodes; ++node) {
        gpu_sim::GPU_Core core{memory};
        std::size_t address_node = node;

        core.program = {
                static_cast<int32_t>(gpu_sim::SpvOp::OpVariable), 1, static_cast<int32_t>(address_node),
                static_cast<int32_t>(gpu_sim::SpvOp::OpLoad), 2, 1,
                static_cast<int32_t>(gpu_sim::SpvOp::OpConstant), 3, 1,
                static_cast<int32_t>(gpu_sim::SpvOp::OpIAdd), 4, 2, 3,
                static_cast<int32_t>(gpu_sim::SpvOp::OpStore), 1, 4,
                static_cast<int32_t>(gpu_sim::SpvOp::HALT)
        };
        core.execute();
        color_assignments[node] = memory.uniform_buffer[address_node];
    }
}