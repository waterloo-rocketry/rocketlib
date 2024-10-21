#include "low_pass_filter.h"
#include <stddef.h>

#include <xc.h>

// Initialize the low-pass filter
w_status_t low_pass_filter_init(double *alpha, double response_time) {
    if (alpha == NULL || response_time <= 0) {
        return W_INVALID_PARAM; // Return specific error for invalid parameters
    }
    // Calculate and store the alpha value using the LOW_PASS_ALPHA macro
    *alpha = LOW_PASS_ALPHA(response_time, response_time);
    return W_SUCCESS; // Return success after initialization
}
// Update the low-pass filter with a new value
bool update_low_pass(double *alpha, uint16_t new_value, double *low_pass_value) {
    // Check if alpha or low_pass_value pointers are NULL
    if (alpha == NULL || low_pass_value == NULL) {
        return false; // Return false if invalid pointers
    }

    // Update the low-pass filter value using the alpha parameter
    *low_pass_value = (*alpha * (*low_pass_value)) + ((1.0 - *alpha) * new_value);
    return true; // Return true after successful update
}