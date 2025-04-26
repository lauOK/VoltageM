#include <stdio.h>
#include <stdint.h>

#include "VoltageManager.h"

enum {
    VOLTAGE_SHUTDOWN = 0,
    VOLTAGE_LOW,
    VOLTAGE_NORMAL,
    VOLTAGE_HIGH,
    VOLTAGE_OVER,
};

static VM_VoltageMan_t VolMa = {
    .adcconfig.refence = 5000u,
    .adcconfig.resolution = 4095u,
    .adcconfig.divider = 6000u,
    .adcconfig.offset = 0u,

    .threshold.hysteresis = 500,
    .threshold.arr = {6000, 9000, 16000, 20000},
};

static uint16_t samplevalue = 5;

int main(void)
{
    uint16_t state = 0;
    uint16_t voltage = 0;

    /* Initialize voltage manager */
    VM_VoltageManaInit(&VolMa, 1);

    for (uint8_t i = 0; i < 20; i++)
    {
        samplevalue += 200;

        printf("Cycle++ %02d :\n", i);

        /* Check voltage state */
        VM_CheckVoltageState(&VolMa, 1, &samplevalue);
        
        /* Get voltage state and value */
        VM_GetVoltageState(&VolMa, 1, &state);
        VM_GetVoltageValue(&VolMa, 1, &voltage);

        printf("Sample : %d, voltage : %d mv  ", samplevalue, voltage);
        
        switch (state)
        {
            case VOLTAGE_SHUTDOWN:  printf("VOLTAGE_SHUTDOWN\n"); break;
            case VOLTAGE_LOW:       printf("VOLTAGE_LOW\n"); break;
            case VOLTAGE_NORMAL:    printf("VOLTAGE_NORMAL\n"); break;
            case VOLTAGE_HIGH:      printf("VOLTAGE_HIGH\n"); break;
            case VOLTAGE_OVER:      printf("VOLTAGE_OVER\n"); break;
            default: break;
        }
        printf("**************************************************************\n");
    }

    for (uint8_t i = 0; i < 20; i++)
    {
        samplevalue -= 200;

        printf("Cycle-- %02d :\n", i);

        VM_CheckVoltageState(&VolMa, 1, &samplevalue);
        VM_GetVoltageState(&VolMa, 1, &state);
        VM_GetVoltageValue(&VolMa, 1, &voltage);

        printf("Sample : %d, voltage : %d mV  ", samplevalue, voltage);
        
        switch (state)
        {
            case VOLTAGE_SHUTDOWN:  printf("VOLTAGE_SHUTDOWN\n"); break;
            case VOLTAGE_LOW:       printf("VOLTAGE_LOW\n"); break;
            case VOLTAGE_NORMAL:    printf("VOLTAGE_NORMAL\n"); break;
            case VOLTAGE_HIGH:      printf("VOLTAGE_HIGH\n"); break;
            case VOLTAGE_OVER:      printf("VOLTAGE_OVER\n"); break;
            default: break;
        }
        printf("**************************************************************\n");
    }

    return 0;
}
