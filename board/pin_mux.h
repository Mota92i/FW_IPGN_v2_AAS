/***********************************************************************************************************************
 * This file was generated by the MCUXpresso Config Tools. Any manual edits made to this file
 * will be overwritten if the respective MCUXpresso Config Tools is used to update this file.
 **********************************************************************************************************************/

#ifndef _PIN_MUX_H_
#define _PIN_MUX_H_

/*!
 * @addtogroup pin_mux
 * @{
 */

/***********************************************************************************************************************
 * API
 **********************************************************************************************************************/

#if defined(__cplusplus)
extern "C" {
#endif

/*!
 * @brief Calls initialization functions.
 *
 */
void BOARD_InitBootPins(void);

#define SOPT2_RMIISRC_EXTAL 0x00u      /*!<@brief RMII clock source select: EXTAL clock */
#define SOPT4_FTM2CH0SRC_FTM 0x00u     /*!<@brief FTM2 channel 0 input capture source select: FTM2_CH0 signal */
#define SOPT5_UART0TXSRC_UART_TX 0x00u /*!<@brief UART 0 transmit data source select: UART0_TX pin */

/*! @name PORTD1 (number 94), SPI0_SCK
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_SPI0_SCK_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_SPI0_SCK_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_SPI0_SCK_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                         /* @} */

/*! @name PORTD0 (number 93), SPI0_CS
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_SPI0_CS_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_SPI0_CS_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_SPI0_CS_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                        /* @} */

/*! @name PORTB16 (number 62), RST_ETH
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_RST_ETH_GPIO GPIOB                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_RST_ETH_GPIO_PIN_MASK (1U << 16U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_RST_ETH_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_RST_ETH_PIN 16U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_RST_ETH_PIN_MASK (1U << 16U)      /*!<@brief PORT pin mask */
                                                         /* @} */

/*! @name PORTB2 (number 55), CDE_LUM_ZONE_3
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_CDE_LUM_ZONE_3_GPIO GPIOB               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_CDE_LUM_ZONE_3_GPIO_PIN_MASK (1U << 2U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_CDE_LUM_ZONE_3_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_CDE_LUM_ZONE_3_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_CDE_LUM_ZONE_3_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                               /* @} */

/*! @name PORTB3 (number 56), CDE_LUM_ZONE_2
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_CDE_LUM_ZONE_2_GPIO GPIOB               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_CDE_LUM_ZONE_2_GPIO_PIN_MASK (1U << 3U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_CDE_LUM_ZONE_2_PORT PORTB               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_CDE_LUM_ZONE_2_PIN 3U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_CDE_LUM_ZONE_2_PIN_MASK (1U << 3U)      /*!<@brief PORT pin mask */
                                                               /* @} */

/*! @name PORTC18 (number 92), UART0_BREAK_DETECT_IN
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_UART0_BREAK_DETECT_IN_GPIO GPIOC                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_UART0_BREAK_DETECT_IN_GPIO_PIN_MASK (1U << 18U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_UART0_BREAK_DETECT_IN_PORT PORTC                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_UART0_BREAK_DETECT_IN_PIN 18U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_UART0_BREAK_DETECT_IN_PIN_MASK (1U << 18U)      /*!<@brief PORT pin mask */
                                                                       /* @} */

/*! @name PORTD7 (number 100), UART0_TX_OUTPUT
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_UART0_TX_OUTPUT_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_UART0_TX_OUTPUT_PIN 7U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_UART0_TX_OUTPUT_PIN_MASK (1U << 7U)      /*!<@brief PORT pin mask */
                                                                /* @} */

/*! @name PORTB10 (number 58), CDE_LUM_ZONE_1
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_CDE_LUM_ZONE_1_GPIO GPIOB                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_CDE_LUM_ZONE_1_GPIO_PIN_MASK (1U << 10U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_CDE_LUM_ZONE_1_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_CDE_LUM_ZONE_1_PIN 10U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_CDE_LUM_ZONE_1_PIN_MASK (1U << 10U)      /*!<@brief PORT pin mask */
                                                                /* @} */

/*! @name PORTB11 (number 59), REG_LUM_GEN
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_REG_LUM_GEN_GPIO GPIOB                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_REG_LUM_GEN_GPIO_PIN_MASK (1U << 11U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_REG_LUM_GEN_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_REG_LUM_GEN_PIN 11U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_REG_LUM_GEN_PIN_MASK (1U << 11U)      /*!<@brief PORT pin mask */
                                                             /* @} */

/*! @name PORTC17 (number 91), LUM_ZONE_3
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LUM_ZONE_3_GPIO GPIOC                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LUM_ZONE_3_GPIO_PIN_MASK (1U << 17U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_LUM_ZONE_3_PORT PORTC                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LUM_ZONE_3_PIN 17U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LUM_ZONE_3_PIN_MASK (1U << 17U)      /*!<@brief PORT pin mask */
                                                            /* @} */

/*! @name PORTC16 (number 90), LUM_ZONE_2
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LUM_ZONE_2_GPIO GPIOC                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LUM_ZONE_2_GPIO_PIN_MASK (1U << 16U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_LUM_ZONE_2_PORT PORTC                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LUM_ZONE_2_PIN 16U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LUM_ZONE_2_PIN_MASK (1U << 16U)      /*!<@brief PORT pin mask */
                                                            /* @} */

/*! @name PORTC15 (number 87), LUM_ZONE_1
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LUM_ZONE_1_GPIO GPIOC                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LUM_ZONE_1_GPIO_PIN_MASK (1U << 15U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_LUM_ZONE_1_PORT PORTC                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LUM_ZONE_1_PIN 15U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LUM_ZONE_1_PIN_MASK (1U << 15U)      /*!<@brief PORT pin mask */
                                                            /* @} */

/*! @name PORTC14 (number 86), LUM_GEN
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_LUM_GEN_GPIO GPIOC                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_LUM_GEN_GPIO_PIN_MASK (1U << 14U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_LUM_GEN_PORT PORTC                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LUM_GEN_PIN 14U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LUM_GEN_PIN_MASK (1U << 14U)      /*!<@brief PORT pin mask */
                                                         /* @} */

/*! @name PORTB22 (number 68), VOLTMETRE_VOIE
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_VOLTMETRE_VOIE_GPIO GPIOB                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_VOLTMETRE_VOIE_GPIO_PIN_MASK (1U << 22U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_VOLTMETRE_VOIE_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_VOLTMETRE_VOIE_PIN 22U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_VOLTMETRE_VOIE_PIN_MASK (1U << 22U)      /*!<@brief PORT pin mask */
                                                                /* @} */

/*! @name PORTB23 (number 69), VOLTMETRE_READ_BASCULE
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_VOLTMETRE_READ_BASCULE_GPIO GPIOB                /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_VOLTMETRE_READ_BASCULE_GPIO_PIN_MASK (1U << 23U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_VOLTMETRE_READ_BASCULE_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_VOLTMETRE_READ_BASCULE_PIN 23U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_VOLTMETRE_READ_BASCULE_PIN_MASK (1U << 23U)      /*!<@brief PORT pin mask */
                                                                        /* @} */

/*! @name PORTB19 (number 65), LUM_PWM
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_LUM_PWM_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_LUM_PWM_PIN 19U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_LUM_PWM_PIN_MASK (1U << 19U)      /*!<@brief PORT pin mask */
                                                         /* @} */

/*! @name PORTB18 (number 64), MOTOR_CLK
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_MOTOR_CLK_PORT PORTB                /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_MOTOR_CLK_PIN 18U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_MOTOR_CLK_PIN_MASK (1U << 18U)      /*!<@brief PORT pin mask */
                                                           /* @} */

/*! @name PORTC0 (number 70), MOTOR_YELLOW
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_MOTOR_YELLOW_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_MOTOR_YELLOW_GPIO_PIN_MASK (1U << 0U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_MOTOR_YELLOW_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_MOTOR_YELLOW_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_MOTOR_YELLOW_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                             /* @} */

/*! @name PORTC1 (number 71), MOTOR_RED
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_MOTOR_RED_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_MOTOR_RED_GPIO_PIN_MASK (1U << 1U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_MOTOR_RED_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_MOTOR_RED_PIN 1U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_MOTOR_RED_PIN_MASK (1U << 1U)      /*!<@brief PORT pin mask */
                                                          /* @} */

/*! @name PORTC2 (number 72), MOTOR_BLUE
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_MOTOR_BLUE_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_MOTOR_BLUE_GPIO_PIN_MASK (1U << 2U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_MOTOR_BLUE_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_MOTOR_BLUE_PIN 2U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_MOTOR_BLUE_PIN_MASK (1U << 2U)      /*!<@brief PORT pin mask */
                                                           /* @} */

/*! @name PORTC3 (number 73), MOTOR_ORANGE
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_MOTOR_ORANGE_GPIO GPIOC               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_MOTOR_ORANGE_GPIO_PIN_MASK (1U << 3U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_MOTOR_ORANGE_PORT PORTC               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_MOTOR_ORANGE_PIN 3U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_MOTOR_ORANGE_PIN_MASK (1U << 3U)      /*!<@brief PORT pin mask */
                                                             /* @} */

/*! @name PORTD5 (number 98), VOLTMETRE_INPUT_CAPTURE
  @{ */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_VOLTMETRE_INPUT_CAPTURE_PORT PORTD               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_VOLTMETRE_INPUT_CAPTURE_PIN 5U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_VOLTMETRE_INPUT_CAPTURE_PIN_MASK (1U << 5U)      /*!<@brief PORT pin mask */
                                                                        /* @} */

/*! @name PORTE0 (number 1), DEBUG_PORT_E0
  @{ */

/* Symbols to be used with GPIO driver */
#define BOARD_INITPINS_DEBUG_PORT_E0_GPIO GPIOE               /*!<@brief GPIO peripheral base pointer */
#define BOARD_INITPINS_DEBUG_PORT_E0_GPIO_PIN_MASK (1U << 0U) /*!<@brief GPIO pin mask */

/* Symbols to be used with PORT driver */
#define BOARD_INITPINS_DEBUG_PORT_E0_PORT PORTE               /*!<@brief PORT peripheral base pointer */
#define BOARD_INITPINS_DEBUG_PORT_E0_PIN 0U                   /*!<@brief PORT pin number */
#define BOARD_INITPINS_DEBUG_PORT_E0_PIN_MASK (1U << 0U)      /*!<@brief PORT pin mask */
                                                              /* @} */

/*!
 * @brief Configures pin routing and optionally pin electrical features.
 *
 */
void BOARD_InitPins(void);

#if defined(__cplusplus)
}
#endif

/*!
 * @}
 */
#endif /* _PIN_MUX_H_ */

/***********************************************************************************************************************
 * EOF
 **********************************************************************************************************************/
