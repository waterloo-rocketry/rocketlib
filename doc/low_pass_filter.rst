Low Pass Filter
===============

Macros
======

.. c:macro:: SAMPLE_FREQ(time_diff_ms)

   Calculates the sample frequency based on the time difference in milliseconds.

   :param time_diff_ms: Time difference in milliseconds between samples
   :return: The sample frequency in Hertz (Hz)

.. code-block:: c

    #define SAMPLE_FREQ(time_diff_ms) (1000.0 / (time_diff_ms))

.. c:macro:: LOW_PASS_ALPHA(TR, time_diff_ms)

   Calculates the alpha value for the low-pass filter based on the response time.

   :param TR: Response time constant
   :param time_diff_ms: Time difference in milliseconds between samples
   :return: The alpha value used for the low-pass filter calculation

.. code-block:: c

    #define LOW_PASS_ALPHA(TR, time_diff_ms) \
        ((SAMPLE_FREQ(time_diff_ms) * TR / 5.0) / (1 + SAMPLE_FREQ(time_diff_ms) * TR / 5.0))

Functions
=========

.. c:function:: w_status_t low_pass_filter_init(double *alpha, double response_time)

   Initializes the low-pass filter by calculating and storing the alpha value based on the given response time.

   :param double *alpha: Pointer to a double where the calculated alpha value will be stored
   :param double response_time: Desired response time for the filter in seconds
   :return: `W_SUCCESS` if initialization is successful, `W_INVALID_PARAM` otherwise
   :rtype: w_status_t

.. c:function:: bool update_low_pass(double *alpha, uint16_t new_value, double *low_pass_value)

   Updates the low-pass filter with a new input value and returns the success status.

   :param double *alpha: Pointer to the alpha value used in the filter calculation
   :param uint16_t new_value: New input value to be filtered
   :param double *low_pass_value: Pointer to the current filtered value, which will be updated
   :return: `true` if the update is successful, `false` otherwise
   :rtype: bool

Usage Example
=============

.. code-block:: c

    #include "low_pass_filter.h"
    #include <stdio.h>

    int main() {
        double alpha;
        double response_time = 1.0; // 1 second response time
        w_status_t status;

        // Initialize the low-pass filter
        status = low_pass_filter_init(&alpha, response_time);
        if (status != W_SUCCESS) {
            printf("Failed to initialize low-pass filter.\n");
            return -1;
        }

        double filtered_value = 100.0;
        uint16_t new_input = 200;

        // Update the low-pass filter with a new input value
        bool result = update_low_pass(&alpha, new_input, &filtered_value);
        if (!result) {
            printf("Update failed due to invalid parameters.\n");
        } else {
            printf("Filtered Value: %.2f\n", filtered_value);
        }

        return 0;
    }

**Output:**
Filtered Value: 150.00


Notes
=====
- Ensure that the `alpha` value is properly initialized before using the `update_low_pass` function.
- The filter effectively smooths out rapid changes in input values, making it suitable for applications where signal stability is crucial.

References
==========
- `Low-Pass Filter <https://en.wikipedia.org/wiki/Low-pass_filter>`_
- `Embedded Systems Signal Processing <https://www.embedded.com/signal-processing-in-embedded-systems/>`_