# X-NUCLEO-53L7A1

Arduino library to support the X-NUCLEO-53L7A1 based on VL53L7CX Time-of-Flight 8x8 multizone ranging sensor with wide field view.

This sensor uses I2C to communicate. And I2C instance is required to access to the sensor.
The APIs provide simple distance measure and multizone detection in both polling and interrupt modes.

## Examples

There are 3 examples with the X-NUCLEO-53L7A1 library:

* X_NUCLEO_53L7A1_HelloWorld: This example code is to show how to get multizone detection and proximity values of the VL53L7CX sensor in polling mode.

* X_NUCLEO_53L7A1_ThresholdsDetection: This example code is to show how to configure the thresholds detection.
  
* X_NUCLEO_53L7A1_MultiSensorRanging: This example code is to show how to make three VL53L7CX ToF sensors run simultaneously.

## Dependencies

This package requires the following Arduino library:

* STM32duino VL53L7CX: https://github.com/stm32duino/VL53L7CX


## Documentation

You can find the source files at  
https://github.com/stm32duino/X-NUCLEO-53L7A1

The VL53L7CX datasheet is available at
https://www.st.com/en/imaging-and-photonics-solutions/vl53l7cx.html
