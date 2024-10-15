// could be fun to have the cumulative error calculated from a ring buffer
// with exponential decay to the oldest value
#include "pid.hpp"
#include "ring_buffer.h"

#include <cstdio>
#include <random>
#include <type_traits>

template <typename T> T random_up_to(T max) {
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

template <typename T>
void process() {
  // add in some delay to the system! make it work harder!
  auto rb = RingBuffer<T, 16>{};
  rb.delay_samples = 4;

  constexpr auto setpoint = T{40.0};
  pid::Gain<T> gains = {.p = 0.15, .i = 0.0, .d = 0.1};
  pid::Errors<T> errors{};

  auto value = T{0.0};
  printf("%f\n", value);

  for (auto counter = 0; counter < 500; counter++) {
    auto [output, e] = process_sample<T>(value, setpoint, gains, errors, T{0.2});
    errors = e;

    // delay the output
    rb.write(output);

    value += rb.read();

    if (counter % 50 == (50 - 1)) {
      value += random_up_to<T>(20.0);
    }

    auto simulation_noise = random_up_to<T>(1.0);
    value += simulation_noise;
    printf("%f\n", value);
  }
}

// simulation of a process with noise
int main() {
  process<double>();
  return 0;
}
