CFLAGS := \
	-std=c99 \
	-Wall \
	-Wextra \
	-pedantic \
	-MMD \
	-DUNIT_TEST \
	-Iinclude

CXXFLAGS := \
	-std=c++20 \
	-Wall \
	-Wextra \
	-pedantic \
	-MMD \
	-DUNIT_TEST \
	-Iinclude \
	-Irockettest

ifeq ($(COVERAGE), 1)

CFLAGS += \
	-fprofile-arcs \
	-ftest-coverage

CXXFLAGS += \
	-fprofile-arcs \
	-ftest-coverage

LDFLAGS += -Wl,-lgcov

endif

ifeq ($(DEBUG), 1)

CFLAGS += -Og -g

CXXFLAGS += -Og -g

else

CFLAGS += -O2

CXXFLAGS += -O2

endif

ROCKETLIB_SRCS := \
	common/crc8.c \
	common/low_pass_filter.c \
	common/mbr.c

TEST_SRCS := \
	rockettest/rockettest.cpp \
	tests/test_crc8.cpp \
	tests/test_low_pass_filter.cpp \
	tests/test_mbr.cpp

ROCKETLIB_OBJS = $(ROCKETLIB_SRCS:.c=.o)
ROCKETLIB_DEPS = $(ROCKETLIB_SRCS:.c=.d)

TEST_OBJS = $(TEST_SRCS:.cpp=.o)
TEST_DEPS = $(TEST_SRCS:.cpp=.d)

unit_test: $(ROCKETLIB_OBJS) $(TEST_OBJS)
	$(CXX) $^ $(LDFLAGS) -o $@

.PHONY: run_test
run_test: unit_test
	./unit_test

ifeq ($(COVERAGE), 1)

coverage.info: run_test
	lcov --capture --branch-coverage --no-external --directory ../common --directory . -o $@

coverage-filtered.info: coverage.info
	lcov --remove $^ "*/tests/*" -o $@ --branch-coverage

html: coverage-filtered.info
	genhtml $^ --flat --branch-coverage --output-directory $@

endif

-include $(ROCKETLIB_DEPS)
-include $(TEST_DEPS)

.PHONY: clean
clean:
	rm -rf $(ROCKETLIB_OBJS) $(ROCKETLIB_DEPS) $(TEST_OBJS) $(TEST_DEPS) *.o ../common/*.gcda ../common/*.gcno *.gcda *.gcno unit_test *.info html
