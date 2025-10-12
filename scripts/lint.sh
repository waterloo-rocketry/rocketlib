#!/bin/sh
set -x
${CLANG_TIDY:-clang-tidy} common/*.c include/*.h --warnings-as-errors="*" --checks="clang-*,misc-*" --extra-arg-before="-std=c99" --extra-arg-before="-pedantic" --extra-arg-before="-Iinclude"
