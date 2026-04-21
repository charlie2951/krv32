# Pre-build MCS file for Vivado boards
|Board|MCS File name| Features|
|:---:|:------------:|:-------|
|Nexys4-DDR|nexys4ddr.mcs|firmware is only uploaded to BRAM, no Flash support for storing firmware|
|Nexys4-DDR|nexys4ddr_flash.mcs (latest)|Firmware will be uploaded to Flash, and after powering off, <p> it will be loaded automatically to BRAM and will start execution|
|Basys3|basys3.mcs|firmware is only uploaded to BRAm, no flash support for firmware storage|
|Basys3|basys3_flash.mcs (8KB program memory)|Firmware will be uploaded to Flash, and after powering off, <p> it will be loaded automatically to BRAM and will start execution|
|Basys3|basys3_flash64KB.mcs (64KB program memory)|Firmware will be uploaded to Flash, and after powering off, <p> it will be loaded automatically to BRAM and will start execution|
|Arty A7 100t|arty_flash.mcs (latest)|Firmware will be uploaded to Flash, and after powering off, <p> it will be loaded automatically to BRAM and will start execution|

**Note:** The source code provided in `src` contains different files for different build options. If you want to use the  previous version (without Flash support for firmware storage) then see `branch 4.0` or download from the release tagged with `V4.0`. Without flash, the firmware will be deleted after powering off, and again, you need to upload the firmware.
