/***************************************************************************//**
 * @file
 * @brief PWM Driver
 *******************************************************************************
 * # License
 * <b>Copyright 2022 Silicon Laboratories Inc. www.silabs.com</b>
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

#ifndef PWM_INIT_PWM1_CONFIG_H
#define PWM_INIT_PWM1_CONFIG_H

#ifdef __cplusplus
extern "C"
{
#endif

// <<< Use Configuration Wizard in Context Menu >>>

// <h>PWM configuration

// <o SL_PWM_PWM1_FREQUENCY> PWM frequency [Hz]
// <i> Default: 10000
#define SL_PWM_PWM1_FREQUENCY       16000

// <o SL_PWM_PWM1_POLARITY> Polarity
// <PWM_ACTIVE_HIGH=> Active high
// <PWM_ACTIVE_LOW=> Active low
// <i> Default: PWM_ACTIVE_HIGH
#define SL_PWM_PWM1_POLARITY        PWM_ACTIVE_HIGH
// </h> end pwm configuration

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <timer channel=OUTPUT> SL_PWM_PWM1
// $[TIMER_SL_PWM_PWM1]
#ifndef SL_PWM_PWM1_PERIPHERAL                  
#define SL_PWM_PWM1_PERIPHERAL                   TIMER1
#endif
#ifndef SL_PWM_PWM1_PERIPHERAL_NO               
#define SL_PWM_PWM1_PERIPHERAL_NO                1
#endif

#ifndef SL_PWM_PWM1_OUTPUT_CHANNEL              
#define SL_PWM_PWM1_OUTPUT_CHANNEL               1
#endif
// TIMER1 CC1 on PD00
#ifndef SL_PWM_PWM1_OUTPUT_PORT                 
#define SL_PWM_PWM1_OUTPUT_PORT                  gpioPortD
#endif
#ifndef SL_PWM_PWM1_OUTPUT_PIN                  
#define SL_PWM_PWM1_OUTPUT_PIN                   0
#endif
// [TIMER_SL_PWM_PWM1]$

// <<< sl:end pin_tool >>>

#ifdef __cplusplus
}
#endif

#endif // PWM_INIT_PWM1_CONFIG_H
