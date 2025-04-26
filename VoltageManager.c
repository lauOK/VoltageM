#include "VoltageManager.h"

#include <math.h>

static void _VM_ConvertVoltage(VM_AdcConfig_t *adc, uint16_t adval)
{
    uint16_t vol;

    vol = adval * adc->refence / adc->resolution;
    vol *= (float)(adc->divider) / (float)1000.0;
    vol += adc->offset;

    adc->voltage = vol;
}

static void _VM_CheckState(VM_Threshold_t *thre, uint16_t voltage)
{
    uint16_t curstate = thre->state;
    uint16_t newstate = 0;

    for (uint16_t i = 0; i < thre->num; i++)
    {
        if(voltage >= thre->arr[i])
        {
            newstate++;
        }
    }

    if(abs(newstate - curstate) == 1)
    {
        if(voltage > thre->arr[curstate] + thre->hysteresis)
        {
            curstate = newstate;
        }   
        else if(voltage < thre->arr[newstate] - thre->hysteresis)
        {
            curstate = newstate;
        }
        else
        {
            //not change
        }
    }
    else
    {
       curstate = newstate; 
    }
    thre->state = curstate;
}

static void _VM_VolAdcInit(VM_AdcConfig_t *adc)
{
    if(adc->resolution == 0)
    {
        adc->resolution = 1;
    }
}

static void _VM_VolThreInit(VM_Threshold_t *thre)
{
    uint16_t cnt = 0;

    for (uint8_t i = 0; i < VM_THRESHOLD_MAX; i++)
    {
        if(thre->arr[i] != 0)
        {
            cnt++;
        }
    }
    thre->num = cnt;
}

void VM_VoltageManaInit(VM_VoltageMan_t *volm, uint8_t vmnum)
{
    for (uint8_t i = 0; i < vmnum; i++)
    {
        _VM_VolAdcInit(&volm[i].adcconfig);
        _VM_VolThreInit(&volm[i].threshold);
    }    
}

void VM_CheckVoltageState(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *newval)
{
    for (uint8_t i = 0; i < vmnum; i++)
    {   
        _VM_ConvertVoltage(&volm[i].adcconfig, newval[i]);
        _VM_CheckState(&volm[i].threshold, volm[i].adcconfig.voltage);
    }
}

void VM_GetVoltageState(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *state)
{
    for (uint8_t  i = 0; i < vmnum; i++)
    {
        state[i] = volm[i].threshold.state;
    }
}

void VM_GetVoltageValue(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *value)
{
    for (uint8_t  i = 0; i < vmnum; i++)
    {
        value[i] = volm[i].adcconfig.voltage;
    }
}

void VM_SetVmThresholdArr(VM_VoltageMan_t *volm, uint16_t *arr)
{
    uint16_t len = sizeof(arr) / sizeof(uint16_t);

    if(len > VM_THRESHOLD_MAX)
    {
        len = VM_THRESHOLD_MAX;
    }

    volm->threshold.num = len;

    for (uint16_t i = 0; i < len; i++)
    {
        volm->threshold.arr[i] = arr[i];
    }
}

void VM_SetVmThresholdHysteresis(VM_VoltageMan_t *volm, uint16_t hysteresis)
{
    volm->threshold.hysteresis = hysteresis;
}
