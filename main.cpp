// could be fun to have the cumulative error calculated from a ring buffer
// with exponential decay to the oldest value
#include "pid.hpp"
#include "pid_tuner.hpp"
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
auto update_gains(const T previous_value, const T target, const pid::Gain<T> gains, const pid::Errors<T> errors, const T learning_rate) {
  // Calculate the gradients of the loss with respect to each gain parameter
  // Update each gain parameter using the gradients and learning rate
  auto p_loss = pid::mse(target, previous_value);
  auto p_adjustment = -learning_rate * p_loss;

  auto i_adjustment = T{0}; //  learning_rate * dx_dt_wrt_i;
  printf("adjustments P: %f, I: %f\n", p_adjustment, i_adjustment);

  auto new_gains = gains;
  new_gains.p -= p_adjustment;

  return new_gains;
}

enum class Tuning { None, Auto };

template <typename T, Tuning UpdateGains = Tuning::None>
void process() {
  // add in some delay to the system! make it work harder!
  auto rb = RingBuffer<T, 16>{};
  rb.delay_samples = 4;

  constexpr auto setpoint = T{40.0};
  constexpr auto num_iterations = 2000;
  pid::Gain<T> gains = {.p = 0.01, .i = 0.01, .d = 0.01};
  pid::Errors<T> errors{};

  constexpr auto learning_rate = T{2e-6};

  auto sensor_value = T{0.0};
  printf("%f\n", sensor_value);

  for (auto counter = 0; counter < num_iterations; counter++) {
    auto [new_value, e] = process_sample<T>(sensor_value, setpoint, gains, errors, T{0.01});
    errors = e;

    // update the gains
    if constexpr (UpdateGains == Tuning::Auto) {
        gains = update_gains(sensor_value, setpoint, gains, errors, learning_rate);
    }

    // delay the output
    rb.write(new_value);

    sensor_value += rb.read();

    if (counter % 50 == (50 - 1)) {
      sensor_value += random_up_to<T>(20.0);
    }

    auto simulation_noise = random_up_to<T>(1.0);
    sensor_value += simulation_noise;
    printf("%f\n", sensor_value);
    printf("P: %f, I: %f, D: %f\n", gains.p, gains.i, gains.d);
  }

}

// simulation of a process with noise
int main() {
  process<double, Tuning::Auto>();
  return 0;
}
