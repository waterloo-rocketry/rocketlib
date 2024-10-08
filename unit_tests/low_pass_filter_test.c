#include <stdio.h>

#include "low_pass_filter.h"
#include "unit_test.h"

int main() {
    status_t status = low_pass_filter_init();

    rocketlib_assert(status == W_SUCCESS);

    return 0;
}
