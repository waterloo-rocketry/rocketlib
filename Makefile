COMMON_C_SRCS := \
	common/crc8.c \
	common/low_pass_filter.c \
	common/mbr.c

TEST_SRCS := \
	tests/test_crc8.cpp \
	tests/test_low_pass_filter.cpp \
	tests/test_mbr.cpp

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

CPP_SRCS := \
	rockettest/rockettest.cpp \
	$(TEST_SRCS)

ifeq ($(COVERAGE), 1)
BUILD_DIR := build-cov
else
BUILD_DIR := build
endif

COMMON_C_OBJS = $(patsubst %.c,$(BUILD_DIR)/%.o,$(COMMON_C_SRCS))
COMMON_C_DEPS = $(COMMON_C_SRCS:.c=.d)

CPP_OBJS = $(patsubst %.cpp,$(BUILD_DIR)/%.o,$(CPP_SRCS))
CPP_DEPS = $(CPP_SRCS:.cpp=.d)

$(BUILD_DIR)/%.o: %.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/%.o: %.cpp
	@mkdir -p $(dir $@)
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(BUILD_DIR)/unit_test: $(COMMON_C_OBJS) $(CPP_OBJS)
	@mkdir -p $(dir $@)
	$(CXX) $^ $(LDFLAGS) -o $@

.PHONY: run_test
run_test: $(BUILD_DIR)/unit_test
	./$(BUILD_DIR)/unit_test

ifeq ($(COVERAGE), 1)

coverage.info: run_test
	lcov --capture --branch-coverage --mcdc-coverage --no-external --directory ../common --directory . -o $@

coverage-filtered.info: coverage.info
	lcov --remove $^ "*/tests/*" -o $@ --branch-coverage --mcdc-coverage

html: coverage-filtered.info
	genhtml $^ --flat --branch-coverage --mcdc-coverage --output-directory $@

endif

-include $(COMMON_C_DEPS)
-include $(CPP_DEPS)

.PHONY: clean
clean:
	rm -rf $(COMMON_C_OBJS) $(COMMON_C_DEPS) $(CPP_OBJS) $(CPP_DEPS) *.o ../common/*.gcda ../common/*.gcno *.gcda *.gcno unit_test *.info html
