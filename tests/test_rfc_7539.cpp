#include <gtest/gtest.h>
#include "cipher.h"

TEST(StructureTest, ShuffleChangesNonZeroState) {
    uint32_t state[4][4];
    uint32_t original[4][4];

    for (int i = 0; i < 16; ++i) {
        reinterpret_cast<uint32_t*>(state)[i] = i + 1; // non-zero
    }

    memcpy(original, state, sizeof(state));

    shuffle(state);

    bool identical = true;
    for (int i = 0; i < 16; ++i) {
        if (reinterpret_cast<uint32_t*>(state)[i] !=
            reinterpret_cast<uint32_t*>(original)[i]) {
            identical = false;
            break;
        }
    }

    EXPECT_FALSE(identical);
}


TEST(StructureTest, DifferentInputsGiveDifferentOutputs) {
    uint32_t s1[4][4];
    uint32_t s2[4][4];

    for (int i = 0; i < 16; ++i) {
        reinterpret_cast<uint32_t*>(s1)[i] = (uint32_t)i;
        reinterpret_cast<uint32_t*>(s2)[i] = (uint32_t)(i + 1000);
    }

    shuffle(s1);
    shuffle(s2);

    int equal_words = 0;
    for (int i = 0; i < 16; ++i) {
        if (reinterpret_cast<uint32_t*>(s1)[i] ==
            reinterpret_cast<uint32_t*>(s2)[i]) {
            ++equal_words;
        }
    }

    EXPECT_LT(equal_words, 4); // heuristic: most words should differ
}
