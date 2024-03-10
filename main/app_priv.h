/*
   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/
#pragma once
#include <stdint.h>
#include <stdbool.h>

#define REPORTING_PERIOD    5 /* Seconds */

extern esp_rmaker_device_t *accel_x_device;
extern esp_rmaker_device_t *accel_y_device;
extern esp_rmaker_device_t *accel_z_device;

void app_driver_init(void);