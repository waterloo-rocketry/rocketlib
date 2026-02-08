#ifndef ROCKETLIB_MX25l25645G_DEFS_H
#define ROCKETLIB_MX25l25645G_DEFS_H

#include <stdbool.h>
#include <stdint.h>

#include "common.h"

typedef enum {

    // ============================================================
    // Read/Write Array Commands (3-Byte Address)
    // ============================================================

    MX_CMD_READ       = 0x03, // Normal Read
    MX_CMD_FAST_READ  = 0x0B, // Fast Read (requires dummy cycles)

    MX_CMD_DREAD      = 0x3B, // Dual Output Fast Read
    MX_CMD_2READ      = 0xBB, // Dual I/O Fast Read

    MX_CMD_QREAD      = 0x6B, // Quad Output Fast Read
    MX_CMD_4READ      = 0xEB, // Quad I/O Fast Read

    MX_CMD_4DTRD      = 0xED, // Quad I/O DDR Read (DTR)

    MX_CMD_PP         = 0x02, // Page Program
    MX_CMD_4PP        = 0x38, // Quad Page Program

    MX_CMD_SE         = 0x20, // Sector Erase (4KB)
    MX_CMD_BE32K      = 0x52, // Block Erase (32KB)
    MX_CMD_BE         = 0xD8, // Block Erase (64KB)

    MX_CMD_CE         = 0xC7, // Chip Erase (also possible: 0x60)

    // ============================================================
    // Read/Write Array Commands (4-Byte Address Command Set)
    // ============================================================

    MX_CMD_READ4B      = 0x13, // Normal Read (4-byte address)
    MX_CMD_FAST_READ4B = 0x0C, // Fast Read (4-byte address)

    MX_CMD_DREAD4B     = 0x3C, // Dual Output Fast Read (4B)
    MX_CMD_2READ4B     = 0xBC, // Dual I/O Fast Read (4B)

    MX_CMD_QREAD4B     = 0x6C, // Quad Output Fast Read (4B)
    MX_CMD_4READ4B     = 0xEC, // Quad I/O Fast Read (4B)

    MX_CMD_4DTRD4B     = 0xEE, // Quad I/O DDR Read (4B)

    MX_CMD_PP4B        = 0x12, // Page Program (4B)
    MX_CMD_4PP4B       = 0x3E, // Quad Page Program (4B)

    MX_CMD_SE4B        = 0x21, // Sector Erase (4KB, 4B)
    MX_CMD_BE32K4B     = 0x5C, // Block Erase (32KB, 4B)
    MX_CMD_BE4B        = 0xDC, // Block Erase (64KB, 4B)

    // ============================================================
    // Register / Setting Commands
    // ============================================================

    MX_CMD_WREN   = 0x06, // Write Enable
    MX_CMD_WRDI   = 0x04, // Write Disable

    MX_CMD_RDSR   = 0x05, // Read Status Register
    MX_CMD_WRSR   = 0x01, // Write Status + Configuration Register

    MX_CMD_RDCR   = 0x15, // Read Configuration Register

    MX_CMD_RDEAR  = 0xC8, // Read Extended Address Register
    MX_CMD_WREAR  = 0xC5, // Write Extended Address Register

    MX_CMD_WPSEL  = 0x68, // Write Protection Selection

    MX_CMD_EQIO   = 0x35, // Enable QPI Mode
    MX_CMD_RSTQIO = 0xF5, // Reset / Exit QPI Mode

    MX_CMD_EN4B   = 0xB7, // Enter 4-byte Address Mode
    MX_CMD_EX4B   = 0xE9, // Exit 4-byte Address Mode

    MX_CMD_PGM_ERS_SUSPEND = 0xB0, // Program/Erase Suspend
    MX_CMD_PGM_ERS_RESUME  = 0x30, // Program/Erase Resume

    MX_CMD_DP    = 0xB9, // Deep Power Down
    MX_CMD_RDP   = 0xAB, // Release from Deep Power Down

    MX_CMD_SBL   = 0xC0, // Set Burst Length

    // ============================================================
    // ID / Security Commands
    // ============================================================

    MX_CMD_RDID   = 0x9F, // Read JEDEC ID

    MX_CMD_RES    = 0xAB, // Read Electronic ID (Legacy)

    MX_CMD_REMS   = 0x90, // Read Manufacturer / Device ID

    MX_CMD_ENSO   = 0xB1, // Enter Secured OTP Mode
    MX_CMD_EXSO   = 0xC1, // Exit Secured OTP Mode

    MX_CMD_RDSCUR = 0x2B, // Read Security Register
    MX_CMD_WRSCUR = 0x2F, // Write Security Register

    MX_CMD_RDDPB  = 0xE0, // Read DPB (Dynamic Protection Bits)
    MX_CMD_WRDPB  = 0xE1, // Write DPB

    MX_CMD_RDECC  = 0x18, // Read ECC Status

    // ============================================================
    // Reset Commands
    // ============================================================

    MX_CMD_RSTEN  = 0x66, // Reset Enable
    MX_CMD_RST    = 0x99, // Reset Memory Device

} command_t;

#endif /* ROCKETLIB_MX25l25645G_DEFS_H */