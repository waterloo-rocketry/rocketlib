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
int lfsshim_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer,
				 lfs_size_t size);

int lfsshim_write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer,
				  lfs_size_t size);

int lfsshim_erase(const struct lfs_config *c, lfs_block_t block);

int lfsshim_sync(const struct lfs_config *c);

#ifdef __cplusplus
}
#endif

#endif // ROCKETLIB_LITTLEFS_SHIM_H
