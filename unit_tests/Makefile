ROCKETLIB_PATH := ..

CFLAGS := \
	-std=c99 \
	-Wall \
	-Wextra \
	-pedantic \
	-MMD \
	-O2 \
	-I$(ROCKETLIB_PATH)/include \
	-D W_DEBUG

SRCS := \
	unit_test.c \
	low_pass_filter_test.c \
	$(ROCKETLIB_PATH)/common/low_pass_filter.c \
	$(ROCKETLIB_PATH)/common/crc8.c

OBJS = $(SRCS:.c=.o)
DEPS = $(SRCS:.c=.d)

unit_test: $(OBJS)
	$(CC) $^ -o $@

-include $(DEPS)

.PHONY: clean
clean:
	rm -f $(OBJS) $(DEPS) *.o unit_test
