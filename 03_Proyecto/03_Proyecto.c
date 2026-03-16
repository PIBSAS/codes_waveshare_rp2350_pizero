#include <stdio.h>
#include "pico/stdlib.h"
#include "hardware/flash.h"

int main()
{
    stdio_init_all();

    while (true) {
        printf("Hello, world!\n");
        printf(" La memoria flas es de: %u bytes\n", PICO_FLASH_SIZE_BYTES);
        sleep_ms(1000);
    }
}
