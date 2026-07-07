#include <stdio.h>
#include <limits.h> // Required for LLONG_MIN and LLONG_MAX

int main() {
    // 1. Check size in bytes
    printf("Size of long long int: %zu bytes\n", sizeof(long long int));
    printf("Size in bits: %zu bits\n\n", sizeof(long long int) * 8);

    // 2. Check value ranges
    printf("Minimum value: %lld\n", LLONG_MIN);
    printf("Maximum value: %lld\n", LLONG_MAX);
    printf("Maximum value (unsigned): %llu\n", ULLONG_MAX);

    return 0;
}
