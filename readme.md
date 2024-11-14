**Security System with Environmental Monitoring and Alerts**


﻿This project implements a security system with environmental monitoring and alert functions using a PIC16F887 microcontroller. The system utilizes multiple sensors to measure environmental factors and provides different alert statuses, making it ideal for applications that require real-time monitoring and security enforcement.

**Features**

- **Password-Protected Access**: Users can enter a 4-digit password for secure access. The system enforces security with a maximum of 3 failed attempts before locking.
- **State Machine Control**: The system is organized as a finite state machine with well-defined states for initialization, password verification, environmental monitoring, event monitoring, alarm, and alert.
- **Environmental Monitoring**: Continuously measures temperature and light intensity, displaying real-time data on an LCD. If thresholds are exceeded, the system transitions to an alarm state.
- **Event Monitoring**: Detects proximity and magnetic fields using a Hall sensor and obstacle sensor. If specific events are detected, the system triggers an alert.
- **Visual and Audible Feedback**: RGB LED lights and a buzzer provide feedback on different states, such as correct password entry, alarms, and alerts.
- **Non-Blocking Timer Implementation**: The system uses a non-blocking timer with millisecond precision to manage timeouts and sensor update intervals, ensuring smooth transitions and readability on the LCD.

**Components**

- **PIC16F887 Microcontroller**
- **16x2 LCD**: Displays system status, sensor readings, and alert messages.
- **Keypad (4x4)**: Allows secure password entry.
- **Temperature Sensor**: Measures environmental temperature.
- **Light Sensor (Phototransistor)**: Detects light intensity.
- **Hall Effect Sensor**: Detects magnetic fields.
- **Obstacle Sensor**: Detects proximity to objects.
- **RGB LED**: Provides visual feedback for different states.
- **Buzzer**: Sounds alerts for certain states.

**Project Structure**

- **main.c**: Contains the core program, including the state machine logic and main system functions.
- **CONFIG.h**: Configuration settings for the PIC16F887 microcontroller.
- **LCD.c and LCD.h**: Functions for LCD initialization and display control.
- **KEYPAD.c and KEYPAD.h**: Functions for keypad initialization and key retrieval.
- **ADC.c and ADC.h**: ADC configuration and functions for reading sensor values.
- **SENSOR.c and SENSOR.h**: Functions for reading specific sensors (Hall, obstacle, temperature, and light).

**How to Use**

1. **Password Entry**: On startup, enter the 4-digit password to access the system.
1. **Environmental Monitoring**: The system displays temperature and light levels on the LCD. If values exceed thresholds, the system transitions to the alarm state.
1. **Event Monitoring**: Continuously monitors sensors for specific events. If detected, the system switches to alert mode.
1. **Alarm and Alert**: In these states, the system displays the status on the LCD and activates the RGB LED and buzzer for appropriate visual and audible feedbac

