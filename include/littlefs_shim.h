#ifndef ROCKETLIB_LITTLEFS_SHIM_H
#define ROCKETLIB_LITTLEFS_SHIM_H

#include "lfs.h"
#include "stm32h7xx_hal_sd.h"

#ifdef __cplusplus
extern "C" {
#endif

// External SD handle used by the shim
extern SD_HandleTypeDef *lfsshim_hsd;

extern uint32_t lfsshim_first_block_offset;

// LittleFS-compatible SD card interface functions
int lfsshim_mount(lfs_t *lfs);

#ifdef __cplusplus
}
#endif

#endif // ROCKETLIB_LITTLEFS_SHIM_H
