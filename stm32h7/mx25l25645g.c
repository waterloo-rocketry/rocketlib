#include "stm32h7xx_hal.h"

// Normal SPI
const QSPI_CommandTypeDef command = {
    .Instruction = 0x0;
    .Address = 0x014;
    .AlternateBytes = 0x0;
    .AdressSize = 0x0; // 8-bits
    .AlternateBytesSize = 0x0;
    .DummyCycles = 0x8;
    .InstructionMode = 0x1; // Mode for a single line
    .AddressMode = 0x1;
    .AlternateBytesMode = 0x1;
    .DataMode = 0x1;  
    .NbData = 0x0;
    .DdrMode = 0; // DDR disabled
    .DdrHoldHalfCycle = 0x0;
    .SIOOMode = 0x0;
}

// Quad SPI
const QSPI_CommandTypeDef command = {
    .Instruction = 0x0;
    .Address = 0x014;
    .AlternateBytes = 0x0;
    .AdressSize = 0x0; // 8-bits
    .AlternateBytesSize = 0x0;
    .DummyCycles = 0x8;
    .InstructionMode = 0x3; // Mode for four lines
    .AddressMode = 0x3;
    .AlternateBytesMode = 0x3;
    .DataMode = 0x3; 
    .NbData = 0x0;
    .DdrMode = 0x1; // DDR enabled
    .DdrHoldHalfCycle = 0x0;
    .SIOOMode = 0x0;
}

// send to switch flash to qpsi mode
wstatus_t qspi_switch() {
    HAL_StatusTypeDef HAL_QSPI_Command (QSPI_HandleTypeDef * hqspi, QSPI_CommandTypeDef * cmd, uint32_t Timeout)
}