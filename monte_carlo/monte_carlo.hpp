#ifndef CPUVGPU_MONTE_CARLO_HPP
#define CPUVGPU_MONTE_CARLO_HPP

#include <vector>
#include <random>
#include <stdexcept>

namespace algorithms {

    /**
     * @brief Predicts future asset prices using a Monte Carlo random walk simulation.
     * * Takes a vector of current prices, applies random price shocks over a number
     * of simulated iterations, and populates the predicted prices buffer.
     */
    inline void predict_asset_prices(float volatility, int simulation_steps, const std::vector<float>& current_prices, std::vector<float>& predicted_prices) {
        if (current_prices.size() != predicted_prices.size()) {
            throw std::invalid_argument("Current and predicted price vector sizes must be identical.");
        }
        if (simulation_steps == 0) {
            throw std::invalid_argument("Simulation steps must be greater than zero.");
        }

        std::mt19937 generator{std::random_device{}()};
        std::normal_distribution<float> distribution{0.0f, volatility};

        int total_elements = static_cast<int>(current_prices.size());

        for (int i = 0; i < total_elements; ++i) {
            float price = current_prices[i];

            for (int step = 0; step < simulation_steps; ++step) {
                float shock = distribution(generator);
                price += price * shock;
            }

            predicted_prices[i] = price;
        }
    }

}

#endif //CPUVGPU_MONTE_CARLO_HPP
