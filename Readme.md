# Pet HUB Core Module
The **Pet HUB Core Module** is the heart of an innovative and integrated smart pet care system. This repository contains the source code for the core module, designed to automate essential pet care functions such as feeding, environmental control, and health monitoring.

## Overview
The **Core Module** leverages the **PIC18F45K22 microcontroller** to manage sensor data acquisition, actuator control, and user interaction. It provides real-time functionality while ensuring reliability and ease of use for pet owners.

### Key Features
- **Automated Feeding**: Dispenses food at scheduled times with adjustable portion sizes.
- **Environmental Monitoring**: Tracks temperature and air quality to ensure a comfortable and hygienic space.
- **Data Persistence**: Stores critical settings like feeding schedules and configurations in EEPROM.
- **User Interface**: Features an OLED display for live updates and pushbutton controls for manual overrides.
- **UART Communication**: Seamlessly exchanges data with the premium communication module.

## Technical Details

### Components
- **Microcontroller**: PIC18F45K22
- **Sensors**:
  - Ultrasonic sensors (HC-SR04) for food and water level detection
  - Temperature sensor (LM35)
  - Odor sensor (MQ-135)
- **Actuators**:
  - Stepper motor (28BYJ-48) for food dispensing
  - Fan for temperature control
- **Display**: OLED (SSD1309 driver)

### Communication Protocols
- **SPI**: For OLED display communication
- **UART**: For data exchange with the communication module (ESP32)

### Tools and Technologies
- **Development Environment**: MPLAB IDE
- **Programming Language**: Embedded C

## Installation and Usage

1. Clone the repository:
   ```bash
   git clone https://github.com/edwinpoulose/PetHub.git
2. Open the source code in MPLAB IDE.

3. Compile and upload the firmware to the PIC18F45K22 microcontroller.

4. Connect the microcontroller to the sensors, actuators, and OLED display as per the pinout.

### Mode Control Table

| Mode      | Increment | Decrement | Manual |
|-----------|-----------|-----------|--------|
| Run       | -         | -         | Active |
| Schedule  | +1        | -1        | Active |
| Portion   | +1        | -1        | Active |
| Time(h)   | +1 to 24  | -1        | Active |
| Time(m)   | +1 to 60  | -1        | Active |
| Temp      | +1        | -1        | Active |

### Pinout

- **RA0**: AN0 TEMP ADC
- **RA1**: AN1 AIR ADC
- **RA4**: MODE (Push Button)
- **RA5**: INCPRESS (Push Button)
- **RA6**: DECPRESS (Push Button)
- **RA7**: MANUAL (Push Button)
- **RB0**: ECHO INT0
- **RB1**: ECHO INT1
- **RB2**: Stepper Motor
- **RB3**: Stepper Motor
- **RB4**: Stepper Motor
- **RB5**: Stepper Motor
- **RC3**: OLED SPI CLK
- **RC5**: OLED SPI SDO
- **RC6**: TX 25
- **RC7**: RX 26
- **RD0**: DIO TRIG1
- **RD1**: DIO TRIG2
- **RD2**: DIO COOLING FAN
- **RD4**: OLED RESET
- **RD5**: OLED DATA
- **RD6**: OLED CMD
