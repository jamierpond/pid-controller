// could be fun to have the cumulative error calculated from a ring buffer
// with exponential decay to the oldest value
#include <cstdio>
#include <cstdlib>
#include "pid.h"

auto random_up_to(float max) {
  return (((rand() % 100) / 100.0f) - 0.5) * max * 2.0f;
}

int main() {
  Gain<float> gains = {
    .p = 0.3f,
    .i = 0.1f,
    .d = 0.3f
  };

  Errors<float> errors = {
    .previous_error = 0,
    .cumulative_error = 0
  };

  auto v = -2.10098f;
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

    auto simulation_noise = random_up_to(2);
    v += simulation_noise;
    printf("%f\n", v);
  }
}
