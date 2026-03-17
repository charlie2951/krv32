# Interfacing example of MPU-9265
The MPU9265 (often seen labeled as MPU-92/65) is a 9-axis MotionTracking device used in robotics, drones, and mobile devices to track orientation and movement in 3D space.<p>
Key Capabilities
The MPU9265 provides 9 Degrees of Freedom (9-DOF) by combining three different sensors into a single package:

* 3-axis Gyroscope: Measures angular velocity (how fast it’s rotating).

* 3-axis Accelerometer: Measures linear acceleration and gravity (to determine "down" and movement).

* 3-axis Magnetometer: Acts as a digital compass (to determine heading relative to North).

*  In-built temperature sensor.

**Sample Demo code:** https://github.com/charlie2951/krv32/edit/v4.0/SDK/examples/demo_mpu9265_unified.c

## SPI Mode pin configuration
**Note:** While using SPI mode, magnetometer reading not available. This is because in-built magnetometer connected via I2C bus internally and using SPI mode, its challanging to get the reading. The current library dies not support magnetometer reading at SPI mode, but available in `I2C` mode.

|MPU9250 Pin|Function|Description|
|:----------:|:------:|:---------:|
|VCC|Power|3.3V|
|GND|Ground|0V|
|SCL / SPC|SCK|Serial Clock|
|SDA / SDI|MOSI|Master Out Slave In|
|ADO / SDO|MISO|Master In Slave Out|
|NCS|CS|Chip Select (Active Low)|

## I2C Mode Pin configuration
<img width="400" height="250" alt="image" src="https://github.com/user-attachments/assets/5ef74693-0637-4dc6-90ce-c1efc67d9789" />

