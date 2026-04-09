"""
MIT License

Copyright (c) 2024-2026 Subir Maity

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
"""
import serial
import time
import sys

def display_logo():
    logo = """
  ██╗  ██╗██████╗ ██╗   ██╗    ██████╗ ██████╗ 
  ██║ ██╔╝██╔══██╗██║   ██║    ╚════██╗╚════██╗
  █████╔╝ ██████╔╝██║   ██║     █████╔╝ █████╔╝
  ██╔═██╗ ██╔══██╗╚██╗ ██╔╝     ╚═══██╗██╔═══╝ 
  ██║  ██╗██║  ██║ ╚████╔╝     ██████╔╝███████╗
  ╚═╝  ╚═╝╚═╝  ╚═╝  ╚═══╝      ╚═════╝ ╚══════╝
                                               
     --- RISC-V 32-Bit Microarchitecture ---
    """
    print(logo)

def main():
    if len(sys.argv) != 3:
        print("Usage: python uart_upload.py <COM_PORT> <HEX_FILE>")
        sys.exit(1)

    PORT = sys.argv[1]
    HEX_FILE = sys.argv[2]
    BAUD = 115200
    print("\n---------------------------------------------------------------------\n")
    print("-----KRV32-UART Flasher for uploading Firmware to Board Flash----\n")
    print("Copyright: Dr. S.K.Maity, School of Electronics Engg, KIIT University--\n")
    display_logo()
    try:
        ser = serial.Serial(PORT, BAUD, timeout=0.1)
    except serial.SerialException as e:
        print("Error opening serial port:", e)
        sys.exit(1)

    time.sleep(2)
    print(f"Connected to {PORT}\n")

    try:
        with open(HEX_FILE, "r") as f:
            for line in f:
                tx = line.strip()
                if tx:
                    ser.write((tx + "\n").encode())
                    print("TX:", tx)

                time.sleep(0.05) #changing to 0.05 from 0.1

                # Read any response from FPGA
                while ser.in_waiting:
                    rx = ser.readline().decode(errors="ignore").strip()
                    if rx:
                        print("RX:", rx)

        # Final read after transmission
        time.sleep(2)
        while ser.in_waiting:
            rx = ser.readline().decode(errors="ignore").strip()
            if rx:
                print("RX:", rx)

        print("\nUpload complete...\n")
        print("\nDisable the BOOT switch and Press RESET button on your board\n")

    except FileNotFoundError:
        print(f"Error: File '{HEX_FILE}' not found")

    finally:
        ser.close()

if __name__ == "__main__":
    main()

