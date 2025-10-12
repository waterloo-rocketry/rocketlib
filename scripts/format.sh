#!/bin/sh
set -x
${CLANG_FORMAT:-clang-format} -i common/*.c include/*.h pic18f26k83/*.c unit_tests/*.c unit_tests/*.h
