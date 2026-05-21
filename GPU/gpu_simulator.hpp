#ifndef CPUVGPU_GPU_SIMULATOR_HPP
#define CPUVGPU_GPU_SIMULATOR_HPP

#include <vector>
#include <map>
#include <cstdint>
#include <stdexcept>
#include <iostream>

namespace gpu_sim {

    enum class SpvOp : int32_t {
        OpConstant = 10,
        OpVariable = 11,
        OpLoad     = 12,
        OpIAdd     = 13,
        OpStore    = 14,
        OpReturn   = 15,
        HALT       = 0
    };

    struct GpuMemory {
        std::vector<int32_t> uniform_buffer;
        GpuMemory(size_t size) : uniform_buffer(size, 0) {}
    };

    /**
     * @brief Simulador de uma Unidade de Execução (EU/CU) de GPU
     */
    struct GPU_Core {
        int32_t pc = 0;

        std::map<uint32_t, int32_t> id_table;

        std::map<uint32_t, uint32_t> pointer_map;

        std::vector<int32_t> program;
        GpuMemory& vram;

        GPU_Core(GpuMemory& m) : vram(m) {}

        void execute() {
            while (true) {
                SpvOp op = static_cast<SpvOp>(program[pc++]);

                if (op == SpvOp::HALT || op == SpvOp::OpReturn) break;

                switch (op) {
                    case SpvOp::OpConstant: {
                        uint32_t result_id = program[pc++];
                        int32_t value = program[pc++];
                        id_table[result_id] = value;
                        break;
                    }

                    case SpvOp::OpVariable: {
                        uint32_t result_id = program[pc++];
                        uint32_t mem_address = program[pc++];
                        pointer_map[result_id] = mem_address;
                        break;
                    }

                    case SpvOp::OpLoad: {
                        uint32_t result_id = program[pc++];
                        uint32_t pointer_id = program[pc++];
                        uint32_t addr = pointer_map[pointer_id];
                        // Simula a latência de busca na VRAM
                        id_table[result_id] = vram.uniform_buffer[addr];
                        break;
                    }

                    case SpvOp::OpIAdd: {
                        uint32_t result_id = program[pc++];
                        uint32_t id_left = program[pc++];
                        uint32_t id_right = program[pc++];
                        id_table[result_id] = id_table[id_left] + id_table[id_right];
                        break;
                    }

                    case SpvOp::OpStore: {
                        uint32_t pointer_id = program[pc++];
                        uint32_t source_id = program[pc++];
                        uint32_t addr = pointer_map[pointer_id];
                        vram.uniform_buffer[addr] = id_table[source_id];
                        break;
                    }

                    default:
                        throw std::runtime_error("Instrução SPIR-V não implementada ou inválida");
                }
            }
        }
    };

} // namespace gpu_sim

#endif
