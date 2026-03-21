#include "esp_wifi.h"
#include "printf.h"
#include "delay.h"

char *ssid = "wifi_AP_ssid";
char *pass = "wifi_password";

/*Limitations:
 Note that currently the CPU doesnt support interrupt. 
To receive response from ESP, interrupt is required.
hence the return value (success or error like status) is
not going to work in this version of KRV-32 cpu.
However, you can call the function but API is working or not,
that you can verify only physical observation such as Wifi AP created or 
connected to AP or not, or data sending to cloud or not.
THis will not print any status. So dont put the API inside if().
Also, ignore the status printed by driver API
*/
int main(void) {
    // 2. Initialize ESP
    ESP_Init();

    // 3. Set to Station + AP Mode
    ESP_SetMode(3); //station+softAP
    printf("Mode set to Station+AP\n");
    delay(1000000);
    

    // 4. API For Setup our Hotspot
    ESP_ConfigureSoftAP("KRV32_Wifi", "abcd1234");
    delay(1000000);
    // 5. Connect to Home Router
      ESP_ConnectToAP(ssid, pass);
        printf("Connected to Internet!\n");
      delay(1000000);

      // example: connect and sending data packet over tcp
      
      ESP_TCP_Connect(0, TCP, "192.168.1.10", 8080);
      //printf("TCP Connected!\n");
     // Send a message/data
        ESP_Send_Data(0, "Hello from Bare metal KRV32 RiscV!\n");
    

    while (1) {
        ;
        // example: sending data packet over tcp

    }
}
