#pragma once
#include "static_test_utils.hpp"
#include "../ring_buffer.h"

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

STATIC_TEST([] {
  EXPECT(modulo<8>(0) == 0);
  return true;
});

static_assert([] {
  RingBuffer<int, 8> rb{};
  { /* buffer is empty */
    auto expected_buffer = std::array<int, 8>{0, 0, 0, 0, 0, 0, 0, 0};
    EXPECT(rb.buffer == expected_buffer);
  }

  { /* write a one */
    auto expected_buffer = std::array<int, 8>{1, 0, 0, 0, 0, 0, 0, 0};
    rb.write(1);
    EXPECT(rb.buffer == expected_buffer);
  }

  { /* write a two */
    auto expected_buffer = std::array<int, 8>{1, 2, 0, 0, 0, 0, 0, 0};
    rb.write(2);
    EXPECT(rb.buffer == expected_buffer);
  }

  { /* set the delay to 2 and read some values */
    rb.delay_samples = 2;
    EXPECT(rb.read() == 0);
    EXPECT(rb.read() == 0);
    EXPECT(rb.read() == 1);
    EXPECT(rb.read() == 2);

    rb.write(3);
    rb.write(4);
    rb.delay_samples = 4;

    EXPECT(rb.read() == 0);
    EXPECT(rb.read() == 0);
    EXPECT(rb.read() == 3);
    EXPECT(rb.read() == 4);
  }

  return true;
}());


