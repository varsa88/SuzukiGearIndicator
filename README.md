# SuzukiGearIndicator

An Arduino-based gear position indicator for Suzuki motorcycles. This project provides a PWM output and supports a 7-segment display for visualizing the current gear.

## Features
- Detects gear position using voltage levels from the gear position sensor.
- Outputs modified gear sensor voltage using PWM.
- Supports a 7-segment display for gear visualization.
- Includes moving average filtering for stable readings.
- Configurable via serial communication.

## Requirements
- Arduino-compatible microcontroller.
- 7-segment display (optional).
- Required libraries:
  - `movingAvg`
  - `SevSeg` (if using the display).

## Setup
1. Clone this repository.
2. Install PlatformIO in your preferred IDE (e.g., VS Code).
3. Ensure the required libraries are installed in your PlatformIO environment.
4. Connect the hardware as per the pin definitions in `src/main.cpp`.
5. Build and upload the code to your microcontroller.

## Usage
- The gear position is displayed on the 7-segment display (if connected).
- The PWM output provides a modified voltage corresponding to the detected gear.
- Use the serial monitor for debugging and configuration.

## License
This project is licensed under the terms specified in the `LICENSE` file.
