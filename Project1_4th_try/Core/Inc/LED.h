#ifndef __NUCLEO476_LED_H
#define __NUCLEO476_LED_H

#include "main.h"

void LED_Init(void);
void LED_Off(void);
void LED_On(void);
void LED_Toggle(void);

void led_init(void);
void led_set(_Bool isOn );
_Bool led_isOn(void );

#endif /* __NUCLEO476_LED_H */
