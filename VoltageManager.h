#ifndef _VOLTAGE_M
#define _VOLTAGE_M

#include <stdint.h>

#define VM_THRESHOLD_MAX (10u)

typedef struct
{
    uint16_t num;
    uint16_t arr[VM_THRESHOLD_MAX];
    uint16_t hysteresis;
    uint16_t state;
}VM_Threshold_t;

typedef struct
{
    uint16_t resolution;
    uint16_t refence;
    uint16_t divider;
    uint16_t offset; 
    uint16_t voltage;
}VM_AdcConfig_t;

typedef struct
{   
    VM_Threshold_t threshold;
    VM_AdcConfig_t adcconfig;
}VM_VoltageMan_t;

void VM_VoltageManaInit(VM_VoltageMan_t *volm, uint8_t vmnum);
void VM_CheckVoltageState(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *newval);
void VM_GetVoltageState(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *state);
void VM_GetVoltageValue(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *value);
void VM_SetVmThresholdArr(VM_VoltageMan_t *volm, uint16_t *arr);
void VM_SetVmThresholdHysteresis(VM_VoltageMan_t *volm, uint16_t hysteresis);

#endif
