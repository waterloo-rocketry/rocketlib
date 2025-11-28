#include "stm32h7xx_hal.h"
#include "lfs.h"
#include "common.h"

#define SD_RW_TIMEOUT_MS 50

SD_HandleTypeDef* lfsshim_hsd;
uint32_t lfsshim_first_block_offset = 0;

int lfsshim_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer,
			lfs_size_t size) {
	uint32_t timeout_ms = SD_RW_TIMEOUT_MS;
	uint32_t block_addr = block + lfsshim_first_block_offset;
	//uint32_t num_blocks = (size + c->block_size - 1) / c->block_size;
	uint32_t num_blocks = size / c->block_size;
	w_assert((size % c->block_size) == 0);
	w_assert(off == 0);

	HAL_StatusTypeDef hal =
		HAL_SD_ReadBlocks(lfsshim_hsd, (uint8_t *)buffer, block_addr, num_blocks, timeout_ms);
	if (hal != HAL_OK) {
		return -1; // LFS_ERR_IO
	}

	// Wait for card to be ready (polling)
	uint32_t start = HAL_GetTick();
	while (HAL_SD_GetCardState(lfsshim_hsd) != HAL_SD_CARD_TRANSFER) {
		if ((HAL_GetTick() - start) > timeout_ms) {
			return -1; // timeout -> LFS_ERR_IO
		}
	}

	return 0; // success
}

int lfsshim_write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer,
			 lfs_size_t size) {
	uint32_t timeout_ms = SD_RW_TIMEOUT_MS;
	uint32_t block_addr = block + lfsshim_first_block_offset;
	//uint32_t num_blocks = (size + c->block_size - 1) / c->block_size;
	uint32_t num_blocks = size / c->block_size;
	w_assert((size % c->block_size) == 0);
	w_assert(off == 0);

	HAL_StatusTypeDef hal =
		HAL_SD_WriteBlocks(lfsshim_hsd, (uint8_t *)buffer, block_addr, num_blocks, timeout_ms);
	if (hal != HAL_OK) {
		return -1; // LFS_ERR_IO
	}

	// Wait for card to be ready (polling)
	uint32_t start = HAL_GetTick();
	while (HAL_SD_GetCardState(lfsshim_hsd) != HAL_SD_CARD_TRANSFER) {
		if ((HAL_GetTick() - start) > timeout_ms) {
			return -1; // timeout -> LFS_ERR_IO
		}
	}

	return 0; // success
}

int lfsshim_erase(const struct lfs_config *c, lfs_block_t block) {
	return 0; // SD does not require explicit erase
}

int lfsshim_sync(const struct lfs_config *c) {
	return 0;
}
