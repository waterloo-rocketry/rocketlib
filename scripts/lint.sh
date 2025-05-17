#!/bin/sh
set -x
clang-tidy common/*.c include/*.h unit_tests/*.c unit_tests/*.h
