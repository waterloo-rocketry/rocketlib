#ifndef ROCKETLIB_LITTLEFS_SHIM_H
#define ROCKETLIB_LITTLEFS_SHIM_H

#include "lfs.h"
#include "stm32h7xx_hal_sd.h"

#ifdef __cplusplus
extern "C" {
#endif

// LittleFS-compatible SD card interface functions
int lfsshim_mount(lfs_t *lfs, SD_HandleTypeDef *hsd, uint32_t first_block_offset);
int lfsshim_mount_mbr(lfs_t *lfs, SD_HandleTypeDef *hsd);

#ifdef __cplusplus
}
#endif

#endif // ROCKETLIB_LITTLEFS_SHIM_H
