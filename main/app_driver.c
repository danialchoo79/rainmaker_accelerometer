/*  Temperature Sensor demo implementation using RGB LED and timer

   This example code is in the Public Domain (or CC0 licensed, at your option.)

   Unless required by applicable law or agreed to in writing, this
   software is distributed on an "AS IS" BASIS, WITHOUT WARRANTIES OR
   CONDITIONS OF ANY KIND, either express or implied.
*/

#include <freertos/FreeRTOS.h>
#include <freertos/timers.h>
#include <sdkconfig.h>
#include <esp_rmaker_core.h>
#include <esp_rmaker_standard_types.h> 
#include <esp_rmaker_standard_params.h> 

#include <app_reset.h>
#include <ws2812_led.h>
#include "app_priv.h"
#include "i2c.h"
#include "adxl345.h"


/* This is the button that is used for toggling the power */
#define BUTTON_GPIO          CONFIG_EXAMPLE_BOARD_BUTTON_GPIO
#define BUTTON_ACTIVE_LEVEL  0
/* This is the GPIO on which the power will be set */
#define OUTPUT_GPIO    19

static TimerHandle_t sensor_timer;

#define WIFI_RESET_BUTTON_TIMEOUT       3
#define FACTORY_RESET_BUTTON_TIMEOUT    10

static int16_t accel_x_axis;
static int16_t accel_y_axis;
static int16_t accel_z_axis;


static void app_sensor_update(TimerHandle_t handle)
{
    accel_x_axis = adxl345_read_x(I2C_CONTROLLER_0);
    accel_y_axis = adxl345_read_y(I2C_CONTROLLER_0);
    accel_z_axis = adxl345_read_z(I2C_CONTROLLER_0);

    // accel_x_axis = 10;
    // accel_y_axis = 20;
    // accel_z_axis = 30;
    
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(accel_x_device, ESP_RMAKER_PARAM_TEMPERATURE),
            esp_rmaker_float(accel_x_axis));
    
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(accel_y_device, ESP_RMAKER_PARAM_TEMPERATURE),
            esp_rmaker_float(accel_y_axis));

    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(accel_z_device, ESP_RMAKER_PARAM_TEMPERATURE),
            esp_rmaker_float(accel_z_axis));
}

esp_err_t app_sensor_init(void)
{
    // SCL pin 7 SDA pin 6
    i2c_init(I2C_CONTROLLER_0, 6, 7);
    adxl345_init(I2C_CONTROLLER_0);

    sensor_timer = xTimerCreate("app_sensor_update_tm", (REPORTING_PERIOD * 1000) / portTICK_PERIOD_MS,
                            pdTRUE, NULL, app_sensor_update);
    if (sensor_timer) {
        xTimerStart(sensor_timer, 0);
        return ESP_OK;
    }
    return ESP_FAIL;
}

void app_driver_init()
{
    app_sensor_init();
    app_reset_button_register(app_reset_button_create(BUTTON_GPIO, BUTTON_ACTIVE_LEVEL),
                WIFI_RESET_BUTTON_TIMEOUT, FACTORY_RESET_BUTTON_TIMEOUT);
}

