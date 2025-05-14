#include "pin_config.h"
#include "common.h"
#include <xc.h>

/**
 * @brief Unlocks the PPS registers for configuration
 *
 * Implements the specific sequence from the PIC18F26K83 datasheet
 * to unlock the PPS registers for modification.
 *
 * @return w_status_t W_SUCCESS if successful
 */
static w_status_t pps_unlock(void) {
    INTCON0bits.GIE = 0; // Disable global interrupts during critical sequence
    PPSLOCK = 0x55; // First unlock sequence value
    PPSLOCK = 0xAA; // Second unlock sequence value
    PPSLOCKbits.PPSLOCKED = 0; // Clear PPSLOCKED bit to enable PPS changes
    INTCON0bits.GIE = 1; // Re-enable global interrupts

    return W_SUCCESS;
}

/**
 * @brief Locks the PPS registers to prevent accidental changes
 *
 * Implements the specific sequence from the PIC18F26K83 datasheet
 * to lock the PPS registers after configuration is complete.
 *
 * @return w_status_t W_SUCCESS if successful
 */
static w_status_t pps_lock(void) {
    INTCON0bits.GIE = 0; // Disable global interrupts during critical sequence
    PPSLOCK = 0x55; // First lock sequence value
    PPSLOCK = 0xAA; // Second lock sequence value
    PPSLOCKbits.PPSLOCKED = 1; // Set PPSLOCKED bit to lock PPS registers
    INTCON0bits.GIE = 1; // Re-enable global interrupts

    return W_SUCCESS;
}

/**
 * @brief Helper function for PPS input mapping
 *
 * Sets up the mapping between a peripheral input and a physical pin.
 *
 * @param pps_register Pointer to the peripheral's PPS register
 * @param port_pin_code Code representing the port and pin (format: 0bPPPNNN where PPP=port,
 * NNN=pin)
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if pointer is NULL
 */
static w_status_t pps_input(volatile unsigned char *pps_register, unsigned char port_pin_code) {
    if (pps_register == NULL) {
        return W_INVALID_PARAM;
    }

    *pps_register = port_pin_code;
    return W_SUCCESS;
}

/**
 * @brief Helper function for PPS output mapping
 *
 * Sets up the mapping between a peripheral output and a physical pin.
 *
 * @param rxy_pps_register Pointer to the pin's PPS register
 * @param peripheral_code Code representing the peripheral output function
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if pointer is NULL
 */
static w_status_t
pps_output(volatile unsigned char *rxy_pps_register, unsigned char peripheral_code) {
    if (rxy_pps_register == NULL) {
        return W_INVALID_PARAM;
    }

    *rxy_pps_register = peripheral_code;
    return W_SUCCESS;
}

/**
 * @brief Configures I2C1 pins for RC3 (SCL) and RC4 (SDA)
 *
 * Sets up PPS registers for the I2C1 module on the PIC18F26K83.
 * Configures both input and output mappings since I2C is bidirectional.
 * Also configures pins for digital mode and open-drain operation.
 *
 * @return w_status_t W_SUCCESS if successful
 */
static w_status_t pps_configure_i2c1(void) {
    // Configure digital mode for I2C pins
    ANSELCbits.ANSC3 = 0; // Set RC3 to digital mode
    ANSELCbits.ANSC4 = 0; // Set RC4 to digital mode

    // Set pins as inputs initially (I2C peripheral will control them)
    TRISCbits.TRISC3 = 1; // Set SCL as input
    TRISCbits.TRISC4 = 1; // Set SDA as input

    // Input mapping for I2C (from pins to peripheral)
    I2C1SCLPPS = 0b010011; // Map RC3 to I2C1 SCL input
    I2C1SDAPPS = 0b010100; // Map RC4 to I2C1 SDA input

    // Output mapping for I2C (from peripheral to pins)
    RC3PPS = 0b100001; // Map I2C1 SCL output to RC3
    RC4PPS = 0b100010; // Map I2C1 SDA output to RC4

    // Enable open-drain mode for I2C pins (required for I2C operation)
    ODCONCbits.ODCC3 = 1; // Set RC3 to open-drain mode
    ODCONCbits.ODCC4 = 1; // Set RC4 to open-drain mode

    return W_SUCCESS;
}

/**
 * @brief Configures SPI1 pins for master mode
 *
 * Sets up PPS registers for the SPI1 module in master mode:
 * - RB1 (SCK) - Clock output
 * - RB2 (SDI) - Data input
 * - RB3 (SDO) - Data output
 *
 * Also configures pin directions and digital mode appropriately.
 *
 * @return w_status_t W_SUCCESS if successful
 */
