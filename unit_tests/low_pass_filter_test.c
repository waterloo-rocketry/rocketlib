#include <math.h>
#include <stdio.h>

#include "common.h"
#include "low_pass_filter.h"

#define EPSILON 0.0001

void test_low_pass_filter_init() {
    double alpha;

    // Test with valid input
    rocketlib_assert(low_pass_filter_init(&alpha, 1.0) == W_SUCCESS);

    // Test with invalid input (NULL pointer)
    rocketlib_assert(low_pass_filter_init(NULL, 1.0) == W_FAILURE);

    // Test with invalid input (negative response time)
    rocketlib_assert(low_pass_filter_init(&alpha, -1.0) == W_FAILURE);

    // Test with invalid input (zero response time)
    rocketlib_assert(low_pass_filter_init(&alpha, 0.0) == W_FAILURE);
}

void test_update_low_pass() {
    double alpha = 0.5;
    double low_pass_value = 100.0;
    uint16_t new_value = 200;

    // Test with valid inputs
    uint16_t result = update_low_pass(&alpha, new_value, &low_pass_value);
    rocketlib_assert(fabs(result - 150.0) < EPSILON);

    // Test with NULL alpha
    rocketlib_assert(update_low_pass(NULL, new_value, &low_pass_value) == 0);

    // Test with NULL low_pass_value
    rocketlib_assert(update_low_pass(&alpha, new_value, NULL) == 0);
}

#define EPSILON 0.0001

void test_low_pass_filter_init() {
    double alpha;

    // Test with valid input
    rocketlib_assert(low_pass_filter_init(&alpha, 1.0) == W_SUCCESS);

    // Test with invalid input (NULL pointer)
    rocketlib_assert(low_pass_filter_init(NULL, 1.0) == W_FAILURE);

    // Test with invalid input (negative response time)
    rocketlib_assert(low_pass_filter_init(&alpha, -1.0) == W_FAILURE);

    // Test with invalid input (zero response time)
    rocketlib_assert(low_pass_filter_init(&alpha, 0.0) == W_FAILURE);
}

void test_update_low_pass() {
    double alpha = 0.5;
    double low_pass_value = 100.0;
    uint16_t new_value = 200;

    // Test with valid inputs
    uint16_t result = update_low_pass(&alpha, new_value, &low_pass_value);
    rocketlib_assert(fabs(result - 150.0) < EPSILON);

    // Test with NULL alpha
    rocketlib_assert(update_low_pass(NULL, new_value, &low_pass_value) == 0);

    // Test with NULL low_pass_value
    rocketlib_assert(update_low_pass(&alpha, new_value, NULL) == 0);
}

int main() {
    test_low_pass_filter_init();
    test_update_low_pass();

    printf("All tests passed!\n");
    return 0;
}
