/***************************************************************************//**
 * @file
 * @brief PWM Driver Instance Initialization
 *******************************************************************************
 * # License
 * <b>Copyright 2020 Silicon Laboratories Inc. www.silabs.com</b>
 *******************************************************************************
 *
 * SPDX-License-Identifier: Zlib
 *
 * The licensor of this software is Silicon Laboratories Inc.
 *
 * This software is provided 'as-is', without any express or implied
 * warranty. In no event will the authors be held liable for any damages
 * arising from the use of this software.
 *
 * Permission is granted to anyone to use this software for any purpose,
 * including commercial applications, and to alter it and redistribute it
 * freely, subject to the following restrictions:
 *
 * 1. The origin of this software must not be misrepresented; you must not
 *    claim that you wrote the original software. If you use this software
 *    in a product, an acknowledgment in the product documentation would be
 *    appreciated but is not required.
 * 2. Altered source versions must be plainly marked as such, and must not be
 *    misrepresented as being the original software.
 * 3. This notice may not be removed or altered from any source distribution.
 *
 ******************************************************************************/

#include "sl_pwm.h"

#include "sl_pwm_init_pwm0_config.h"

#include "sl_pwm_init_pwm1_config.h"


#include "em_gpio.h"


sl_pwm_instance_t sl_pwm_pwm0 = {
  .timer = SL_PWM_PWM0_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_PWM0_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_PWM0_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_PWM0_OUTPUT_PIN),
#if defined(SL_PWM_PWM0_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_PWM0_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_pwm1 = {
  .timer = SL_PWM_PWM1_PERIPHERAL,
  .channel = (uint8_t)(SL_PWM_PWM1_OUTPUT_CHANNEL),
  .port = (uint8_t)(SL_PWM_PWM1_OUTPUT_PORT),
  .pin = (uint8_t)(SL_PWM_PWM1_OUTPUT_PIN),
#if defined(SL_PWM_PWM1_OUTPUT_LOC)
  .location = (uint8_t)(SL_PWM_PWM1_OUTPUT_LOC),
#endif
};

sl_pwm_instance_t sl_pwm_pwm_r = {
  .timer = TIMER0,
  .channel = (uint8_t)(0),
  .port = (uint8_t)(gpioPortA),
  .pin = (uint8_t)(4),
};

sl_pwm_instance_t sl_pwm_pwm_g = {
  .timer = TIMER0,
  .channel = (uint8_t)(1),
  .port = (uint8_t)(gpioPortD),
  .pin = (uint8_t)(1),
};

sl_pwm_instance_t sl_pwm_pwm_b = {
  .timer = TIMER0,
  .channel = (uint8_t)(2),
  .port = (uint8_t)(gpioPortA),
  .pin = (uint8_t)(3),
};


void sl_pwm_init_instances(void)
{

  GPIO_PinModeSet(gpioPortA,3,
                    gpioModePushPull,
                    0);
  GPIO_PinModeSet(gpioPortD,1,
                    gpioModePushPull,
                    0);
  GPIO_PinModeSet(gpioPortA,4,
                    gpioModePushPull,
                    0);

  GPIO_PinModeSet(gpioPortD,0,
                    gpioModePushPull,
                    0);
  GPIO_PinModeSet(gpioPortC,2,
                    gpioModePushPull,
                    0);

  sl_pwm_config_t pwm_pwm0_config = {
    .frequency = SL_PWM_PWM0_FREQUENCY,
    .polarity = SL_PWM_PWM0_POLARITY,
  };

  sl_pwm_init(&sl_pwm_pwm0, &pwm_pwm0_config);

  sl_pwm_config_t pwm_pwm1_config = {
    .frequency = SL_PWM_PWM1_FREQUENCY,
    .polarity = SL_PWM_PWM1_POLARITY,
  };

  sl_pwm_init(&sl_pwm_pwm1, &pwm_pwm1_config);

  sl_pwm_config_t pwm_pwm_rgb_config = {
     .frequency = 16000,
     .polarity = SL_PWM_PWM1_POLARITY,
   };
  sl_pwm_init(&sl_pwm_pwm_r, &pwm_pwm_rgb_config);
  sl_pwm_init(&sl_pwm_pwm_g, &pwm_pwm_rgb_config);
  sl_pwm_init(&sl_pwm_pwm_b, &pwm_pwm_rgb_config);

}


