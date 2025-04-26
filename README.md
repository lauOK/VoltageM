# Voltage Management Module

A lightweight C library for managing voltage thresholds and ADC configurations in embedded systems. This module provides hysteresis-based state transitions to avoid frequent toggling between states.

---

## Features

- **ADC Voltage Conversion**: Convert raw ADC values to actual voltage using configurable parameters.
- **Threshold Management**: Define multiple voltage thresholds with hysteresis to determine system states.
- **State Monitoring**: Track voltage states and retrieve current voltage values.
- **Configurable**: Easily set thresholds, hysteresis, and ADC parameters at runtime.

---

## Dependencies

- C99 compiler (GCC, Clang, etc.)
- Standard libraries: `stdint.h`, `math.h`

---

## Integration

1. **Add Files**: Copy `VoltageManager.c` and `VoltageManager.h` into your project.

2. **Include Header**: 

   ```c
   #include "VoltageManager.h"
   ```

---

## Configuration

### Data Structures

#### `VM_AdcConfig_t`

ADC configuration parameters:

```c
uint16_t resolution;  // ADC resolution (e.g., 4096 for 12-bit)
uint16_t refence;     // Reference voltage (mV) - Note: Typo in code ("refence").
uint16_t divider;     // Voltage divider ratio (e.g., 1000 for 1:1 divider)
uint16_t offset;      // Calibration offset (mV)
```

#### `VM_Threshold_t`

Threshold configuration:

```c
uint16_t arr[VM_THRESHOLD_MAX];  // Sorted thresholds (ascending, 0-terminated)
uint16_t hysteresis;             // Hysteresis value (mV)
uint16_t state;                  // Current state (0 to num thresholds)
```

---

## Usage

### Initialization

```c
VM_VoltageMan_t voltageManager;  // Declare a voltage manager instance

// Initialize ADC and threshold configurations
VM_VoltageManaInit(&voltageManager, 1);

// Example ADC configuration (12-bit ADC, 3300mV reference, 2:1 divider)
voltageManager.adcconfig.resolution = 4096;
voltageManager.adcconfig.refence = 3300;     // Typo preserved to match code
voltageManager.adcconfig.divider = 2000;     // 2000 / 1000 = 2:1 ratio
voltageManager.adcconfig.offset = 50;        // 50mV offset

// Example thresholds: 3000mV, 3200mV (sorted ascending)
uint16_t thresholds[] = {3000, 3200, 0};     // 0-terminated array
VM_SetVmThresholdArr(&voltageManager, thresholds);
VM_SetVmThresholdHysteresis(&voltageManager, 100);  // 100mV hysteresis
```

### Update and Read States

```c
uint16_t rawAdcValues[1] = {2048};  // Example raw ADC value
uint16_t currentState[1];
uint16_t currentVoltage[1];

// Convert ADC value and update state
VM_CheckVoltageState(&voltageManager, 1, rawAdcValues);

// Retrieve results
VM_GetVoltageState(&voltageManager, 1, currentState);
VM_GetVoltageValue(&voltageManager, 1, currentVoltage);

// Example output:
// currentVoltage[0] = 2048 * 3300 / 4096 * 2 + 50 = ~3355mV
// currentState[0] = 2 (if voltage ≥ 3200mV + hysteresis)
```

---

## API Reference

### Core Functions

| Function                      | Description                                                  |
| ----------------------------- | ------------------------------------------------------------ |
| `VM_VoltageManaInit`          | Initialize ADC resolution and thresholds.                    |
| `VM_CheckVoltageState`        | Convert ADC values, update voltage states with hysteresis logic. |
| `VM_GetVoltageState`          | Retrieve current state (0 to `num` thresholds).              |
| `VM_GetVoltageValue`          | Retrieve converted voltage in mV.                            |
| `VM_SetVmThresholdArr`        | Set sorted threshold array (ascending, 0-terminated).        |
| `VM_SetVmThresholdHysteresis` | Set hysteresis value to prevent state oscillation.           |

### State Logic Rules

- **State Calculation**:  
  States increment when voltage crosses a threshold upward, decrement when crossing downward.
- **Hysteresis**:  
  Prevents state oscillation near threshold boundaries.  
  Example: Transition from state 1 to 2 only if voltage exceeds `threshold[1] + hysteresis`.

---

## Known Limitations

- The `refence` field in `VM_AdcConfig_t` has a typo (should be `reference`) but is preserved for code compatibility.
- Threshold array must be sorted in ascending order and terminated with `0`.

---

## Contributing

Contributions are welcome!  

1. Fork the repository.  
2. Create a feature branch.  
3. Submit a pull request with detailed documentation.  

---

## License

[MIT License](LICENSE)
