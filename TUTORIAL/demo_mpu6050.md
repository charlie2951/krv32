### This demo is about interfacing MPU6050 accelerometer and Gyro sensor 
Note: MPU6050 sensor can be connected to any one hardware I2C port . 
If using I2C0, Connect the sensor pin SDA to PMOD JA pin-3 and SCL to pin-4.
Also, connect VDD and GND with the proper pins discussed earlier.
<p>
     Sample code can be found here: https://github.com/charlie2951/krv32/blob/v4.0/SDK/examples/test_mpu6050.c
</p>
compile the code and upload the firmware and see the acceleration and gyro data along with temperature in serial
monitor.
