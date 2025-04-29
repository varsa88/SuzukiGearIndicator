# Circuit Diagram

This document provides the circuit diagram for the Suzuki Gear Indicator project.

## ASCII Circuit Diagram

Below is an ASCII representation of the circuit diagram for the Suzuki Gear Indicator project:

```
+-------------------+       +-------------------+
|                   |       |                   |
|   Arduino Board   |       |  7-Segment Display|
|                   |       |                   |
+-------------------+       +-------------------+
         |                           |
         |                           |
         |                           |
         +----> PWM Pin (D3)         |
         |                           |
         +----> GND                  |
         |                           |
         +----> A0 (Gear Sensor)     |
```

This is a simplified representation. For a more detailed diagram, consider using a graphical tool like Fritzing or KiCad.

## 7-Segment Display Details

The 7-segment display is used to visualize the current gear. Below are the details for connecting the display:

### Pin Connections
- **Segment Pins (A-G, DP)**: Connect to Arduino digital pins 11, 10, 12, 13, 4, 7, 8, and 9 respectively.
- **Digit Pins**: Connect to Arduino digital pin 6.
- **Common Pin**: Connect to GND (for common cathode) or VCC (for common anode).

### Configuration
- The display is configured as a **common anode** type in the code.
- Resistors (e.g., 220Ω or 330Ω) should be placed on each segment pin to limit current.

### Notes
- Ensure the brightness is set appropriately in the code to avoid flickering.
- The display is refreshed continuously in the `loop()` function to maintain visibility.

### Explicit Pin Connections

Below is a detailed list of pin connections for the Suzuki Gear Indicator project:

#### Arduino Board
- **A0**: Connected to the gear position sensor.
- **D3 (PWM Pin)**: Outputs the modified gear sensor voltage.
- **D6**: Controls the digit pin of the 7-segment display.
- **D11-D13, D4, D7-D9**: Connected to the segment pins (A-G, DP) of the 7-segment display.
- **GND**: Common ground for all components.

#### 7-Segment Display
- **Segment A**: Connected to Arduino pin D11.
- **Segment B**: Connected to Arduino pin D10.
- **Segment C**: Connected to Arduino pin D12.
- **Segment D**: Connected to Arduino pin D13.
- **Segment E**: Connected to Arduino pin D4.
- **Segment F**: Connected to Arduino pin D7.
- **Segment G**: Connected to Arduino pin D8.
- **Decimal Point (DP)**: Connected to Arduino pin D9.
- **Digit Pin**: Connected to Arduino pin D6.
- **Common Pin**: Connected to GND (for common cathode) or VCC (for common anode).

#### Gear Position Sensor
- **Signal Pin**: Connected to Arduino pin A0.
- **Power Pin**: Connected to 5V.
- **Ground Pin**: Connected to GND.