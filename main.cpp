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

void saxpy_gpu(gpu_sim::GpuMemory& memory, int32_t multiplier, const std::vector<int32_t>& input_x, std::vector<int32_t>& result_y);
void monte_carlo_gpu(gpu_sim::GpuMemory& memory, int32_t shock_factor, int32_t simulation_steps, const std::vector<int32_t>& current_prices, std::vector<int32_t>& predicted_prices);
void bubble_sort_gpu(gpu_sim::GpuMemory& memory, const std::vector<int32_t>& input_elements, std::vector<int32_t>& sorted_elements);
void constraint_solver_gpu(gpu_sim::GpuMemory& memory, const std::vector<int32_t>& graph_constraints, std::vector<int32_t>& color_assignments);

int main() {
    std::cout << "Iniciando contador..." << std::endl;

    auto tempo_inicio_programa = std::chrono::high_resolution_clock::now();

    std::cout << "CPU" << std::endl;

    std::cout << "...\n\n" << std::endl;

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
    std::cout << "Tempo SAXPY: " << duracao_saxpy.count() << " ms\n\n" << std::endl;

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
    std::cout << "Tempo Monte Carlo: " << duracao_monte_carlo.count() << " ms\n\n" << std::endl;

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
    std::cout << "Tempo Bubble Sort: " << duracao_bubble_sort.count() << " ms\n\n" << std::endl;

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
    std::cout << "Tempo Constraint Solver: " << duracao_constraint.count() << " ms\n\n" << std::endl;

    auto tempo_fim_programa = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao_total = tempo_fim_programa - tempo_inicio_programa;

    auto tempo_fim_cpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao_total_cpu = tempo_fim_cpu - tempo_inicio_programa;

    std::cout << "Fim do contador CPU\n\n" << std::endl;

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

    std::chrono::duration<double, std::milli> tempo_da_pausa = fim_pausa_usuario - inicio_pausa_usuario;

    std::cout << "GPU" << std::endl;

    std::cout << "SAXPY" << std::endl;

    auto tempo_inicio_saxpy_gpu = std::chrono::high_resolution_clock::now();
    try {
        gpu_sim::GpuMemory memory{1024};
        int32_t multiplier = 2;
        std::vector<int32_t> input_x = {1, 2, 3, 4};
        std::vector<int32_t> result_y = {10, 20, 30, 40};

        saxpy_gpu(memory, multiplier, input_x, result_y);

        for (std::size_t i = 0; i < result_y.size(); ++i) {
            std::cout << result_y[i] << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
    auto tempo_fim_saxpy_gpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao_saxpy_gpu = tempo_fim_saxpy_gpu - tempo_inicio_saxpy_gpu;
    std::cout << "Tempo SAXPY: " << duracao_saxpy_gpu.count() << " ms\n\n" << std::endl;

    std::cout << "Monte Carlo" << std::endl;
    auto tempo_inicio_monte_carlo_gpu = std::chrono::high_resolution_clock::now();
    try {
        gpu_sim::GpuMemory memory{1024};
        int32_t shock_factor = 5;
        int32_t simulation_steps = 10;
        std::vector<int32_t> current_prices = {100, 200, 300, 400};
        std::vector<int32_t> predicted_prices = {0, 0, 0, 0};

        monte_carlo_gpu(memory, shock_factor, simulation_steps, current_prices, predicted_prices);

        for (std::size_t i = 0; i < predicted_prices.size(); ++i) {
            std::cout << predicted_prices[i] << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
    auto tempo_fim_monte_carlo_gpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao_monte_carlo_gpu = tempo_fim_monte_carlo_gpu - tempo_inicio_monte_carlo_gpu;
    std::cout << "Tempo Monte Carlo: " << duracao_monte_carlo_gpu.count() << " ms\n\n" << std::endl;

    std::cout << "Bubble Sort" << std::endl;
    auto tempo_inicio_bubble_gpu = std::chrono::high_resolution_clock::now();
    try {
        gpu_sim::GpuMemory memory{1024};
        std::vector<int32_t> input_elements = {40, 20, 10, 30};
        std::vector<int32_t> sorted_elements = {0, 0, 0, 0};

        bubble_sort_gpu(memory, input_elements, sorted_elements);

        for (std::size_t i = 0; i < sorted_elements.size(); ++i) {
            std::cout << sorted_elements[i] << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }
    auto tempo_fim_bubble_gpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao_bubble_gpu = tempo_fim_bubble_gpu - tempo_inicio_bubble_gpu;
    std::cout << "Tempo Bubble Sort: " << duracao_bubble_gpu.count() << " ms\n\n" << std::endl;

    std::cout << "Constraint Solver" << std::endl;
    auto tempo_inicio_constraint_gpu = std::chrono::high_resolution_clock::now();

    try {
        gpu_sim::GpuMemory memory{1024};
        std::vector<int32_t> graph_constraints = {1, 1, 0, 1};
        std::vector<int32_t> color_assignments = {0, 0, 0, 0};

        constraint_solver_gpu(memory, graph_constraints, color_assignments);

        for (std::size_t i = 0; i < color_assignments.size(); ++i) {
            std::cout << color_assignments[i] << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::exception& exception) {
        std::cerr << exception.what() << std::endl;
    }

    auto tempo_fim_constraint_gpu = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double, std::milli> duracao_constraint_gpu = tempo_fim_constraint_gpu - tempo_inicio_constraint_gpu;
    std::cout << "Tempo Constraint Solver: " << duracao_constraint_gpu.count() << " ms\n\n" << std::endl;

    tempo_fim_programa = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duracao_total_descontada = (tempo_fim_programa - tempo_inicio_programa) - tempo_da_pausa;

    std::cout << "Tempo Total do Programa: " << duracao_total_descontada.count() << " ms" << std::endl;

    return 0;
}
