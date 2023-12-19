#ifndef PIN_CONFIG_H
#define PIN_CONFIG_H

// $[CMU]
// [CMU]$

// $[LFXO]
// [LFXO]$

// $[PRS.ASYNCH0]
// [PRS.ASYNCH0]$

// $[PRS.ASYNCH1]
// [PRS.ASYNCH1]$

// $[PRS.ASYNCH2]
// [PRS.ASYNCH2]$

// $[PRS.ASYNCH3]
// [PRS.ASYNCH3]$

// $[PRS.ASYNCH4]
// [PRS.ASYNCH4]$

// $[PRS.ASYNCH5]
// [PRS.ASYNCH5]$

// $[PRS.ASYNCH6]
// [PRS.ASYNCH6]$

// $[PRS.ASYNCH7]
// [PRS.ASYNCH7]$

// $[PRS.ASYNCH8]
// [PRS.ASYNCH8]$

// $[PRS.ASYNCH9]
// [PRS.ASYNCH9]$

// $[PRS.ASYNCH10]
// [PRS.ASYNCH10]$

// $[PRS.ASYNCH11]
// [PRS.ASYNCH11]$

// $[PRS.SYNCH0]
// [PRS.SYNCH0]$

// $[PRS.SYNCH1]
// [PRS.SYNCH1]$

// $[PRS.SYNCH2]
// [PRS.SYNCH2]$

// $[PRS.SYNCH3]
// [PRS.SYNCH3]$

// $[GPIO]
// GPIO SWV on PA03
#ifndef GPIO_SWV_PORT                           
#define GPIO_SWV_PORT                            gpioPortA
#endif
#ifndef GPIO_SWV_PIN                            
#define GPIO_SWV_PIN                             3
#endif

// [GPIO]$

// $[TIMER0]
// TIMER0 CC0 on PC02
#ifndef TIMER0_CC0_PORT                         
#define TIMER0_CC0_PORT                          gpioPortC
#endif
#ifndef TIMER0_CC0_PIN                          
#define TIMER0_CC0_PIN                           2
#endif

// [TIMER0]$

// $[TIMER1]
// TIMER1 CC1 on PD00
#ifndef TIMER1_CC1_PORT                         
#define TIMER1_CC1_PORT                          gpioPortD
#endif
#ifndef TIMER1_CC1_PIN                          
#define TIMER1_CC1_PIN                           0
#endif

// [TIMER1]$

// $[TIMER2]
// [TIMER2]$

// $[TIMER3]
// TIMER3 CC0 on PC02
#ifndef TIMER3_CC0_PORT                         
#define TIMER3_CC0_PORT                          gpioPortC
#endif
#ifndef TIMER3_CC0_PIN                          
#define TIMER3_CC0_PIN                           2
#endif

// [TIMER3]$

// $[USART0]
// USART0 RX on PA06
#ifndef USART0_RX_PORT                          
#define USART0_RX_PORT                           gpioPortA
#endif
#ifndef USART0_RX_PIN                           
#define USART0_RX_PIN                            6
#endif

// USART0 TX on PA05
#ifndef USART0_TX_PORT                          
#define USART0_TX_PORT                           gpioPortA
#endif
#ifndef USART0_TX_PIN                           
#define USART0_TX_PIN                            5
#endif

// [USART0]$

// $[USART1]
// [USART1]$

// $[USART2]
// [USART2]$

// $[I2C1]
// [I2C1]$

// $[LETIMER0]
// [LETIMER0]$

// $[IADC0]
// [IADC0]$

// $[ACMP0]
// [ACMP0]$

// $[ACMP1]
// [ACMP1]$

// $[I2C0]
// [I2C0]$

// $[PTI]
// [PTI]$

// $[MODEM]
// [MODEM]$

// $[CUSTOM_PIN_NAME]
#ifndef key3_PORT                               
#define key3_PORT                                gpioPortB
#endif
#ifndef key3_PIN                                
#define key3_PIN                                 1
#endif

// [CUSTOM_PIN_NAME]$

#endif // PIN_CONFIG_H

