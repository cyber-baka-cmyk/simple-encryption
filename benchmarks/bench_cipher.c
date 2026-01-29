#include <stdio.h>
#include <time.h>
#include <string.h>
#include "cipher.h"

static double now_sec(void) {
    struct timespec ts;
    clock_gettime(CLOCKS_PER_SEC, &ts);
    return ts.tv_sec + ts.tv_nsec / 1e9;
}

int main(void) {
    unsigned char key[64];

    unsigned char buf[64];
    memset(buf, 0x42, sizeof(buf));

    const int iterations = 100000;
    double start = now_sec();

    for (int i = 0; i < iterations; i++) {
        for (size_t j = 0; j < sizeof(buf); j++) {
            buf[j] ^= key[j];
        }
    }

    double end = now_sec();
    double elapsed = end - start;
    double bytes = (double)iterations * sizeof(buf);
    double mbps = (bytes / (1024.0 * 1024.0)) / elapsed;

    printf("XOR throughput: %.2f MiB/s (%.2f s for %.0f bytes)\n",
           mbps, elapsed, bytes);
           
    return 0;
}
