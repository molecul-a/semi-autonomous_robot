# Autonomous Line Following Robot with Obstacle Detection Using Arduino Uno

## Description

This repository contains the code for an autonomous line-following robot equipped with obstacle detection capabilities, built using an Arduino Uno. The robot uses QTR sensors to follow a line and the APDS9960 sensor to detect obstacles in its path. When an obstacle is detected, the robot stops and displays the detected color using an RGB LED.

## Features

- **Line Following:** Uses QTR sensors to follow a predefined line on the ground.
- **Obstacle Detection:** Utilizes the APDS9960 sensor to detect obstacles and stop the robot.
- **Color Detection:** Reads color values from the APDS9960 sensor and displays the detected color using an RGB LED.
- **Proportional-Derivative Control:** Ensures smooth and accurate line following with adjustable parameters.
- **Test Modes:** Includes modes for testing sensor readings and proximity detection.

## Components

- **Arduino Uno:** The main microcontroller for the project.
- **QTR Sensors:** 8 sensors arranged in a line for detecting the path.
- **APDS9960 Sensor:** Used for proximity and color detection.
- **Motors:** Controlled using PWM signals to adjust speed based on sensor input.
- **RGB LED:** Displays colors detected by the APDS9960 sensor.
- **Motor Driver:** Interface for controlling the motors' speed and direction.

## Setup and Calibration

### Wiring
- Connect the QTR sensors to analog pins A0-A7.
- Connect the APDS9960 sensor to the appropriate I2C pins (A4 for SDA, A5 for SCL).
- Connect the motors and motor driver to the specified digital pins.
- Connect the RGB LED to the specified digital pins.

### Calibration
- During setup, the QTR sensors will be calibrated by sliding the sensors across the line. Ensure the sensors are exposed to the brightest and darkest readings.
- Calibration data will be printed if `TEST_MODE_SENSORS` is enabled.

## Usage

### Upload the Code
- Upload the provided code to your Arduino Uno board.

### Run the Robot
- Position the robot on the line and power it on.
- The robot will start following the line using the QTR sensors.
- When an obstacle is detected within the specified distance, the robot will stop and display the detected color using the RGB LED.

### Adjusting Parameters
- Tune the `Kp` and `Kd` values for the PD controller to achieve optimal line-following performance.
- Adjust the `DISTANSE` value for proximity detection as needed.

## Demonstration

[![Video Presentation](https://img.youtube.com/vi/YOUR_VIDEO_ID/0.jpg)](https://www.youtube.com/watch?v=YOUR_VIDEO_ID)

![Robot Image](<mxfile host="www.diagrameditor.com" modified="2022-03-24T15:46:08.498Z" agent="Mozilla/5.0 (Windows NT 10.0; Win64; x64) AppleWebKit/537.36 (KHTML, like Gecko) Chrome/99.0.4844.51 Safari/537.36" etag="6oRmTdzgsPjeubIo2eiI" version="12.1.3" type="device" pages="1"><diagram id="mQIuZwDBntlPRkmFd9cg" name="Page-1">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</diagram></mxfile>)

## License

This project is licensed under the MIT License. See the `LICENSE` file for details.

## Contributions

Contributions are welcome! Feel free to submit issues and pull requests to improve the functionality and performance of the robot.

## Contact

For any questions or feedback, please reach out to [Your Name] at [Your Email].
