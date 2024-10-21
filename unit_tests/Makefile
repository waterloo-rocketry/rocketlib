ROCKETLIB_SRC := ../common/low_pass_filter.c
UNIT_TEST_SRC := unit_test.c low_pass_filter_test.c

run_all_test: low_pass_filter_test
	./low_pass_filter_test

low_pass_filter_test:
	gcc -Og -g -I ../include -D W_DEBUG $(ROCKETLIB_SRC) $(UNIT_TEST_SRC) -o $@

.PHONY: clean
clean:
	rm -rf low_pass_filter_test
