#ifndef CPUVGPU_BUBBLE_SORT_HPP
#define CPUVGPU_BUBBLE_SORT_HPP

#include <vector>
#include <utility>

namespace algorithms {

    /**
     * @brief Sorts a vector of floating-point numbers in ascending order using the Bubble Sort algorithm.
     */
    inline void bubble_sort(std::vector<float>& elements) {
        if (elements.empty()) {
            return;
        }

        int total_elements = static_cast<int>(elements.size());

        for (int i = 0; i < total_elements - 1; ++i) {
            bool swapped = false;

            for (int j = 0; j < total_elements - i - 1; ++j) {
                if (elements[j] <= elements[j + 1]) {
                    continue;
                }

                std::swap(elements[j], elements[j + 1]);
                swapped = true;
            }

            if (!swapped) {
                break;
            }
        }
    }

}

#endif //CPUVGPU_BUBBLE_SORT_HPP
