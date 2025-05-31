#include "pin_config.h"
#include "common.h"
#include <xc.h>

/**
 * @brief Unlocks the PPS registers for configuration
 */
static w_status_t pps_unlock(void) {
    uint8_t gie_state = INTCON0bits.GIE; // Save current GIE state
    INTCON0bits.GIE = 0; // Disable global interrupts during critical sequence
    PPSLOCK = 0x55; // First unlock sequence value
    PPSLOCK = 0xAA; // Second unlock sequence value
    PPSLOCKbits.PPSLOCKED = 0; // Clear PPSLOCKED bit to enable PPS changes
    INTCON0bits.GIE = gie_state; // Restore original GIE state
    return W_SUCCESS;
}

/**
 * @brief Locks the PPS registers to prevent accidental changes
 */
static w_status_t pps_lock(void) {
    uint8_t gie_state = INTCON0bits.GIE; // Save current GIE state
    INTCON0bits.GIE = 0; // Disable global interrupts during critical sequence
    PPSLOCK = 0x55; // First lock sequence value
    PPSLOCK = 0xAA; // Second lock sequence value
    PPSLOCKbits.PPSLOCKED = 1; // Set PPSLOCKED bit to lock PPS registers
    INTCON0bits.GIE = gie_state; // Restore original GIE state
    return W_SUCCESS;
}

/**
 * @brief Get TRIS register address for the specified port
 */
static volatile uint8_t* get_tris_register(uint8_t port) {
    switch (port) {
        case 0U: return &TRISA;
        case 1U: return &TRISB;
        case 2U: return &TRISC;
        default: return NULL;
    }
}

/**
 * @brief Get ANSEL register address for the specified port
 */
static volatile uint8_t* get_ansel_register(uint8_t port) {
    switch (port) {
        case 0U: return &ANSELA;
        case 1U: return &ANSELB;
        case 2U: return &ANSELC;
        default: return NULL;
    }
}

/**
 * @brief Get ODCON register address for the specified port
 */
static volatile uint8_t* get_odcon_register(uint8_t port) {
    switch (port) {
        case 0U: return &ODCONA;
        case 1U: return &ODCONB;
        case 2U: return &ODCONC;
        default: return NULL;
    }
}

/**
 * @brief Get PPS output register address for the specified port and pin
 */
static volatile uint8_t* get_pps_output_register(uint8_t port, uint8_t pin) {
    if (port == 0U) { // PORTA
        switch (pin) {
            case 0U: return &RA0PPS;
            case 1U: return &RA1PPS;
            case 2U: return &RA2PPS;
            case 3U: return &RA3PPS;
            case 4U: return &RA4PPS;
            case 5U: return &RA5PPS;
            case 6U: return &RA6PPS;
            case 7U: return &RA7PPS;
            default: return NULL;
        }
    } else if (port == 1U) { // PORTB
        switch (pin) {
            case 0U: return &RB0PPS;
            case 1U: return &RB1PPS;
            case 2U: return &RB2PPS;
            case 3U: return &RB3PPS;
            case 4U: return &RB4PPS;
            case 5U: return &RB5PPS;
            case 6U: return &RB6PPS;
            case 7U: return &RB7PPS;
            default: return NULL;
        }
    } else if (port == 2U) { // PORTC
        switch (pin) {
            case 0U: return &RC0PPS;
            case 1U: return &RC1PPS;
            case 2U: return &RC2PPS;
            case 3U: return &RC3PPS;
            case 4U: return &RC4PPS;
            case 5U: return &RC5PPS;
            case 6U: return &RC6PPS;
            case 7U: return &RC7PPS;
            default: return NULL;
        }
    }
    return NULL;
}

/**
 * @brief Validate pin configuration parameters
 */
static w_status_t validate_pin_config(pin_config_t pin_config) {
    if (pin_config.port > 2U || pin_config.pin > 7U) {
        return W_INVALID_PARAM;
    }
    return W_SUCCESS;
}

/**
 * @brief Configure a pin for digital I/O
 */
