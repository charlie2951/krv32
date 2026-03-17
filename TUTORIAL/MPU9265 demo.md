# Interfacing example of MPU-9265
The MPU9265 (often seen labeled as MPU-92/65) is a 9-axis MotionTracking device used in robotics, drones, and mobile devices to track orientation and movement in 3D space.<p>
Key Capabilities
The MPU9265 provides 9 Degrees of Freedom (9-DOF) by combining three different sensors into a single package:

3-axis Gyroscope: Measures angular velocity (how fast it’s rotating).

3-axis Accelerometer: Measures linear acceleration and gravity (to determine "down" and movement).

3-axis Magnetometer: Acts as a digital compass (to determine heading relative to North).

MPU9250 Pin,Function,Description
VCC,Power,3.3V
GND,Ground,0V
SCL / SPC,SCK,Serial Clock
SDA / SDI,MOSI,Master Out Slave In
ADO / SDO,MISO,Master In Slave Out
NCS,CS,Chip Select (Active Low)
