#include <stdio.h>
#include <stdlib.h>

#include "common.h"
#include "low_pass_filter_test.h"

int main() {
	test_low_pass_filter_init();
	test_update_low_pass();
	printf("\nAll tests passed!\n");
	return 0;
}

void w_assert_fail(const char *file, int line, const char *statement) {
	printf("Assert Fail %s:%d %s\n", file, line, statement);
	exit(1);
}
