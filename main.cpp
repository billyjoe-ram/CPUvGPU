#include <iostream>
#include <chrono>
#include <vector>
#include <string>
#include "CPU/cpu_simulator.hpp"
#include "GPU/gpu_simulator.hpp"

void saxpy_riscv_cpu(cpu_sim::RISCV_CPU& cpu, float multiplier, const std::vector<float>& input_x, std::vector<float>& result_y);
void monte_carlo_riscv_cpu(cpu_sim::RISCV_CPU& cpu, float volatility, std::size_t simulation_steps, const std::vector<float>& current_prices, std::vector<float>& predicted_prices);
void bubble_sort_riscv_cpu(cpu_sim::RISCV_CPU& cpu, const std::vector<float>& input_elements, std::vector<float>& sorted_elements);
void constraint_solver_riscv_cpu(cpu_sim::RISCV_CPU& cpu, int total_colors, const std::vector<int32_t>& graph_bitmasks, std::vector<int32_t>& color_assignments);

int main() {
    std::cout << "Iniciando contador..." << std::endl;

    auto tempo_inicio_programa = std::chrono::high_resolution_clock::now();

    std::cout << "CPU" << std::endl;

    std::cout << "..." << std::endl;

    std::cout << "SAXPY" << std::endl;

    auto tempo_inicio_saxpy = std::chrono::high_resolution_clock::now();

    try {
        cpu_sim::RISCV_CPU cpu{2048};
        float multiplier = 2.5f;
        std::vector<float> input_x = {1.0f, 2.0f, 3.0f, 4.0f};
        std::vector<float> result_y = {10.0f, 20.0f, 30.0f, 40.0f};

        saxpy_riscv_cpu(cpu, multiplier, input_x, result_y);

        for (std::size_t i = 0; i < result_y.size(); ++i) {
            std::cout << result_y[i] << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return 1;
    }

    auto tempo_fim_saxpy = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao_saxpy = tempo_fim_saxpy - tempo_inicio_saxpy;
    std::cout << "Tempo SAXPY: " << duracao_saxpy.count() << " ms" << std::endl;

    std::cout << "MONTE CARLO" << std::endl;

    auto tempo_inicio_monte_carlo = std::chrono::high_resolution_clock::now();

    try {
        cpu_sim::RISCV_CPU cpu{8192};
        float volatility = 0.05f;
        std::size_t simulation_steps = 10;
        std::vector<float> current_prices = {100.0f, 50.0f, 200.0f};
        std::vector<float> predicted_prices = {0.0f, 0.0f, 0.0f};

        monte_carlo_riscv_cpu(cpu, volatility, simulation_steps, current_prices, predicted_prices);

        for (std::size_t i = 0; i < predicted_prices.size(); ++i) {
            std::cout << predicted_prices[i] << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
        return 1;
    }

    auto tempo_fim_monte_carlo = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao_monte_carlo = tempo_fim_monte_carlo - tempo_inicio_monte_carlo;
    std::cout << "Tempo Monte Carlo: " << duracao_monte_carlo.count() << " ms" << std::endl;

    std::cout << "BUBBLE SORT" << std::endl;

    auto tempo_inicio_bubble_sort = std::chrono::high_resolution_clock::now();

    try {
        std::vector<float> input_data = {45.2f, 12.1f, 89.5f, 3.7f, 22.9f, 67.3f, 1.1f};
        std::vector<float> sorted_output(input_data.size(), 0.0f);

        std::size_t required_memory = 400 + (input_data.size() * sizeof(float));
        cpu_sim::RISCV_CPU cpu(required_memory);

        bubble_sort_riscv_cpu(cpu, input_data, sorted_output);

        std::cout << "Original elements: ";
        for (float val : input_data) {
            std::cout << val << " ";
        }
        std::cout << "\n";

        std::cout << "Sorted elements:   ";
        for (float val : sorted_output) {
            std::cout << val << " ";
        }
        std::cout << "\n";
    }
    catch (const std::exception& error) {
        std::cerr << "Execution error: " << error.what() << "\n";
        return 1;
    }

    auto tempo_fim_bubble_sort = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duracao_bubble_sort = tempo_fim_bubble_sort - tempo_inicio_bubble_sort;
    std::cout << "Tempo Bubble Sort: " << duracao_bubble_sort.count() << " ms" << std::endl;

    std::cout << "CONSTRAINT SOLVER" << std::endl;

    auto tempo_inicio_constraint = std::chrono::high_resolution_clock::now();

    try {
        std::vector<int32_t> graph_bitmasks = {2, 5, 2};
        std::vector<int32_t> color_output(graph_bitmasks.size(), 0);
        int total_colors_available = 3;

        std::size_t required_memory = 2048;
        cpu_sim::RISCV_CPU cpu(required_memory);

        constraint_solver_riscv_cpu(cpu, total_colors_available, graph_bitmasks, color_output);

        std::cout << "Node color configuration:\n";
        for (std::size_t i = 0; i < color_output.size(); ++i) {
            std::cout << "Node " << i << " -> Color Mask: " << color_output[i] << "\n";
        }
    }
    catch (const std::exception& error) {
        std::cerr << "Solver execution failed: " << error.what() << "\n";
        return 1;
    }

    auto tempo_fim_constraint = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao_constraint = tempo_fim_constraint - tempo_inicio_constraint;
    std::cout << "Tempo Constraint Solver: " << duracao_constraint.count() << " ms" << std::endl;

    auto tempo_fim_programa = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao_total = tempo_fim_programa - tempo_inicio_programa;

    auto tempo_fim_cpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao_total_cpu = tempo_fim_cpu - tempo_inicio_programa;

    std::cout << "Fim do contador CPU" << std::endl;

    std::cout << "..." << std::endl;

    std::cout << "Iniciar Programa GPU? S/N ";

    auto inicio_pausa_usuario = std::chrono::high_resolution_clock::now();

    std::string input;
    std::getline(std::cin, input);

    if (input != "S" && input != "s") {
        std::cout << "Tempo Total do Programa: " << duracao_total_cpu.count() << " ms" << std::endl;

        return 0;
    }
    auto fim_pausa_usuario = std::chrono::high_resolution_clock::now();

    auto tempo_atual_pos_input = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> tempo_bruto_total = tempo_atual_pos_input - tempo_inicio_programa;

    std::chrono::duration<double, std::milli> tempo_da_pausa = fim_pausa_usuario - inicio_pausa_usuario;

    std::cout << "GPU" << std::endl;

    std::chrono::duration<double, std::milli> duracao_total_descontada = tempo_bruto_total - tempo_da_pausa;

    std::cout << "Tempo Total do Programa: " << duracao_total_descontada.count() << " ms" << std::endl;

    gpu_sim::GpuMemory vram(1024);

    vram.uniform_buffer[100] = 10; // Endereço 100 contém o valor 10
    vram.uniform_buffer[101] = 20; // Endereço 101 contém o valor 20
    vram.uniform_buffer[102] = 0;  // Endereço 102 guardará o resultado

    gpu_sim::GPU_Core core(vram);

    /** * Representação binária do SPIR-V montado anteriormente:
     * No SPIR-V real, os IDs são gerados sequencialmente.
     */
    core.program = {
            (int32_t)gpu_sim::SpvOp::OpVariable, 1, 100,
            (int32_t)gpu_sim::SpvOp::OpVariable, 2, 101,
            (int32_t)gpu_sim::SpvOp::OpVariable, 3, 102,

            (int32_t)gpu_sim::SpvOp::OpLoad, 4, 1,
            (int32_t)gpu_sim::SpvOp::OpLoad, 5, 2,

            (int32_t)gpu_sim::SpvOp::OpIAdd, 6, 4, 5,

            (int32_t)gpu_sim::SpvOp::OpStore, 3, 6,

            (int32_t)gpu_sim::SpvOp::OpReturn
    };

    std::cout << "Iniciando Kernel SPIR-V na GPU..." << std::endl;

    try {
        core.execute();

        std::cout << "Execução finalizada." << std::endl;
        std::cout << "Resultado na VRAM (addr 102): " << vram.uniform_buffer[102] << std::endl;

        std::cout << "ID %6 (Resultado da ALU): " << core.id_table[6] << std::endl;
    }
    catch (const std::exception& e) {
        std::cerr << "GPU Exception: " << e.what() << std::endl;
    }

    return 0;
}
