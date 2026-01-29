#include <gtest/gtest.h>
#include <cstring>
#include "cipher.h"

static int count_diff_bits(const uint32_t a[4][4], const uint32_t b[4][4]) {
    int diff = 0;
    for (int i = 0; i < 16; ++i) {
        uint32_t x = reinterpret_cast<const uint32_t*>(a)[i];
        uint32_t y = reinterpret_cast<const uint32_t*>(b)[i];
        diff += __builtin_popcount(x ^ y);
    }
    return diff;
}

TEST(AvalancheTest, SingleBitFlipCausesManyChanges) {
    uint32_t s1[4][4];
    uint32_t s2[4][4];

    // deterministic-ish input
    for (int i = 0; i < 4; ++i) {
        for (int j = 0; j < 4; ++j) {
            s1[i][j] = (uint32_t)(i * 4 + j);
        }
    }
    std::memcpy(s2, s1, sizeof(s1));

    // flip one bit
    s2[0][0] ^= 1u;

    shuffle(s1);
    shuffle(s2);

    int diff_bits = count_diff_bits(s1, s2);

    // 16 words * 32 bits = 512 bits total
    // require at least half of them to differ (toy threshold)
    EXPECT_GT(diff_bits, 256);
}
