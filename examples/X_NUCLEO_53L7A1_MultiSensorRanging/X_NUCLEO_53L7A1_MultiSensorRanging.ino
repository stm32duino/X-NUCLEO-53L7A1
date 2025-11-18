/**
 ******************************************************************************
 * @file    X_NUCLEO_53L7A1_MultiSensorRanging.ino
 * @author  STMicroelectronics
 * @version V1.0.0
 * @date    16 January 2023
 * @brief   Arduino test application for the X-NUCLEO-53L7A1 based on VL53L7CX
 *          proximity sensor.
 *          This application makes use of C++ classes obtained from the C
 *          components' drivers.
 ******************************************************************************
 * @attention
 *
 * <h2><center>&copy; COPYRIGHT(c) 2021 STMicroelectronics</center></h2>
 *
 * Redistribution and use in source and binary forms, with or without modification,
 * are permitted provided that the following conditions are met:
 *   1. Redistributions of source code must retain the above copyright notice,
 *      this list of conditions and the following disclaimer.
 *   2. Redistributions in binary form must reproduce the above copyright notice,
 *      this list of conditions and the following disclaimer in the documentation
 *      and/or other materials provided with the distribution.
 *   3. Neither the name of STMicroelectronics nor the names of its contributors
 *      may be used to endorse or promote products derived from this software
 *      without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS "AS IS"
 * AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE ARE
 * DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT HOLDER OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR
 * SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER
 * CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY,
 * OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 *
 ******************************************************************************
 */

/* Includes ------------------------------------------------------------------*/
#include <Arduino.h>
#include <Wire.h>
#include <vl53l7cx_class.h>

#ifdef ARDUINO_SAM_DUE
  #define DEV_I2C Wire1
#else
  #define DEV_I2C Wire
#endif
#define SerialPort Serial

/* Please uncomment the line below if you want also to use the satellites */
#define SATELLITES_MOUNTED

#define LPN_TOP_PIN 5
#define I2C_RST_TOP_PIN 3
#define PWREN_TOP_PIN A3

#ifdef SATELLITES_MOUNTED
  #define LPN_LEFT_PIN 6
  #define I2C_RST_LEFT_PIN 7 
  #define PWREN_LEFT_PIN A0
  
  #define LPN_RIGHT_PIN 4  
  #define I2C_RST_RIGHT_PIN 8
  #define PWREN_RIGHT_PIN A5
#endif

// Components.
VL53L7CX sensor_vl53l7cx_top(&DEV_I2C, LPN_TOP_PIN,I2C_RST_TOP_PIN);
#ifdef SATELLITES_MOUNTED
  VL53L7CX sensor_vl53l7cx_left(&DEV_I2C, LPN_LEFT_PIN,I2C_RST_LEFT_PIN);
  VL53L7CX sensor_vl53l7cx_right(&DEV_I2C, LPN_RIGHT_PIN,I2C_RST_RIGHT_PIN);
#endif

uint8_t status;

/* Setup ---------------------------------------------------------------------*/

void setup()
{
  // Initialize serial for output.
  SerialPort.begin(460800);

  // Initialize I2C bus.
  DEV_I2C.begin();

  // Enable PWREN top pin if present
  if (PWREN_TOP_PIN >= 0) {
    pinMode(PWREN_TOP_PIN, OUTPUT);
    digitalWrite(PWREN_TOP_PIN, HIGH);
    delay(10);
  }

#ifdef SATELLITES_MOUNTED
  // Enable PWREN left pin if present
  if (PWREN_LEFT_PIN >= 0) {
    pinMode(PWREN_LEFT_PIN, OUTPUT);
    digitalWrite(PWREN_LEFT_PIN, HIGH);
    delay(10);
  }
  // Enable PWREN right pin if present
  if (PWREN_RIGHT_PIN >= 0) {
    pinMode(PWREN_RIGHT_PIN, OUTPUT);
    digitalWrite(PWREN_RIGHT_PIN, HIGH);
    delay(10);
  }
#endif

  // Configure VL53L7CX top component.
  sensor_vl53l7cx_top.begin();
  
  // Switch off VL53L7CX top component.
  sensor_vl53l7cx_top.vl53l7cx_off();

#ifdef SATELLITES_MOUNTED
  // Configure (if present) VL53L7CX left component.
  sensor_vl53l7cx_left.begin();

  //Switch off (if present) VL53L7CX left component.
  sensor_vl53l7cx_left.vl53l7cx_off();

  // Configure (if present) VL53L7CX right component.
  sensor_vl53l7cx_right.begin();

  // Switch off (if present) VL53L7CX right component.
  sensor_vl53l7cx_right.vl53l7cx_off();
#endif

  //Initialize all the sensors
  status=sensor_vl53l7cx_top.init_sensor(0x54);  
  
#ifdef SATELLITES_MOUNTED
  status=sensor_vl53l7cx_left.init_sensor(0x56);
  status=sensor_vl53l7cx_right.init_sensor(0x58);
#endif

  // Start Measurements
  status=sensor_vl53l7cx_top.vl53l7cx_start_ranging();   
  
#ifdef SATELLITES_MOUNTED
   status=sensor_vl53l7cx_left.vl53l7cx_start_ranging();
   status=sensor_vl53l7cx_right.vl53l7cx_start_ranging();
#endif
}

void loop()
{
  VL53L7CX_ResultsData results;
  uint8_t NewDataReady = 0;
  char report[128];
  uint8_t status;
  
  do {
    status = sensor_vl53l7cx_top.vl53l7cx_check_data_ready(&NewDataReady);
  } while (!NewDataReady);

  if ((!status) && (NewDataReady != 0)) {
    // Read measured distance. RangeStatus = 5 and 9 means valid data
    sensor_vl53l7cx_top.vl53l7cx_get_ranging_data(&results);

    snprintf(report, sizeof(report), "VL53L7CX Top: Status = %3u, Distance = %5u mm, Signal = %6u kcps/spad\r\n",
             results.target_status[0],
             results.distance_mm[0],
             results.signal_per_spad[0]);
    SerialPort.print(report);
  }

#ifdef SATELLITES_MOUNTED
  NewDataReady = 0;

  do {
    status = sensor_vl53l7cx_left.vl53l7cx_check_data_ready(&NewDataReady);
  } while (!NewDataReady);


  if ((!status) && (NewDataReady != 0)) {
    // Read measured distance. RangeStatus = 5 and 9 means valid data
    sensor_vl53l7cx_left.vl53l7cx_get_ranging_data(&results);
    snprintf(report, sizeof(report), "VL53L7CX Left: Status = %3u, Distance = %5u mm, Signal = %6u kcps/spad\r\n",
             results.target_status[0],
             results.distance_mm[0],
             results.signal_per_spad[0]);
    SerialPort.print(report);
  }

  NewDataReady = 0;
  do {
    status = sensor_vl53l7cx_right.vl53l7cx_check_data_ready(&NewDataReady);
  } while (!NewDataReady);

  if ((!status) && (NewDataReady != 0)) {
    // Read measured distance. RangeStatus = 5 and 9 means valid data
    sensor_vl53l7cx_right.vl53l7cx_get_ranging_data(&results);
    snprintf(report, sizeof(report), "VL53L7CX Right: Status = %3u, Distance = %5u mm, Signal = %6u kcps/spad\r\n",
             results.target_status[0],
             results.distance_mm[0],
             results.signal_per_spad[0]);
    SerialPort.print(report);
  }

#endif
}
