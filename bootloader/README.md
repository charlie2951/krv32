# Bootloader User manual
KRV-32 contains a software bootloader designed using a Python script. In the hardware, when the device enters into boot mode, it reads data from UART and save the data into Program memory which is BRAM.
## Prerequisite: 
Make sure that the `pyserial` library, along with `python` is installed on your system.<p>

## Uploading the firmware
Follow these steps to upload the generated HEX file into program memory:
1. Copy your generated hex file which is `firmware.hex` into bootloader folder.
2. Enable the `BOOT switch` to HIGH which is `SW0` i.e. `J15` pin of Nexys-4 DDR board, and press `RESET` button i.e. `CPU RESET` in the board, to enter into BOOT mode.
3. Open terminal and invoke the bootloader using `python boot.py <COM PORT NO> <file name>`. For example: `python boot.py COM5 firmware.hex`
4. Make sure that you are using correct COM port no. For linux user, the format of serial port is something like `/dev/ttyUSB0`. Verify it from your system.
5. If everything is OK, then firmware will be loaded into memory using UART, and details will be printed on the terminal.
6. Now, Disable the `BOOT` switch and press `RESET` to start executing the user code.

`Note:` The firmware is loaded into memory which is BRAM. Since it is volatile in nature, the data will be lost after power off. You need to re-programme it by repeating the above steps in case of power-off.
