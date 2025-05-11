#!/bin/sh
set -x
${CLANG_FORMAT:-clang-format} -i--style=file:rocketlib/.clang-format *.c *.h
