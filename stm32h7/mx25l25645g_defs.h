#ifndef ROCKETLIB_MX25l25645G_DEFS_H
#define ROCKETLIB_MX25l25645G_DEFS_H

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

typedef enum {
    MX_CMD_READ = 0x03,
    MX_CMD_FAST_READ = 0x0B,
    MX_CMD_2READ = 0xBB,
    MX_CMD_DREAD = 0x3B,
    MX_CMD_4READ = 0xEB,
    MX_CMD_QREAD = 0x6B,
    MX_CMD_4DTRD = 0xED,
    MX_CMD_PP = 0x02,
    MX_CMD_4PP = 0x38,
    MX_CMD_SE = 0x20,
    MX_CMD_BE32K = 0x52,
    MX_CMD_BE = 0xD8,
    MX_CMD_CE = 0xC7
} command_t;

#endif /* ROCKETLIB_MX25l25645G_DEFS_H */