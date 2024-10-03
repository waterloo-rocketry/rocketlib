#ifndef ROCKETLIB_UNIT_TEST_H
#define ROCKETLIB_UNIT_TEST_H

void rocketlib_assert_fail(const char *file, int line, const char *statement);

#define rocketlib_assert(statement)                                                                \
    if (!(statement)) {                                                                            \
        rocketlib_assert_fail(__FILE__, __LINE__, #statement);                                     \
    }

#endif
