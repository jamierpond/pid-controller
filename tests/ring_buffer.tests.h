#pragma once
#include "../ring_buffer.h"

// Tests ======================================================================

static_assert(modulo<8>(0) == 0);
static_assert(modulo<8>(1) == 1);
static_assert(modulo<8>(6) == 6);
static_assert(modulo<8>(8) == 0);
static_assert(modulo<8>(9) == 1);
static_assert(modulo<8>(-1) == 7);
static_assert(modulo<8>(-9) == 7);

static_assert(is_power_of_two(16));
static_assert(is_power_of_two(32));
static_assert(!is_power_of_two(31));

static_assert([] {
  for (auto i = 0; i < 31; i++) {
    if (!is_power_of_two(1 << i)) {
      return false;
    }
  }
  return true;
}());

static_assert([] {
  RingBuffer<int, int, 8> rb{};
  { /* buffer is empty */
    auto expected_buffer = std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0};
    if (expected_buffer != rb.buffer) {
      return false;
    }
  }

  { /* write a one */
    auto expected_buffer = std::array<int, 8>{1, 0, 0, 0, 0, 0, 0, 0};
    rb.write(1);
    if (expected_buffer != rb.buffer) {
      return false;
    }
  }

  { /* write a two */
    auto expected_buffer = std::array<int, 8>{1, 2, 0, 0, 0, 0, 0, 0};
    rb.write(2);
    if (expected_buffer != rb.buffer) {
      return false;
    }
  }

  { /* set the delay to 2 and read some values */
    rb.delay_samples = 2;
    if (rb.read() != 0) { return false; }
    if (rb.read() != 0) { return false; }
    if (rb.read() != 1) { return false; }
    if (rb.read() != 2) { return false; }

    rb.write(3);
    rb.write(4);
    rb.delay_samples = 4;

    if (rb.read() != 0) { return false; }
    if (rb.read() != 0) { return false; }
    if (rb.read() != 3) { return false; }
    if (rb.read() != 4) { return false; }
  }

  return true;
}());


