ROCKETLIB_SRC := ../common/low_pass_filter.c ../common/crc8.c
UNIT_TEST_SRC := unit_test.c low_pass_filter_test.c

.PHONY: run_test
run_test: unit_test
	./unit_test

unit_test:
	gcc -Og -g -I ../include -D W_DEBUG $(ROCKETLIB_SRC) $(UNIT_TEST_SRC) -o $@

.PHONY: clean
clean:
	rm -rf unit_test
