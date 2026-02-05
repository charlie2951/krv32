import serial
import time
import argparse
import sys

def main():
    parser = argparse.ArgumentParser(description="Read clean text from a serial port.")
    parser.add_argument("port", help="Serial port (e.g., COM3 or /dev/ttyUSB0)")
    parser.add_argument("-b", "--baud", type=int, default=115200, help="Baud rate")
    
    args = parser.parse_args()

    try:
        ser = serial.Serial(args.port, args.baud, timeout=1)
        print(f"--- Listening on {args.port} ---")
        
        time.sleep(2)

        while True:
            if ser.in_waiting > 0:
                raw_data = ser.readline()
                try:
                    # decode with 'ignore' to drop individual bad bytes
                    # then strip to remove whitespace/newlines
                    line = raw_data.decode('utf-8', errors='ignore').strip()
                    
                    # Only print if there is actual text left
                    if line:
                        print(line)
                        
                except Exception:
                    # Silently skip any other processing errors
                    pass

    except serial.SerialException:
        print(f"Error: Could not open {args.port}.")
        sys.exit(1)
    except KeyboardInterrupt:
        print("\nStopped.")
    finally:
        if 'ser' in locals() and ser.is_open:
            ser.close()

if __name__ == "__main__":
    main()