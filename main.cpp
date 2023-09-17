// Generate prime numbers using Multi-Core CPUs
// Algorithm: Sieve of Eratosthenes
// Article: https://medium.com/@mckev/generate-prime-numbers-using-multi-cpu-cores-e2d55c478e24


#include <atomic>
#include <cmath>
#include <cstring>
#include <iostream>
#include <vector>
#include <cilk/cilk.h>


const uint64_t max = 80000000000;
const uint64_t sieve_buffer_size = max / (8 * sizeof(std::atomic_uint64_t)) + 1;
std::vector<std::atomic_uint64_t> sieve_buffer(sieve_buffer_size);


bool is_prime(uint64_t num) {
    uint64_t byte_index = num / (8 * sizeof(std::atomic_uint64_t));
    uint8_t bit_index = num % (8 * sizeof(std::atomic_uint64_t));
    return !(sieve_buffer[byte_index] & ((std::atomic_uint64_t) 1 << bit_index));
}

void mark_as_composite(uint64_t num) {
    uint64_t byte_index = num / (8 * sizeof(std::atomic_uint64_t));
    uint8_t bit_index = num % (8 * sizeof(std::atomic_uint64_t));
    sieve_buffer[byte_index] |= (std::atomic_uint64_t) 1 << bit_index;
}

void sieve() {
    mark_as_composite(0);
    mark_as_composite(1);
    uint64_t sqrt_max = std::sqrt(max);
    for (uint64_t i = 2; i <= sqrt_max; i++) {
        if (is_prime(i)) {
            cilk_for (uint64_t j = 2 * i; j <= max; j += i) {
                mark_as_composite(j);
            }
        }
    }
}

uint64_t count_primes() {
    uint64_t result = 0;
    for (uint64_t i = 0; i <= max; i++) {
        if (is_prime(i)) {
            result++;
        }
    }
    return result;
}


int main() {
    std::cout << "Sieving primes under " << max << " on CPU" << std::endl;
    sieve();
    std::cout << "Number of primes: " << count_primes() << std::endl;
    return 0;
}