static w_status_t configure_pin_digital(pin_config_t pin_config, uint8_t direction) {
    w_status_t status = validate_pin_config(pin_config);
    if (status != W_SUCCESS) {
        return status;
    }

    volatile uint8_t *ansel_reg = get_ansel_register(pin_config.port);
    volatile uint8_t *tris_reg = get_tris_register(pin_config.port);

    if (ansel_reg == NULL || tris_reg == NULL) {
        return W_INVALID_PARAM;
    }

    // Set pin to digital mode
    *ansel_reg &= ~(1U << pin_config.pin);

    // Set direction (0=output, 1=input)
    if (direction != 0U) {
        *tris_reg |= (1U << pin_config.pin);
    } else {
        *tris_reg &= ~(1U << pin_config.pin);
    }

    return W_SUCCESS;
}

/**
 * @brief Set pin to open-drain mode
 */
static w_status_t configure_pin_open_drain(pin_config_t pin_config) {
    w_status_t status = validate_pin_config(pin_config);
    if (status != W_SUCCESS) {
        return status;
    }

    volatile uint8_t *odcon_reg = get_odcon_register(pin_config.port);
    if (odcon_reg == NULL) {
        return W_INVALID_PARAM;
    }

    *odcon_reg |= (1U << pin_config.pin);
    return W_SUCCESS;
}

/**
 * @brief Get port-pin code for PPS input mapping
 */
static uint8_t get_port_pin_code(pin_config_t pin_config) {
    return (uint8_t)((pin_config.port << 3) | pin_config.pin);
}

/**
 * @brief Configure PPS output mapping
 */
static w_status_t configure_pps_output(pin_config_t pin_config, uint8_t peripheral_code) {
    w_status_t status = validate_pin_config(pin_config);
    if (status != W_SUCCESS) {
        return status;
    }

    volatile uint8_t *pps_reg = get_pps_output_register(pin_config.port, pin_config.pin);
    if (pps_reg == NULL) {
        return W_INVALID_PARAM;
    }

    *pps_reg = peripheral_code;
    return W_SUCCESS;
}

/**
 * @brief Get I2C PPS codes for the specified module
 */
static w_status_t get_i2c_pps_codes(uint8_t i2c_module, uint8_t *scl_code, uint8_t *sda_code) {
    switch (i2c_module) {
        case 1U:
            *scl_code = PPS_I2C1_SCL_OUTPUT;
            *sda_code = PPS_I2C1_SDA_OUTPUT;
            return W_SUCCESS;
        case 2U:
            *scl_code = PPS_I2C2_SCL_OUTPUT;
            *sda_code = PPS_I2C2_SDA_OUTPUT;
            return W_SUCCESS;
        default:
            return W_INVALID_PARAM;
    }
}

/**
 * @brief Configure I2C PPS input registers
 */
static w_status_t configure_i2c_input_pps(uint8_t i2c_module, i2c_pin_config_t pin_config) {
    uint8_t scl_port_pin_code = get_port_pin_code(pin_config.scl);
    uint8_t sda_port_pin_code = get_port_pin_code(pin_config.sda);

    switch (i2c_module) {
        case 1U:
            I2C1SCLPPS = scl_port_pin_code;
            I2C1SDAPPS = sda_port_pin_code;
            return W_SUCCESS;
        case 2U:
            I2C2SCLPPS = scl_port_pin_code;
            I2C2SDAPPS = sda_port_pin_code;
            return W_SUCCESS;
        default:
            return W_INVALID_PARAM;
    }
}

/**
 * @brief Get SPI PPS codes for the specified module
 */
static w_status_t get_spi_pps_codes(uint8_t spi_module, uint8_t *sck_code, uint8_t *sdo_code) {
    switch (spi_module) {
        case 1U:
            *sck_code = PPS_SPI1_SCK_OUTPUT;
            *sdo_code = PPS_SPI1_SDO_OUTPUT;
            return W_SUCCESS;
        case 2U:
            *sck_code = PPS_SPI2_SCK_OUTPUT;
            *sdo_code = PPS_SPI2_SDO_OUTPUT;
            return W_SUCCESS;
        default:
            return W_INVALID_PARAM;
    }
}

