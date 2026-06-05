#include "../GPU/gpu_simulator.hpp"

void bubble_sort_gpu(gpu_sim::GpuMemory& memory, const std::vector<int32_t>& input_elements, std::vector<int32_t>& sorted_elements) {
    if (input_elements.size() != sorted_elements.size()) {
        throw std::invalid_argument("Input and sorted element vector sizes must be identical.");
    }

    std::size_t total_elements = input_elements.size();
    for (std::size_t i = 0; i < total_elements; ++i) {
        memory.uniform_buffer[i] = input_elements[i];
    }

    for (std::size_t i = 0; i < total_elements - 1; ++i) {
        for (std::size_t j = 0; j < total_elements - i - 1; ++j) {
            gpu_sim::GPU_Core core{memory};

            std::size_t address_j = j;
            std::size_t address_next = j + 1;

            if (memory.uniform_buffer[address_j] > memory.uniform_buffer[address_next]) {
                core.program = {
                        static_cast<int32_t>(gpu_sim::SpvOp::OpVariable), 1, static_cast<int32_t>(address_j),
                        static_cast<int32_t>(gpu_sim::SpvOp::OpVariable), 2, static_cast<int32_t>(address_next),
                        static_cast<int32_t>(gpu_sim::SpvOp::OpLoad), 3, 1,
                        static_cast<int32_t>(gpu_sim::SpvOp::OpLoad), 4, 2,
                        static_cast<int32_t>(gpu_sim::SpvOp::OpStore), 1, 4,
                        static_cast<int32_t>(gpu_sim::SpvOp::OpStore), 2, 3,
                        static_cast<int32_t>(gpu_sim::SpvOp::HALT)
                };
                core.execute();
            }
        }
    }

    for (std::size_t i = 0; i < total_elements; ++i) {
        sorted_elements[i] = memory.uniform_buffer[i];
    }
}