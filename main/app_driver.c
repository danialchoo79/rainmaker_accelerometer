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
#include <math.h>


/* This is the button that is used for toggling the power */
#define BUTTON_GPIO          CONFIG_EXAMPLE_BOARD_BUTTON_GPIO
#define BUTTON_ACTIVE_LEVEL  0
/* This is the GPIO on which the power will be set */
#define OUTPUT_GPIO    19
#define PI 3.14159265358979323846


static TimerHandle_t sensor_timer;

#define WIFI_RESET_BUTTON_TIMEOUT       3
#define FACTORY_RESET_BUTTON_TIMEOUT    10
#define NUM_DATA_POINTS 15

static float accel_x_axis;
static float accel_y_axis;
static float accel_z_axis;

// static int16_t roll_angle;
// static int16_t pitch_angle;

static void app_sensor_update(TimerHandle_t handle)
{
    // Using No- Turn Single Point Calibration Scheme
    // Measure average for 10 data points for X,Y and minus measured value
    // For the z axis, i used Z_Measured - (Z_1g - S_z)
    // For more info, check ADXL345 Datasheet

    accel_x_axis = (float)((adxl345_read_x(I2C_CONTROLLER_0) - 24)*3.9)/1000.0f;
    accel_y_axis = (float)((adxl345_read_y(I2C_CONTROLLER_0) + 9)*3.9)/1000.0f;
    accel_z_axis = (float)((adxl345_read_z(I2C_CONTROLLER_0) + 8)*3.9)/1000.0f;

    // // Calculate roll and pitch in radians https://www.analog.com/en/resources/app-notes/an-1057.html
    // roll_angle = atan2((double)accel_y_axis, sqrt((double)accel_x_axis * (double)accel_x_axis + (double)accel_z_axis * (double)accel_z_axis));
    // pitch_angle = atan2((double)accel_x_axis, sqrt((double)accel_y_axis * (double)accel_y_axis + (double)accel_z_axis * (double)accel_z_axis));

    // // Convert radians to degrees
    // roll_angle = roll_angle * (180.0 / PI);
    // pitch_angle = pitch_angle * (180.0 / PI);

    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(accel_x_device, ESP_RMAKER_PARAM_TEMPERATURE),
            esp_rmaker_float(accel_x_axis));
    
    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(accel_y_device, ESP_RMAKER_PARAM_TEMPERATURE),
            esp_rmaker_float(accel_y_axis));

    esp_rmaker_param_update_and_report(
            esp_rmaker_device_get_param_by_type(accel_z_device, ESP_RMAKER_PARAM_TEMPERATURE),
            esp_rmaker_float(accel_z_axis));
    
    //  esp_rmaker_param_update_and_report(
    //         esp_rmaker_device_get_param_by_type(pitch_angle, ESP_RMAKER_PARAM_TEMPERATURE),
    //         esp_rmaker_float(pitch_angle));
    
    // esp_rmaker_param_update_and_report(
    //         esp_rmaker_device_get_param_by_type(roll_angle, ESP_RMAKER_PARAM_TEMPERATURE),
    //         esp_rmaker_float(roll_angle));

}

esp_err_t app_sensor_init(void)
{
    // SDA pin 7 SCL pin 0
    i2c_init(I2C_CONTROLLER_0, 7, 0);
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

