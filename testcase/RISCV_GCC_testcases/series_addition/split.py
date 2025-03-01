def split_32bit_to_8bit(input_file, output_file):
    """
    Reads 32-bit data from an input text file, splits it into four 8-bit chunks,
    and writes the chunks to an output text file.

    Args:
        input_file (str): Path to the input text file.
        output_file (str): Path to the output text file.
    """

    try:
        with open(input_file, 'r') as infile, open(output_file, 'w') as outfile:
            for line in infile:
                try:
                    # Remove any leading/trailing whitespace and convert to integer
                    hex_value = line.strip()
                    if hex_value.startswith("0x"):
                        int_value = int(hex_value, 16)
                    else:
                        int_value = int(hex_value, 16)

                    # Ensure it's a 32-bit value
                    if not (0 <= int_value <= 0xFFFFFFFF):
                        raise ValueError("Input value is not a 32-bit integer.")

                    # Split into 8-bit chunks
                    byte1 = (int_value >> 24) & 0xFF
                    byte2 = (int_value >> 16) & 0xFF
                    byte3 = (int_value >> 8) & 0xFF
                    byte4 = int_value & 0xFF

                    # Write the chunks to the output file (hexadecimal representation)
                    outfile.write(f"{byte1:02X} {byte2:02X} {byte3:02X} {byte4:02X}\n")

                except ValueError as e:
                    print(f"Error processing line '{line.strip()}': {e}")
                except Exception as e:
                    print(f"An unexpected error occurred processing line '{line.strip()}': {e}")

    except FileNotFoundError:
        print(f"Error: Input file '{input_file}' not found.")
    except Exception as e:
        print(f"An unexpected error occurred: {e}")

# Get input file name from the user
input_filename = "firmware.hex"

# Get output file name from the user
output_filename = "firmware.txt"

split_32bit_to_8bit(input_filename, output_filename)

print(f"Data split and written to '{output_filename}'.")
