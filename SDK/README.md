# KRV-32 Soft SoC SDK User Manual
## Directory structure
`include`: contains all driver headers <p>
`<appname>.c`: Main C code for application programme<p>
`Makefile` for automatic building and compilation and hex file generation<p>
`makehex.py` used to generate `.hex` file from `.bin` file (controlled via `Makefile`)<p>
`sections.lds` loader script<p>
`start.s` startup assembly code to initialize stacks and clear all regs<p>
`boot.py` uploading hex file to FPGA BRAM via UART <p>

## Compilation and Building
Make sure that the RISC-V GNU toolchain is installed and added to the path. <p>
Usages:
`make`: will build the default **main.c** <p>
`make APP=appname`: will build the specific code mentioned in file `appname.c`<p>
`make clean`: will clean all temporary files.<p>
Then upload the generated `firmware.hex` using provided python script `boot.py` by typing: <p> `python3 boot.py COM_PORT_NO firmware.hex`

## Driver API for Sensors(see `include` directory for detailed usages for all functions)
### Header:`uart.h`
`Note`: Baud rate of UART1 and UART2 is 115200. Currently, it is fixed and can't be changed. In later version we shall make UART baud rate user programmable.<p>
UART-1 is the native UART connected to USB to UART bridge in hardware.<p>
UART-2 is used for sensor interfacing (such as ESP01 wifi etc) via serial port.<p>
 Usages: <p>
`uart_sendline(UART1, "sample string\n\r")`: print the string on `UART1` <p>
### `gpio.h`
Device driver to control GPIO of Nexys-4 DDR FPGA board. GPIO-0 to GPIO-7 mapped to PMOD JB (1 to 4 and 7 to 10) of
FPGA board. GPIO-8 to GPIO-15 mapped to eight on-board LEDs. GPIO-0 to GPIO-7 is internally pulled up to HIGH to configure as input. <p>
Usages:<p>
### `ssd1306.h`
SSD1306 OLED I2C display library. <p>
### `tm1637.h`
Driver for `TM1637` seven-segmen display using bit-banging method(2 wire interface, CLK and DIO).<p>
### `mpu6050.h`
Support for `MPU6050` I2C accelerometer and Gyro sensor.

# Other useful API
### `delay.h`
`delay(value in microsec)`: Usages-> `delay(1000000)`: provides typically 1 sec delay  <p>
### `my_string.h`
Support string operation similar to `string.h`<p>
### `softi2c.h`
Software implementation of `I2C` master using bit-banging using GPIO. Note that GPIO-0 -> SDA and GPIO-1 -> SCL. <p>
**...Adding other APIs soon, SDK is under development**

