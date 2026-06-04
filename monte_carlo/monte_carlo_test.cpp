#include <iostream>
#include <vector>
#include "monte_carlo.hpp"

int monte_carlo_test() {
    try {
        std::vector<float> current_prices = {100.0f, 50.5f, 250.25f, 10.10f};
        std::vector<float> predicted_prices(4, 0.0);

        algorithms::predict_asset_prices(0.02, 5000, current_prices, predicted_prices);

        for (std::size_t i = 0; i < predicted_prices.size(); ++i) {
            std::cout << "Asset " << i << " - Current: " << current_prices[i]
                      << " | Predicted: " << predicted_prices[i] << "\n";
        }
    }
    catch (const std::invalid_argument& e) {
        std::cerr << "Error: " << e.what() << "\n";
        return 1;
    }

    return 0;
}