/**
 * @brief Configure SPI PPS input registers
 */
static w_status_t configure_spi_input_pps(uint8_t spi_module, spi_pin_config_t pin_config) {
    uint8_t sck_port_pin_code = get_port_pin_code(pin_config.sck);
    uint8_t sdi_port_pin_code = get_port_pin_code(pin_config.sdi);

    switch (spi_module) {
        case 1U:
            SPI1SCKPPS = sck_port_pin_code;
            SPI1SDIPPS = sdi_port_pin_code;
            return W_SUCCESS;
        case 2U:
            SPI2SCKPPS = sck_port_pin_code;
            SPI2SDIPPS = sdi_port_pin_code;
            return W_SUCCESS;
        default:
            return W_INVALID_PARAM;
    }
}

/**
 * @brief Configure UART PPS input registers
 */
static w_status_t configure_uart_input_pps(uint8_t uart_module, pin_config_t rx_pin) {
    uint8_t rx_port_pin_code = get_port_pin_code(rx_pin);

    switch (uart_module) {
        case 1U:
            U1RXPPS = rx_port_pin_code;
            return W_SUCCESS;
        case 2U:
            U2RXPPS = rx_port_pin_code;
            return W_SUCCESS;
        case 3U:
            U3RXPPS = rx_port_pin_code;
            return W_SUCCESS;
        case 4U:
            U4RXPPS = rx_port_pin_code;
            return W_SUCCESS;
        case 5U:
            U5RXPPS = rx_port_pin_code;
            return W_SUCCESS;
        default:
            return W_INVALID_PARAM;
    }
}

/**
 * @brief Get UART TX PPS code for the specified module
 */
static w_status_t get_uart_tx_pps_code(uint8_t uart_module, uint8_t *tx_code) {
    switch (uart_module) {
        case 1U:
            *tx_code = PPS_UART1_TX_OUTPUT;
            return W_SUCCESS;
        case 2U:
            *tx_code = PPS_UART2_TX_OUTPUT;
            return W_SUCCESS;
        case 3U:
            *tx_code = PPS_UART3_TX_OUTPUT;
            return W_SUCCESS;
        case 4U:
            *tx_code = PPS_UART4_TX_OUTPUT;
            return W_SUCCESS;
        case 5U:
            *tx_code = PPS_UART5_TX_OUTPUT;
            return W_SUCCESS;
        default:
            return W_INVALID_PARAM;
    }
}

/**
 * @brief Get CCP PPS code for the specified module
 */
static w_status_t get_ccp_pps_code(uint8_t ccp_module, uint8_t *pps_code) {
    switch (ccp_module) {
        case 1U:
            *pps_code = PPS_CCP1_OUTPUT;
            return W_SUCCESS;
        case 2U:
            *pps_code = PPS_CCP2_OUTPUT;
            return W_SUCCESS;
        case 3U:
            *pps_code = PPS_CCP3_OUTPUT;
            return W_SUCCESS;
        case 4U:
            *pps_code = PPS_CCP4_OUTPUT;
            return W_SUCCESS;
        case 5U:
            *pps_code = PPS_CCP5_OUTPUT;
            return W_SUCCESS;
        default:
            return W_INVALID_PARAM;
    }
}

w_status_t pps_configure_i2c(uint8_t i2c_module, i2c_pin_config_t pin_config) {
    w_status_t status;
    w_status_t final_status = W_SUCCESS;
    uint8_t scl_pps_code, sda_pps_code;

    // Validate module number
    if (i2c_module < 1U || i2c_module > 2U) {
        return W_INVALID_PARAM;
    }

    // Get PPS codes for this I2C module
    status = get_i2c_pps_codes(i2c_module, &scl_pps_code, &sda_pps_code);
    if (status != W_SUCCESS) {
        return status;
    }

    // Unlock PPS registers
    status = pps_unlock();
    if (status != W_SUCCESS) { 
        return status; 
    }

    // Configure SCL pin
    status = configure_pin_digital(pin_config.scl, 1U); // Input initially
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }
    status = configure_pin_open_drain(pin_config.scl);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure SDA pin
    status = configure_pin_digital(pin_config.sda, 1U); // Input initially
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }
    status = configure_pin_open_drain(pin_config.sda);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure PPS input mappings
    status = configure_i2c_input_pps(i2c_module, pin_config);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure PPS output mappings
    status = configure_pps_output(pin_config.scl, scl_pps_code);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }
    status = configure_pps_output(pin_config.sda, sda_pps_code);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

