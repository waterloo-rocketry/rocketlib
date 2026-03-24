COMMON_C_SRCS := \
	common/crc8.c \
	common/low_pass_filter.c \
	common/mbr.c

COMMON_C_HEADERS := \
	include/common.h \
	include/crc8.h \
	include/electrical.h \
	include/low_pass_filter.h \
	include/mbr.h

PIC18_C_SRCS := \
	pic18f26k83/i2c.c \
	pic18f26k83/pwm.c \
	pic18f26k83/timer.c

PIC18_C_HEADERS := \
	include/pic18f26k83/i2c.h \
	include/pic18f26k83/pwm.h \
	include/timer.h

STM32H7_C_SRCS := \
	stm32h7/littlefs_sd_shim.c

STM32H7_C_HEADERS := \
	include/stm32/littlefs_sd_shim.h

INCLUDE_PATHS := \
	include

TEST_SRCS := \
	tests/test_crc8.cpp \
	tests/test_low_pass_filter.cpp \
	tests/test_mbr.cpp

ROCKETLIB_SUBMODULE_PATH := .

include flows/firmware-library.mk
