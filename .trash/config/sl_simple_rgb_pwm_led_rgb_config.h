/***************************************************************************//**
 * @file
 * @brief Simple RGB PWM Led Driver Configuration
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

#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_CONFIG_H
#define SL_SIMPLE_RGB_PWM_LED_RGB_CONFIG_H

// <<< Use Configuration Wizard in Context Menu >>>

// <h>Simple RGB PWM LED Configuration
// <o SL_SIMPLE_RGB_PWM_LED_RGB_FREQUENCY> PWM frequency [Hz]
// <i> Sets the frequency of the PWM signal
// <i> 0 = Don't care
// <i> Default: 10000
#define SL_SIMPLE_RGB_PWM_LED_RGB_FREQUENCY      10000

// <o SL_SIMPLE_RGB_PWM_LED_RGB_RESOLUTION> PWM resolution <2-65536>
// <i> Specifies the PWM (dimming) resolution. I.e. if you want a
// <i> dimming resolution that takes the input values from 0 to 99,
// <i> set this value to 100
// <i> Default: 256
#define SL_SIMPLE_RGB_PWM_LED_RGB_RESOLUTION     256

// <o SL_SIMPLE_RGB_PWM_LED_RGB_RED_POLARITY> Red LED Polarity
// <SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_LOW=> Active low
// <SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_HIGH=> Active high
// <i> Default: SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_LOW
#define SL_SIMPLE_RGB_PWM_LED_RGB_RED_POLARITY   SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_LOW

// <o SL_SIMPLE_RGB_PWM_LED_RGB_GREEN_POLARITY> Green LED Polarity
// <SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_LOW=> Active low
// <SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_HIGH=> Active high
// <i> Default: SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_LOW
#define SL_SIMPLE_RGB_PWM_LED_RGB_GREEN_POLARITY SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_LOW

// <o SL_SIMPLE_RGB_PWM_LED_RGB_BLUE_POLARITY> Blue LED Polarity
// <SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_LOW=> Active low
// <SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_HIGH=> Active high
// <i> Default: SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_LOW
#define SL_SIMPLE_RGB_PWM_LED_RGB_BLUE_POLARITY  SL_SIMPLE_RGB_PWM_LED_POLARITY_ACTIVE_LOW
// </h> end led configuration

// <<< end of configuration section >>>

// <<< sl:start pin_tool >>>

// <timer channel=RED,GREEN,BLUE> SL_SIMPLE_RGB_PWM_LED_RGB
// $[TIMER_SL_SIMPLE_RGB_PWM_LED_RGB]
#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_PERIPHERAL    
#define SL_SIMPLE_RGB_PWM_LED_RGB_PERIPHERAL     TIMER0
#endif
#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_PERIPHERAL_NO 
#define SL_SIMPLE_RGB_PWM_LED_RGB_PERIPHERAL_NO  0
#endif

#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_RED_CHANNEL   
#define SL_SIMPLE_RGB_PWM_LED_RGB_RED_CHANNEL    2
#endif
// TIMER0 CC2 on PA04
#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_RED_PORT      
#define SL_SIMPLE_RGB_PWM_LED_RGB_RED_PORT       gpioPortA
#endif
#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_RED_PIN       
#define SL_SIMPLE_RGB_PWM_LED_RGB_RED_PIN        4
#endif

#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_GREEN_CHANNEL 
#define SL_SIMPLE_RGB_PWM_LED_RGB_GREEN_CHANNEL  1
#endif
// TIMER0 CC1 on PD01
#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_GREEN_PORT    
#define SL_SIMPLE_RGB_PWM_LED_RGB_GREEN_PORT     gpioPortD
#endif
#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_GREEN_PIN     
#define SL_SIMPLE_RGB_PWM_LED_RGB_GREEN_PIN      1
#endif

#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_BLUE_CHANNEL  
#define SL_SIMPLE_RGB_PWM_LED_RGB_BLUE_CHANNEL   0
#endif
// TIMER0 CC0 on PA00
#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_BLUE_PORT     
#define SL_SIMPLE_RGB_PWM_LED_RGB_BLUE_PORT      gpioPortA
#endif
#ifndef SL_SIMPLE_RGB_PWM_LED_RGB_BLUE_PIN      
#define SL_SIMPLE_RGB_PWM_LED_RGB_BLUE_PIN       0
#endif
// [TIMER_SL_SIMPLE_RGB_PWM_LED_RGB]$

// <<< sl:end pin_tool >>>

#endif // SL_SIMPLE_RGB_PWM_LED_RGB_CONFIG_H
