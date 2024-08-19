#pragma once
#include <array>
#include <cstdint>
#include <type_traits>

template <typename T>
constexpr bool is_power_of_two(T value) {
  static_assert(std::is_integral_v<T>, "T must be an integral type");
  return value && !(value & (value - 1));
}

template <auto Divisor>
constexpr auto modulo(auto dividend) {
  return dividend & (Divisor - 1);
}

template <typename T, auto Size, typename CountType = std::int32_t>
struct RingBuffer {
  static_assert(is_power_of_two(Size), "Size must be a power of two");
  static_assert(std::is_integral_v<CountType>, "CountType must be an integral type");
  static_assert(std::is_signed_v<CountType>, "CountType must be a signed type");

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

#ifdef STATIC_TESTS
#include "tests/ring_buffer.tests.h"
#endif

