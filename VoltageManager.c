#include "VoltageManager.h"
#include <math.h>

/**
 * \brief Convert ADC raw value to voltage
 * \param adc Pointer to ADC configuration structure
 * \param adval Raw ADC value
 */
static void _VM_ConvertVoltage(VM_AdcConfig_t *adc, uint16_t adval)
{
    uint16_t vol;

    /* Voltage calculation steps */
    vol = adval * adc->refence / adc->resolution;
    /* Convert divider ratio */
    vol *= (float)(adc->divider) / (float)1000.0;  
    /* Apply calibration offset */
    vol += adc->offset;  

    adc->voltage = vol;
}

/**
 * \brief Check voltage state with hysteresis
 * \param thre Pointer to threshold configuration
 * \param voltage Calculated voltage value
 */
static void _VM_CheckState(VM_Threshold_t *thre, uint16_t voltage)
{
    uint16_t curstate = thre->state;
    uint16_t newstate = 0;

    /* Determine new state based on threshold array */
    for (uint16_t i = 0; i < thre->num; i++) {
        if(voltage >= thre->arr[i]) {
            newstate++;
        }
    }
    /* Hysteresis handling */
    if(abs(newstate - curstate) == 1) {
        if(voltage > thre->arr[curstate] + thre->hysteresis) {
            curstate = newstate;
        }   
        else if(voltage < thre->arr[newstate] - thre->hysteresis) {
            curstate = newstate;
        }
    } else {
        curstate = newstate; 
    }
    thre->state = curstate;
}

/**
 * \brief Initialize ADC configuration
 * \param adc Pointer to ADC configuration structure
 */
static void _VM_VolAdcInit(VM_AdcConfig_t *adc)
{
    /* Ensure non-zero resolution */
    if(adc->resolution == 0) {
        adc->resolution = 1;
    }
}

/**
 * \brief Initialize threshold configuration
 * \param thre Pointer to threshold structure
 */
static void _VM_VolThreInit(VM_Threshold_t *thre)
{
    uint16_t cnt = 0;

    /* Count non-zero threshold values */
    for (uint8_t i = 0; i < VM_THRESHOLD_MAX; i++) {
        if(thre->arr[i] != 0) {
            cnt++;
        }
    }
    thre->num = cnt;
}

/**
 * \brief Initialize voltage manager instances
 * \param volm Pointer to voltage manager array
 * \param vmnum Number of manager instances
 */
void VM_VoltageManaInit(VM_VoltageMan_t *volm, uint8_t vmnum)
{
    for (uint8_t i = 0; i < vmnum; i++) {
        _VM_VolAdcInit(&volm[i].adcconfig);
        _VM_VolThreInit(&volm[i].threshold);
    }    
}

/**
 * \brief Check voltage states for all manager instances
 * \param volm Voltage manager array pointer
 * \param vmnum Number of manager instances
 * \param newval Array of new ADC raw values
 */
void VM_CheckVoltageState(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *newval)
{
    for (uint8_t i = 0; i < vmnum; i++)
    {   
        _VM_ConvertVoltage(&volm[i].adcconfig, newval[i]);
        _VM_CheckState(&volm[i].threshold, volm[i].adcconfig.voltage);
    }
}

/**
 * \brief Get current voltage states
 * \param volm Voltage manager array pointer
 * \param vmnum Number of manager instances
 * \param state Output array for storing states
 */
void VM_GetVoltageState(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *state)
{
    for (uint8_t  i = 0; i < vmnum; i++)
    {
        state[i] = volm[i].threshold.state;
    }
}

/**
 * \brief Get current voltage values
 * \param volm Voltage manager array pointer
 * \param vmnum Number of manager instances
 * \param value Output array for storing voltage values
 */
void VM_GetVoltageValue(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *value)
{
    for (uint8_t  i = 0; i < vmnum; i++)
    {
        value[i] = volm[i].adcconfig.voltage;
    }
}

/**
 * \brief Set threshold array for voltage manager
 * \param volm Voltage manager instance pointer
 * \param arr Threshold array pointer
 * \note Array length will be auto-detected (max VM_THRESHOLD_MAX)
 */
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

/**
 * \brief Set hysteresis value for threshold
 * \param volm Voltage manager instance pointer
 * \param hysteresis Hysteresis value (in mV)
 */
void VM_SetVmThresholdHysteresis(VM_VoltageMan_t *volm, uint16_t hysteresis)
{
    volm->threshold.hysteresis = hysteresis;
}
