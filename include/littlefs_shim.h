#ifndef SD_LFS_SHIM_H
#define SD_LFS_SHIM_H

#include "lfs.h"
#include "stm32h7xx_hal.h"  // or appropriate HAL header for your MCU

#ifdef __cplusplus
extern "C" {
#endif

// External SD handle used by the shim
extern SD_HandleTypeDef* lfs_shim_hsd;

extern uint32_t first_sector_offset;

// LittleFS-compatible SD card interface functions
int sd_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
            void *buffer, lfs_size_t size);

int sd_write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
             const void *buffer, lfs_size_t size);

int sd_erase(const struct lfs_config *c, lfs_block_t block);

int sd_sync(const struct lfs_config *c);

#ifdef __cplusplus
}
#endif

#endif // SD_LFS_SHIM_H
