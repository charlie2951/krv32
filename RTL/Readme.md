# KRV-32 SoC IP User Guide
## Module Descriptions
`top.v` implements the top module <p>
`cpu.v` KRV-32 RISC-V CPU supporting RV32I instructions <p>
`progmem.v` Program Memory to store HEX code of program data <p>
`gpio_ip.v` Onboard LED interfacing IP <p>
`pwm.v` Verilog module to generate PWM signal <p>
`regs_pwm` Memory mapping and register configuration for PWM hardware
`pwm_ip.v` top level PWM module IP interfacing IP with CPU that combines `pwm.v` and `regs_pwm.v` <p>
`uart_tx.v` UART transmitter implemented in Verilog (supports only 9600 baud) <p>
`regs_uart.v` is the memory map register configuration for CPU interfacing and `uart_ip.v` is the top level wrapper module to connect KRV-32 CPU with UART transmitter.<p>
`uart_rx.v` UART receiver implemented in Verilog (supports only 9600 baud) <p>
`regs_uart_rx.v` is the memory map register configuration for CPU interfacing, and `uart_ip_rx.v` is the top-level wrapper module to connect KRV-32 CPU with UART receiver.<p>





