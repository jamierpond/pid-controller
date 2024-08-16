// could be fun to have the cumulative error calculated from a ring buffer
// with exponential decay to the oldest value
#include <cstdio>
#include <random>
#include <type_traits>
#include "pid.h"

template<typename T>
T random_up_to(T max) {
    static_assert(std::is_arithmetic_v<T>, "T must be an arithmetic type");

    static std::random_device rd;
    static std::mt19937 gen(rd());

    if constexpr (std::is_integral_v<T>) {
        std::uniform_int_distribution<T> dis(-max, max);
        return dis(gen);
    } else {
        std::uniform_real_distribution<T> dis(-max, max);
        return dis(gen);
    }
}

// simulation of a process with noise
int main() {
  pid::Gain<float> gains = {
    .p = 0.3f,
    .i = 0.1f,
    .d = 0.3f
  };

  pid::Errors<float> errors {};

  auto v = -2.10098f; // some initial value
  for (auto counter = 0; counter < 500; counter++) {
    auto [output, e] = process_sample(v, 40.0f, gains, errors);
    errors = e;

    v += output;
    if (counter % 200 == 0) {
      v += random_up_to(100.0f);
    }

    if (counter % 100 == 0) {
      v += random_up_to(50.0f);
    }

    auto simulation_noise = random_up_to(2.0);
    v += simulation_noise;
    printf("%f\n", v);
  }
}
