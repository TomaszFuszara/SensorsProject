/*
 * DHT11.h
 *
 *  Created on: Apr 11, 2025
 *      Author: tomek
 */

#ifndef DHT11_H
#define DHT11_H

#include "main.h"

// Define the DHT11 pin and port
#define DHT11_PORT GPIOB
#define DHT11_PIN  GPIO_PIN_9

// Global variables (optional: you could return RH directly instead)
extern uint8_t RHI, RHD, SUM;
extern float RH;

// Function declarations
void microDelay(uint16_t delay);
uint8_t DHT11_Start(void);
uint8_t DHT11_Read(void);
void DHT11_ReadHumidity(void);

#endif
