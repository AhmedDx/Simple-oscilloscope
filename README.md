# Simple-oscilloscope
Simple oscilloscope using MAX7219 4-in-1 8x8 module

## ✍ Overview
This project implements a simple digital oscilloscope using an Arduino Uno microcontroller and a MAX7219 4‑in‑1 8×8 LED Matrix Module.

The system samples an analog input signal, converts it into digital values using the internal Analog-to-Digital Converter (ADC), and displays the waveform in real time on the LED matrix.

The goal of this project is to demonstrate how a microcontroller can capture, process, and visualize analog signals using basic hardware components.

##  📌 Project Objective
The main objectives of this project are:

- Implement a basic digital oscilloscope
- Understand how analog signals are sampled using ADC
- Visualize signals using an LED matrix display
- Practice microcontroller programming and hardware interfacing

## Components Used
- Arduino Uno (1)
- MAX7219 4‑in‑1 8×8 LED Matrix Module (1)
- Push Buttons(2)
- RES (3)
- Cap (2)

##  🔍 MAX7219 4‑in‑1 8×8 LED Matrix Module
![MAX7219 4‑in‑1 8×8 LED Matrix Module](https://makerselectronics.com/wp-content/uploads/2023/01/lcd-01-090-1-560x560.jpg)

The MAX7219 4-in-1 8×8 LED Matrix Module is a display device used to control multiple LED matrices efficiently using a microcontroller such as the Arduino Uno.

This module contains four 8×8 LED matrices connected together and driven by the MAX7219 integrated circuit. The driver simplifies the process of controlling many LEDs by handling the multiplexing, scanning, and brightness control internally.


## 👨‍💻Software Application
The circuit was designed and tested using Proteus Design Suite, a professional software tool widely used for electronic circuit design and simulation.
Proteus allows engineers and students to create electronic schematics, simulate circuit behavior, and verify circuit performance before building the physical hardware.

## 💯 How the System Works
1. The analog signal is connected to one of the analog input pins of the Arduino.
2. The internal ADC converts the signal from analog voltage (0–5V) to digital values.
3. A timer interrupt controls the sampling frequency.
4. The sampled values are stored in an array.
5. The data is sent to the MAX7219 LED matrix driver.
6. The LED matrix displays the waveform in real time.

## Analog-to-Digital Conversion (ADC) using Arduino UNO
![arduino uno](https://components101.com/sites/default/files/inline-images/Arduino-uno-pin-diagram.png)

The ADC inside the Arduino (ATmega328P) converts analog voltages into digital values.

Key Characteristics 🔑
- Resolution: 10-bit
- Digital Range: 0 – 1023
- Input Voltage Range: 0 – 5V

In this project:

Sampling frequency ≈ 1 kHz
Each value is scaled to 0–7 to match the 8 rows of the LED matrix
A 32-point waveform buffer is used for display.
More about ADC: [ADC](https://docs.arduino.cc/learn/microcontrollers/analog-input/)

##   📍   Learning Outcomes
From this project we learned:

- Microcontroller-based signal acquisition
- Working with ADC in embedded systems
- Driving LED matrices using MAX7219
- Real-time data visualization
- Hardware and software integration
