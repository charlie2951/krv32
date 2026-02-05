import serial
import time
import sys

def main():
    if len(sys.argv) != 3:
        print("Usage: python uart_upload.py <COM_PORT> <HEX_FILE>")
        sys.exit(1)

    PORT = sys.argv[1]
    HEX_FILE = sys.argv[2]
    BAUD = 115200
    print("\n---------------------------------------------------------------------\n")
    print("-----KRV32-UART Bootloader for uploading HEX file to PROGRAM Memory----\n")
    print("Copyright: Dr. S.K.Maity, School of Electronics Engg, KIIT University--\n")

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

                time.sleep(0.1)

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

        print("\nUpload complete\n")
        print("\nDisable the BOOT switch and Press RESET button on your board\n")

    except FileNotFoundError:
        print(f"Error: File '{HEX_FILE}' not found")

    finally:
        ser.close()

if __name__ == "__main__":
    main()
