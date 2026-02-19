### Interfacing 7 segment display (TM1637 module)
Note: GPIO-2 is used as data pin and GPIO-3 is used a Clock pin. GPIO-2 mapped to PMOD JB pin3 and 
GPIO3 mapped to PMOD JB pin4. Connect Vdd to JB 6 and GND to JB5. (Refer to Nexys4 DDR manual for PMOD pin connections).
You can select any other GPIO in between 0 to 7 for this. It is declared
in this section `tm1637_config_t display = {
        .dio_pin = 2,
        .clk_pin = 3
    }`
    
```
#include "tm1637.h"
#include "delay.h"

int main(void) {
    // Define my pins here 
    tm1637_config_t display = {
        .dio_pin = 2,
        .clk_pin = 3
    };

    // Initialize with the config 
    tm_init(&display);

    //test-1:display string
    tm_display_string("HI  ");
        delay(5000000);

    while(1) {
        // Displaying a simple message
        
        
        // Running a scroll
       // tm_display_scroll("HELLO SUBIR", 1000);

        //display clock
        tm_display_clock(8,30,1);

    }

}
```
**API used:** <p>
`tm_display_string()`: display string <p>
`tm_display_scroll()` : enable scrolling display <p>
`tm_display_clock()` : Display number in clock format.
