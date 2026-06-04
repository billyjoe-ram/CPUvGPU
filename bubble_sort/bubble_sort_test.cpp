#include <iostream>
#include <vector>
#include "bubble_sort.hpp"

int bubble_sort_test() {
    try {
        std::vector<float> data = {64.5f, 34.2f, 25.0f, 12.1f, 22.9f, 11.0f, 90.7f};

        algorithms::bubble_sort(data);

        int size = static_cast<int>(data.size());

        for (int i = 0; i < size; ++i) {
            std::cout << data[i] << " ";
        }
        std::cout << "\n";
    }
    catch (...) {
        return 1;
    }

    return 0;
}
