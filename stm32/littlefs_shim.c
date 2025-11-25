#include "main.h"
#include "lfs.h"

SD_HandleTypeDef* lfs_shim_hsd = &hsd2;
uint32_t first_sector_offset = 0;

int sd_read(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, void *buffer,
			lfs_size_t size) {

	uint32_t timeout_ms = 50U;
	uint32_t block_addr = block + first_sector_offset;
	//uint32_t num_blocks = (size + c->block_size - 1) / c->block_size;
	uint32_t num_blocks = 1;
	if((off+size)>512){
		for(;;){}
	}
	uint8_t blockbuf[512];
	HAL_StatusTypeDef hal =
		HAL_SD_ReadBlocks(lfs_shim_hsd, blockbuf, block_addr, num_blocks, timeout_ms);
	uint8_t* buf = (uint8_t*)buffer;
	for(size_t i = 0; i < size; i++) {
		buf[i] = blockbuf[i+off];
	}
	if (hal != HAL_OK) {
		return -1; // LFS_ERR_IO
	}

	// Wait for card to be ready (polling)
	uint32_t start = HAL_GetTick();
	while (HAL_SD_GetCardState(lfs_shim_hsd) != HAL_SD_CARD_TRANSFER) {
		if ((HAL_GetTick() - start) > timeout_ms) {
			return -1; // timeout -> LFS_ERR_IO
		}
	}

	return 0; // success
}

int sd_write(const struct lfs_config *c, lfs_block_t block, lfs_off_t off, const void *buffer,
			 lfs_size_t size) {
	uint32_t timeout_ms = 50U;
	uint32_t block_addr = block + first_sector_offset;
	uint32_t num_blocks = (size + c->block_size - 1) / c->block_size;
	HAL_StatusTypeDef hal =
		HAL_SD_WriteBlocks(lfs_shim_hsd, (uint8_t *)buffer, block_addr, num_blocks, timeout_ms);
	if (hal != HAL_OK) {
		return -1; // LFS_ERR_IO
	}

	// Wait for card to be ready (polling)
	uint32_t start = HAL_GetTick();
	while (HAL_SD_GetCardState(lfs_shim_hsd) != HAL_SD_CARD_TRANSFER) {
		if ((HAL_GetTick() - start) > timeout_ms) {
			return -1; // timeout -> LFS_ERR_IO
		}
	}

	return 0; // success
}

int sd_erase(const struct lfs_config *c, lfs_block_t block) {
	return 0; // SD does not require explicit erase
}

int sd_sync(const struct lfs_config *c) {
	return 0;
}
