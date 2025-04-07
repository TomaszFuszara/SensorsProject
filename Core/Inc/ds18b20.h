/*
 * ds18b20.h
 *
 *  Created on: Apr 7, 2025
 *      Author: tomek
 */
#pragma once
#include "stm32f1xx.h"

#ifndef INC_DS18B20_H_

#define DS18B20_ROM_CODE_SIZE		8
// Inicjalizacja 1-wire
// return - HAL_OK/HAL_ERROR
HAL_StatusTypeDef ds18b20_init(void);
// Odczytaj adres czujnika i policz CRC
// rom_code - miejsce do zapisu odczytanych danych
// return - HAL_OK/HAL_ERROR
HAL_StatusTypeDef ds18b20_read_address(uint8_t* rom_code);
// Rozpocznij pomiar temperatury
// rom_code - adres czujnika lub NULL
// return - HAL_OK/HAL_ERROR
HAL_StatusTypeDef ds18b20_start_measure(const uint8_t* rom_code);
// Pobierz temperaturę
// rom_code - adres czujnika lub NULL
// return - temperatura w stopniach Celsjusza
float ds18b20_get_temp(const uint8_t* rom_code);

#define INC_DS18B20_H_



#endif /* INC_DS18B20_H_ */
