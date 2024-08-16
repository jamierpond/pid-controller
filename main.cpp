// could be fun to have the cumulative error calculated from a ring buffer
// with exponential decay to the oldest value
#include "pid.h"
#include <cstdio>
#include <random>
#include <type_traits>
#include "ring_buffer.h"

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


// simulation of a process with noise
int main() {
  pid::Gain<float> gains = {.p = 0.15, .i = 0.0f, .d = 0.1f};
  pid::Errors<float> errors{};

  constexpr auto setpoint = 40.0f;
  auto v = 0.0f;
  printf("%f\n", v);

  // add in some delay to the system! make it work harder!
  auto rb = RingBuffer<float, int, 16>{};
  rb.delay_samples = 4;

  for (auto counter = 0; counter < 500; counter++) {
    auto [output, e] = process_sample(v, setpoint, gains, errors, 0.2f);
    errors = e;

    // delay the output
    rb.write(output);

    v += rb.read();

    if (counter % 50 == (50 - 1)) {
      v += random_up_to(20.0f);
    }

    auto simulation_noise = random_up_to(1.0);
    v += simulation_noise;
    printf("%f\n", v);
  }
}