static w_status_t pps_configure_spi1(void) {
    // Configure digital mode for SPI pins
    ANSELBbits.ANSELB1 = 0; // Set RB1 to digital mode
    ANSELBbits.ANSELB2 = 0; // Set RB2 to digital mode
    ANSELBbits.ANSELB3 = 0; // Set RB3 to digital mode

    // Set correct pin directions for SPI master mode
    TRISBbits.TRISB1 = 0; // SCK as output (master mode)
    TRISBbits.TRISB2 = 1; // SDI as input (master reads data)
    TRISBbits.TRISB3 = 0; // SDO as output (master sends data)

    // Input mapping for SPI (from pins to peripheral)
    SPI1SCKPPS = 0b001001; // Map RB1 to SPI1 SCK input (for slave mode or feedback)
    SPI1SDIPPS = 0b001010; // Map RB2 to SPI1 SDI input

    // Output mapping for SPI (from peripheral to pins)
    RB1PPS = 0b011110; // Map SPI1 SCK output to RB1
    RB3PPS = 0b011111; // Map SPI1 SDO output to RB3

    return W_SUCCESS;
}

/**
 * @brief Configures UART1 pins for communication
 *
 * Sets up PPS registers for the UART1 module:
 * - RC6 (TX) - Transmit output
 * - RC7 (RX) - Receive input
 *
 * Also configures pin directions and digital mode appropriately.
 *
 * @return w_status_t W_SUCCESS if successful
 */
static w_status_t pps_configure_uart1(void) {
    // Configure digital mode for UART pins
    ANSELCbits.ANSC6 = 0; // Set RC6 to digital mode
    ANSELCbits.ANSC7 = 0; // Set RC7 to digital mode

    // Set correct pin directions for UART
    TRISCbits.TRISC6 = 0; // TX as output
    TRISCbits.TRISC7 = 1; // RX as input

    // Input mapping for UART (from pin to peripheral)
    U1RXPPS = 0b010111; // Map RC7 to UART1 RX input

    // Output mapping for UART (from peripheral to pin)
    RC6PPS = 0b010011; // Map UART1 TX output to RC6

    return W_SUCCESS;
}

/**
 * @brief Configures timer external clock inputs
 *
 * Maps a physical pin to serve as the external clock input for a timer.
 * Also configures the pin as a digital input.
 *
 * @param timer Timer number to configure (0, 1, 3, 5)
 * @param port Port number where clock input is located (0=PORTA, 1=PORTB, 2=PORTC)
 * @param pin Pin number within the port (0-7)
 * @return w_status_t W_SUCCESS if successful, W_INVALID_PARAM if parameters are invalid
 */
static w_status_t
pps_configure_timer_clk(unsigned char timer, unsigned char port, unsigned char pin) {
    // Create the port-pin PPS input code (format: 0bPPPPPP where bits 5-3=port, bits 2-0=pin)
    unsigned char port_pin_code = (port << 3) | pin;

    // Configure the pin for digital input
    switch (port) {
        case 0: // PORTA
            if (pin < 8) {
                ANSELA &= ~(1 << pin); // Set pin to digital mode
                TRISA |= (1 << pin); // Set pin as input
            }
            break;
        case 1: // PORTB
            if (pin < 8) {
                ANSELB &= ~(1 << pin); // Set pin to digital mode
                TRISB |= (1 << pin); // Set pin as input
            }
            break;
        case 2: // PORTC
            if (pin < 8) {
                ANSELC &= ~(1 << pin); // Set pin to digital mode
                TRISC |= (1 << pin); // Set pin as input
            }
            break;
        default:
            return; // Invalid port
    }

    // Map the pin to the appropriate timer input
    switch (timer) {
        case 0:
            T0CKIPPS = port_pin_code; // Timer0 external clock input
            break;
        case 1:
            T1CKIPPS = port_pin_code; // Timer1 external clock input
            break;
        case 3:
            T3CKIPPS = port_pin_code; // Timer3 external clock input
            break;
        case 5:
            T5CKIPPS = port_pin_code; // Timer5 external clock input
            break;
        default:
            return W_INVALID_PARAM; // Invalid timer
    }

    return W_SUCCESS;
}

/**
 * @brief Initializes all PPS configurations for peripherals
 *
 * This function sets up all Peripheral Pin Select (PPS) mappings
 * for the PIC18F26K83 microcontroller. It configures:
 * - I2C1 pins (RC3/RC4)
 * - SPI1 pins (RB1/RB2/RB3)
 * - UART1 pins (RC6/RC7)
 *
 * Note that PWM/CCP PPS configuration is handled by the PWM module itself.
 * This function should be called after basic MCU initialization but before
 * initializing any peripherals.
 *
 * @return w_status_t W_SUCCESS if successful
 */
w_status_t pin_init(void) {
    // Unlock PPS registers before making any changes
    pps_unlock();

    // Configure all required peripheral pins
    pps_configure_i2c1();
    pps_configure_spi1();
    pps_configure_uart1();

    // Note: PWM/CCP PPS configuration is handled by the PWM module (pwm.c)
    // When pwm_init() is called, it will configure the necessary PPS settings

    // Lock PPS registers after all configurations
    pps_lock();

    return W_SUCCESS;
}