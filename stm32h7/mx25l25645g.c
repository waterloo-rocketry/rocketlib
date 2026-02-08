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
// .Address = 0x014;
// .AlternateBytes = 0x0;
// .AlternateBytesSize = QSPI_ALTERNATE_BYTES_8_BITS;
.DummyCycles = 0;
.InstructionMode = QSPI_INSTRUCTION_1_LINE;
.AddressMode = QSPI_ADDRESS_NONE;
.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
.DataMode = QSPI_DATA_NONE;
// .NbData = 0x0;
.DdrMode = QSPI_DDR_MODE_DISABLE;
// .DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
}
;

// Quad SPI command for flash
QSPI_CommandTypeDef CMD_BASE_QSPI = {.Instruction = 0x0;
// .Address = 0x014;
// .AlternateBytes = 0x0;
// .AddressSize = QSPI_ADDRESS_8_BITS;
.AlternateBytesSize =
	QSPI_ALTERNATE_BYTES_8_BITS; // may need to change to QSPI_ADDRESS_24_BITS or 32
.DummyCycles = 0; // was 8
.InstructionMode = QSPI_INSTRUCTION_4_LINES;
.AddressMode = QSPI_ADDRESS_NONE;
.AlternateByteMode = QSPI_ALTERNATE_BYTES_NONE;
.DataMode = QSPI_DATA_NONE;
.NbData = 0;
.DdrMode = QSPI_DDR_MODE_DISABLE;
.DdrHoldHalfCycle = QSPI_DDR_HHC_ANALOG_DELAY;
.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;
}
;

static HAL_StatusTypeDef mx25l_exit_qpi(void) {
	QSPI_CommandTypeDef cmd = CMD_BASE_QSPI;
	cmd.Instruction = MX_CMD_RSTQIO; // 0xF5
	cmd.DataMode = QSPI_DATA_NONE;
	cmd.NbData = 0;
	return HAL_QSPI_Command(&hqspi, &cmd, Timeout);
}

// Enable QSPI mode on the flash
HAL_StatusTypeDef mx25l_enter_qpi(void) {
	QSPI_CommandTypeDef cmd = CMD_BASE;
	cmd.Instruction = MX_CMD_EQIO;
	cmd.DataMode = QSPI_DATA_NONE;
	cmd.NbData = 0;
	return HAL_QSPI_Command(&hqspi, &cmd, Timeout);
}

// static HAL_StatusTypeDef qspi_write_enable(void) {
// 	QSPI_CommandTypeDef cmd = CMD_BASE_QSPI;
// 	cmd.Instruction = MX_CMD_WREN;
// 	cmd.DataMode = QSPI_DATA_NONE;
// 	cmd.NbData = 0;

// 	return HAL_QSPI_Command(&hqspi, &cmd, Timeout);
// }

// static HAL_StatusTypeDef qspi_mx25l_wait_wip0(void) {
// 	// Command: Read Status Register (RDSR)
// 	QSPI_CommandTypeDef cmd = {0};
// 	cmd.InstructionMode = QSPI_INSTRUCTION_1_LINE;
// 	cmd.Instruction = MX_CMD_RDSR;
// 	cmd.AddressMode = QSPI_ADDRESS_NONE;
// 	cmd.DataMode = QSPI_DATA_1_LINE;
// 	cmd.NbData = 1;
// 	cmd.DummyCycles = 0;
// 	cmd.DdrMode = QSPI_DDR_MODE_DISABLE;
// 	cmd.SIOOMode = QSPI_SIOO_INST_EVERY_CMD;

// 	// Polling config: wait until WIP=0
// 	QSPI_AutoPollingTypeDef cfg = {0};
// 	cfg.Match = 0x00; // WIP must become 0
// 	cfg.Mask = 0x01; // Check only bit0
// 	cfg.MatchMode = QSPI_MATCH_MODE_AND;
// 	cfg.StatusBytesSize = 1;
// 	cfg.Interval = 0x10;
// 	cfg.AutomaticStop = QSPI_AUTOMATIC_STOP_ENABLE;

// 	return HAL_QSPI_AutoPolling(&hqspi, &cmd, &cfg, Timeout);
// }

int main(void) {
	HAL_Init();
	SystemClock_Config();

	// Initialize the QSPI MSP
	MX_GPIO_Init();

	// Initialize the QSPI mode with 1 instruction line, CMD_BASE
	MX_QUADSPI_Init();

	// Enable QPI mode on flash
	mx25l_enter_qpi();

	// From now on instruction becomes 4 lines since qspi is enabled, QSPI_CMD_BASE

	while (1) {}
}