#ifndef ROCKETLIB_STM32H7_CAN_H
#define ROCKETLIB_STM32H7_CAN_H

#include <stdbool.h>

#if (defined(STM32H503xx) || defined(STM32H523xx))
#include "stm32h5xx_hal.h"
#elif (defined(STM32H723xx) || defined(STM32H733xx) || defined(STM32H750xx) || defined(STM32H753xx))
#include "stm32h7xx_hal.h"
#else
#error "rocketlib/stm32_common/stm32_can: unsupported MCU type"
#endif

#include "can_message/can.h"

#ifdef __cplusplus
extern "C" {
#endif

/**
 * @brief Start STM32 CAN Controller
 *
 * @param handle pointer to FDCAN_HandleTypeDef struct in `main.c`
 * @param receive_callback CAN message receive handler, the function would be called when a CAN
 * message is received
 * @return `true` if succeed
 * @note This depends on STM32CubeMX generated HAL code has already initialized CAN controller
 */
bool stm32_can_init(FDCAN_HandleTypeDef *handle,
                    void (*receive_callback)(const can_msg_t *message));

/**
 * @brief Send a CAN message
 *
 * @param message CAN message to be sent
 * @return `true` if succeed
 */
bool stm32_can_send(const can_msg_t *message);

/**
 * @brief Check if CAN controller is ready to send a message
 *
 * @return `true` if ready to send a message
 */
bool stm32_can_send_rdy(void);

#ifdef __cplusplus
}
#endif

#endif
