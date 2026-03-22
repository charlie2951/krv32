![](https://komarev.com/ghpvc/?username=charlie2951&color=brightgreen) 

# A subset of RISC-V architecture (RV32I) SoC implementation for FPGA 
A tiny version of `RISC-V` for low-resource FPGA developed by the **Integrated System Design lab, School of Electronics, KIIT University** <p>
**Objective**
The main objective of this project is to prototype a RISC-V 32-bit CPU with an **RV32 base integer (RV32I) instruction set**. The CPU is coded using Verilog HDL from scratch and most of the instruction sets are implemented. The code is compatible with the GNU RISC-V toolchain (both assembler and compiler).
## Features of KRV-32 soft IP SOC
1. Supports RISC-V RV-32I extension
2. Compatible with GNU toolchain
3. Soft-IP, configurable as per your FPGA
4. On-board LEDS can be interfaced using the C SDK
5. Fully functional UART Transmitter and Receiver (currently supports 115200 baud rate only), one for debug and another for sensor interfacing
6. BRAM-based BOOTLOADER support (does not need to generate bitstream every time while changing application code)
7. Two dedicated hardware I2C master controllers support both slow and fast modes for sensor interfacing.
8. Soft SPI library to access SPI peripherals and flash memory(single spi supported only).
9. Dedicated hardware IP for onboard 7-segment display.
10. Crypto core based on 32 bit AES, both encryption and decryption
11. Hardware timer and free-running counter for generating an accurate delay
12. Tested in Artix-7 series FPGA embedded in Digilent Nexys-4 DDR board
13. Timing violations are fixed at 100MHz.
14. SDK with customized `Makefile` and a variety of drivers for sensor interfacing
15. Several example demos have been provided
   <p></p>
   
## Supported Sensors APIs
|Peripherals and sensor module| Remarks |
|:----------------------------|:-------:|
|SSD1306 OLED Display| :white_check_mark:| 
|ADXL 345 accelerometer| :white_check_mark:|
|BMP180 barometric pressure<p> and temperature sensor| :white_check_mark:|
|DHT11 temperature and humidity sensor|:white_check_mark:|
|MPU6050|:white_check_mark:|
|MPU9265|:white_check_mark:|
|Winbond W25Q series flash|:white_check_mark:|
|TM1637 segment display|:white_check_mark:|
|On board 7-segment display|:white_check_mark:|
|ESP01 Wifi| Limited support|
|ADS1115 I2C ADC|Experimental|
|MCP4725 I2C DAC|planned|

## General-purpose driver APIs
|Driver details| Remarks |
|:------------|:-------:|
|Hardware SPI(SPI1x)|:white_check_mark:|
|Hardware I2C|:white_check_mark:|
|UART|:white_check_mark:|
|GPIO|:white_check_mark:|
|Timer+Counter|:white_check_mark:|
|Soft GPIO-based SPI |:white_check_mark:|

## Supported C Libraries
|Library details| Remarks |
|:------------|:-------|
|`my_string.h`|string operation support similar to standard `sprintf`<p> optimized for bare-metal-RISCV for memory saving.|
|`printf.h`|support standard printing on serial console <p>in standard `printf()` format via system uart (UART-1)|
|`delay.h`| provides software delay using loop method<p>Use hardware timer for accurate delay|
|`esp_wifi.h`|Library for `ESP-01` series wifi module interface via <p>`AT` command over UART-2|
|`thingspeak.h`| Library to send data to `thingspeak` IoT cloud|
|`spi.h`|Library for hardware SPI (single SPI mode only)|
|`i2c.h`|Support for hardware `i2c`|
|`soft_spi.h`|Software SPI library using GPIO bit-banging|
|`uart.h`|For UART-1 and UART-2|
|`timer.h`|Library for system timer and counter|
|`gpio.h`|Library for GPIO operation|



## Directory Description
`src`: contains all source code i.e. RTL code and constraints and tcl file for creating vivado projects.<p>
`bootloader`: Contains a Python script to upload HEX file into FPGA BRAM i.e. program memory via UART.<p>
`SDK`: Contains the C headers and application code for sensors and internal IPs to be compiled using RISC-V GNU toolchain<p>
`TUTORIAL` : Contains sample demo programs <p>
`mcs`: Pre-build MCS file for Nexys4-DDR and Basys-3 board <p>

***Known issues/bugs*** <p>
-all resolved, no known issue<p>
-ESP01 wifi library unable to print status
-AES crypto SDK library not verified, in development phase.
<p>


