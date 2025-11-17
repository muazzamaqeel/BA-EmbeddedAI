/**
  ******************************************************************************
  * @file    gt911_conf.h
  * @brief   Specific configuration for GT911 Touch Controller on STM32N6570-DK
  ******************************************************************************
  */

  #ifndef GT911_CONF_H
  #define GT911_CONF_H
  
  #ifdef __cplusplus
  extern "C" {
  #endif
  
  /* Enable/Disable auto calibration */
  #define GT911_AUTO_CALIBRATION_ENABLED      0U
  
  /* LCD resolution (depends on your panel) */
  #define GT911_MAX_X_LENGTH                  800U
  #define GT911_MAX_Y_LENGTH                  480U
  
  /* I2C bus used by GT911 on STM32N6570-DK */
  #define GT911_I2C_ADDRESS                   0x28U  /* GT911 default is 0x28/0x29 depending on pin */
  #define GT911_I2C_HANDLE                    hi2c4  /* the BSP usually uses I2C4 for touchscreen */
  
  /* Optional: reset and interrupt pins (as routed on the DK board) */
  #define GT911_RST_PORT                      GPIOH
  #define GT911_RST_PIN                       GPIO_PIN_2
  
  #define GT911_INT_PORT                      GPIOH
  #define GT911_INT_PIN                       GPIO_PIN_3
  
  #ifdef __cplusplus
  }
  #endif
  #endif /* GT911_CONF_H */
  