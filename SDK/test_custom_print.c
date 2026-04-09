#include<stdint.h>
#include "printf.h"

// --- Main Entry Point ---
int main(void) {
        
    printf("System Booting...\n");
    printf("Value: %d | Hex: %x | Bin: %b\n", 42, 42, 42);
    printf("Hello, %s!\n", "Embedded World");

    while(1);
    return 0;
}