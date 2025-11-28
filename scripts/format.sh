#!/bin/sh
set -x
${CLANG_FORMAT:-clang-format} -i common/*.c include/*.h pic18f26k83/*.c stm32h7/*.c tests/*.cpp tests/*.hpp
