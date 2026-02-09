#include "tm1637.h"
#include "delay.h"

int main(void) {
    /* Define my pins here */
    tm1637_config_t display = {
        .dio_pin = 2,
        .clk_pin = 3
    };

    /* Initialize with the config */
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