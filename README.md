# Building-Health Monitoring using Internet of Things (IoT)

## What this project is about
Currently, the maintenance of buildings is a manual process.

This project aims to develop a self-monitoring solution of the structural integrity of buildings, through the use of Internet of Things (IoT).

The entire project consists of 3 sensors connected on a common network. The sensors are -- (1) Flex Sensor, (2) Tilt Sensor, (3) Rain Sensor.

This repository will only cover (2) Tilt Sensor.

It uses the ESP32-C6 board, which supports up to WiFi6 in 2.4GHsz band. For Serial Communications, I2C is used.

## Build, Flash firmware, and Visualize

1. Wire the ADXL345 according to this diagram. https://www.electronicwings.com/esp32/adxl345-accelerometer-interfacing-with-esp32
2. I am using SDA and SCL pins of 0 and 7 respectively. You can change this pins under app_driver.c in i2c_init function call.
3. Connect your device with a USB-Type C Cable. Choose the Right COM Port. Build the Project. Flash it unto your ESP Device.
4. Once Flashed, Open Serial Monitor, and use the ESP Rainmaker App to scan the QR Code. This will perform provisioning of WiFi.
5. Once Provisioned, you will see x,y,z in +/- 2g sensor readings on your ESP Rainmaker App.

## What to expect here?

- This project uses an accelerometer to sense gravitational force of up to +/-2g and sends that information wirelessly, to be visualized in ESP Rainmaker App.
- The sensor value is sent every x number of seconds. Settings can be changed in app_priv.h.
- It will consistently send data in x,y,z g force readings over +/-2g sensitivity range.
- You can check the g foirce reading changes in the ESP Rainmaker phone app.

## Hardware?

This project, for (2) Tilt Sensor, comes with its portable power source, 9V Battery, Bulk Converter and a Junction Box Casing.

This is the overall concept of the project. These components can be changed to suit the design needs.

### Reset to Factory

Press and hold the BOOT button for more than 3 seconds to reset the board to factory defaults. You will have to provision the board again to use it.

