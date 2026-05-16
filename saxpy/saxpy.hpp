#ifndef CPUVGPU_SAXPY_HPP
#define CPUVGPU_SAXPY_HPP

#include <vector>
#include <stdexcept>

namespace algebra {

/**
 * Realiza a operação Y = a * X + Y.
 * Segue o princípio KISS: direto ao ponto, sem classes desnecessárias.
 */
    void apply_saxpy(float multiplier, const std::vector<float>& input_x, std::vector<float>& result_y) {
        if (input_x.size() != result_y.size()) {
            throw std::invalid_argument("Vector sizes must be identical for SAXPY operation.");
        }

        for (std::size_t i = 0; i < input_x.size(); ++i) {
            float multiplied_x = multiplier * input_x[i];
            result_y[i] = multiplied_x + result_y[i];
        }
    }

}

#endif //CPUVGPU_SAXPY_HPP
