#!/bin/sh
set -x
find ../.. -path ../../canlib -prune -o -path ../../mcc_generated_files -prune -o -type f \( -iname \*.c -o -iname \*.h \) | xargs clang-format -i
