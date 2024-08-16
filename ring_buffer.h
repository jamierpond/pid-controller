#pragma once
#include <array>
#include <type_traits>

template <typename T> constexpr bool is_power_of_two(T value) {
  static_assert(std::is_integral_v<T>, "T must be an integral type");
  return value && !(value & (value - 1));
}

template <auto Divisor>
constexpr auto modulo(auto dividend) {
  return dividend & (Divisor - 1);
}

template <typename T, typename CountType, CountType Size> struct RingBuffer {
  static_assert(is_power_of_two(Size), "Size must be a power of two");

  constexpr void write(T value) {
    buffer[write_pointer] = value;
    write_pointer = modulo<Size>(write_pointer + 1);
  }

  constexpr T read() {
    auto real_read_pointer = modulo<Size>(read_pointer - delay_samples);
    auto value = buffer[real_read_pointer];
    buffer[real_read_pointer] = T{};
    read_pointer = modulo<Size>(read_pointer + 1);
    return value;
  }

  CountType delay_samples = 0;
  CountType write_pointer = 0;
  CountType read_pointer = 0;
  std::array<T, Size> buffer{};
};


