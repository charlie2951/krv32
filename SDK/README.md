# KRV-32 Soft SoC User Manual
## Directory structure
`include`: contains all driver headers <p>
`main.c`: Main C code for application programme<p>
`Makefile` for automatic building and compilation and hex file generation<p>
`makehex.py` used to generate `.hex` file from `.bin` file (controlled via `Makefile`)<p>
`sections.lds` loader script<p>
`start.s` startup assembly code to initialize stacks and clear all regs<p>

## Driver API 
### Header:`delay.h`
`delay(value in microsec)`: Usages-> `delay(1000000)`: provides typically 1 sec delay  <p>
### Header:`uart.h`
`Note`: Baud rate of UART1 is 9600 and UART2 is 115200. Currently, it is fixed and can't be changed.<p>
UART-1 is the native UART connected to USB to UART bridge in hardware.<p>
UART-2 is used for sensor interfacing (such as ESP01 wifi etc) via serial port.<p>
 Usages: <p>
`uart_sendline(UART1, "sample string\n\r")`: print the string on `UART1` <p>
### Header:`led_driver.h`
Device driver to access EIGHT on board LEDs connected to Nexys-4 DDR board.<p>
Usages:<p>
`led_write(0xFF)`-> display 0xFF in LED. <p>
**...Adding other APIs soon, SDK is under development**

