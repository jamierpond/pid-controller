// could be fun to have the cumulative error calculated from a ring buffer
// with exponential decay to the oldest value
#include <cstdio>
#include <cstdlib>

template <typename T> struct Gain {
  T p{}, i{}, d{};
};

template <typename T> struct Errors {
  T previous_error{}, cumulative_error{};
};

template <typename T>
struct Response {
  T output;
  Errors<T> errors{};
};

template <typename T>
constexpr Response<T> process_sample(T sensor_input, T setpoint, const Gain<T>& gain, const Errors<T>& e) {
  auto current_error = setpoint - sensor_input;
  auto cumulative_error = e.cumulative_error + current_error;

  auto proportional = gain.p * current_error;
  auto integral = gain.i * cumulative_error;
  auto derivative = gain.d * (current_error - e.previous_error);

  return {
    .output = proportional + integral + derivative,
    .errors = {current_error, cumulative_error}
  };
}

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
