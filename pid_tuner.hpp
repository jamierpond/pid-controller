#pragma once

namespace pid {

// Loss function (Mean Squared Error)
template <typename T> T mse(const T target, const T output) {
  auto error = target - output;
  return error * error; // MSE
}


} // namespace pid
