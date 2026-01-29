#include <gtest/gtest.h>
#include <chrono>
#include <climits>
#include "cipher.h"

static long measure_shuffle_ns(uint32_t seed) {
    uint32_t state[4][4];

    for (int i = 0; i < 16; ++i) {
        reinterpret_cast<uint32_t*>(state)[i] = seed + i;
    }

    auto start = std::chrono::high_resolution_clock::now();
    shuffle(state);
    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration_cast<std::chrono::nanoseconds>(end - start).count();
}

TEST(SideChannelTest, TimingRoughlyStableAcrossInputs) {
    const int samples = 200;
    long min_t = LONG_MAX;
    long max_t = 0;

    for (int i = 0; i < samples; ++i) {
        long t = measure_shuffle_ns((uint32_t)(i * 1234567u));
        if (t < min_t) min_t = t;
        if (t > max_t) max_t = t;
    }

    ASSERT_GT(min_t, 0);
    double ratio = (double)max_t / (double)min_t;

    EXPECT_LT(ratio, 10.0); // relaxed threshold
}
