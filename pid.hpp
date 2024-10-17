#pragma once

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
    const T sensor_input,
    const T setpoint,
    const Gain<T> gain,
    const Errors<T> e,
    const T cumulative_error_backoff_coeff = T{1.0}
) {
  auto current_error = setpoint - sensor_input;
  auto cumulative_error = e.cumulative_error + current_error;

  auto proportional = gain.p * current_error;
  auto integral = gain.i * cumulative_error * cumulative_error_backoff_coeff;
  auto derivative = gain.d * (current_error - e.previous_error);

  return {
    .output = proportional + integral + derivative,
    .errors = {
      .previous_error = current_error,
      .cumulative_error = cumulative_error
    }
  };
}

template <typename T>
T partial_derivative_wrt_p(
    const T sensor_input,
    const T setpoint,
    const Errors<T> e
) {
  auto current_error = setpoint - sensor_input;
  auto cumulative_error = e.cumulative_error + current_error;
  return current_error;
}

} // namespace pid
