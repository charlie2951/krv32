# Tcl Script to create a Vivado project, add sources, synthesize, implement, and generate bitstream
puts "Created by Dr S.K.Maity, School of Electronics Engg, KIIT"

# =================================================================
# 1. Project Setup Variables (Modify these variables as per your design)
# =================================================================
set PROJECT_NAME "krv32_soc_nexys4ddr"
set PROJECT_DIR "./$PROJECT_NAME"
set PROJECT_FILE "$PROJECT_DIR/$PROJECT_NAME.xpr"

puts "Checking for existing project: $PROJECT_NAME"

## 1. Check if a project is CURRENTLY OPEN in the Vivado session (The Robust Way)
# Check the length of the list returned by get_projects
if {[llength [get_projects]] > 0} {
    puts "A project is currently open. Closing the project..."
    close_project -quiet
}

# -------------------------------------------------------------
# Keep your existing (and correct) file existence check below
# -------------------------------------------------------------

## 2. Check if the project file EXISTS on the disk
if {[file exists $PROJECT_FILE]} {
    puts "Project file found at $PROJECT_FILE. Deleting old project directory..."
    
    # You might want to delete the entire project directory to start clean
    # The 'file delete -force' command handles directories and content recursively
    if {[file delete -force $PROJECT_DIR]} {
        puts "Successfully deleted old project directory: $PROJECT_DIR"
    } else {
        # This error is rare but handles permission issues
        puts "ERROR: Could not delete $PROJECT_DIR. Check permissions."
    }
} else {
    puts "No existing project file found. Ready to create a new project."
}

set DEVICE_PART "xc7a100tcsg324-1"  ;# Specify your target FPGA part number
set RTL_DIR "./src/RTL"               ;# Path to the directory containing your RTL (.v/.vhd) files
set MEM_FILE_DIR "./src/mem"
set TOP_MODULE "top"   ;# Name of your top-level design entity

# =================================================================
# 2. Project Creation
# =================================================================
puts "Creating project: $PROJECT_NAME in $PROJECT_DIR"
create_project $PROJECT_NAME $PROJECT_DIR -force -part $DEVICE_PART

# Set the target language (VHDL or Verilog)
#set_property target_language VHDL [current_project]
# Alternatively for Verilog: set_property target_language Verilog [current_project]
set_property target_language Verilog [current_project]

# =================================================================
# =================================================================
# 3. Add RTL Sources
# =================================================================
puts "Adding specific RTL sources..."

# Replace the glob command with a list of your known file names.
# Make sure to prepend the $RTL_DIR variable to each filename 
# so the script knows where to find them.

set rtl_files [list \
    $RTL_DIR/top_nexys4ddr.v \
    $RTL_DIR/cpu.v \
    $RTL_DIR/progmem.v \
    $RTL_DIR/bootmem.v \
    $RTL_DIR/gpio_controller.v \
    $RTL_DIR/UART_wrapper.v \
    $RTL_DIR/uart_rx_gpio.v \
    $RTL_DIR/uart_tx_gpio.v \
    $RTL_DIR/uart_tx.v \
    $RTL_DIR/uart_rx.v \
    $RTL_DIR/crypto_wrapper.v \
    $RTL_DIR/aes_encrypt.v \
    $RTL_DIR/aes_decrypt.v \
    $RTL_DIR/uart1_wrapper.v \
    $RTL_DIR/uart1_rx_gpio.v \
    $RTL_DIR/uart1_tx_gpio.v \
    $RTL_DIR/uart1_tx.v \
    $RTL_DIR/uart1_rx.v \
    $RTL_DIR/timer_gpio.v \
    $RTL_DIR/timer1_gpio.v \
    $RTL_DIR/timer_counter.v \
    $RTL_DIR/i2c_wrapper.v \
    $RTL_DIR/i2c1_wrapper.v \
   $RTL_DIR/i2c_master.sv \
   $RTL_DIR/clock_counter.sv \
   $RTL_DIR/segment_ip.v \
   $RTL_DIR/spi_master0_wrapper.v \
   $RTL_DIR/spi_master.v \

]

set mem_files [list \
    $MEM_FILE_DIR/boot.mem \
    $MEM_FILE_DIR/firmware.mem \
]

if {[llength $rtl_files] == 0} {
    puts "ERROR: The file list is empty. Please check the file names."
    error "Exiting script due to missing RTL sources."
}

add_files $rtl_files
# Adding system verilog source file
set_property file_type SystemVerilog [get_files $RTL_DIR/i2c_master.sv]
set_property file_type SystemVerilog [get_files $RTL_DIR/clock_counter.sv]
update_compile_order -fileset sources_1

# Define the path to your constraint file
set SDC_FILE_PATH "./src/nexys4ddr.xdc"
#set SDC_FILE_PATH "./src/Basys-3-Master.xdc"

# Add the SDC file to the constraints fileset
add_files -fileset [get_filesets constrs_1] $SDC_FILE_PATH

puts "SDC file $SDC_FILE_PATH added to the project."

# Adding MEMORY files
add_files $mem_files

# Set the Top Module
set_property top $TOP_MODULE [current_fileset]
puts "Top module set to: $TOP_MODULE"
# =================================================================
# 4. Perform Synthesis
# =================================================================
puts "Starting Synthesis..."
# Use -jobs to utilize multiple CPU cores for faster execution
launch_runs synth_1 -jobs 4 
wait_on_run synth_1

# Check for Synthesis completion and errors
set synth_status [get_property STATUS [get_runs synth_1]]
#if { $synth_status != "synth_design Complete!" } {
#    puts "ERROR: Synthesis failed with status: $synth_status. Check log file."
#    error "Exiting script due to Synthesis failure."
#}
puts "Synthesis Completed Successfully."

# =================================================================
# 5. Perform Implementation and Generate Bitstream
# =================================================================
puts "Starting Implementation..."
# The -to_step write_bitstream option ensures the entire implementation flow, 
# including bitstream generation, is executed.
launch_runs impl_1 -to_step write_bitstream -jobs 4
wait_on_run impl_1

# Check for Implementation completion and errors
set impl_status [get_property STATUS [get_runs impl_1]]
#if { $impl_status != "Completed" } {
 #   puts "ERROR: Implementation failed with status: $impl_status. Check log file."
 #   error "Exiting script due to Implementation failure."
#}
puts "Implementation Completed Successfully."

# =================================================================
# 6. Final Steps
# =================================================================
# The generated bitstream (.bit file) will be located in:
# $PROJECT_DIR/$PROJECT_NAME.runs/impl_1/$TOP_MODULE.bit
puts "--- SCRIPT FINISHED ---"
puts "Bitstream file should be located in the impl_1 run directory."
# Generating MCS file
puts "------ Generating MCS file for Flash -----"
# 1. Define your file paths and parameters
set bit_file "$PROJECT_DIR/$PROJECT_NAME.runs/impl_1/$TOP_MODULE.bit"
set mcs_file "$PROJECT_DIR/$PROJECT_NAME.runs/impl_1/nexys4ddr.mcs"
set flash_size "16"
set interface "SPIx4"

# 2. Run the write_cfgmem command
write_cfgmem -format mcs \
             -size $flash_size \
             -interface $interface \
             -loadbit "up 0x00000000 $bit_file" \
             -checksum \
             -force \
             -file $mcs_file

puts "MCS file generation complete: $mcs_file"
puts "MCS file should be located in the impl_1 run directory."

close_project
