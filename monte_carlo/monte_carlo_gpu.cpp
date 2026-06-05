#include "../GPU/gpu_simulator.hpp"

/**
 * @brief Translates and executes the Monte Carlo asset price prediction algorithm on the simulated GPU.
 */
void monte_carlo_gpu(gpu_sim::GpuMemory& memory, int32_t shock_factor, int32_t simulation_steps, const std::vector<int32_t>& current_prices, std::vector<int32_t>& predicted_prices) {
    if (current_prices.size() != predicted_prices.size()) {
        throw std::invalid_argument("Current and predicted price vector sizes must be identical.");
    }
    if (simulation_steps <= 0) {
        throw std::invalid_argument("Simulation steps must be greater than zero.");
    }

    std::size_t total_elements = current_prices.size();
    for (std::size_t i = 0; i < total_elements; ++i) {
        gpu_sim::GPU_Core core{memory};

        std::size_t address_pred = total_elements + i;
        memory.uniform_buffer[address_pred] = current_prices[i];

        for (int32_t step = 0; step < simulation_steps; ++step) {
            core.program = {
                    static_cast<int32_t>(gpu_sim::SpvOp::OpVariable), 1, static_cast<int32_t>(address_pred),
                    static_cast<int32_t>(gpu_sim::SpvOp::OpLoad), 2, 1,
                    static_cast<int32_t>(gpu_sim::SpvOp::OpConstant), 3, shock_factor,
                    static_cast<int32_t>(gpu_sim::SpvOp::OpIAdd), 4, 2, 3,
                    static_cast<int32_t>(gpu_sim::SpvOp::OpStore), 1, 4,
                    static_cast<int32_t>(gpu_sim::SpvOp::HALT)
            };
            core.pc = 0;
            core.execute();
        }
        predicted_prices[i] = memory.uniform_buffer[address_pred];
    }
}