cleanup:
    // Always lock PPS registers before returning
    pps_lock();
    return final_status;
}

w_status_t pps_configure_spi(uint8_t spi_module, spi_pin_config_t pin_config, bool use_ss) {
    w_status_t status;
    w_status_t final_status = W_SUCCESS;
    uint8_t sck_pps_code, sdo_pps_code;

    // Validate module number
    if (spi_module < 1U || spi_module > 2U) {
        return W_INVALID_PARAM;
    }

    // Get PPS codes for this SPI module
    status = get_spi_pps_codes(spi_module, &sck_pps_code, &sdo_pps_code);
    if (status != W_SUCCESS) {
        return status;
    }

    // Unlock PPS registers
    status = pps_unlock();
    if (status != W_SUCCESS) { 
        return status; 
    }

    // Configure SCK pin (output for master mode)
    status = configure_pin_digital(pin_config.sck, 0U);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure SDI pin (input for master mode)
    status = configure_pin_digital(pin_config.sdi, 1U);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure SDO pin (output for master mode)
    status = configure_pin_digital(pin_config.sdo, 0U);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure SS pin if requested (output for master mode)
    if (use_ss) {
        status = configure_pin_digital(pin_config.ss, 0U);
        if (status != W_SUCCESS) { 
            final_status = status;
            goto cleanup; 
        }
    }

    // Configure PPS input mappings
    status = configure_spi_input_pps(spi_module, pin_config);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure PPS output mappings
    status = configure_pps_output(pin_config.sck, sck_pps_code);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }
    status = configure_pps_output(pin_config.sdo, sdo_pps_code);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

cleanup:
    // Always lock PPS registers before returning
    pps_lock();
    return final_status;
}

w_status_t pps_configure_uart(uint8_t uart_module, uart_pin_config_t pin_config) {
    w_status_t status;
    w_status_t final_status = W_SUCCESS;
    uint8_t tx_pps_code;

    // Validate module number
    if (uart_module < 1U || uart_module > 5U) {
        return W_INVALID_PARAM;
    }

    // Get TX PPS code for this UART module
    status = get_uart_tx_pps_code(uart_module, &tx_pps_code);
    if (status != W_SUCCESS) {
        return status;
    }

    // Unlock PPS registers
    status = pps_unlock();
    if (status != W_SUCCESS) { 
        return status; 
    }

    // Configure TX pin (output)
    status = configure_pin_digital(pin_config.tx, 0U);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure RX pin (input)
    status = configure_pin_digital(pin_config.rx, 1U);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure PPS input mapping
    status = configure_uart_input_pps(uart_module, pin_config.rx);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure PPS output mapping
    status = configure_pps_output(pin_config.tx, tx_pps_code);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

cleanup:
    // Always lock PPS registers before returning
    pps_lock();
    return final_status;
}

w_status_t pps_configure_pwm(uint8_t ccp_module, pwm_pin_config_t pin_config) {
    w_status_t status;
    w_status_t final_status = W_SUCCESS;
    uint8_t pps_code;

    // Validate module number
    if (ccp_module < 1U || ccp_module > 5U) {
        return W_INVALID_PARAM;
    }

    // Get PPS code for this CCP module
    status = get_ccp_pps_code(ccp_module, &pps_code);
    if (status != W_SUCCESS) {
        return status;
    }

    // Unlock PPS registers
    status = pps_unlock();
    if (status != W_SUCCESS) { 
        return status; 
    }

    // Configure output pin (PWM output)
    status = configure_pin_digital(pin_config.output, 0U);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure PPS output mapping
    status = configure_pps_output(pin_config.output, pps_code);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

cleanup:
    // Always lock PPS registers before returning
    pps_lock();
    return final_status;
}

