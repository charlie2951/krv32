/*
This is a demo code to interface BMP180 I2C pressure and temp sensor with
thingspeak IOT cloud over https GET method. ESP01 wifi module is connected to UART2
to communicate with WiFI and to transmit data.
  */

#include "thingspeak.h"
#include "delay.h"
#include "bmp180.h"
#include "printf.h"

#define MY_API_KEY "your_write_api_key_thingspeak" //create an account and create a channel and note down write API key
char *ssid = "your_wifi_ssid";
char *pw = "password_wifi";

void main() {
 //setting up BMP180 over I2C0 port
    i2c_init(&I2C0, 249); //initialize 12c with a freq of 50KHz, set 499 to make it 100Khz
    if (bmp180_init(&I2C0) != 0){
        printf("Unable to detect BMP180 sensor! \r\n ");
         while(1); // Error
    }
    printf("BMP180 found in I2C bus! \r\n ");

    // Initial ESP and WiFi setup...
    ESP_Init();
    delay(1000000);
    //Set to Station Mode
    ESP_SetMode(1);
    printf("Mode set to Station\n");
    delay(2000000);
    ESP_ConnectToAP(ssid,pw);
    delay(10000000);
    while(1) {
        int sensors[3];//total 3 data we are sending 
        //Read BMP180 sensor data
        int32_t raw_t = bmp180_get_raw_temp(&I2C0);
        sensors[0] = bmp180_calculate_temp(raw_t); // e.g., 255 = 25.5C
        int32_t raw_p = bmp180_get_raw_pressure(&I2C0, BMP180_STANDARD);
       sensors[1] = bmp180_calculate_pressure(raw_p, BMP180_STANDARD);
       // 3. Calculate Altitude (Relative to Sea Level)
        sensors[2] = bmp180_get_altitude(sensors[1], SEA_LEVEL_PRESSURE);
      

        // One line to send everything!
      //in this library API, it accept upto 8 integer type sensor data
      //process your data locally before sending
        ThingSpeak_Send(MY_API_KEY, sensors, 3); 

        // ThingSpeak requires 15 seconds between updates for free accounts
        delay(5000000); 
        delay(5000000);
        delay(10000000);
    }
}
