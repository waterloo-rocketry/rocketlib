#include "common.h"
#include "low_pass_filter.h"
#include <math.h>
#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

#define EPSILON 0.0001

void test_low_pass_filter_init() {
    double alpha;
    // Test with valid input
    w_assert(low_pass_filter_init(&alpha, 1.0) == W_SUCCESS);
    // Test with invalid input (NULL pointer)
    w_assert(low_pass_filter_init(NULL, 1.0) == W_INVALID_PARAM);
    // Test with invalid input (negative response time)
    w_assert(low_pass_filter_init(&alpha, -1.0) == W_INVALID_PARAM);
    // Test with invalid input (zero response time)
    w_assert(low_pass_filter_init(&alpha, 0.0) == W_INVALID_PARAM);
}

void test_update_low_pass() {
    double alpha = 0.5;
    double low_pass_value = 100.0;
    uint16_t new_value = 200;
    w_status_t result;

    // Test with valid inputs
    result = update_low_pass(&alpha, new_value, &low_pass_value);
    w_assert(result == W_SUCCESS);
    w_assert(fabs(low_pass_value - 150.0) < EPSILON);

    // Test with NULL alpha
    result = update_low_pass(NULL, new_value, &low_pass_value);
    w_assert(result == W_INVALID_PARAM);

    // Test with NULL low_pass_value
    result = update_low_pass(&alpha, new_value, NULL);
    w_assert(result == W_INVALID_PARAM);
}

int main() {
    test_low_pass_filter_init();
    test_update_low_pass();
    printf("All tests passed!\n");
    return 0;
}