w_status_t pps_configure_external_interrupt(uint8_t int_number, ext_int_pin_config_t pin_config) {
    w_status_t status;
    w_status_t final_status = W_SUCCESS;

    // Validate interrupt number (INT0, INT1, INT2)
    if (int_number > 2U) {
        return W_INVALID_PARAM;
    }

    // Unlock PPS registers
    status = pps_unlock();
    if (status != W_SUCCESS) { 
        return status; 
    }

    // Configure pin as digital input
    status = configure_pin_digital(pin_config.input, 1U);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure PPS input mapping based on interrupt number
    uint8_t port_pin_code = get_port_pin_code(pin_config.input);
    switch (int_number) {
        case 0U:
            INT0PPS = port_pin_code;
            break;
        case 1U:
            INT1PPS = port_pin_code;
            break;
        case 2U:
            INT2PPS = port_pin_code;
            break;
        default:
            final_status = W_INVALID_PARAM;
            goto cleanup;
    }

cleanup:
    // Always lock PPS registers before returning
    pps_lock();
    return final_status;
}

w_status_t pps_configure_timer_clk(uint8_t timer, pin_config_t pin_config) {
    w_status_t status;
    w_status_t final_status = W_SUCCESS;

    // Validate timer number (expanded to support more timers)
    if (timer > 6U) {
        return W_INVALID_PARAM;
    }

    // Unlock PPS registers
    status = pps_unlock();
    if (status != W_SUCCESS) { 
        return status; 
    }

    // Configure pin as digital input
    status = configure_pin_digital(pin_config, 1U);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure PPS input mapping based on timer
    uint8_t port_pin_code = get_port_pin_code(pin_config);
    switch (timer) {
        case 0U:
            T0CKIPPS = port_pin_code;
            break;
        case 1U:
            T1CKIPPS = port_pin_code;
            break;
        case 2U:
            T2CKIPPS = port_pin_code;
            break;
        case 3U:
            T3CKIPPS = port_pin_code;
            break;
        case 4U:
            T4CKIPPS = port_pin_code;
            break;
        case 5U:
            T5CKIPPS = port_pin_code;
            break;
        case 6U:
            T6CKIPPS = port_pin_code;
            break;
        default:
            final_status = W_INVALID_PARAM;
            goto cleanup;
    }

cleanup:
    // Always lock PPS registers before returning
    pps_lock();
    return final_status;
}

w_status_t pps_configure_timer_gate(uint8_t timer, pin_config_t pin_config) {
    w_status_t status;
    w_status_t final_status = W_SUCCESS;

    // Validate timer number
    if (timer > 6U) {
        return W_INVALID_PARAM;
    }

    // Unlock PPS registers
    status = pps_unlock();
    if (status != W_SUCCESS) { 
        return status; 
    }

    // Configure pin as digital input
    status = configure_pin_digital(pin_config, 1U);
    if (status != W_SUCCESS) { 
        final_status = status;
        goto cleanup; 
    }

    // Configure PPS input mapping for timer gate
    uint8_t port_pin_code = get_port_pin_code(pin_config);
    switch (timer) {
        case 0U:
            T0GPPS = port_pin_code;
            break;
        case 1U:
            T1GPPS = port_pin_code;
            break;
        case 2U:
            T2GPPS = port_pin_code;
            break;
        case 3U:
            T3GPPS = port_pin_code;
            break;
        case 4U:
            T4GPPS = port_pin_code;
            break;
        case 5U:
            T5GPPS = port_pin_code;
            break;
        case 6U:
            T6GPPS = port_pin_code;
            break;
        default:
            final_status = W_INVALID_PARAM;
            goto cleanup;
    }

cleanup:
    // Always lock PPS registers before returning
    pps_lock();
    return final_status;
}

