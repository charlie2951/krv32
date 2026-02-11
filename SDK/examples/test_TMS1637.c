/*
MIT License

Copyright (c) 2024-2026 Subir Maity

Permission is hereby granted, free of charge, to any person obtaining a copy
of this software and associated documentation files (the "Software"), to deal
in the Software without restriction, including without limitation the rights
to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
copies of the Software, and to permit persons to whom the Software is
furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all
copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
SOFTWARE.
*/
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

