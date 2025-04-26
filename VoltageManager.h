#ifndef _VOLTAGE_M
#define _VOLTAGE_M

#include <stdint.h>

/** \brief Maximum number of voltage thresholds */
#define VM_THRESHOLD_MAX (10u)

/**
 * \brief Voltage threshold configuration structure
 */
typedef struct
{
    uint16_t num;           /**< Number of valid thresholds */
    uint16_t arr[VM_THRESHOLD_MAX]; /**< Threshold array (unit: mV) */
    uint16_t hysteresis;    /**< Hysteresis value (unit: mV) */
    uint16_t state;         /**< Current voltage state */
} VM_Threshold_t;

/**
 * \brief ADC configuration structure
 */
typedef struct
{
    uint16_t resolution;    /**< ADC resolution (bits) */
    uint16_t refence;       /**< Reference voltage (mV) */
    uint16_t divider;       /**< Voltage divider ratio (kΩ) */
    uint16_t offset;        /**< Calibration offset (mV) */
    uint16_t voltage;       /**< Calculated voltage value (mV) */
} VM_AdcConfig_t;

/**
 * \brief Voltage manager composite structure
 */
typedef struct
{   
    VM_Threshold_t threshold;   /**< Threshold configuration */
    VM_AdcConfig_t adcconfig;   /**< ADC configuration */
} VM_VoltageMan_t;

/* Public API declarations */
void VM_VoltageManaInit(VM_VoltageMan_t *volm, uint8_t vmnum);
void VM_CheckVoltageState(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *newval);
void VM_GetVoltageState(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *state);
void VM_GetVoltageValue(VM_VoltageMan_t *volm, uint8_t vmnum, uint16_t *value);
void VM_SetVmThresholdArr(VM_VoltageMan_t *volm, uint16_t *arr);
void VM_SetVmThresholdHysteresis(VM_VoltageMan_t *volm, uint16_t hysteresis);

#endif
