#include <iostream>
#include <vector>
#include "constraint_solver.hpp"

int constraint_solver_test() {
    try {
        std::vector<std::vector<int>> graph = {
                {0, 1, 1, 1},
                {1, 0, 1, 0},
                {1, 1, 0, 1},
                {1, 0, 1, 0}
        };

        std::vector<int> color_assignments(4, 0);
        int total_colors = 3;

        algorithms::solve_graph_coloring(total_colors, graph, color_assignments);

        for (std::size_t i = 0; i < color_assignments.size(); ++i) {
            std::cout << "Node " << i << " -> Assigned Color: " << color_assignments[i] << "\n";
        }
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Validation Error: " << e.what() << "\n";
        return 1;
    }
    catch (const std::runtime_error& e) {
        std::cerr << "Solver Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}
