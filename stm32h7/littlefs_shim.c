#include "common.h"
#include "lfs.h"
#include "stm32h7xx_hal.h"

#define SD_RW_TIMEOUT_MS 50

SD_HandleTypeDef *lfsshim_hsd;
uint32_t lfsshim_first_block_offset = 0;

static int lfsshim_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer,
						lfs_size_t size) {
	uint32_t block_addr = block + lfsshim_first_block_offset;

	w_assert((size % c->block_size) == 0);
	w_assert(off == 0);

	uint32_t num_blocks = size / c->block_size;

	HAL_StatusTypeDef hal =
		HAL_SD_ReadBlocks(lfsshim_hsd, (uint8_t *)buffer, block_addr, num_blocks, SD_RW_TIMEOUT_MS);
	if (hal != HAL_OK) {
		return -1; // LFS_ERR_IO
	}

	// Wait for card to be ready (polling)
	uint32_t start = HAL_GetTick();
	while (HAL_SD_GetCardState(lfsshim_hsd) != HAL_SD_CARD_TRANSFER) {
		if ((HAL_GetTick() - start) > SD_RW_TIMEOUT_MS) {
			return -1; // timeout -> LFS_ERR_IO
		}
	}

	return 0; // success
}

static int lfsshim_write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off,
						 const void *buffer, lfs_size_t size) {
	uint32_t block_addr = block + lfsshim_first_block_offset;

	w_assert((size % c->block_size) == 0);
	w_assert(off == 0);

	uint32_t num_blocks = size / c->block_size;

	HAL_StatusTypeDef hal = HAL_SD_WriteBlocks(
		lfsshim_hsd, (uint8_t *)buffer, block_addr, num_blocks, SD_RW_TIMEOUT_MS);
	if (hal != HAL_OK) {
		return -1; // LFS_ERR_IO
	}

	// Wait for card to be ready (polling)
	uint32_t start = HAL_GetTick();
	while (HAL_SD_GetCardState(lfsshim_hsd) != HAL_SD_CARD_TRANSFER) {
		if ((HAL_GetTick() - start) > SD_RW_TIMEOUT_MS) {
			return -1; // timeout -> LFS_ERR_IO
		}
	}

	return 0; // success
}

static int lfsshim_erase(const struct lfs_config *c, lfs_block_t block) {
	return 0; // SD does not require explicit erase
}

static int lfsshim_sync(const struct lfs_config *c) {
	return 0;
}

// configuration of the filesystem is provided by this struct
const struct lfs_config cfg = {
	// block device operations
	.read = lfsshim_read,
	.prog = lfsshim_write,
	.erase = lfsshim_erase,
	.sync = lfsshim_sync,

	// block device configuration
	.read_size = 512,
	.prog_size = 512,
	.block_size = 512,
	.block_count = 0,
	.block_cycles = -1,
	.cache_size = 512,
	.lookahead_size = 512,
	.compact_thresh = -1,
	.name_max = 0,
	.file_max = 0,
	.attr_max = 0,
	.metadata_max = 0,
	.inline_max = -1};

int lfsshim_mount(lfs_t *lfs) {
	memset(lfs, 0, sizeof(lfs_t));

	if (lfs_mount(lfs, &cfg) != 0) {
		return W_IO_ERROR;
	}
	return 0; // success
}
