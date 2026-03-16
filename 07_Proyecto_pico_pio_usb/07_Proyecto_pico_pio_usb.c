/* HOST + HID Joystick */
#include <stdio.h>
#include "pico/stdlib.h"
#include "tusb.h"
#include "pio_usb.h"

//static usb_descriptor_buffers_t desc_buffers;

int main()
{
    stdio_init_all();
    sleep_ms(2000);

    printf("PIO USB HOST HID Example\n");

    pio_usb_configuration_t pio_cfg = PIO_USB_DEFAULT_CONFIG;
    pio_cfg.pin_dp =28;
    // DP, DM = DP + 1 por eso no se declara, los pines de la waveshare para pico pio usb son:
    /* DP = GPIO 28   3V3 => R22 2K2 ohm => D1_P(DP1 A6) => GPIO 28 => R8 22R => USB_D_P 
       DM = GPIO 29   3V3 => R23 2K2 ohm => D1_P(DN1 A7) => GPIO 29 => R9 22R => USB_D_N

       R22 y R23 Pull-up simetrico, tipico de implementaciones bit-banged USB.
       Sirve para:
       - Estabilizar el bus.
       - Simular el pull-up de dispositivo USB
       R8 y R9 Se usan para controlar impedancia y reducir ringing.
    */

    // Inicializa TinyUSB
    tusb_init();

    // Inicializar PIO USB HOST
    pio_usb_host_init(&pio_cfg);

    while (true) {
        tuh_task(); /* HOST task */
        sleep_ms(1);
    }
}

/* Se llama cuando conectan un dispositivo HID */
void tuh_hid_mount_cb(uint8_t dev_addr,
                      uint8_t instance,
                      uint8_t const* desc_report,
                      uint16_t desc_len)
{
    (void) desc_report;
    (void) desc_len;
    printf("HID device connected\n");

    // empezar a recibir reports
    tuh_hid_receive_report(dev_addr, instance);
}

/* Se llama cuando llega un reporte HID */
void tuh_hid_report_received_cb(uint8_t dev_addr,
                                uint8_t instance,
                                uint8_t const* report,
                                uint16_t len)
{
    printf("HID report recibido: ");

    for(int i=0;i<len;i++)
        printf("%02X ", report[i]);

    printf("\n");

    // pedir el siguiente reporte
    tuh_hid_receive_report(dev_addr, instance);
}