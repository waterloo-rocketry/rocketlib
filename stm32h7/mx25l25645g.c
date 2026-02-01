#include "mx25l25645g_defs.h"
#include "stm32h7xx_hal.h"

QSPI_HandleTypeDef hqspi;
static uint32_t Timeout = 100;

static const QSPI_InitTypeDef qspi_init_cfg = {
	.ClockPrescaler = 4,
	.FifoThreshold = 4,
	.SampleShifting = QSPI_SAMPLE_SHIFTING_NONE,
	.FlashSize = 24, // MX25L25645G = 32MB
	.ChipSelectHighTime = QSPI_CS_HIGH_TIME_2_CYCLE,
	.ClockMode = QSPI_CLOCK_MODE_0,
	.FlashID = QSPI_FLASH_ID_1,
	.DualFlash = QSPI_DUALFLASH_DISABLE,
};

void MX_QUADSPI_Init(void) {
	hqspi.Instance = QUADSPI;
	hqspi.Init = qspi_init_cfg;
	if (HAL_QSPI_Init(&hqspi) != HAL_OK) {
		Error_Handler();
	}
}

// Normal SPI command for flash
const QSPI_CommandTypeDef CMD_BASE = {.Instruction = 0x0;
.Address = 0x014;
.AlternateBytes = 0x0;
.AddressSize = QSPI_ADDRESS_8_BITS;
.AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
.DummyCycles = 0;
.InstructionMode = QSPI_INSTRUCTION_NONE;
.AddressMode = QSPI_ADDRESS_NONE;
.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
.DataMode = QSPI_DATA_NONE;
.NbData = 0x0;
.DdrMode = QSPI_DDR_MODE_DISABLE;
.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
}
;

// Quad SPI command for flash
QSPI_CommandTypeDef QSPI_CMD_BASE = {.Instruction = 0x0;
.Address = 0x014;
.AlternateBytes = 0x0;
.AddressSize = QSPI_ADDRESS_8_BITS;
.AlternateBytesSize =
	QSPI_ALTERNATE_BYTES_8_BITS; // may need to change to QSPI_ADDRESS_24_BITS or 32
.DummyCycles = 0; // was 8
.InstructionMode = QSPI_INSTRUCTION_1_LINE;
.AddressMode = QSPI_ADDRESS_NONE;
.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
.DataMode = QSPI_DATA_NONE;
.NbData = 0;
.DdrMode = QSPI_DDR_MODE_DISABLE;
.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
}
;

static HAL_StatusTypeDef qspi_read_status(uint8_t *sr) {
	QSPI_CommandTypeDef cmd = QSPI_CMD_BASE;
	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction = MX_CMD_RDSR;
	cmd.DataMode = QSPI_DATA_1_LINE;
	cmd.NbData = 1;

	if (HAL_QSPI_Command(&hqspi, &cmd, Timeout) != HAL_OK) {
		return HAL_ERROR;
	}
	return HAL_QSPI_Receive(&hqspi, sr, Timeout);
}

static HAL_StatusTypeDef qspi_read_config(uint8_t *cr) {
	QSPI_CommandTypeDef cmd = QSPI_CMD_BASE;
	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction = MX_CMD_RDCR;
	cmd.DataMode = QSPI_DATA_1_LINE;
	cmd.NbData = 1;

	if (HAL_QSPI_Command(&hqspi, &cmd, Timeout) != HAL_OK) {
		return HAL_ERROR;
	}
	return HAL_QSPI_Receive(&hqspi, cr, Timeout);
}

static HAL_StatusTypeDef qspi_write_enable(void) {
	QSPI_CommandTypeDef cmd = QSPI_CMD_BASE;
	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction = MX_CMD_WREN;
	cmd.DataMode = QSPI_DATA_1_LINE;
	cmd.NbData = 0;

	return HAL_QSPI_Command(&hqspi, &cmd, Timeout);
}

static HAL_StatusTypeDef qspi_mx25l_wait_wip0(void) {
	// Command: Read Status Register (RDSR)
	QSPI_CommandTypeDef cmd = {0};
	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction = MX_CMD_RDSR;
	cmd.AddressMode = QSPI_ADDRESS_NONE;
	cmd.DataMode = QSPI_DATA_1_LINE;
	cmd.NbData = 1;
	cmd.DummyCycles = 0;
	cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

	// Polling config: wait until WIP=0
	QSPI_AutoPollingTypeDef cfg = {0};
	cfg.Match = 0x00; // WIP must become 0
	cfg.Mask = 0x01; // Check only bit0
	cfg.MatchMode = QSPI_MATCH_MODE_AND;
	cfg.StatusBytesSize = 1;
	cfg.Interval = 0x10;
	cfg.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

	return HAL_QSPI_AutoPolling(&hqspi, &cmd, &cfg, Timeout);
}

// Enable QSPI mode on the MCU
HAL_StatusTypeDef mx25l_enable_qe(void) {
	// Initialize status register and config register
	uint8_t sr = 0, cr = 0;

	if (qspi_read_status(&sr) != HAL_OK) {
		return HAL_ERROR;
	}
	if (qspi_read_config(&cr) != HAL_OK) {
		return HAL_ERROR;
	}

	sr |= (1u << 6); // QE = bit 6
	uint8_t status_config_bytes[2] = {sr, cr};

	if (qspi_write_enable() != HAL_OK) {
		return HAL_ERROR;
	}

	QSPI_CommandTypeDef cmd = QSPI_CMD_BASE;
	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
	cmd.Instruction = MX_CMD_WRSR;
	cmd.DataMode = QSPI_DATA_1_LINE;
	cmd.NbData = 2;

	if (HAL_QSPI_Command(&hqspi, &cmd, Timeout) != HAL_OK) {
		return HAL_ERROR;
	}
	if (HAL_QSPI_Transmit(&hqspi, status_config_bytes, Timeout) != HAL_OK) {
		return HAL_ERROR;
	}

	// Wait for the write to finish (WIP = 0)
	if (qspi_mx25l_wait_wip0() != HAL_OK) {
		return HAL_ERROR;
	}

	// Verify QE really latched
	if (qspi_read_status(&sr) != HAL_OK) {
		return HAL_ERROR;
	}
	return (sr & (1u << 6)) ? HAL_OK : HAL_ERROR;
}

int main(void) {
	// Change flash to qspi

	HAL_Init();
	SystemClock_Config();

	// Initialize the QSPI MSP
	MX_GPIO_Init();
	// Initialize the QSPI mode according to the parameters in the QSPI_InitTypeDef
	MX_QUADSPI_Init();

	// Enable quad mode
	mx25l_enable_qe();

	while (1) {}
}