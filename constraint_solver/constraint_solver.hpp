#ifndef CPUVGPU_CONSTRAINT_SOLVER_HPP
#define CPUVGPU_CONSTRAINT_SOLVER_HPP

#include <vector>
#include <stdexcept>

namespace algorithms {
    /**
     * @brief Checks if it is safe to assign a specific color to a given node based on graph constraints.
     */
    inline bool is_assignment_valid(int node, const std::vector<std::vector<int>>& graph, const std::vector<int>& color_assignments, int assigned_color) {
        int total_nodes = static_cast<int>(graph.size());
        for (int neighbor = 0; neighbor < total_nodes; ++neighbor) {
            if (graph[node][neighbor] == 1 && color_assignments[neighbor] == assigned_color) {
                return false;
            }
        }
        return true;
    }

    /**
     * @brief Internal recursive backtracking algorithm to find a valid constraint-satisfying configuration.
     */
    inline bool solve_constraints_backtracking(int node, int total_colors, const std::vector<std::vector<int>>& graph, std::vector<int>& color_assignments) {
        int total_nodes = static_cast<int>(graph.size());
        if (node == total_nodes) {
            return true;
        }

        for (int current_color = 1; current_color <= total_colors; ++current_color) {
            if (is_assignment_valid(node, graph, color_assignments, current_color)) {
                color_assignments[node] = current_color;

                if (solve_constraints_backtracking(node + 1, total_colors, graph, color_assignments)) {
                    return true;
                }

                color_assignments[node] = 0;
            }
        }

        return false;
    }

    /**
     * @brief Solves a graph coloring constraint satisfaction problem using recursive backtracking.
     * * Takes an adjacency matrix graph, available colors count, and populates the color assignments vector buffer.
     */
    inline void solve_graph_coloring(int total_colors, const std::vector<std::vector<int>>& graph, std::vector<int>& color_assignments) {
        if (graph.empty() || graph.size() != graph[0].size()) {
            throw std::invalid_argument("Graph must be a non-empty square adjacency matrix.");
        }
        if (graph.size() != color_assignments.size()) {
            throw std::invalid_argument("Graph and color assignments vector sizes must be identical.");
        }
        if (total_colors <= 0) {
            throw std::invalid_argument("Total colors available must be greater than zero.");
        }

        int total_nodes = static_cast<int>(graph.size());
        for (int i = 0; i < total_nodes; ++i) {
            color_assignments[i] = 0;
        }

        if (!solve_constraints_backtracking(0, total_colors, graph, color_assignments)) {
            throw std::runtime_error("No valid solution found satisfying the given constraints.");
        }
    }

}

#endif //CPUVGPU_CONSTRAINT_SOLVER_HPP
