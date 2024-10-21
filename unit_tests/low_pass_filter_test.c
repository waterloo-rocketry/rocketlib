#include <stdio.h>

#include "common.h"
#include "low_pass_filter.h"

int main() {
    w_status_t status = low_pass_filter_init();

    w_assert(status == W_SUCCESS);

    return 0;
}
