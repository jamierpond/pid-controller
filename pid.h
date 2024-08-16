#include <span>

namespace pid {

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
constexpr Response<T> process_sample(
    T sensor_input,
    T setpoint,
    const Gain<T>& gain,
    const Errors<T>& e,
    const T cumulative_error_backoff_coeff = 1.0f
) {
  auto current_error = setpoint - sensor_input;
  auto cumulative_error = e.cumulative_error + current_error;

  auto proportional = gain.p * current_error;
  auto derivative = gain.d * (current_error - e.previous_error);
  auto integral = gain.i * cumulative_error * cumulative_error_backoff_coeff;

  return {
    .output = proportional + integral + derivative,
    .errors = {
      .previous_error = current_error,
      .cumulative_error = cumulative_error
    }
  };
}


template <typename T>
constexpr Errors<T> process_buffer(
    const std::span<T>& sensor_input,
    const std::span<T>& setpoint,
    const std::span<Gain<T>>& output_buffer,
    const Gain<T>& gain,
    const Errors<T>& last_errors
  ) {
  for (auto i = 0; i < sensor_input.size(); i++) {
    auto [output, errors] = process_sample(sensor_input[i], setpoint[i], gain, last_errors);
    last_errors = errors;
    output_buffer[i] = output;
  }
  return last_errors;
}

} // namespace pid
