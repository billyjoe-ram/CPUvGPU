#include <iostream>
#include <vector>
#include "saxpy.hpp"

int saxpy_test() {
    std::cout << "Hello, World!" << std::endl;

    std::cout << "SAXPY" << std::endl;

    auto input_x = std::vector<float>{1.0f, 2.0f, 3.0f, 4.0f};
    auto result_y = std::vector<float>{10.0f, 20.0f, 30.0f, 40.0f};

    float scalar = 2.5f;

    try {
        algorithms::apply_saxpy(scalar, input_x, result_y);

        for (float value : result_y) {
            std::cout << value << " ";
        }
        std::cout << std::endl;
    }
    catch (const std::invalid_argument& error) {
        std::cerr << "Calculation Error: " << error.what() << std::endl;
        return 1;
    }
    return 0;
}
