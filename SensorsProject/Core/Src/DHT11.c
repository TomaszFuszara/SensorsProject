#include "DHT11.h"
#include <stdio.h>

uint8_t RHI, RHD, SUM;
float RH = 0;

extern TIM_HandleTypeDef htim1;
uint32_t pMillis, cMillis;

void microDelay(uint16_t delay)
{
    __HAL_TIM_SET_COUNTER(&htim1, 0);
    while (__HAL_TIM_GET_COUNTER(&htim1) < delay);
}

uint8_t DHT11_Start(void)
{
    uint8_t Response = 0;
    GPIO_InitTypeDef GPIO_InitStructPrivate = {0};

    GPIO_InitStructPrivate.Pin = DHT11_PIN;
    GPIO_InitStructPrivate.Mode = GPIO_MODE_OUTPUT_PP;
    GPIO_InitStructPrivate.Speed = GPIO_SPEED_FREQ_LOW;
    GPIO_InitStructPrivate.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructPrivate);

    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, 0);
    HAL_Delay(20);
    HAL_GPIO_WritePin(DHT11_PORT, DHT11_PIN, 1);
    microDelay(30);

    GPIO_InitStructPrivate.Mode = GPIO_MODE_INPUT;
    GPIO_InitStructPrivate.Pull = GPIO_PULLUP;
    HAL_GPIO_Init(DHT11_PORT, &GPIO_InitStructPrivate);

    microDelay(40);
    if (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
    {
        microDelay(80);
        if (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN)) Response = 1;
    }

    pMillis = HAL_GetTick();
    cMillis = HAL_GetTick();
    while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) && pMillis + 2 > cMillis)
    {
        cMillis = HAL_GetTick();
    }

    return Response;
}

uint8_t DHT11_Read(void)
{
    uint8_t a, b = 0;
    for (a = 0; a < 8; a++)
    {
        pMillis = HAL_GetTick();
        cMillis = HAL_GetTick();
        while (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) && pMillis + 2 > cMillis)
        {
            cMillis = HAL_GetTick();
        }

        microDelay(40);
        if (!HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN))
            b &= ~(1 << (7 - a));
        else
            b |= (1 << (7 - a));

        pMillis = HAL_GetTick();
        cMillis = HAL_GetTick();
        while (HAL_GPIO_ReadPin(DHT11_PORT, DHT11_PIN) && pMillis + 2 > cMillis)
        {
            cMillis = HAL_GetTick();
        }
    }
    return b;
}

void DHT11_ReadHumidity(void)
{
    if (DHT11_Start())
    {
        RHI = DHT11_Read();
        RHD = DHT11_Read();
        uint8_t skip1 = DHT11_Read();
        uint8_t skip2 = DHT11_Read();
        SUM = DHT11_Read();

        uint8_t checksum = (RHI + RHD + skip1 + skip2) & 0xFF;

        if (checksum == SUM)
        {
            RH = (float)RHI + (float)(RHD / 10.0);
        }
        else
        {
            RH = 0.0;
            printf("Checksum mismatch. Invalid data.\r\n");
        }
    }
    else
    {
        RH = 0.0;
        printf("Sensor did not respond.\r\n");
    }
}
